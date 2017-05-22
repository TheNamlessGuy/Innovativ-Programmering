#!/usr/bin/env python
#-*- coding: utf-8 -*-
from os import walk, rename
def add_copyright(info_file, change_dir, taboo_file_type = "", change_file_type = ""):
    check_values = ['BEGIN COPYRIGHT', 'END COPYRIGHT']
    copyright_info = read_lines(info_file)
    if is_file(change_dir):
        write_to_file(copyright_info, change_dir, check_values, change_file_type)
    else:
        for (root, dirs, files) in walk(change_dir):
            for file_dir in files:
                if not file_dir.endswith(taboo_file_type) or taboo_file_type is "":
                    write_to_file(copyright_info, root + '/' + file_dir, check_values, change_file_type)
            break

def read_lines(file_dir):
    with open(file_dir) as open_file: return open_file.readlines()

def is_file(check_dir):
    try:
        open(check_dir)
        return True
    except:
        return False

def write_to_file(copyright_info, file_dir, check_values, change_file_type):
    lines = read_lines(file_dir)
    before_lines, begin_end_lines = separate_lines(lines, check_values)
    begin_end_lines = check_if_on_same_item(begin_end_lines, check_values) #If begin and end on same line, separate
    begin_end_lines = add_copyright_info(copyright_info, begin_end_lines)
    before_lines = convert_to_string(before_lines)
    compiled_list = []
    for i in range(len(begin_end_lines)):
        compiled_list.append(before_lines[i])
        compiled_list.append(begin_end_lines[i])
    if not before_lines[-1] in compiled_list:
        compiled_list.append(before_lines[-1])
    compiled_list = ''.join(compiled_list)
    file_overwrite(file_dir, compiled_list)
    update_file_dir(file_dir, change_file_type)

def separate_lines(lines_input, check_values):
    lines = lines_input[:]
    return_other = []
    return_begin_end = []
    number_of_copyrights = get_number_of_copyrights(lines, check_values[1])
    for i in range(number_of_copyrights):
        end_index = get_index_of(check_values[1], lines) + 1
        copy_lines = lines[:end_index]
        lines = remove_list(lines, copy_lines)
        return_other_update, return_begin_end_update = get_separated_lines(copy_lines, check_values)
        return_other.append(return_other_update)
        return_begin_end.append(return_begin_end_update)
    return_other.append(lines)
    return return_other, return_begin_end

def get_index_of(string, lst):
    for i in range(len(lst)):
        item = lst[i]
        if string in item:
            return i
    return -1

def get_number_of_copyrights(lines, search):
    return_value = 0
    for line in lines:
        return_value += line.count(search)
    return return_value

def remove_list(lst, lst_to_remove):
    length = len(lst_to_remove)
    return lst[length:]

def get_separated_lines(lines, check_values):
    others = []
    begin_end = []
    begin_end_record = False
    for line in lines:
        if check_values[0] in line:
            begin_end_record = True
        if begin_end_record:
            begin_end.append(line)
        else:
            others.append(line)
        if check_values[1] in line:
            begin_end_record = False
    return others, begin_end

def check_if_on_same_item(lines, check_values):
    for i in range(len(lines)):
        line = lines[i]
        if check_values[1] in line[0]:
            index = line[0].index(check_values[1])
            str_1 = line[0][:index]
            str_2 = line[0][index:]
            lines[i] = [str_1, str_2]
    return lines

def add_copyright_info(copyright_info, lines):
    for i in range(len(lines)):
        lines[i] = remove_empty_lines(lines[i])
        if not check_line_doesnt_contain(lines[i], copyright_info):
            lines[i].insert(1, '')
            copy_info = ''.join(copyright_info)
            lines[i][1] = copy_info
        lines[i] = ''.join(lines[i])
    return lines

def remove_empty_lines(lines):
    lines_to_remove = []
    for line in lines:
        if line == "\n":
            lines_to_remove.append(line)
    for i in lines_to_remove:
        lines.remove(i)
    return lines

def check_line_doesnt_contain(line_input, copy_info_input):
    copy_info = copy_info_input[:]
    line = line_input[:]
    line = ''.join(line)
    copy_info = ''.join(copy_info)
    if copy_info in line:
        return True
    return False

def convert_to_string(lst):
    for i in range(len(lst)):
        lst[i] = ''.join(lst[i])
    return lst

def update_file_dir(file_dir, change_file_type):
    if change_file_type == "":
        return file_dir
    index_of_current_end = file_dir.rindex(".") + 1
    current_end = file_dir[index_of_current_end:]
    new_file_dir = file_dir.replace(current_end, change_file_type)
    rename(file_dir, new_file_dir)
    return file_dir

def file_overwrite(file_dir, line):
    with open(file_dir, "w") as open_file:
        open_file.write(line)

from sys import argv
if len(argv) >= 3:
    info_file = argv[1]
    change_dir = argv[2]
else:
    print("Need at least two in-values")
    exit()
taboo_file_type = ""
change_file_type = ""
if len(argv) >= 4:
    taboo_file_type = argv[3]
if len(argv) >= 5:
    change_file_type = argv[4]
#info_file = "Copyright_info.txt2"
#change_dir = "needs_copyright.txt2"
#change_dir = "./folder_stuff"
add_copyright(info_file, change_dir, taboo_file_type, change_file_type)
