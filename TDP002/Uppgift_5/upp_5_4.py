#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
def run_terminal():
    current_path = get_exec_dir()
    running = True
    while running:
            user_input = input('\033[0m' + current_path + " | command> ")
            user_input = user_input.split()
            #accepted_inputs = ['pwd', 'exit', 'cd', 'ls', 'cat', 'rm']
            if len(user_input) == 0:
                pass
            elif user_input[0].startswith("exit"):
                running = False
            elif user_input[0].startswith("PARTAY"):
                partay()
            #else:
            #    if user_input[0] in accepted_inputs:
            #        eval(user_input[0] + "(user_input)")
            elif user_input[0].startswith("pwd"):
                pwd(current_path)
            elif user_input[0].startswith("cd"):
                current_path = cd(user_input, current_path)
            elif user_input[0].startswith("ls"):
                ls(user_input, current_path)
            elif user_input[0].startswith("cat"):
                cat(user_input, current_path)
            elif user_input[0].startswith("rm"):
                rm(user_input, current_path)

def pwd(current_path):
    print(current_path)

def cd(user_input, current_path):
    if len(user_input) == 1:
        return get_exec_dir()
    else:
        if user_input[1] == '..':
            return go_up_a_folder(current_path)
        elif os.path.isdir(current_path + '/' + user_input[1]):
            current_path += '/' + user_input[1]
            return current_path
    return current_path

def ls(user_input, current_path):
    path = current_path
    if len(user_input) > 1:
        path = user_input[1]
    for (dirpath, dirs, files) in os.walk(path):
        print('\033[94m' + ', '.join(dirs) + '\033[92m ' + ', '.join(files) + '\033[0m')
        break

def cat(user_input, current_path):
    if len(user_input) == 1:
        print("The cat command requires a file to read")
    else:
        file_dir = user_input[1]
        for line in open(current_path + '/' + file_dir):
            print(line, end='')
        print()

def rm(user_input, current_path):
    if len(user_input) == 1:
        print("The rm command requires a file to remove")
    else:
        file_dir = user_input[1]
        if os.path.isfile(file_dir):
            user_input = input("Do you want to remove the file '" + file_dir + "'? (y/n): ")
            if user_input == 'y':
                os.remove(current_path + "/" + file_dir)
                print("'" + file_dir + "' removed")
            elif user_input == 'n':
                print("'" + file_dir + "' not removed")
            else:
                print("That's not a valid answer")
        

def get_exec_dir():
    current_path = os.path.realpath(__file__)
    current_path, file = os.path.split(current_path)
    return current_path

def go_up_a_folder(current_path):
    current_path = current_path.split('/')
    if not current_path[-1] == "home":
        if current_path[-1] == '':
            del current_path[-1]
        del current_path[-1]
    return '/'.join(current_path)

def partay():
    from random import shuffle
    colors = ['\033[94m', '\033[95m', '\033[91m', '\033[92m', '\033[93m', '\033[96m']
    for i in range(10):
        shuffle(colors)
        print(colors[0] + 'P' + colors[1] + 'A' + colors[2] + 'R' + colors[3] + 'T' + colors[4] + 'A' + colors[5] + 'Y' + '\033[0m')

run_terminal()
