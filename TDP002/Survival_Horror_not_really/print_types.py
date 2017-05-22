#!/usr/bin/env python
# -*- coding: utf-8 -*-
def print_warning(print_string, str_end="\n"):
    print('\033[1;33m' + print_string + '\033[1;m' + str_end, end='')

def spoopy(print_string, str_end="\n"):
    print('\033[1;30m' + print_string + '\033[1;m' + str_end, end='')