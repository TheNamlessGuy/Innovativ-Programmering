var length = undefined;
var url = undefined;
var id = undefined;

function getGroupJoinData() {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState != 4 || xmlhttp.status != 200)
            return;
        var json = JSON.parse(xmlhttp.responseText);
        
        if (json["error"].length == 0) {
            if (json["result"].length > length) {
                length = json["result"].length;
                postMessage(json);
            }
        } else {
            postMessage(json);
        }
        setTimeout("getGroupJoinData()", 2000);
    }

    xmlhttp.open("GET", url + "/groupReqs?id=" + id, true);
    xmlhttp.send();
}

self.addEventListener("message", function(event) {
    length = -1;
    url = event.data.url;
    id = event.data.id;
    getGroupJoinData();
});
