#!/usr/bin/env python
# -*- coding: utf-8 -*-
import soko_main as smain
import soko_obstacles as sobs


def create_player(x, y):
    return ['player', x, y]


def create_player_on_storage(x, y):
    return ['player_storage', x, y]


def get_player_icon():
    return "@"


def get_player_on_storage_icon():
    return "+"


def get_player(level):
    for icon in level:
        if icon[0] == 'player' or icon[0] == 'player_storage':
            return True
    return False


def player_on_storage(player, level):
    if level[player[1]][player[2]] == get_player_on_storage_icon():
        return True
    return False


def move_player(player, level, old_player, player_input):
    level = sobs.check_for_box(level, player[:], player_input)
    if smain.can_move_to_loc(player, player_input, level):
        movement_dict = {'a': [0, -1],
                         'd': [0, 1],
                         'w': [-1, 0],
                         's': [1, 0]}
        player_update = movement_dict.get(player_input, [0, 0])
        if not player_update == [0, 0]:
            level.remove(player)
        player[1] += player_update[0]
        player[2] += player_update[1]
    player, level = smain.update_player_icon(level, old_player, player)
    return level, player
