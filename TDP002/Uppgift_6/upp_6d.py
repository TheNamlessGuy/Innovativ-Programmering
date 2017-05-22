#!/usr/bin/evn python
#-*- coding: utf-8 -*-
def quicksort(db, function = lambda e: e[0]):
    less = []
    equal = []
    greater = []
    if len(db) > 1:
        pivot = function(db[0])
        for i in range(len(db)):
            element = function(db[i])
            if element < pivot:
                less.append(db[i])
            if element == pivot:
                equal.append(db[i])
            if element > pivot:
                greater.append(db[i])
        return quicksort(less, function)+equal+quicksort(greater, function)
    else:
        return db

db = [
    ('j', 'g', 'f'), ('a', 'u', 'h'), ('k', 'l', 'n'), ('o', 'i', 'd'),
    ('b', 's', 'x'), ('@', '.', '!'), ('p', 's', 'q'), ('o', 'e', 'z')
]
print("0", quicksort(db, lambda e: e[0]), '\n')
print("1", quicksort(db, lambda e: e[1]), '\n')
print("2", quicksort(db, lambda e: e[2]))

