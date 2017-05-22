#!/usr/bin/env python
# -*- coding: utf-8 -*-
def generate_list(function, loop_times):
    return list(map(function, range(1, loop_times+1)))

def mirror(x):
    return x

def stars(n):
    return '*' * n

def fun_function(x):
    return "THIS IS WHAT X IS: '" + str(x) + "'"

print(generate_list(mirror, 4))
print(generate_list(stars, 5))
print(', '.join(generate_list(fun_function, 6)))
