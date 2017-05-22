#!/usr/bin/env python
# -*- coding: utf-8 -*-
def linear_search(haystack, needle, function = lambda e: [x for x in e.values()]):
    return_list = []
    for item in haystack:
        value = function(item)
        if needle in value:
            return_list.append(item)
    return return_list

haystack = [{'title': 'Harry Potter', 'actor': 'Daniel Radcliffe', 'score': 15},
            {'title': 'Stupid Movie', 'actor': 'Rad McAwesome', 'score': -1}]
print(linear_search(haystack, 'Harry Potter'))
print(linear_search(haystack, 'Daniel Radcliffe', lambda e: e['actor']))
print(linear_search(haystack, -1))
