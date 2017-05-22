#!/usr/bin/env python
# -*- coding: utf-8 -*-
from functools import reduce
def uppg_1_2(num):
    add_val = lambda x, y: x + y
    mult_val = lambda x, y: x * y
    function_list = [add_val, mult_val]
    return_list = []
    for function in function_list:
        return_list.append(reduce(function, range(1, num+1)))
    return return_list

print(uppg_1_2(512))
