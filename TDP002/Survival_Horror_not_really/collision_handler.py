#!/usr/bin/env python
# -*- coding: utf-8 -*-
import level_handler
import objects
def player_can_move_to_loc(move_dict, obj, level):
    poss_x = obj.get_loc()["x"] + move_dict["x"]
    poss_y = obj.get_loc()["y"] + move_dict["y"]
    icon = level_handler.get_icon_at_loc(level, poss_x, poss_y)
    if icon is None or type(icon) == objects.Key or type(icon) == objects.Win_area:
        return True
    elif type(icon) is objects.Door:
        if obj.get_key_count() > 0:
            icon.unlock()
            obj.use_key()
            return True
    elif type(icon) is objects.Secret_door:
        icon.discover()
        return True
    return False

def enemy_can_move_to_loc(move_dict, obj, level):
    poss_x = obj.get_loc()["x"] + move_dict["x"]
    poss_y = obj.get_loc()["y"] + move_dict["y"]
    icon = level_handler.get_icon_at_loc(level, poss_x, poss_y)
    if icon is None or type(icon) == objects.Player or type(icon) == objects.Key or type(icon) == objects.Win_area:
        return True
    return False