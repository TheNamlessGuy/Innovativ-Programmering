#!/usr/bin/env python
#-*- coding: utf-8 -*-
from os import path, rename, walk, listdir
def add_copyright(info_file, input_dir, taboo="", change_type=""):
    with open(info_file) as open_file: copy_info = open_file.readlines()
    if path.isfile(input_dir): #If the input directory is a file
        if not input_dir.endswith(taboo) or taboo is "": #If the input file is not of the taboo filetype or taboo is empty
            write_to_file(copy_info, input_dir, change_type)
    else: #If the input directory is a folder
        for filename in listdir(input_dir):
            if not filename.endswith(taboo) or taboo is "": #If the input file is not of the taboo filetype or taboo is empty
                write_to_file(copy_info, input_dir + "/" + filename, change_type)
                    
def write_to_file(info, file_dir, change_type):
    with open(file_dir) as open_file: lines = open_file.readlines()
    recording = False
    lines_to_remove = []
    for i in range(len(lines)):
        if "BEGIN COPYRIGHT" in lines[i]: #Start recording
            recording = True
        if recording:
            if not "BEGIN COPYRIGHT" in lines[i] and not "END COPYRIGHT" in lines[i]: #If not BEGIN COPYRIGHT or END COPYRIGHT in line, remove line
                lines_to_remove.append(lines[i])
        if "END COPYRIGHT" in lines[i]: #Stop recording
            recording = False
    lines = remove_lines(lines, lines_to_remove) #Remove all lines between the copyrights
    added = False
    for i in range(len(lines)):
        if "BEGIN COPYRIGHT" in lines[i]:
            if "END COPYRIGHT" in lines[i]: #If on same row, start writing right after BEGIN COPYRIGHT
                index = lines[i].index("BEGIN COPYRIGHT") + len("BEGIN COPYRIGHT")
            else: #If not on the same row, start typing at the end of the line
                index = len(lines[i])
            added = True
            lines[i] = lines[i][:index] + ''.join(info) + lines[i][index:] #Add info
    if not change_type is "" and added: #If change type is not empty, and you've changed the current file
        old_dir = file_dir
        file_dir = file_dir[:(file_dir.rindex(".") + 1)]
        file_dir += change_type
        rename(old_dir, file_dir)
    with open(file_dir, "w") as open_file: open_file.writelines(lines) #Overwrite the old file

def remove_lines(lines, to_remove):
    recording = False
    for line in to_remove:
        lines.remove(line)
    return lines

from sys import argv
if len(argv) >= 3:
    info_file = argv[1] #info_file = "Copyright_info.txt2"
    input_dir = argv[2] #input_dir = "needs_copyright.txt2"/"./folder_stuff"
else:
    print("Need at least two in-values")
    exit()
taboo_file_type = ""
change_file_type = ""
if len(argv) >= 4:
    taboo_file_type = argv[3]
if len(argv) >= 5:
    change_file_type = argv[4]
add_copyright(info_file, input_dir, taboo_file_type, change_file_type)
