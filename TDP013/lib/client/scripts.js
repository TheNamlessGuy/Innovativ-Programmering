var username = undefined;
var myID = undefined;
var url = "http://localhost:3000";
var socket = undefined;
var worker = undefined;

function check_cookie() {
    if (document.cookie == "")
        window.open(url, "_self");
}

$(document).ready(function() {
    check_cookie();
    username = document.cookie.split(';')[0].split('=')[2];
    socket = io(url);
    $.getJSON(url + "/userIDof?usr=" + username, function(json) {
        if (json["error"].length == 0)
            myID = json["result"];
        else
            popup(json["error"], 2000, true);
    });

    document.title = "IPSoc - " + username;

    $("#yourLink").text("Hello, " + username + "!");

    $("#search_btn").click(function() {
        search();
    });

    $("#search_box").keypress(function(e) {
        if (e.which == 13) {
            search();
        }
    });

    $("#yourLink").click(function() {
        getUserData(myID);
    });
    
    $("#logout").click(function() {
        stopWorker();
        del_cookie();
        window.open(url, "_self");
    });

    $("#groupName_btn").click(function() {
        createGroup();
    });

    $("#groupJoinLink").click(function() {
        getGroupJoinData();
    });

    socket.on('joinchatresponse', function(decision, id) {
        if (decision) {
            setChatData(id);
        } else {
            popup("You are already in this chat!", 2000, true);
        }
    });

    getHomeData();
});

function getGroupJoinData() {
    stopWorker();
    startWorker("workers/worker_groupJoin.js");
    worker.onmessage = function(event) {
        if (event.data.error.length == 0)
            setGroupJoinData(event.data.result);
        else
            popup(event.data.error, 2000, true);
    }
    worker.postMessage({url: url, id: myID});
}

function setGroupJoinData(data) {
    var content_main = $("#content_main");
    content_main.html("<div class='title'>Your Group Requests:</div>");
    $("#sidebar").toggleClass("sidebar", false);
    $("#sidebar").html("");
    
    for (index in data) {
        data[index]["usrName"] = fix_string(data[index]["usrName"]);
        data[index]["groupName"] = fix_string(data[index]["groupName"]);
        content_main.append("<div class='homePost fgColor'> User <b>" +
                            data[index]["usrName"] +
                            "</b> wants to join group <b>" +
                            data[index]["groupName"] +
                            "</b>:<br>" +
                            "<button type='button' class='acceptGroup_btn' id='" +
                            data[index]["usrID"] + " " + data[index]["groupID"] +
                            "'>Accept</button>" +
                            "<button type='button' class='declineGroup_btn' id='" +
                            data[index]["usrID"] + " " + data[index]["groupID"] +
                            "'>Decline</button>" +
                            "</div>");
    }
    
    $(".acceptGroup_btn").click(function() {
        groupJoinResponse(true, $(this));
    });
    
    $(".declineGroup_btn").click(function() {
        groupJoinResponse(false, $(this));
    });
}

function groupJoinResponse(decision, obj) {
    var uid = obj.attr("id").split(" ")[0];
    var gid = obj.attr("id").split(" ")[1];

    $.getJSON(url + "/groupReqResponse?gid=" + gid +
         "&uid=" + uid + "&dec=" + decision, function(json) {
             if (json["error"].length == 0) {
                 getGroupJoinData();
             } else {
                 popup(json["error"], 2000, true);
             }
         });
}

function search() {
    stopWorker();
    var term = $("#search_box").val();
    $("#search_box").val("");
    $.getJSON(url + "/search?term=" + term, function(json) {
        if (json["error"].length == 0)
            setSearchData(json["result"]);
        else
            popup(json["error"], 2000, true);
    });
}

function setSearchData(data) {
    $("#content_main").html("<div class='floatleft fgColor' id='groups'></div><div class='floatright fgColor' id='users'></div>");
    $("#sidebar").toggleClass("sidebar", false);
    $("#sidebar").html("");

    var groups = $("#groups");
    var users = $("#users");
    groups.html("<div class='title'>Groups</div>");
    users.html("<div class='title'>Users</div>");

    for (index in data["groups"]) {
        data["groups"][index]["name"] = fix_string(data["groups"][index]["name"]);
        groups.append("<div class='search-results' id=" +
                      data["groups"][index]["ID"] +
                      ">" +
                      data["groups"][index]["name"] +
                      "</div><br>");
    }
    
    if (data["groups"].length == 0) {
        groups.append("No groups found...");
    }

    for (index in data["users"]) {
        data["users"][index]["name"] = fix_string(data["users"][index]["name"]);
        users.append("<div class='search-results' id=" +
                      data["users"][index]["ID"] +
                      ">" +
                      data["users"][index]["name"] +
                      "</div><br>");
    }

    if (data["users"].length == 0) {
        users.append("No users found...");
    }
    
    $(".search-results").click(function() {
        if ($(this).parent().attr("id") == "groups")
            getGroupData($(this).attr("id"));
        else
            getUserData($(this).attr("id"));
    });

    if (groups.height() > users.height()) {
        users.height(groups.height());
    } else {
        groups.height(users.height());
    }
}

function getHomeData(){
    stopWorker();
    startWorker("workers/worker_home.js");
    worker.onmessage = function(event) {
        if (event.data.error.length == 0)
            setHomeData(event.data.result);
        else
            popup(event.data.error, 2000, true);
    }
    worker.postMessage({url: url, username: username});
}

function setHomeData(data) {
    var content_main = $("#content_main");
    $("#sidebar").toggleClass("sidebar", false);
    $("#sidebar").html("");
    content_main.html("");
    
    var message_array = [];
    for (index in data) {
        for (index2 in data[index]["messages"]) {
            message_array.push(data[index]["messages"][index2]);
            message_array[message_array.length - 1]["name"] = data[index]["name"];
            message_array[message_array.length - 1]["ID"] = data[index]["ID"];
        }
    }

    message_array.sort(function(a, b) {
        return new Date(a.time) < new Date(b.time);
    });

    for (index in message_array) {
        message_array[index]["name"] = fix_string(message_array[index]["name"]);
        message_array[index]["msg"] = fix_string(message_array[index]["msg"]);
        content_main.append('<div class="homePost fgColor"><div class="link groupLink" id=' +
                            message_array[index]["ID"] +
                            '>' +
                            message_array[index]["name"] +
                            "</div><br>" +
                            message_array[index]["msg"] + "</div>");
    }
    
    $(".groupLink").click(function() {
        getGroupData($(this).attr("id"));
    });
}

function getGroupData(id) {
    stopWorker();
    startWorker("workers/worker_group.js");
    worker.onmessage = function(event) {
        if (event.data.error.length == 0)
            setGroupData(event.data.result, id);
        else
            popup(event.data.error, 2000, true);
    }
    worker.postMessage({url: url, id: id});
}

function setGroupData(data, id) {
    var content_main = $("#content_main");
    var sidebar = $("#sidebar");
    sidebar.toggleClass("sidebar", true);
    sidebar.toggleClass("fgColor", true);
    sidebar.html("");
    data["name"] = fix_string(data["name"]);
    content_main.html("<button type='button' class='btn btn-success floatright group_btn' id='group_btn'>Join Group</button>" +
                      "<button type='button' class='btn btn-success floatright group_btn' id='chat_btn'>Join Chat</button>" + 
                      "<div class='title' id='curGroupName'>" + data["name"] + "</div>");
    $("#chat_btn").hide();
    
    data["posts"].sort(function(a, b) {
        return new Date(a.time) < new Date(b.time);
    });

    for (index in data["members"]) {
        data["members"][index]["username"] = fix_string(data["members"][index]["username"]);
        sidebar.append('<div class="link userLink" id=' +
                       data["members"][index]["_id"] +
                       '>' +
                       data["members"][index]["username"] +
                       "</div><br>");
        
        if (data["members"][index]["username"] == fix_string(username)) {
            $("#chat_btn").show();
            $("#group_btn").text("Leave Group");
            content_main.append('<div class="input-group">' +
                                '<input type="text" class="form-control" placeholder="Post a message..." id="msg_box">' +
                                '<span class="input-group-btn">' +
                                '<button type="button" class="btn btn-default" id="msg_btn">Post</button>' +
                                '</span></div>');
            $("#msg_btn").click(function() {
                var msg = $("#msg_box").val();
                if (msg.length == 0) {
                    popup("You have to write a message to post!", 2000, true);
                    return;
                }
                $.getJSON(url + "/postMessage?group=" + id + "&msg=" + msg, function(json) {
                    if (json["error"].length == 0) {
                        $("#msg_box").val("");
                        getGroupData(id);
                    } else {
                        popup(json["error"], 2000, true);
                    }
                });
            });
        }
    }
    
    for (index in data["posts"]) {
        data["posts"][index]["msg"] = fix_string(data["posts"][index]["msg"]);
        data["posts"][index]["msg"] = data["posts"][index]["msg"].split("\\n").join("<br>");
        content_main.append('<div class="homePost fgColor"><div class="groupName">' +
                            data["name"] +
                            "</div><br>" +
                            data["posts"][index]["msg"] +
                            "</div>");
    }

    $(".userLink").click(function() {
        getUserData($(this).attr("id"));
    });

    $("#group_btn").click(function() {
        if ($(this).text() == "Join Group") {
            $.getJSON(url + "/joinGroup?id=" + id + "&usr=" + myID, function(json) {
                if (json["error"].length == 0)
                    popup("The group admin has gotten your request", 2000, false);
                else
                    popup(json["error"], 2000, true);
            });
        } else {
            $.getJSON(url + "/leaveGroup?gid=" + id + "&uid=" + myID, function(json) {
                if (json["error"].length == 0) {
                    location.reload();
                } else {
                    popup(json["error"], 2000, true);
                }
            });
        }
    });
    
    $("#chat_btn").click(function() {
        if ($(this).is(":hidden")) {
            popup("Can't join chat if you are not part of the group!", 2000, true);
            return;
        }
        socket.emit('joinchat', id, username);
    });
}

function setChatData(groupID) {
    stopWorker();
    var content_main = $("#content_main");
    $("#sidebar").toggleClass("sidebar", false);
    $("#sidebar").html("");
    content_main.html("<div id='chat_frame' class='fgColor' style='overflow-y: scroll'>" +
                      "<ul id='chatlist'>" +
                      "</ul>" +
                      "<div id='chat_frame_btm'></div>" +
                      "</div>" +
                      "<div class='input-group' id='chat_type_container'></div>");
    $("#chat_type_container").html('<input type="text" class="form-control height-8vh" placeholder="Post a message..." id="chat_type_text">' +
                                  '<span class="input-group-btn">' +
                                  '<button type="button" class="btn btn-default height-8vh" id="chat_type_btn">Post</button>' +
                                  '</span>');
    
    $("#chat_type_btn").click(function() {
        var chat_msg = $("#chat_type_text").val();
        $("#chat_type_text").val("");
        
        if (chat_msg.length == 0) { return; }
        if (chat_msg[0] == "/")
            socket.emit('adminmsg', chat_msg);
        else
            socket.emit('chatmsg', chat_msg);
    });
    
    socket.on('chatmsg', function(username, message) {
        $("#chatlist").append("<li><b>" + username + "</b>: " + message + "</li>");
        $("#chat_frame").scrollIntoView($("#chat_frame_btm"));
    });

    socket.on('adminmsgresponse', function(msg) {
        $("#chatlist").append("<li><i>" + msg + "</i></li>");
        $("#chat_frame").scrollIntoView($("#chat_frame_btm"));
    });
    
    socket.on('userjoining', function(name) {
        $("#chatlist").append("<li><i>Welcome, " + name + "</i></li>");
        $("#chat_frame").scrollIntoView($("#chat_frame_btm"));
    });

    socket.on('disconnect', function(name) {
        $("#chatlist").append("<li><i>" + name + " disconnected</i></li>");
        $("#chat_frame").scrollIntoView($("#chat_frame_btm"));
    });
}

function getUserData(id) {
    stopWorker();
    startWorker("workers/worker_user.js");
    worker.onmessage = function(event) {
        if (event.data.error.length == 0)
            setUserData(event.data.result);
        else
            popup(event.data.error, 2000, true);
    }
    worker.postMessage({url: url, id: id});
}

function setUserData(data) {
    var content_main = $("#content_main");
    $("#sidebar").toggleClass("sidebar", false);
    $("#sidebar").html("");
    data["name"] = fix_string(data["name"]);
    if (data["name"].slice(-1) != "s") {
        content_main.html("<div class='title'>" +
                          data["name"] +
                          "'s groups:</div><br>");
    } else {
        content_main.html("<div class='title'>" +
                          data["name"] +
                          "' groups:</div><br>");
    }
    
    for (index in data["result"]) {
        
        data["result"][index]["name"] = fix_string(data["result"][index]["name"]);
        content_main.append("<div class='link linkGroup' id=" +
                            data["result"][index]["ID"] +
                            ">" +
                            data["result"][index]["name"] +
                            "</div><br>");
    }
    $(".linkGroup").click(function() {
        getGroupData($(this).attr("id"));
    });
}

function createGroup() {
    var name = $("#groupNameBox").val();
    if (name.length == 0) {
        popup("You need to put in a name for your group!", 2000, true);
        return;
    }
    $("#groupNameBox").val("");

    $.getJSON(url + "/createGroup?name=" + name + "&usr=" + myID, function(json) {
        if (json["error"].length == 0) {
            getGroupData(json["ID"]);
        } else {
            popup(json["error"], 2000, true);
        }
    });
}

function del_cookie() {
    document.cookie = "IPSoc_sessauth=username=" + username + "; expires=" + "Thu, 01 Jan 1970 00:00:00 GMT" + "; path=/";
}

function fix_string(str) {
    return str.split(">").join("&#62;").split("<").join("&#60;");
}

function startWorker(file) {
    if (worker == undefined) {
        worker = new Worker(file);
    } else
        popup("Worker tried to switch while still active!", 2000, true);
}

function stopWorker() {
    if (worker != undefined)
        worker.terminate();
    worker = undefined;
}

function popup(text, dur, error) {
    var errorModal = $("#errorModal").on("shown.bs.modal", function() {
        clearTimeout(errorModal.data("hideInterval"));
        var id = setTimeout(function() {
            errorModal.modal("hide");
        }, dur);
        errorModal.data("hideInterval", id);
    });

    if (error) {
        $("#errorModalText").css("background-color",  "#CC6633");
    } else {
        $("#errorModalText").css("background-color", "#CCFFE5");
    }
    $("#errorModalText").html(text);
    errorModal.modal("show");
}
