#!/usr/bin/env python
#-*- coding: utf-8 -*-
def insertion_sort(search_list, function = lambda e: e[0]):
    for i in range(1, len(search_list)):
        j = i
        value1 = str(function(search_list[j-1]))
        value2 = str(function(search_list[j]))
        while j > 0 and value1 > value2:
            search_list[j - 1], search_list[j] = search_list[j], search_list[j - 1]
            j -= 1
            value1 = function(search_list[j-1])
            value2 = function(search_list[j])
    return search_list

db = [
    ('j', 'g', 'f'), ('a', 'u', 'h'), ('k', 'l', 'n'), ('o', 'i', 'd'),
    ('b', 's', 'x'), ('@', '.', '!'), ('p', 's', 'q'), ('o', 'e', 'z')
]
print("0", insertion_sort(db, lambda e: e[0]), '\n')
print("1", insertion_sort(db, lambda e: e[1]), '\n')
print("2", insertion_sort(db, lambda e: e[2]))
