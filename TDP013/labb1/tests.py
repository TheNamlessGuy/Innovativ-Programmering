from selenium import webdriver
from sys import stdout

browser = webdriver.Firefox()
browser.get('http://www-und.ida.liu.se/~maxbr431/TDP013/labb_1/test.html')

inputElement = browser.find_element_by_id('textinput')
button = browser.find_element_by_id('sendButton')
errorMessage = browser.find_element_by_id('error')

# Test One (Normal Message)
stdout.write("Testing normal message... ")
inputElement.send_keys("Hi, this is my first 'tweet'")
button.click()
if errorMessage.text == "":
    print("Works!")
else:
    print("Failed!")

stdout.write("Testing empty message... ")
inputElement.send_keys("")
button.click()
if errorMessage.text != "":
    print("Works!")
else:
    print("Failed!")

stdout.write("Testing too long message... ")
inputElement.send_keys("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
button.click()
if errorMessage.text != "":
    print("Works!")
else:
    print("Failed!")

stdout.write("Testing that messages got published... ")
if browser.find_element_by_id('posts').text != "":
    print("Works!")
else:
    print("Failed!")

stdout.write("Testing if checkbox exists... ")
checkbox = browser.find_element_by_name('0')
if (checkbox != None):
    print("Works!")
else:
    print("Failed!")

stdout.write("Testing messages disappearing at refresh... ")
browser.refresh()
if browser.find_element_by_id('posts').text == "":
    print("Works!")
else:
    print("Failed!")

browser.quit()
