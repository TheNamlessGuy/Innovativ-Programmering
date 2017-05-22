var assert = require('assert');
var request = require('request');

var url = "http://localhost:3000";

var Server = require('../lib/server/Express.js');

function emptyServer(done) {
    request.get(url + "/DEL", function(err, res, body) {
        done();
    });
}

before(function(done) {
    runningServer = new Server();
    runningServer.start();
    done();
});

after(function(done) {
    runningServer.stop();
    done();
});
describe('Tests', function() {

    describe('/', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and a HTML file', function(done) {
            request.get(url + "/", function(err, res, body) {
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
        it('returns 200 and true on unregistered call', function(done) {
            request.post(url + "/register", {usr: "registerTest1", pw: "registerTest1"}, function(err, res, body) {
                assert.equal(err, null);
                assert.equal(res.statusCode, 200);
                assert.equal(body, "true");
                done();
            });
        });
        it('returns 200 and false on registered call', function(done) {
            request.post(url + "/register", {usr: "registerTest2", pw: "registerTest2"}, function(err, res, body) {
                assert.equal(err, null);
                assert.equal(res.statusCode, 200);
                assert.equal(body, "true");
                
                request.post(url + "/register", {usr: "registerTest2", pw: "registerTest2"}, function(err, res, body) {
                    assert.equal(err, null);
                    assert.equal(res.statusCode, 200);
                    assert.equal(body, "false");
                    done();
                });
            });
        });
    });

    describe('/login', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and false on unregistered login', function(done) {
            request.post(url + "/login", {usr: "loginTest1", pw: "loginTest1"}, function(err, res, body) {
                assert.equal(err, null);
                assert.equal(res.statusCode, 200);
                assert.equal(body, "false");
                done();
            });
        });
        it('returns 200 and true on registered login', function(done) {
            request.post(url + "/register", {usr: "loginTest2", pw: "loginTest2"}, function(err, res, body) {
                assert.equal(err, null);
                assert.equal(res.statusCode, 200);
                assert.equal(body, "true");
                
                request.post(url + "/login", {usr: "loginTest2", pw: "loginTest2"}, function(err, res, body) {
                    assert.equal(err, null);
                    assert.equal(res.statusCode, 200);
                    assert.equal(body, "true");
                    
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
        it('returns 200 and empty array if username does not exist', function(done) {
            request.get(url + "/homeData?usr=AAA", function(err, res, body) {
                assert.equal(err, null);
                assert.equal(res.statusCode, 200);
                assert.equal(body, "[]");
                done();
            });
        });
        it('returns 200 and filled JSON if username exists', function(done) {
            request.post(url + "/register", {usr: "homeData", pw: "homeData"}, function(err, res, body) {
                assert.equal(err, null);
                assert.equal(body, "true");
                assert.equal(res.statusCode, 200);

                request.get(url + "/homeData?usr=homeData", function(err, res, body) {
                    assert.equal(err, null);
                    assert.equal(res.statusCode, 200);

                    var success = false;
                    try {
                        JSON.parse(body);
                        success = true;
                    } catch(err) {}
                    
                    assert.equal(success, true);
                    done();
                });
            });
        });
    });
    
    describe('/createGroup', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON containing status false on false input data', function(done) {
            
        });
    });

    describe('/groupData', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and empty array if ID does not exist', function(done) {
            request.get(url + "/groupData", function(err, res, body) {
                assert.equal(err, null);
                assert.equal(res.statusCode, 200);
                assert.equal(body, "[]");
            });
        });
        it('returns 200 and a JSON if ID exists', function(done) {
            
        });
    });
    











//HEJHEJ NYA HÄR (fungerar felfritt)

    describe('*', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 404 if page is not found', function(done) {
            request.get(url + "/a", function(err, res, body) {
                assert.equal(err, null);
                assert.equal(res.statusCode, 404); //va 17 e body
                done();
            });
        });
    });

    describe('/postMessage', function() {
        beforeEach(function(done) {
            emptyServer(done);
        });
        it('returns 200 and JSON containing error message when posting message in non-existent group', function(done) {
            request.get(url + "/postMessage?group=000000000000000000000000", function(err, res, body) {
                normalTests(err, res, body, false);
                done();
            });
        });
        it('returns 200 and JSON containing empty error message if when posting message to group', function(done) {
            request.post({url: url + "/register", form: {usr: "registerTest1", pw: "registerTest1"}}, function(err, res, body) {
                normalTests(err, res, body);

                request.get(url + "/userIDof", function(err, res, body) {
                    body = normalTests(err, res, body);
                    var userID = body["result"];

                    request.get(url + "/createGroup?name=a&usr=" + userID, function(err, res, body) {
                        body = normalTests(err, res, body);
                        var groupID = body["ID"];

                        request.get(url + "/postMessage?group=" + groupID + "&msg=hej", function(err, res, body) {
                            normalTests(err, res, body);
                            done();
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
        it('returns 200 and JSON containing error message when failing to find group', function(done) {
            request.get(url + "/leaveGroup?gid=000000000000000000000000", function(err, res, body) {
                normalTests(err, res, body, false);
                done()
            });
        });
        it('returns 200 and JSON containing error message when failing to find user', function(done) {
            request.post({url: url + "/register", form: {usr: "registerTest", pw: "RegisterTest"}}, function(err, res, body) {
                normalTests(err, res, body);

                request.get(url + "/userIDof", function(err, res, body) {
                    body = normalTests(err, res, body);
                    var userID = body["result"];

            });
        });
    });

});
