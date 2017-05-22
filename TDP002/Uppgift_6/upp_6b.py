#!/usr/bin/env python
#-*- coding: utf-8 -*-
def binary_search(haystack, needle, function = lambda e, f: e == f):
    left = 0
    right = len(haystack)
    mid = (left + right) // 2
    needle = str(needle)
    element = haystack[mid]
    while left <= right:
        if function(element, needle):
            return element
        elif element > needle:
            right = mid - 1
        elif element < needle:
            left = mid + 1
        mid = (left + right)//2
        element = str(haystack[mid])
    return -1

haystack = [str(x) for x in range(1, 100)]
haystack = [{'title': 'Harry Potter', 'actor': 'Daniel Radcliffe', 'score': 15},
            {'title': 'Stupid Movie', 'actor': 'Rad McAwesome', 'score': -1}]
print(binary_search(haystack, 'Stupid Movie', lambda e, f: e['title'] == f))
