var express = require('express');
var app = express();
HTTPserver = require('http').createServer(app);
var io = require('socket.io').listen(HTTPserver);
var path = require('path');
var MongoClient = require('mongodb').MongoClient;
var bodyParser = require('body-parser');
var ObjectID = require('mongodb').ObjectID;

app.use(require('cors')());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
  extended: true
}));
var url = "mongodb://localhost:12345"

var Server = function() {
    this.db = null;
    this.runningServer = null;
    app.use(express.static(path.join(__dirname, '/../client')));
    
    function multisearch(list, curr, resultlist, coll, callback) {
        if (curr == list.length) {
            callback(resultlist);
        } else {
            this.db.collection(coll).find({_id: list[curr]}).toArray(function(err, result) {
                if (result != null && result.length != 0) {
                    resultlist.push(result[0]);
                }
                multisearch(list, curr + 1, resultlist, coll, callback);
            });
        }
    }

    app.get('/userIDof', function(req, res) {
        /*
         * Get the ID of a username
         * params:
         *    usr: username to get ID of.
         */
        var return_value = {error: "", result: ""};
        this.db.collection('users').findOne({username: req.query.usr}, function(err, doc) {
            if (err || doc == undefined) {
                return_value["error"] = "Could not find user. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            
            return_value["result"] = doc["_id"];
            res.send(return_value);
        });
    });

    app.use('/home', function(req, res) {
        /*
         * Sends the starting page .html file.
         * no params.
         */
        res.sendFile(path.join(__dirname, "/../client", "home.html"));
    });

    app.get('/homeData', function(req, res) {
        /*
         * Gets the data of the users specialized homepage.
         * Returns JSON containing:
         *    Messages from groups the user is part of (with group names).
         * params:
         *    usr: username of user to get data for
         */
        var return_value = {error: "", result: []};
        this.db.collection('users').find({username: req.query.usr}).toArray(function(err, result) {
            if (err || result.length == 0) {
                return_value["error"] = "Could not find user. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            
            var groups = result[0]["groups"];
            multisearch(groups, 0, [], 'groups', function(resultlist) {
                if (resultlist.length == 0) {
                    res.send(return_value);
                    return;
                }
                var count = 0;
                for (i = 0; i < resultlist.length; i++) {
                    this.db.collection('messages').find({ID: resultlist[i]["_id"]}).toArray(function(err, messagesData) {
                        this.db.collection('groups').find({_id: messagesData[0]["ID"]}).toArray(function(err, groups) {
                            count = count + 1;
                            return_value["result"].push({name: groups[0]["name"], ID: groups[0]["_id"], messages: messagesData[0]["messages"]});
                            if (count == resultlist.length)
                                res.send(return_value);
                        });
                    });
                }
            });
        });
    });

    app.get('/groupData', function(req, res) {
        /*
         * Get the data of a groups specialized page.
         * Returns JSON containing:
         *    Posts, name and members of the group in question.
         * params:
         *    id: ID of the group
         */
        var return_value = {error: "", result: {}};
        this.db.collection('groups').findOne({_id: ObjectID(req.query.id)}, function(err, doc) {
            if (err || doc == undefined) {
                return_value["error"] = "Could not find group. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            return_value["result"]["name"] = doc["name"];

            multisearch(doc["members"], 0, [], 'users', function(resultlist) {
                return_value["result"]["members"] = resultlist;

                this.db.collection('messages').findOne({ID: ObjectID(req.query.id)}, function(err, messages) {
                    if (err || messages == undefined) {
                        return_value["error"] = "Could not find messages for group. Please try refreshing the site";
                        res.send(return_value);
                        return;
                    }
                    
                    return_value["result"]["posts"] = messages["messages"];
                    res.send(return_value);
                });
            });
        });
    });

    app.get('/userData', function(req, res) {
        /*
         * Get the data of a users specialized page.
         * Returns JSON containing:
         *    ID's and names of groups the user is a member of. 
         * params:
         *    id: ID of the user
         */
        var return_value = {error: "", result: {}};
        this.db.collection('users').findOne({_id: ObjectID(req.query.id)}, function(err, doc) {
            if (err || doc == undefined) {
                return_value["error"] = "Could not find user. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            return_value["result"] = {name: doc["username"], result: []}

            multisearch(doc["groups"], 0, [], 'groups', function(resultlist) {
                for (index in resultlist) {
                    return_value["result"]["result"].push({name: resultlist[index]["name"], ID: resultlist[index]["_id"]});
                }

                res.send(return_value);
            });
        });
    });

    app.get('/search', function(req, res) {
        /*
         * Searches for a certain query.
         * Searches both users and groups.
         * If query is empty, return everything.
         * Returns JSON containing results.
         * params:
         *    term: the term to search for
         */
        var return_value = {error: "", result: {groups: [], users: []}};
        this.db.collection('groups').find({}).toArray(function(err, docs) {
            if (err || docs == undefined) {
                return_value["error"] = "Could not get group data. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            
            for (index in docs) {
                if (docs[index]["name"].indexOf(req.query.term) > -1) {
                    return_value["result"]["groups"].push({name: docs[index]["name"], ID: docs[index]["_id"]});
                }
            }
            
            this.db.collection('users').find({}).toArray(function(err, docs) {
                if (err || docs == undefined) {
                    return_value["error"] = "Could not get user data. Please try refreshing the site";
                    res.send(return_value);
                    return;
                }
            
                for (index in docs) {
                    if (docs[index]["username"].indexOf(req.query.term) > -1) {
                        return_value["result"]["users"].push({name: docs[index]["username"], ID: docs[index]["_id"]});
                    }
                }
                
                res.send(return_value);
            });
        })
    });

    app.post('/login', function(req, res) {
        /*
         * When the user wants to log in.
         * Returns true on found user, false on not found.
         * params:
         *    usr: username user is trying to log in with
         *    pw: password user is trying to log in with
         */
        var return_value = {error: ""};
        this.db.collection('users').find({username: req.body.usr, password: req.body.pw}).toArray(function(err, docs) {
            if (err || docs.length == 0) {
                return_value["error"] = "That username/password combination could not be found. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            
            res.send(return_value);
        });
    });

    app.post('/register', function(req, res) {
        /*
         * When the user wants to register.
         * Returns true if no user is found with that username,
         * and false if one is found.
         * params:
         *    usr: username user is trying to register
         *    pw: password user is trying to register
         */
        var return_value = {error: ""};
        this.db.collection('users').find({username: req.body.usr}).toArray(function(err, docs) {
            if (err || docs.length > 0) {
                return_value["error"] = "A user with that username already exists";
                res.send(return_value);
                return;
            }
            
            var user = {username: req.body.usr, password: req.body.pw, groups: [], joinReqs: []};
            this.db.collection('users').insert([user], function(err, result) {
                if (err) {
                    return_value["error"] = "Could not add user to database. Please try refreshing the site";
                    res.send(return_value);
                    return;
                }
                    
                res.send(return_value);
            });
        });
    });
    
    app.get('/createGroup', function(req, res) {
        /*
         * User wants to create a group.
         * Returns a JSON containing:
         *    Whether or not the operation worked, and the group ID
         * params:
         *    name: New group name.
         *    usr: ID of user who is creating the group (becomes admin)
         */
        var return_value = {error: ""};
        this.db.collection('groups').findOne({name: req.query.name}, function(err, doc) {
            if (err || doc != undefined) {
                return_value["error"] = "A group with that name already exists";
                res.send(return_value);
                return;
            }

            var data = {name: req.query.name, admin: ObjectID(req.query.usr), members: [ObjectID(req.query.usr)]};
            this.db.collection('groups').insert([data], function(err, groupRes) {
                if (err) {
                    return_value["error"] = "Could not add group to database. Please try refreshing the site";
                    res.send(return_value);
                    return;
                }
                
                data = {ID: groupRes["ops"][0]["_id"], messages: []};
                this.db.collection('messages').insert([data], function(err, messageRes) {
                    if (err) {
                        return_value["error"] = "Could not create message list. Please try refreshing the site";
                        res.send(return_value);
                        return;
                    }
                    
                    this.db.collection('users').findOne({_id: ObjectID(req.query.usr)}, function(err, userDoc) {
                        if (err || userDoc == undefined) {
                            return_value["error"] = "Could not find user. Please try refreshing the site";
                            res.send(return_value);
                            return;
                        }

                        data = userDoc["groups"];
                        data.push(groupRes["ops"][0]["_id"]);
                        this.db.collection('users').update({username: userDoc["username"]}, {$set: {groups: data}}, function(err, userRes) {
                            if (err) {
                                return_value["error"] = "Could not set user group list. Please try refreshing the site";
                                res.send(return_value);
                                return;
                            }
                            
                            return_value["ID"] = groupRes["ops"][0]["_id"];
                            res.send(return_value);
                        });
                    });
                });
            });
        });
    });

    app.get('/groupReqs', function(req, res) {
        /*
         * When a user wants to see all the requests
         * to join groups the user is admin of.
         * Returns an array containing JSONs containing:
         *    User requesting's ID and name, and the group's ID and name.
         * params:
         *    id: ID of admin
         */
        var return_value = {error: "", result: []};
        var count = 0;
        this.db.collection('users').findOne({_id: ObjectID(req.query.id)}, function(err, userRes) {
            if (err || userRes == undefined) {
                return_value["error"] = "Could not find admin. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            
            if (userRes["joinReqs"].length == 0) {
                res.send(return_value);
                return;
            }
            for (index in userRes["joinReqs"]) {
                this.db.collection('users').findOne({_id: ObjectID(userRes["joinReqs"][index]["user"])}, function(err, joinUser) {
                    if (err || joinUser == undefined) {
                        return_value["error"] = "Could not find user requesting to join. Please try refreshing the site";
                        res.send(return_value);
                        return;
                    }
                    
                    this.db.collection('groups').findOne({_id: ObjectID(userRes["joinReqs"][index]["group"])}, function(err, joinGroup) {
                        if (err) {
                            return_value["error"] = "Could not find group user is requesting to join. Please try refreshing the site";
                            res.send(return_value);
                            return;
                        }
                        count = count + 1;
                        return_value["result"].push({usrName: joinUser["username"], usrID: joinUser["_id"], groupName: joinGroup["name"], groupID: joinGroup["_id"]});

                        if (count == userRes["joinReqs"].length) {
                            res.send(return_value);
                            return;
                        }
                    });
                });
            }
        });
    });

    function removeRequest(toRemove, req, res) {
        /*
         * Removes a request to join a group from
         * the groups admins group-joining messages.
         * params:
         *    toRemove: the request to remove.
         *    req & res: the data from the request.
         */
        var return_value = {error: ""};
        this.db.collection('groups').findOne({_id: ObjectID(req.query.gid)}, function(err, groupRes) {
            if (err || groupRes == undefined) {
                return_value["error"] = "Could not find group. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            
            this.db.collection('users').findOne({_id: ObjectID(groupRes["admin"])}, function(err, userRes) {
                if (err || userRes == undefined) {
                    return_value["error"] = "Could not find admin. Please try refreshing the site";
                    res.send(return_value);
                    return;
                }

                var index = -1;
                for (i in userRes["joinReqs"]) {
                    if (userRes["joinReqs"][i]["user"] == toRemove["user"] &&
                        userRes["joinReqs"][i]["group"] == toRemove["group"]) {
                        index = i;
                        break;
                    }
                }
                
                if (index == -1) {
                    return_value["error"] = "Could not find the request to remove. Please try refreshing the site";
                    res.send(return_value);
                    return;
                }
                userRes["joinReqs"].splice(index, 1);

                this.db.collection('users').update({_id: ObjectID(groupRes["admin"])}, {$set: {joinReqs: userRes["joinReqs"]}}, function(err, result) {
                    if (err) {
                        return_value["error"] = "Could not update join request list. Please try refreshing the site";
                        res.send(return_value);
                        return;
                    }
                    
                    res.send(return_value);
                });
            });
        });
    }
    
    app.get('/groupReqResponse', function(req, res) {
        /*
         * Accept or decline a request to join a group.
         * Returns true on success, and false otherwise.
         * params:
         *    gid: id of group user wants to join.
         *    uid: id of user who wants to join.
         *    dec: whether or not admin accepted or declined the request.
         */
        var return_value = {error: ""};
        if (req.query.dec == "true") {
            this.db.collection('groups').findOne({_id: ObjectID(req.query.gid)}, function(err, groupRes) {
                if (err || groupRes == undefined) {
                    return_value["error"] = "Could not find group. Please try refreshing the site";
                    res.send(return_value);
                    return;
                }

                groupRes["members"].push(ObjectID(req.query.uid));

                this.db.collection('groups').update({_id: ObjectID(req.query.gid)}, {$set: {members: groupRes["members"]}}, function(err, result) {
                    if (err) {
                        return_value["error"] = "Could not update member list of group. Please try refreshing the site";
                        res.send(return_value);
                        return;
                    }

                    this.db.collection('users').findOne({_id: ObjectID(req.query.uid)}, function(err, userRes) {
                        if (err || userRes == undefined) {
                            return_value["error"] = "Could not get user. Please try refreshing the site";
                            res.send(return_value);
                            return;
                        }

                        userRes["groups"].push(ObjectID(req.query.gid));

                        this.db.collection('users').update({_id: ObjectID(req.query.uid)}, {$set: {groups: userRes["groups"]}}, function(err, result2) {
                            if (err) {
                                return_value["error"] = "Could not update users group list. Please try refreshing the site";
                                res.send(return_value);
                                return;
                            }

                            removeRequest({user: req.query.uid, group: req.query.gid}, req, res);
                        });
                    });
                });
            });
        } else {
            removeRequest({user: req.query.uid, group: req.query.gid}, req, res);
        }
    });
    
    app.get('/joinGroup', function(req, res) {
        /*
         * User requests to join group,
         * admin gets a request.
         * Returns true on success, false otherwise.
         * params:
         *    id: group ID
         *    usr: user who wants to join ID
         */
        var return_value = {error: ""};
        this.db.collection('groups').findOne({_id: ObjectID(req.query.id)}, function(err, groupRes) {
            if (err || groupRes == undefined) {
                return_value["error"] = "Could not find group. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            if (groupRes["members"].indexOf(ObjectID(req.query.usr)) > -1) {
                return_value["error"] = "Could not find user in group member list. Please try refreshing the site";
                res.send(return_value);
                return;
            }

            this.db.collection('users').findOne({_id: ObjectID(groupRes["admin"])}, function(err, userRes) {
                if (err || userRes == undefined) {
                    return_value["error"] = "Could not find group admin. Please try refreshing the site";
                    res.send(return_value);
                    return;
                }

                for (index in userRes["joinReqs"]) {
                    if (userRes["joinReqs"][index]["user"] == req.query.usr) {
                        return_value["error"] = "Could not find request in request list. Please try refreshing the site";
                        res.send(return_value);
                        return;
                    }
                }
                
                userRes["joinReqs"].push({user: ObjectID(req.query.usr), group: ObjectID(req.query.id)});
                this.db.collection('users').update({_id: ObjectID(groupRes["admin"])}, {$set: {joinReqs: userRes["joinReqs"]}}, function(err, result) {
                    if (err) {
                        return_value["error"] = "Could not update join requests for group admin. Please try refreshing the site";
                        res.send(return_value);
                        return;
                    }

                    res.send(return_value);
                });
            });
        });
    });

    app.get('/leaveGroup', function(req, res) {
        /*
         * User wants to leave group.
         * If user is group admin, remove entire group.
         * Returns true on success, false otherwise.
         * params:
         *    uid: ID of user who wants to leave
         *    gid: ID of group user wants to leave
         */
        var return_value = {error: ""};
        this.db.collection('groups').findOne({_id: ObjectID(req.query.gid)}, function(err, groupRes) {
            if (err || groupRes == undefined) {
                return_value["error"] = "Could not find group. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            
            if (groupRes["admin"] == req.query.uid) {
                // USER IS ADMIN START
                this.db.collection('groups').remove({_id: ObjectID(req.query.gid)}, function(err, result) {
                    if (err) {
                        return_value["error"] = "Could not remove group. Please try refreshing the site";
                        res.send(return_value);
                        return;
                    }
                    
                    this.db.collection('users').findOne({_id: ObjectID(groupRes["admin"])}, function(err, userRes) {
                        if (err || userRes == undefined) {
                            return_value["error"] = "Could not find admin. Please try refreshing the site";
                            res.send(return_value);
                            return;
                        }
                        
                        var index = 0;
                        while (index < userRes["joinReqs"].length) {
                            if (userRes["joinReqs"][index]["group"] == req.query.gid) {
                                userRes["joinReqs"].splice(index, 1);
                            } else {
                                index = index + 1;
                            }
                        }
                        this.db.collection('users').update({_id: ObjectID(groupRes["admin"])}, {$set: {joinReqs: userRes["joinReqs"]}}, function(err, result) {
                            if (err) {
                                return_value["error"] = "Could not update admins join request list. Please try refreshing the site";
                                res.send(return_value);
                                return;
                            }
                            
                            this.db.collection('messages').remove({ID: ObjectID(req.query.gid)}, function(err, result) {
                                if (err) {
                                    return_value["error"] = "Could not remove message list. Please try refreshing the site";
                                    res.send(return_value);
                                    return;
                                }
                                
                                res.send(return_value);
                            });
                        });
                    });
                });
                return;
                // USER IS ADMIN END
            }
            
            var index = -1;
            for (i in groupRes["members"]) {
                if (groupRes["members"][i] == req.query.uid) {
                    index = i;
                }
            }
            
            if (index == -1) {
                return_value["error"] = "Could not find user in member list. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            groupRes["members"].splice(index, 1);
            this.db.collection('groups').update({_id: ObjectID(req.query.gid)}, {$set: {members: groupRes["members"]}}, function(err, result) {
                if (err) {
                    return_value["error"] = "Could not update group member list. Please try refreshing the site";
                    res.send(return_value);
                    return;
                }

                this.db.collection('users').findOne({_id: ObjectID(req.query.uid)}, function(err, memberRes) {
                    if (err || memberRes == undefined) {
                        return_value["error"] = "Could not find user. Please try refreshing the site";
                        res.send(return_value);
                        return;
                    }
                    
                    for (index in memberRes["groups"]) {
                        if (memberRes["groups"][index] == req.query.gid) {
                            memberRes["groups"].splice(index, 1);
                            break;
                        }
                    }
                    
                    this.db.collection('users').update({_id: ObjectID(req.query.uid)}, {$set: {groups: memberRes["groups"]}}, function(err, result) {
                        if (err) {
                            return_value["error"] = "Could not update user group list. Please try refreshing the site";
                            res.send(return_value);
                            return;
                        }
                        
                        res.send(return_value);
                    });
                });
            });
        });
    });
    
    app.get('/postMessage', function(req, res) {
        /*
         * User wants to post a message to a group.
         * Returns true on success, false otherwise.
         * params:
         *    group: ID of group to post message to
         *    msg: message to post
         */
        var return_value = {error: ""};
        this.db.collection('messages').findOne({ID: ObjectID(req.query.group)}, function(err, msgRes) {
            if (err || msgRes == undefined) {
                return_value["error"] = "Could not find messages for group. Please try refreshing the site";
                res.send(return_value);
                return;
            }
            
            var date = new Date();
            msgRes["messages"].push({time: date.toUTCString(), msg: req.query.msg});
            
            this.db.collection('messages').update({ID: ObjectID(req.query.group)}, {$set: {messages: msgRes["messages"]}}, function(err, result) {
                if (err) {
                    return_value["error"] = "Could not update message list. Please try refreshing the site";
                    res.send(return_value);
                    return;
                }
                
                res.send(return_value);
            });
        });
    });
    
    /**
     * v TESTING ONLY
     */
    app.use('/DEL', function(req, res) {
        this.db.collection('users').remove({}, function(err, removed) {
            if (err) {throw err;}
            //console.log("Deleted all users");
        });
        this.db.collection('groups').remove({}, function(err, removed) {
            if (err) {throw err;}
            //console.log("Deleted all groups");
        });
        this.db.collection('messages').remove({}, function(err, removed) {
            if (err) { throw err; }
            //console.log("Deleted all messages");
        });
        res.send("Done");
    });
    /**
     * ^ TESTING ONLY
     */

    app.use('/', function(req, res) {
        /*
         * Sends the login/register page .html file.
         * no params.
         */
        res.sendFile(path.join(__dirname, '/../client', 'index.html'));
    });

    app.use('*', function(req, res, next) {
        /*
         * Sends 404 status if page is not found.
         * no params.
         */
        res.sendStatus(404);
    });
    
    app.use(function(err, req, res, next) {
        /*
         * Sends 500 status on unhandled errors.
         * no params.
         */
        res.sendStatus(500);
    });

    io.on('connection', function(socket) {
        socket.on('joinchat', function(groupID, username) {

            var index = -1;
            for (i in io.sockets.in(groupID).sockets) {
                if (io.sockets.in(groupID).sockets[i].username == username)
                    index = i;
            }

            if (index != -1) {
                socket.emit('joinchatresponse', false, groupID);
                return;
            }

            socket.username = username;
            socket.room = groupID;
            socket.hp = 50;
            socket.join(groupID);
            
            if (io.sockets.in(groupID)["hasAdmin"] == undefined ||
                io.sockets.in(groupID)["hasAdmin"] == false) {
                socket.admin = true;
                io.sockets.in(groupID)["hasAdmin"] = true;
            } else {
                socket.admin = false;
            }

            socket.emit('joinchatresponse', true, groupID);
            io.sockets.in(groupID).emit('userjoining', username);
        });
        socket.on('chatmsg', function(msg) {
            io.sockets.in(socket.room).emit('chatmsg', socket.username, msg);
        });
        socket.on('adminmsg', function(msg) {
            if (!socket.admin) {
                socket.emit('adminmsgresponse', 'You are not admin');
                return;
            }
            var splitMsg = msg.split(" ");
            if (splitMsg[0] == "/hit" && splitMsg.length == 3) {
                var clients = io.sockets.in(socket.room).sockets;
                for (index in clients) {
                    if (clients[index].username == splitMsg[1]) {
                        clients[index].hp = clients[index].hp - splitMsg[2];
                        if (!clients[index].admin)
                            socket.emit('adminmsgresponse', 'Player ' + splitMsg[1] + ' was damaged for ' + splitMsg[2]);
                        clients[index].emit('adminmsgresponse', 'You were damaged for ' + splitMsg[2]);
                        break;
                    }
                }
                return;
            }
        });
        socket.on('disconnect', function() {
            if (socket.room != undefined) {
                io.sockets.in(socket.room).emit('disconnect', socket.username);
                if (socket.admin == true) {
                    if (io.sockets.in(socket.room).sockets.length > 0) {
                        var newAdmin = io.sockets.in(socket.room).sockets[0];
                        newAdmin.admin = true;
                        io.sockets.in(newAdmin.room).emit('adminmsgresponse', newAdmin.username + " is now admin");
                    } else {
                        io.sockets.in(socket.room)["hasAdmin"] = false;
                    }
                }
            }
        });
    });

    this.start = function() {
        this.runningServer = HTTPserver.listen(3000, function() {
            MongoClient.connect(url, function(err, db) {
                if (err) { throw err; }
                this.db = db;
                console.log("Starting app at http://%s:%s", "localhost", "3000");
            });
        });
    };

    this.stop = function() {
        if (this.runningServer != undefined)
            this.runningServer.close();
        if (this.db != undefined)
            this.db.close();
        console.log("App at http://%s:%s closed", "localhost", "3000");
    }
}

if (require.main === module) {
    var server = new Server();
    server.start();
} else {
    module.exports = Server;
}
