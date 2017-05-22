var latest_date = undefined;
var url = undefined;
var username = undefined;

function getHomeData() {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState != 4 || xmlhttp.status != 200)
            return;
        var json = JSON.parse(xmlhttp.responseText);
        if (json["error"].length == 0) {
            var new_latest_date = get_latest_date(json["result"]);
            if (new_latest_date > latest_date) {
                latest_date = new_latest_date;
                postMessage(json);
            }
        } else {
            postMessage(json);
        }
        setTimeout("getHomeData()", 2000);
    }

    xmlhttp.open("GET", url + "/homeData?usr=" + username, true);
    xmlhttp.send();
}

function get_latest_date(json) {
    var return_value = latest_date;
    for (i in json) {
        for (j in json[i]["messages"]) {
            if (new Date(json[i]["messages"][j]["time"]) > return_value)
                return_value = new Date(json[i]["messages"][j]["time"]);
        }
    }
    return return_value;
}

self.addEventListener("message", function(event) {
    latest_date = new Date("Thu, 01 Jan 1970 00:00:00 GMT");
    url = event.data.url;
    username = event.data.username;
    getHomeData();
});
