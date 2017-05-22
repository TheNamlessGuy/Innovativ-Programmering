messages = new Array();

function Message(text) {
    this.text = text;
    this.read = false;
}

Message.prototype.toggleCheck = function() {
    this.read = !this.read;
};

function buttonClick() {
    var textinput = document.getElementById('textinput');
    if (textinput.value.length > 0 && textinput.value.length <= 140) {
        document.getElementById('error').innerHTML = "";
        var m = new Message(textinput.value);
        messages.push(m);
        
        updateSite();
    } else {
        document.getElementById('error').innerHTML = "Error: Message is either too short or too long!";
    }
    textinput.value = "";
};

function checkboxChecked(element) {
    messages[element.name].toggleCheck();
    element.checked = messages[element.name].read;
    updateSite();
}

function updateSite() {
    var posts = document.getElementById('posts');
    posts.innerHTML = "";

    for (var i = 0; i < messages.length; i++) {
        var toPrint = "";
        if (messages[i].read) {
            toPrint += '<p class="read">';
        } else {
            toPrint += '<p class="unread">';
        }

        toPrint += '<input type="checkbox" name="' + i +
            '" onchange="checkboxChecked(this)" class="messageCheckbox"';
        if (messages[i].read) {
            toPrint += "checked";
        }
        toPrint += ">" + messages[i].text + "</p>";
        posts.innerHTML = toPrint + posts.innerHTML;
    }
}
