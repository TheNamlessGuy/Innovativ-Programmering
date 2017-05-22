#!/usr/bin/env python
# -*- coding: utf-8 -*-
def compose(func1, func2):
    return lambda x: func1(func2(x))

def add_five(x):
    return x + 5

def mult_ten(x):
    return x * 10

comp = compose(add_five, mult_ten)
int_to_run = 10
print("THIS SHOULD BE", int_to_run, "* 10 + 5:", comp(int_to_run))
