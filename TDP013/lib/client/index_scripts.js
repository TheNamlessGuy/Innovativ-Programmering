url = "http://localhost:3000";

function check_cookie() {
    if (document.cookie != "")
        window.open(url + "/home", "_self");
}

$(document).ready(function(){
    $("#login_btn").click(function() {
        register_login(false);
    });
    
    $("#register_btn").click(function() {
        register_login(true);
    });
});

function register_login(register) {
    var username = $("#username").val();
    var password = $("#password").val();
    $("#username").val("");
    $("#password").val("");

    if (username.length == 0 || password.length == 0) {
        popup("You have to fill in a username and a password!", 2000, true);
        return;
    }
    if (username.indexOf("=") > -1) {
        popup("Your username can't use the character '='!", 2000, true);
        return;
    }

    var go_to = url;
    if (register) {
        go_to = go_to + "/register";
    } else {
        go_to = go_to + "/login";
    }

    $.post(go_to,
           {
               usr: username,
               pw: password
           },
           function(json,status) {
               if (status != "success") {
                   popup("Something went wrong, try again!", 2000, true);
                   return;
               }
               if (json["error"].length == 0) {
                   document.cookie = "IPSoc_sessauth=username=" + username + ";;path=/"
                   window.open(url + "/home", "_self");
               } else {
                   if (register)
                       popup(json["error"], 2000, true);
                   else
                       popup(json["error"], 2000, true);
               }
           });
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
