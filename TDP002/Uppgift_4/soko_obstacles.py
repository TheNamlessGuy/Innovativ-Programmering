#!/usr/bin/env python
# -*- coding: utf-8 -*-
import soko_player as splay
import soko_main as smain


def get_wall_icon():
    return "#"


def get_box_icon():
    return "o"


def get_empty_storage_icon():
    return "."


def get_filled_storage_icon():
    return "*"


def create_wall(x, y):
    if isinstance(x, int):
        if isinstance(y, int):
            return ['wall', x, y]


def create_box(x, y):
    if isinstance(x, int):
        if isinstance(y, int):
            return ['box', x, y]


def create_empty_storage(x, y):
    if isinstance(x, int):
        if isinstance(y, int):
            return ['e_storage', x, y]


def create_filled_storage(x, y):
    if isinstance(x, int):
        if isinstance(y, int):
            return ['f_storage', x, y]


def check_for_box(level, player, player_input):
    
    
    if player_input == "a":
        player[2] -= 1
    elif player_input == "d":
        player[2] += 1
    elif player_input == "w":
        player[1] -= 1
    elif player_input == "s":
        player[1] += 1
    for i in range(len(level)):
        icon = level[i]
        if icon[1] == player[1] and icon[2] == player[2]:
            if icon[0] == 'box' or icon[0] == 'f_storage':
                box = create_box(player[1], player[2])
                if icon[0] == 'f_storage':
                    level[i] = create_empty_storage(box[1], box[2])
                else:
                    level.remove(icon)
                box, level = move_box(level, player_input, box)
                break
    return level


def move_box(level, player_input, box):
    old_box = box[:]
    if smain.can_move_to_loc(box, player_input, level):
        movement_dict = {'a': [0, -1],
                         'd': [0, 1],
                         'w': [-1, 0],
                         's': [1, 0]}
        box_update = movement_dict.get(player_input, [0, 0])
        box[1] += box_update[0]
        box[2] += box_update[1]
    box, level = smain.update_box_icon(level, box, old_box)
    return box, level
