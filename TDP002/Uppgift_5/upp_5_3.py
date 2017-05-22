#!/usr/bin/env python
# -*- coding: utf-8 -*-


def contains(search_value, search_list):
    list_len = len(search_list)
    counter = 0
    while counter < list_len:
        #Every element in the input list
        if search_list[counter] == search_value:
            #If it equals search_value
            return True
        counter += 1
    return False

haystack = 'Can you find the needle in this haystack?'.split()
#['Can', 'you', 'find', 'the', 'needle', 'in', 'this', 'haystack?']
print('Can:', contains('Can', haystack))
print('you:', contains('you', haystack))
print('find:', contains('find', haystack))
print('the:', contains('the', haystack))
print('needle:', contains('needle', haystack))
print('in:', contains('in', haystack))
print('this:', contains('this', haystack))
print('haystack', contains('haystack', haystack))
print('haystack?:', contains('haystack?', haystack))
print('bababababa:', contains('bababababa', haystack))
print('a:', contains('a', haystack))
