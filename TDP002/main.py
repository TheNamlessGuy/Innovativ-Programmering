#!/usr/bin/env python
#-*- coding: utf-8 -*-
from os import walk, remove
def delete(path):
    for (root, dirs, files) in walk(path):
        for file_dir in files:
            if file_dir.endswith("~"):
                input_data = input("Do you want to delete the file '" + file_dir + "'? ")
                if input_data in ['yes', 'y']:
                    remove(root + "/" + file_dir)

value = "."
delete(value)
