var latest_date = undefined;
var url = undefined;
var id = undefined;
var sentOnce = undefined;

function getGroupData() {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState != 4 || xmlhttp.status != 200)
            return;
        var json = JSON.parse(xmlhttp.responseText);
        
        if (json["error"].length == 0) {
            var new_latest_date = get_latest_date(json["result"]["posts"]);
            if (json["result"]["posts"].length == 0 && !sentOnce) {
                postMessage(json);
                sentOnce = true;
            }
            else if (new_latest_date > latest_date) {
                latest_date = new_latest_date;
                postMessage(json);
            }
        } else {
            postMessage(json);
        }
        setTimeout("getGroupData()", 2000);
    }

    xmlhttp.open("GET", url + "/groupData?id=" + id, true);
    xmlhttp.send();
}

function get_latest_date(json) {
    var return_value = latest_date;
    for (i in json) {
        if (new Date(json[i].time) > return_value)
            return_value = new Date(json[i].time);
    }
    return return_value;
}

self.addEventListener("message", function(event) {
    latest_date = new Date("Thu, 01 Jan 1970 00:00:00 GMT");
    sentOnce = false;
    url = event.data.url;
    id = event.data.id;
    getGroupData();
});
