#!/usr/bin/env python
# -*- coding: utf-8 -*-
def partial(function, value):
    return lambda x: function(value, x)

def add(n, m):
    return n+m

add_five = partial(add, 5)
int_to_add = 10
print("THIS SHOULD BE", int_to_add, "+ 5:", add_five(int_to_add))