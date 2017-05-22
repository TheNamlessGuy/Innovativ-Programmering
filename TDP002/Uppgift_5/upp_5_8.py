#!/usr/bin/env python
# -*- coding: utf-8 -*-
from upp_5_6 import partial
from upp_5_7 import compose
def make_filter_map(filter_func, map_func):
    map_function = partial(map, map_func)
    #lambda x: map(map_func, x)
    filter_function = partial(filter, filter_func)
    #lambda x: filter(filter_func, x)
    combo_function = compose(map_function, filter_function)
    #lambda x: map_function(filter_function(x))
    return_function = compose(list, combo_function)
    #lambda x: list(combo_function(x))
    return return_function

map_f = lambda x: x*x
filter_f = lambda x: x % 2 == 1
finished_function = make_filter_map(filter_f, map_f)
print(finished_function(range(10)))
