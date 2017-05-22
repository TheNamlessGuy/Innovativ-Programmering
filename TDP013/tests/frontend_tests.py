from selenium import webdriver
import unittest
import time

url = "http://localhost:3000"

def emptyServer():
    browser = webdriver.Firefox()
    browser.get(url + "/DEL")
    time.sleep(1)
    browser.quit()

def register(browser, username, password):
    browser.find_element_by_id('username').send_keys(username)
    browser.find_element_by_id('password').send_keys(password)
    browser.find_element_by_id('register_btn').click()
    time.sleep(1)

def login(browser, username, password):
    browser.find_element_by_id('username').send_keys(username)
    browser.find_element_by_id('password').send_keys(password)
    browser.find_element_by_id('login_btn').click()
    time.sleep(1)

def logout(browser):
    browser.find_element_by_id('dropdownMenu').click()
    browser.find_element_by_id('logout').click()
    time.sleep(1)

def createGroup(browser, groupName):
    browser.find_element_by_id('dropdownMenu').click()
    browser.find_element_by_id('createGroupLink').click()
    time.sleep(1)
    browser.find_element_by_id('groupNameBox').send_keys(groupName)
    browser.find_element_by_id('groupName_btn').click()
    time.sleep(1)

def postGroupMessage(browser, message):
    browser.find_element_by_id('msg_box').send_keys(message)
    browser.find_element_by_id('msg_btn').click()
    time.sleep(1)

def gotoYourPage(browser):
    browser.find_element_by_id('yourLink').click()
    time.sleep(1)

def gotoHomePage(browser):
    browser.find_element_by_id('homepageLink').click()
    time.sleep(1)

def gotoGroupPage(browser, groupName):
    search(browser, groupName)
    browser.find_element_by_class_name('search-results').click()
    time.sleep(1)

def gotoGroupRequestsPage(browser):
    browser.find_element_by_id('dropdownMenu').click()
    browser.find_element_by_id('groupJoinLink').click()
    time.sleep(1)

def search(browser, searchTerm):
    browser.find_element_by_id('search_box').send_keys(searchTerm)
    browser.find_element_by_id('search_btn').click()
    time.sleep(1)

def joinLeaveGroup(browser, groupName):
    gotoGroupPage(browser, groupName)
    browser.find_element_by_id('group_btn').click()
    time.sleep(1)

def joinGroup(browser, groupName):
    joinLeaveGroup(browser, groupName)

def leaveGroup(browser, groupName):
    joinLeaveGroup(browser, groupName)

def getErrorText(browser):
    return browser.find_element_by_id('errorModalText').text

def acceptGroupJoinRequest(browser):
    browser.find_element_by_class_name('acceptGroup_btn').click()
    time.sleep(1)

def declineGroupJoinRequest(browser):
    browser.find_element_by_class_name('declineGroup_btn').click()
    time.sleep(1)

def joinChat(browser):
    browser.find_element_by_id('chat_btn').click()
    time.sleep(1)

def postChatMessage(browser, message):
    browser.find_element_by_id('chat_type_text').send_keys(message)
    browser.find_element_by_id('chat_type_btn').click()
    time.sleep(1)

class TestRegister(unittest.TestCase):
    def test_regNoInput(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "", "")
        self.assertEqual(getErrorText(browser),
                         "You have to fill in a username and a password!")
        browser.quit()

    def test_regNoUsername(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "", "password")
        self.assertEqual(getErrorText(browser),
                         "You have to fill in a username and a password!")
        browser.quit()

    def test_regNoPassword(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "")
        self.assertEqual(getErrorText(browser),
                         "You have to fill in a username and a password!")
        browser.quit()
    
    def test_regUsernameAndPassword(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url,
                         url + "/home")
        browser.quit()

class TestLogin(unittest.TestCase):
    def test_logNoInput(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        login(browser, "", "")
        self.assertEqual(getErrorText(browser),
                         "You have to fill in a username and a password!")
        browser.quit()

    def test_logNoUsername(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        login(browser, "", "password")
        self.assertEqual(getErrorText(browser),
                         "You have to fill in a username and a password!")
        browser.quit()

    def test_logNoPassword(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        login(browser, "username", "")
        self.assertEqual(getErrorText(browser),
                         "You have to fill in a username and a password!")
        browser.quit()
    
    def test_logUsernameAndPassword(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        login(browser, "username", "password")
        self.assertEqual(getErrorText(browser),
                         "That username/password combination could not be found. Please try refreshing the site")
        browser.quit()

    def test_logRegisterAndLogin(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url,
                         url + "/home")
        logout(browser)
        login(browser, "username", "password")
        self.assertEqual(browser.current_url,
                         url + "/home")
        browser.quit()

class TestHome(unittest.TestCase):
    def test_homeShowsUsername(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url,
                         url + "/home")
        self.assertEqual(browser.find_element_by_id("yourLink").text,
                         "Hello, username!")
        browser.quit()

    def test_homeLogout(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        logout(browser)
        self.assertEqual(browser.current_url, url + "/")
        browser.quit()
        
    def test_homeCreateGroupPostMessageSeeMessageHomePage(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        createGroup(browser, "Gruppis")
        self.assertEqual(browser.find_element_by_id('curGroupName').text,
                         "Gruppis")
        self.assertEqual(browser.find_element_by_id('group_btn').text,
                         "Leave Group")
        postGroupMessage(browser, "Post1")
        postGroupMessage(browser, "Post2")
        self.assertEqual(browser.find_elements_by_class_name('homePost')[0].text,
                         "Gruppis\n\nPost2")
        self.assertEqual(browser.find_elements_by_class_name('homePost')[1].text,
                         "Gruppis\n\nPost1")
        gotoHomePage(browser)
        self.assertEqual(browser.find_elements_by_class_name('homePost')[0].text,
                         "Gruppis\nPost2")
        self.assertEqual(browser.find_elements_by_class_name('homePost')[1].text,
                         "Gruppis\nPost1")
        browser.quit()
    
    def test_homeMultiGroupMessagesCorrectOrder(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        createGroup(browser, "Grupp1")
        self.assertEqual(browser.find_element_by_id('curGroupName').text,
                         "Grupp1")
        postGroupMessage(browser, "Post1")
        createGroup(browser, "Grupp2")
        self.assertEqual(browser.find_element_by_id('curGroupName').text,
                         "Grupp2")
        postGroupMessage(browser, "Post2")
        gotoGroupPage(browser, "Grupp1")
        postGroupMessage(browser, "Post3")
        gotoHomePage(browser)
        elements = browser.find_elements_by_class_name('homePost')
        self.assertEqual(elements[0].text, "Grupp1\nPost3")
        self.assertEqual(elements[1].text, "Grupp2\nPost2")
        self.assertEqual(elements[2].text, "Grupp1\nPost1")
        browser.quit()

    def test_homeCreateGroupNoName(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        createGroup(browser, "")
        self.assertEqual(getErrorText(browser),
                        "You need to put in a name for your group!")
        browser.quit()

    def test_homePostEmptyMessageInGroup(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        createGroup(browser, "Gruppis")
        postGroupMessage(browser, "")
        self.assertEqual(getErrorText(browser),
                        "You have to write a message to post!")
        browser.quit()

    def test_homeLeaveGroupNotAdmin(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        createGroup(browser, "Gruppis")
        logout(browser)
        register(browser, "username2", "password")
        self.assertEqual(browser.current_url, url + "/home")
        joinGroup(browser, "Gruppis")
        self.assertEqual(getErrorText(browser),
                         "The group admin has gotten your request")
        logout(browser)
        login(browser, "username", "password")
        gotoGroupRequestsPage(browser)
        acceptGroupJoinRequest(browser)
        logout(browser)
        login(browser, "username2", "password")
        leaveGroup(browser, "Gruppis")
        gotoYourPage(browser)
        self.assertEqual(len(browser.find_elements_by_class_name("linkGroup")),
                         0)
        search(browser, "Gruppis")
        self.assertEqual(len(browser.find_elements_by_class_name('search-results')),
                         1)
        browser.quit()

    def test_homeDeclineGroupJoinRequest(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        createGroup(browser, "Gruppis")
        logout(browser)
        register(browser, "username2", "password")
        self.assertEqual(browser.current_url, url + "/home")
        joinGroup(browser, "Gruppis")
        self.assertEqual(browser.find_element_by_id('errorModalText').text,
                         "The group admin has gotten your request")
        logout(browser)
        login(browser, "username", "password")
        gotoGroupRequestsPage(browser)
        declineGroupJoinRequest(browser)
        logout(browser)
        login(browser, "username2", "password")
        gotoGroupPage(browser, "Gruppis")
        self.assertEqual(browser.find_element_by_id('group_btn').text,
                         "Join Group")
        browser.quit()

    def test_homeAdminLeaveGroup(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        createGroup(browser, "Gruppis")
        leaveGroup(browser, "Gruppis")
        gotoYourPage(browser)
        self.assertEqual(len(browser.find_elements_by_class_name("linkGroup")),
                         0)
        search(browser, "Gruppis")
        self.assertEqual(len(browser.find_elements_by_class_name('search-results')),
                         0)
        browser.quit()
    
    def test_homeSearchForUser(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        search(browser, "username")
        self.assertEqual(len(browser.find_elements_by_class_name('search-results')),
                         1)
        browser.quit()
    def test_homeUserPageShowsCurrentGroups(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        createGroup(browser, "Grupp1")
        gotoYourPage(browser)
        self.assertEqual(len(browser.find_elements_by_class_name('linkGroup')),
                         1)
        self.assertEqual(browser.find_element_by_class_name('linkGroup').text,
                         "Grupp1")
        createGroup(browser, "Grupp2")
        gotoYourPage(browser)
        self.assertEqual(len(browser.find_elements_by_class_name('linkGroup')),
                         2)
        self.assertEqual(browser.find_elements_by_class_name('linkGroup')[0].text,
                         "Grupp1")
        self.assertEqual(browser.find_elements_by_class_name('linkGroup')[1].text,
                         "Grupp2")
        leaveGroup(browser, "Grupp2")
        gotoYourPage(browser)
        self.assertEqual(len(browser.find_elements_by_class_name('linkGroup')),
                         1)
        self.assertEqual(browser.find_element_by_class_name('linkGroup').text,
                         "Grupp1")
        browser.quit()

class TestChat(unittest.TestCase):
    def test_chatCantJoinWhenNotInGroup(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        createGroup(browser, "Gruppis")
        logout(browser)
        register(browser, "username2", "password")
        gotoGroupPage(browser, "Gruppis")
        self.assertEqual(browser.find_element_by_id('chat_btn').is_displayed(),
                         False)
        browser.quit()

    def test_chatCanJoinChatAndUseItWhenInGroup(self):
        emptyServer()
        browser = webdriver.Firefox()
        browser.get(url)
        register(browser, "username", "password")
        self.assertEqual(browser.current_url, url + "/home")
        createGroup(browser, "Gruppis")
        joinChat(browser)
        postChatMessage(browser, "ChatMsg1")
        chatMessage = browser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, username\nusername: ChatMsg1")
        browser.quit()

    def test_chatThreePeopleInSameChatCanReadEachothersMessages(self):
        emptyServer()
        
        userOneBrowser = webdriver.Firefox()
        userOneBrowser.get(url)
        register(userOneBrowser, "userOne", "password")
        self.assertEqual(userOneBrowser.current_url, url + "/home")
        createGroup(userOneBrowser, "Gruppis")
        
        userTwoBrowser = webdriver.Firefox()
        userTwoBrowser.get(url)
        register(userTwoBrowser, "userTwo", "password")
        self.assertEqual(userTwoBrowser.current_url, url + "/home")
        joinGroup(userTwoBrowser, "Gruppis")
        
        userThreeBrowser = webdriver.Firefox()
        userThreeBrowser.get(url)
        register(userThreeBrowser, "userThree", "password")
        self.assertEqual(userThreeBrowser.current_url, url + "/home")
        joinGroup(userThreeBrowser, "Gruppis")

        gotoGroupRequestsPage(userOneBrowser)
        acceptGroupJoinRequest(userOneBrowser)
        acceptGroupJoinRequest(userOneBrowser)
        gotoGroupPage(userOneBrowser, "Gruppis")
        joinChat(userOneBrowser)

        gotoGroupPage(userTwoBrowser, "Gruppis")
        joinChat(userTwoBrowser)
        gotoGroupPage(userThreeBrowser, "Gruppis")
        joinChat(userThreeBrowser)

        postChatMessage(userOneBrowser, "ChatMsg1")
        chatMessage = userOneBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userOne\nWelcome, userTwo\nWelcome, userThree\nuserOne: ChatMsg1")
        chatMessage = userTwoBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userTwo\nWelcome, userThree\nuserOne: ChatMsg1")
        chatMessage = userThreeBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userThree\nuserOne: ChatMsg1")

        postChatMessage(userTwoBrowser, "ChatMsg2")
        chatMessage = userOneBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userOne\nWelcome, userTwo\nWelcome, userThree\nuserOne: ChatMsg1\nuserTwo: ChatMsg2")
        chatMessage = userTwoBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userTwo\nWelcome, userThree\nuserOne: ChatMsg1\nuserTwo: ChatMsg2")
        chatMessage = userThreeBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userThree\nuserOne: ChatMsg1\nuserTwo: ChatMsg2")
        
        postChatMessage(userThreeBrowser, "ChatMsg3")
        chatMessage = userOneBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userOne\nWelcome, userTwo\nWelcome, userThree\nuserOne: ChatMsg1\nuserTwo: ChatMsg2\nuserThree: ChatMsg3")
        chatMessage = userTwoBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userTwo\nWelcome, userThree\nuserOne: ChatMsg1\nuserTwo: ChatMsg2\nuserThree: ChatMsg3")
        chatMessage = userThreeBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userThree\nuserOne: ChatMsg1\nuserTwo: ChatMsg2\nuserThree: ChatMsg3")

        userOneBrowser.quit()
        userTwoBrowser.quit()
        userThreeBrowser.quit()

    def test_chatTwoPeopleInDifferentGroupChatsCantSeeEachothersMessages(self):
        emptyServer()
        
        userOneBrowser = webdriver.Firefox()
        userOneBrowser.get(url)
        register(userOneBrowser, "userOne", "password")
        self.assertEqual(userOneBrowser.current_url, url + "/home")
        createGroup(userOneBrowser, "Grupp1")
        joinChat(userOneBrowser)
        
        userTwoBrowser = webdriver.Firefox()
        userTwoBrowser.get(url)
        register(userTwoBrowser, "userTwo", "password")
        self.assertEqual(userTwoBrowser.current_url, url + "/home")
        createGroup(userTwoBrowser, "Grupp2")
        joinChat(userTwoBrowser)

        postChatMessage(userOneBrowser, "Grupp1Message")
        chatMessage = userOneBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userOne\nuserOne: Grupp1Message")
        chatMessage = userTwoBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userTwo")
        
        postChatMessage(userTwoBrowser, "Grupp2Message")
        chatMessage = userOneBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userOne\nuserOne: Grupp1Message")
        chatMessage = userTwoBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userTwo\nuserTwo: Grupp2Message")

        userOneBrowser.quit()
        userTwoBrowser.quit()

    def test_chatAdminCanPostSpecialMessagesButOthersCant(self):
        emptyServer()
        
        userOneBrowser = webdriver.Firefox()
        userOneBrowser.get(url)
        register(userOneBrowser, "userOne", "password")
        self.assertEqual(userOneBrowser.current_url, url + "/home")
        createGroup(userOneBrowser, "Gruppis")
        
        userTwoBrowser = webdriver.Firefox()
        userTwoBrowser.get(url)
        register(userTwoBrowser, "userTwo", "password")
        self.assertEqual(userTwoBrowser.current_url, url + "/home")
        joinGroup(userTwoBrowser, "Gruppis")
        
        gotoGroupRequestsPage(userOneBrowser)
        acceptGroupJoinRequest(userOneBrowser)
        gotoGroupPage(userOneBrowser, "Gruppis")
        joinChat(userOneBrowser)

        gotoGroupPage(userTwoBrowser, "Gruppis")
        joinChat(userTwoBrowser)

        postChatMessage(userOneBrowser, "/hit userTwo 5")
        chatMessage = userOneBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userOne\nWelcome, userTwo\nPlayer userTwo was damaged for 5")
        chatMessage = userTwoBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userTwo\nYou were damaged for 5")
        
        postChatMessage(userTwoBrowser, "/hit userOne 5")
        chatMessage = userOneBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userOne\nWelcome, userTwo\nPlayer userTwo was damaged for 5")
        chatMessage = userTwoBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userTwo\nYou were damaged for 5\nYou are not admin")
        
        userOneBrowser.quit()
        
        chatMessage = userTwoBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userTwo\nYou were damaged for 5\nYou are not admin\nuserOne disconnected\nuserTwo is now admin")

        postChatMessage(userTwoBrowser, "/hit userTwo 5")
        chatMessage = userTwoBrowser.find_element_by_id('chatlist').text
        self.assertEqual(chatMessage,
                         "Welcome, userTwo\nYou were damaged for 5\nYou are not admin\nuserOne disconnected\nuserTwo is now admin\nYou were damaged for 5")
        userTwoBrowser.quit()

unittest.main()
