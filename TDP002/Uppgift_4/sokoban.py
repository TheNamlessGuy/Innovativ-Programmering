#!/usr/bin/env python
# -*- coding: utf-8 -*-
import soko_player as splay
import soko_main as smain


def play():
    dict_of_levels = smain.get_levels()
    print("Welcome to Sokoban, please choose a level:")
    for key in sorted(dict_of_levels):
        print(key + ".", dict_of_levels[key])
    level_select = input("Choose: ")
    if not level_select in dict_of_levels.keys():
        print("That's not a level in the list")
    else:
        level, player = smain.sokoban_load("levels/" + dict_of_levels[level_select] + ".sokoban")
        winning_condition = smain.get_number_of_icons(level, "e_storage")
        has_won = False
        while not has_won:
            old_player = player[:]
            smain.sokoban_display(level[:])
            number_of_filled_storage = smain.get_number_of_icons(level, "f_storage")
            if number_of_filled_storage == winning_condition:
                has_won = True
            else:
                player_input = input("Make your move (a)left, (d)right, (w)up, (s)down, (restart): ")
                if player_input == "q":
                    has_won = True
                elif player_input == "restart":
                    level, player = smain.sokoban_load("levels/" + dict_of_levels[level_select] + ".sokoban")
                else:
                    level, player = splay.move_player(player, level, old_player, player_input)
        print("Congratulations! You beat the level '" + dict_of_levels[level_select] + "'!")
        smain.update_progress(dict_of_levels[level_select])


play()
