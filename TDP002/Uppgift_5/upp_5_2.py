#!/usr/bin/env python
# -*- coding: utf-8 -*-
def dbsearch(db, key_value, search_value):
    return_list = []
    for person in db:
        if search_value in person[key_value]:
            return_list.append(person)
    return return_list

def TEST():
    return [
        {'name': 'Jakob', 'position': 'assistant'},
        {'name': 'Åke', 'position': 'assistant'},
        {'name': 'Ola', 'position': 'examiner'},
        {'name': 'Henrik', 'position': 'assistant'}
    ]

print(dbsearch(TEST(), 'position', 'examiner'))
print(dbsearch(TEST(), 'name', 'Åke'))
print(dbsearch(TEST(), 'position', 'assistant'))
