#!/usr/bin/env python
# -*- coding: utf-8 -*-
import soko_player as splay
import soko_obstacles as sobs

def sokoban_display(level):
    largest_x = get_largest_loc(level, 1)
    largest_y = get_largest_loc(level, 2)
    for i in range(largest_x):
        for j in range(largest_y):
            found_icon = False
            for icon in level:
                if icon[1] == i and icon[2] == j:
                    found_icon = True
                    print(get_icon(icon), end='')
                    level.remove(icon)
                    break
            if not found_icon:
                print(" ", end='')
        print()

def get_icon(icon):
    return_icon = {'wall': sobs.get_wall_icon(),
                   'box': sobs.get_box_icon(),
                   'e_storage': sobs.get_empty_storage_icon(),
                   'f_storage': sobs.get_filled_storage_icon(),
                   'player': splay.get_player_icon(),
                   'player_storage': splay.get_player_on_storage_icon()}[icon[0]]
    return return_icon


def get_largest_loc(walls, index):
    current_largest = walls[0][index]
    for wall in walls:
        if wall[index] > current_largest:
            current_largest = wall[index]
    current_largest += 1
    return current_largest


def sokoban_load(file_path):
    level = []
    player = []
    with open(file_path) as file:
        lines = file.readlines()
    for i in range(len(lines)):
        for j in range(len(lines[i])):
           icon_dict = {sobs.get_wall_icon(): sobs.create_wall(i, j),
                        sobs.get_box_icon(): sobs.create_box(i, j),
                        sobs.get_empty_storage_icon(): sobs.create_empty_storage(i, j),
                        sobs.get_filled_storage_icon(): sobs.create_filled_storage(i, j),
                        "@": ['player', i, j]}
           append_icon = icon_dict.get(lines[i][j], " ")
           if append_icon[0] == 'player':
               player = append_icon
           if not append_icon == " ":
               level.append(append_icon)
    return level, player

def get_levels():
    return_dict = {}
    from os import walk
    levels = []
    for (dirpath, dirnames, filenames) in walk("./levels"):
        levels.extend(filenames)
        break
    for file_dirs in levels:
        if not file_dirs.endswith("sokoban"):
            levels.remove(file_dirs)
    progress = get_current_progress()
    for i in range(len(levels)):
        level = levels[i][:-8]
        if "_" in level:
            number_index = level.index("_") + 1
            if level[number_index:].isdigit():
                if int(level[number_index:]) <= progress:
                    current_key = int(level[number_index:])
                    if current_key > 9:
                        return_dict[str(current_key)] = level
                    else:
                        return_dict["0" + str(current_key)] = level
    return return_dict


def update_player_icon(level, old_player, player):
    if old_player == player:
        return player, level
    if player[0] == 'player_storage':
        level.append(sobs.create_empty_storage(old_player[1], old_player[2]))
    for icon in level:
        if icon[1] == player[1] and icon[2] == player[2]:
            level.remove(icon)
            player = splay.create_player_on_storage(icon[1], icon[2])
            level.append(player)
            return player, level
    player = splay.create_player(player[1], player[2])
    level.append(player)
    return player, level


def update_box_icon(level, box, old_box):
    if old_box == box:
        for icon in level:
            if icon[1] == box[1] and icon[2] == box[2]:
                if icon[0] == 'e_storage' or icon[0] == 'f_storage':
                    box = sobs.create_filled_storage(box[1], box[2])
                level.remove(icon)
                break
        level.append(box)
        return box, level
    if box[0] == 'f_storage':
        box = sobs.create_empty_storage(box[1], box[2])
        level.append(box)
        return box, level
    for icon in level:
        if icon[1] == box[1] and icon[2] == box[2]:
            level.remove(icon)
            box = sobs.create_filled_storage(icon[1], icon[2])
            level.append(box)
            return box, level
    box = sobs.create_box(box[1], box[2])
    level.append(box)
    return box, level


def can_move_to_loc(input_obj, player_input, level):
    obj = input_obj[:]
    taboo_list = ['wall', 'box', 'f_storage']
    movement_dict = {'a': [0, -1],
                     'd': [0, 1],
                     'w': [-1, 0],
                     's': [1, 0]}
    obj_update = movement_dict.get(player_input, [0, 0])
    obj[1] += obj_update[0]
    obj[2] += obj_update[1]
    for icon in level:
        if icon[1] == obj[1] and icon[2] == obj[2]:
            if icon[0] in taboo_list:
                return False
            break
    return True


def get_number_of_icons(level, icon):
    number_of_icons = 0
    for row in level:
        for char in row:
            if char == icon:
                number_of_icons += 1
    return number_of_icons


def get_current_progress():
    from os import path
    current_progress = 1
    if path.isfile(".progress.txt"):
        progress_file = open(".progress.txt", "r")
        current_progress = int(progress_file.readline())
        progress_file.close()
    else:
        progress_file = open(".progress.txt", "w")
        progress_file.write("1")
        progress_file.close()
    return current_progress


def update_progress(beaten_level):
    number_index = beaten_level.index("_") + 1
    beaten_level = beaten_level[number_index:]
    if beaten_level.isdigit():
        beaten_level = int(beaten_level) + 1
        current_progress = get_current_progress()
        if beaten_level > current_progress:
            progress_file = open(".progress.txt", "w")
            progress_file.write(str(beaten_level))
            progress_file.close()
