#!/usr/bin/env python
# -*- coding: utf-8 -*-
from os import walk
import icons
import objects
import print_types
def choose_level():
    level = []
    while not is_valid_level(level):
        levels = load_levels_from_files()
        print_levels(levels[:])
        player_input = input("Choose: ")
        if player_input in ["exit", "q"]:
            return None, None, None, None
        level_name = get_level_name(player_input, levels)
        level, player, enemies = get_level(level_name)
        if not is_valid_level(level):
            print_types.print_warning("Not a valid input, please enter the number next to the level name")
    return level, level_name, player, enemies

def is_valid_level(level):
    if len(level) > 0:
        return True
    return False

def print_levels(levels):
    levels = clean_names(levels)
    for i in range(len(levels)):
        if "_" in levels[i]:
            levels[i] = levels[i].split("_")
            levels[i] = " ".join(levels[i])
        levels[i] = levels[i].title()
        print(str(i+1) + ". " + levels[i])

def clean_names(lst):
    for file_name in lst:
        if not file_name.endswith(".surv_hor"):
            lst.remove(file_name)
    for i in range(len(lst)):
        lst[i] = lst[i][:-9]
        if "_" in lst[i]:
            lst[i] = lst[i].split("_")
            lst[i] = " ".join(lst[i])
    return lst


def load_levels_from_files():
    levels = []
    for (root, dirs, files) in walk("./levels"):
        levels.extend(files)
        break
    return levels

def get_level_name(player_input, levels):
    try:
        player_input = int(player_input)
        level = levels[player_input-1]
        level = "./levels/" + level
        return level
    except Exception:
        return ""

def get_level(level_dir):
    try:
        with open(level_dir) as open_file:
            level = open_file.readlines()
        level = convert_level(level)
        player = get_objects(level, objects.Player)[0]
        enemies = get_objects(level, objects.Enemy)
        return level, player, enemies
    except Exception:
        return [], None, []

def convert_level(level):
    return_list = []
    for x in range(len(level)):
        for y in range(len(level[x])):
            icon_dict = {icons.get_player_icon(): objects.Player(x, y),
                         icons.get_enemy_icon(): objects.Enemy(x, y),
                         icons.get_wall_icon(): objects.Wall(x, y),
                         icons.get_key_icon(): objects.Key(x, y),
                         icons.get_door_icon(): objects.Door(x, y),
                         icons.get_win_icon(): objects.Win_area(x, y),
                         icons.get_secret_door_icon(): objects.Secret_door(x, y)}
            add_icon = icon_dict.get(level[x][y], "")
            if not add_icon == "":
                return_list.append(add_icon)
    return return_list

def render_level(input_level, player):
    level = input_level[:]
    largest_x = get_largest_pos(level, "x")
    largest_y = get_largest_pos(level, "y")
    for x in range(largest_x):
        for y in range(largest_y):
            if player.get_loc()["x"] == x and player.get_loc()["y"] == y:
                if player.isdead():
                    print(icons.get_icon("dead"), end='')
                elif player.has_given_up():
                    print(icons.get_icon(objects.Enemy(0, 0)), end='')
                else:
                    print(icons.get_icon(player), end='')
            else:
                found_icon = False
                for icon in level:
                    if icon.get_loc()["x"] == x and icon.get_loc()["y"] == y:
                        found_icon = True
                        print(icons.get_icon(icon), end='')
                        level.remove(icon)
                        break
                if not found_icon:
                    print(icons.get_icon(" "), end='')
        print()

def get_largest_pos(level, index):
    current_largest = level[0].get_loc()[index]
    for icon in level:
        if icon.get_loc()[index] > current_largest:
            current_largest = icon.get_loc()[index]
    current_largest += 1
    return current_largest

def get_objects(level, obj_type):
    return_list = []
    for icon in level:
        if isinstance(icon, obj_type):
            return_list.append(icon)
    return return_list

def get_icon_at_loc(level, x, y, taboo=[]):
    for icon in level:
        if icon.get_loc()["x"] == x and icon.get_loc()["y"] == y:
            if not type(icon) in taboo:
                return icon
    return None

def update_level(level, enemies):
    for icon in level:
        if isinstance(icon, objects.Enemy):
            level.remove(icon)
        elif isinstance(icon, objects.Door):
            if icon.is_unlocked():
                level.remove(icon)
        elif isinstance(icon, objects.Key):
            if icon.is_picked_up():
                level.remove(icon)
        elif isinstance(icon, objects.Secret_door):
            if icon.is_discovered():
                level.remove(icon)
    for enemy in enemies:
        level.append(enemy)
    return level