var length = undefined;
var url = undefined;
var id = undefined;
var sentOnce = undefined;

function getUserData() {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState != 4 || xmlhttp.status != 200)
            return;
        var json = JSON.parse(xmlhttp.responseText);
        
        if (json["error"].length == 0) {
            if (json["result"]["result"].length > length) {
                length = json["result"]["result"].length;
                postMessage(json);
            } else if (json["result"]["result"].length == 0 && !sentOnce) {
                postMessage(json);
                sentOnce = true;
            }
        } else {
            postMessage(json);
        }
        setTimeout("getUserData()", 2000);
    }

    xmlhttp.open("GET", url + "/userData?id=" + id, true);
    xmlhttp.send();
}

self.addEventListener("message", function(event) {
    length = 0;
    sentOnce = false;
    url = event.data.url;
    id = event.data.id;
    getUserData();
});
