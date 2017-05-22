#!/usr/bin/env python
# -*- coding: utf-8 -*-
import objects
def wall_icon():
    return "\033[1;37m" + get_wall_icon() + "\033[1;m"

def get_wall_icon():
    return "#"

def door_icon():
    return '\033[1;32m' + get_door_icon() + '\033[1;m'

def get_door_icon():
    return "D"

def key_icon():
    return '\033[1;33m' + get_key_icon() + '\033[1;m'

def get_key_icon():
    return "K"

def enemy_icon():
    return "\033[1;31m" + get_enemy_icon() + "\033[1;m"

def get_enemy_icon():
    return "E"

def player_icon():
    return "\033[1;34m" + get_player_icon() + "\033[1;m"

def get_player_icon():
    return "P"

def dead_player_icon():
    return "\033[1;35m" + get_player_icon() + "\033[1;m"

def win_icon():
    return "\033[1;36m" + get_win_icon() + "\033[1;m"

def get_win_icon():
    return "W"

def secret_door_icon():
    return "\033[0;37m" + get_wall_icon() + "\033[1;m"

def get_secret_door_icon():
    return "H"

def get_icon(icon_type):
    type_dict = {"dead": dead_player_icon(),
                 objects.Wall: wall_icon(),
                 objects.Enemy: enemy_icon(),
                 objects.Player: player_icon(),
                 objects.Door: door_icon(),
                 objects.Key: key_icon(),
                 objects.Win_area: win_icon(),
                 objects.Secret_door: secret_door_icon()}
    return type_dict.get(type(icon_type), " ")