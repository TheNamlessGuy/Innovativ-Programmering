var assert = require('assert');
var request = require('request');

var url = "http://localhost:3000";

var Server = require('../lib/server/Express.js');

function emptyServer(done) {
    request.get(url + "/DEL", function(err, res, body) {
        done();
    });
}

function normalTests(err, res, body, equal) {
    if (equal == undefined) { equal = true; }
    assert.equal(err, null);
    assert.equal(res.statusCode, 200);

    var success = false;
    try {
        body = JSON.parse(body);
        success = true;
    } catch(err) {}

    assert.equal(success, true);
    if (equal)
        assert.equal(body["error"], "");
    else
        assert.notEqual(body["error"], "");
    
    return body;
}

before(function(done) {
    runningServer = new Server();
    runningServer.start();
    done();
});

after(function(done) {
    runningServer.stop();
    emptyServer(done);
});
describe('Tests', function() {

    describe('/', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and a HTML file', function(done) {
            request.get(url, function(err, res, body) {
                assert.equal(err, null);
                assert.equal(body.split("\n")[0], "<!DOCTYPE html>");
                assert.equal(res.statusCode, 200);
                done();
            });
        });
    });

    describe('/register', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON containing empty error on unregistered call', function(done) {
            request.post({url: url + "/register", form: {usr: "registerTest1", pw: "registerTest1"}}, function(err, res, body) {
                normalTests(err, res, body);
                done();
            });
        });
        it('returns 200 and JSON containing error message on registered call', function(done) {
            request.post({url: url + "/register", form: {usr: "registerTest2", pw: "registerTest2"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.post({url: url + "/register", form: {usr: "registerTest2", pw: "registerTest2"}}, function(err, res, body) {
                    normalTests(err, res, body, false);
                    done();
                });
            });
        });
    });

    describe('/login', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON containing error message on unregistered login', function(done) {
            request.post({url: url + "/login", form: {usr: "loginTest1", pw: "loginTest1"}}, function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON containing empty error on registered login', function(done) {
            request.post({url: url + "/register", form: {usr: "loginTest2", pw: "loginTest2"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.post({url: url + "/login", form: {usr: "loginTest2", pw: "loginTest2"}}, function(err, res, body) {
                    normalTests(err, res, body);                    
                    done();
                });
            });
        });
    });
    
    describe('/home', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and a HTML file', function(done) {
            request.get(url + "/home", function(err, res, body) {
                assert.equal(err, null);
                assert.equal(body.split("\n")[0], "<!DOCTYPE html>");
                assert.equal(res.statusCode, 200);
                done();
            });
        });
    });

    describe('/homeData', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON containing error message if username does not exist', function(done) {
            request.get(url + "/homeData?usr=AAA", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON containing empty error if username exists', function(done) {
            request.post({url: url + "/register", form: {usr: "homeData", pw: "homeData"}}, function(err, res, body) {
                normalTests(err, res, body);

                request.get(url + '/homeData?usr=homeData', function(err, res, body) {
                    normalTests(err, res, body);
                    done();
                });
            });
        });
        it('returns 200 and JSON containing empty error if username exists and user has joined one or more groups', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);

                request.get(url + "/userIDof?usr=a", function(err, res, body) {
                    body = normalTests(err, res, body);

                    var userID = body["result"];
                    request.get(url + "/createGroup?name=gruppisar&usr=" + userID, function(err, res, body) {
                        normalTests(err, res, body);

                        request.get(url + "/homeData?usr=a", function(err, res, body) {
                            normalTests(err, res, body);
                            done();
                        });
                    });
                });
            });
        });
    });
    
    describe('/createGroup', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON containing error message on false input data', function(done) {
            request.get(url + "/createGroup?name=a&usr=000000000000000000000000", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON containing no error message on real input', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);

                request.get(url + "/userIDof?usr=a", function(err, res, body) {
                    body = normalTests(err, res, body);

                    var userID = body["result"];

                    request.get(url + "/createGroup?name=gruppis&usr=" + userID, function(err, res, body) {
                        normalTests(err, res, body);
                        done();
                    });
                });
            });
        });
    });

    describe('/groupData', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON containing error message if ID does not exist', function(done) {
            request.get(url + "/groupData", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON containing empty error message if ID exists', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);

                request.get(url + "/userIDof?usr=a", function(err, res, body) {
                    body = normalTests(err, res, body);
                    
                    var userID = body["result"];
                    request.get(url + "/createGroup?name=hej&usr=" + userID, function(err, res, body) {
                        body = normalTests(err, res, body);
                        
                        var groupID = body["ID"];
                        request.get(url + "/groupData?id=" + groupID, function(err, res, body) {
                            normalTests(err, res, body);
                            done();
                        });
                    });
                });
            });
        });
    });
    
    describe('/userIDof', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON containing error message on false data', function(done) {
            request.get(url + "/userIDof?usr=000000000000000000000000", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON containing no error message on true data', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.get(url + "/userIDof?usr=a", function(err, res, body) {
                    normalTests(err, res, body);
                    done();
                });
            });
        });
    });
    
    describe('/userData', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON with error message on false data', function(done) {
            request.get(url + "/userData?id=000000000000000000000000", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON with no error message on real data', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.get(url + "/userIDof?usr=a", function(err, res, body) {
                    body = normalTests(err, res, body);
                    
                    var userID = body["result"];
                    request.get(url + "/createGroup?name=grupp&usr=" + userID, function(err, res, body) {
                        body = normalTests(err, res, body);

                        var groupID = body["ID"];
                        request.get(url + "/postMessage?msg=hej&group=" + groupID, function(err, res, body) {
                            normalTests(err, res, body);

                            request.get(url + "/userData?id=" + userID, function(err, res, body) {
                                normalTests(err, res, body);
                                done();
                            });
                        });
                    });
                });
            });
        });
    });
    
    describe('/search', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON if search for something that does not exist', function(done) {
            request.get(url + "/search?term=hej", function(err, res, body) {
                normalTests(err, res, body);
                done();
            });
        });
        it('returns 200 and correct search data in JSON format', function(done) {
            request.post({url: url + "/register", form: {usr: "myusr", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.get(url + "/userIDof?usr=myusr", function(err, res, body) {
                    body = normalTests(err, res, body);

                    var userID = body["result"];
                    request.get(url + "/createGroup?name=mygroup&usr=" + userID, function(err, res, body) {
                        normalTests(err, res, body);

                        request.get(url + "/search?term=group", function(err, res, body) {
                            body = normalTests(err, res, body);
                            
                            assert.equal(body["result"]["groups"].length, 1);
                            assert.equal(body["result"]["users"].length, 0);

                            request.get(url + "/search?term=myusr", function(err, res, body) {
                                body = normalTests(err, res, body);

                                assert.equal(body["result"]["groups"].length, 0);
                                assert.equal(body["result"]["users"].length, 1);
                                done();
                            });
                        });
                    });
                });
            });
        });
    });

    describe('/groupReqs', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON with error if false data', function(done) {
            request.get(url + "/groupReqs?id=123456789012345678901234", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON with no error and empty result if user exists but has no requests', function(done) {
            request.post({url: url + "/register", form: {usr: "groupReqs", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.get(url + "/userIDof?usr=groupReqs", function(err, res, body) {
                    body = normalTests(err, res, body);
                    
                    var userID = body["result"];
                    request.get(url + "/groupReqs?id=" + userID, function(err, res, body) {
                        body = normalTests(err, res, body);
                        
                        assert.equal(body["result"].length, 0);
                        done();
                    });
                });
            });
        });
        it('returns 200 and JSON with no error and a result if user exists and has at least one request', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.get(url + "/userIDof?usr=a", function(err, res, body) {
                    body = normalTests(err, res, body);
                    
                    var userID = body["result"];
                    request.get(url + "/createGroup?name=hej&usr=" + userID, function(err, res, body) {
                        body = normalTests(err, res, body);
                        
                        var groupID = body["ID"];
                        request.post({url: url + "/register", form: {usr: "b", pw: "b"}}, function(err, res, body) {
                            normalTests(err, res, body);

                            request.get(url + "/userIDof?usr=b", function(err, res, body) {
                                body = normalTests(err, res, body);
                                
                                var secondUserID = body["result"];
                                request.get(url + "/joinGroup?id=" + groupID + "&usr=" + secondUserID, function(err, res, body) {
                                    normalTests(err, res, body);
                                    
                                    request.get(url + "/groupReqs?id=" + userID, function(err, res, body) {
                                        body = normalTests(err, res, body);
                                        
                                        assert.equal(body["result"].length, 1);
                                        done();
                                    });
                                });
                            });
                        });
                    });
                });
            });
        });
    });

    describe('/groupReqResponse', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON with error at false data when declining request', function(done) {
            request.get(url + "/groupReqResponse?gid=000000000000000000000000&uid=000000000000000000000000&dec=false", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON with error at false data when acccepting request', function(done) {
            request.get(url + "/groupReqResponse?gid=000000000000000000000000&uid=000000000000000000000000&dec=true", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON with no error when accepting a real request', function(done) {
            request.post({url: url + "/register", form: {usr: "admin", pw: "admin"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.post({url: url + "/register", form: {usr: "user", pw: "user"}}, function(err, res, body) {
                    normalTests(err, res, body);
                    
                    request.get(url + "/userIDof?usr=admin", function(err, res, body) {
                        body = normalTests(err, res, body);
                        
                        var adminID = body["result"];
                        request.get(url + "/userIDof?usr=user", function(err, res, body) {
                            body = normalTests(err, res, body);
                            
                            var userID = body["result"];
                            request.get(url + "/createGroup?name=grupp&usr=" + adminID, function(err, res, body) {
                                body = normalTests(err, res, body);
                                
                                var groupID = body["ID"];
                                request.get(url + "/joinGroup?id=" + groupID + "&usr=" + userID, function(err, res, body) {
                                    normalTests(err, res, body);

                                    request.get(url + "/groupReqResponse?uid=" + userID + "&gid=" + groupID + "&dec=true", function(err, res, body) {
                                        normalTests(err, res, body);
                                        done();
                                    });
                                });
                            });
                        });
                    });
                });
            });
        });
        it('returns 200 and JSON with no error when declining a real request', function(done) {
            request.post({url: url + "/register", form: {usr: "admin", pw: "admin"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.post({url: url + "/register", form: {usr: "user", pw: "user"}}, function(err, res, body) {
                    normalTests(err, res, body);
                    
                    request.get(url + "/userIDof?usr=admin", function(err, res, body) {
                        body = normalTests(err, res, body);
                        
                        var adminID = body["result"];
                        request.get(url + "/userIDof?usr=user", function(err, res, body) {
                            body = normalTests(err, res, body);
                            
                            var userID = body["result"];
                            request.get(url + "/createGroup?name=grupp&usr=" + adminID, function(err, res, body) {
                                body = normalTests(err, res, body);
                                
                                var groupID = body["ID"];
                                request.get(url + "/joinGroup?id=" + groupID + "&usr=" + userID, function(err, res, body) {
                                    normalTests(err, res, body);

                                    request.get(url + "/groupReqResponse?uid=" + userID + "&gid=" + groupID + "&dec=false", function(err, res, body) {
                                        normalTests(err, res, body);
                                        done();
                                    });
                                });
                            });
                        });
                    });
                });
            });
        });
    });

    describe('/joinGroup', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON containing error on false data', function(done) {
            request.get(url + "/joinGroup?id=000000000000000000000000&usr=000000000000000000000000", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON with no error if group and user exists', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.get(url + "/userIDof?usr=a", function(err, res, body) {
                    body = normalTests(err, res, body);
                    
                    var adminID = body["result"];
                    request.get(url + "/createGroup?name=gruppis&usr=" + adminID, function(err, res, body) {
                        body = normalTests(err, res, body);
                        
                        var groupID = body["ID"];
                        request.post({url: url + "/register", form: {usr: "b", pw: "b"}}, function(err, res, body) {
                            normalTests(err, res, body);
                            
                            request.get(url + "/userIDof?usr=b", function(err, res, body) {
                                body = normalTests(err, res, body);
                                
                                var userID = body["result"];
                                request.get(url + "/joinGroup?id=" + groupID + "&usr=" + userID, function(err, res, body) {
                                    normalTests(err, res, body);
                                    done();
                                });
                            });
                        });
                    });
                });
            });
        });
    });

    describe('/leaveGroup', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON with error on false data', function(done) {
            request.get(url + "/leaveGroup?uid=000000000000000000000000&gid=000000000000000000000000", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON with no error if regular member leaves', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.post({url: url + "/register", form: {usr: "b", pw: "b"}}, function(err, res, body) {
                    normalTests(err, res, body);

                    request.get(url + "/userIDof?usr=a", function(err, res, body) {
                        body = normalTests(err, res, body);
                        
                        var adminID = body["result"];
                        request.get(url + "/userIDof?usr=b", function(err, res, body) {
                            body = normalTests(err, res, body);
                            
                            var userID = body["result"];
                            request.get(url + "/createGroup?name=grupp&usr=" + adminID, function(err, res, body) {
                                body = normalTests(err, res, body);
                                
                                var groupID = body["ID"];
                                request.get(url + "/joinGroup?id=" + groupID + "&usr=" + userID, function(err, res, body) {
                                    normalTests(err, res, body);

                                    request.get(url + "/groupReqResponse?gid=" + groupID + "&uid=" + userID + "&dec=true", function(err, res, body) {
                                        normalTests(err, res, body);
                                        
                                        request.get(url + "/leaveGroup?uid=" + userID + "&gid=" + groupID, function(err, res, body) {
                                            normalTests(err, res, body);
                                            done();
                                        });
                                    });
                                });
                            });
                        });
                    });
                });
            });
        });
        it('returns 200 and JSON with no error if admin leaves', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);
                request.get(url + "/userIDof?usr=a", function(err, res, body) {
                    body = normalTests(err, res, body);
                    
                    var adminID = body["result"];
                    request.get(url + "/createGroup?name=grupp&usr=" + adminID, function(err, res, body) {
                        body = normalTests(err, res, body);
                        
                        var groupID = body["ID"];
                        request.get(url + "/leaveGroup?uid=" + adminID + "&gid=" + groupID, function(err, res, body) {
                            normalTests(err, res, body);
                            done();
                        });
                    });
                });
            });
        });
    });

    describe('/postMessage', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON with error on false data', function(done) {
            request.get(url + "/postMessage?group=000000000000000000000000&msg=hej", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON with no error on correct data', function(done) {
            request.post({url: url + "/register", form: {usr: "a", pw: "a"}}, function(err, res, body) {
                normalTests(err, res, body);
                
                request.get(url + "/userIDof?usr=a", function(err, res, body) {
                    body = normalTests(err, res, body);
                    
                    var adminID = body["result"];
                    request.get(url + "/createGroup?name=gruppis&usr=" + adminID, function(err, res, body) {
                        body = normalTests(err, res, body);
                        
                        var groupID = body["ID"];
                        request.get(url + "/postMessage?msg=hej&group=" + groupID, function(err, res, body) {
                            normalTests(err, res, body);
                            done();
                        });
                    });
                });
            });
        });
    });
});
