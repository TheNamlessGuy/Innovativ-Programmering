#!/usr/bin/env python
# -*- coding: utf-8 -*-
import level_handler, not_cheats
global running, game_over, won
def play():
    global running, game_over, won
    won = False
    game_over = False
    running = True
    movement_dict = {'w': {"x": -1, "y": 0},
                     's': {"x": 1, "y": 0},
                     'a': {"x": 0, "y": -1},
                     'd': {"x": 0, "y": 1}}
    player_input = ''
    print("\033[1;m" + "Welcome to Survival Horror (not really)! Please choose a level:")
    level, level_name, player, enemies = level_handler.choose_level()
    if level is None:
        running = False
    while running:
        print("KEYS:", player.get_key_count())
        level_handler.render_level(level, player)
        player_input = input("Commands: (w, s, a, d, restart, exit, menu)> ")
        if player_input in ["exit", "q"]:
            running = False
        elif player_input == "restart":
            level, player, enemies = level_handler.get_level(level_name)
        elif player_input == "menu":
            print("\033[1;m" + "Welcome to Survival Horror (not really)! Please choose a level:")
            level, level_name, player, enemies = level_handler.choose_level()
        else:
            if player_input in not_cheats.totes_not_cheats_i_promise_pls_dont_look():
                player, enemies = not_cheats.not_cheating_i_swear(player_input, level, player, enemies)
            else:
                for char in player_input:
                    player_update = movement_dict.get(char, {"x": 0, "y": 0})
                    player.move(player_update, level)
                for enemy in enemies:
                    enemy.move(player, level)
            level = level_handler.update_level(level, enemies)
        if player.isdead():
            level_handler.render_level(level, player)
            running = False
            game_over = True
        elif player.has_won():
            level_handler.render_level(level, player)
            running = False
            won = True
    if game_over:
        print("You lost! Try again")
    elif won:
        print("You won! Congratulations!")
    else:
        print("Bye-bye!")

play()