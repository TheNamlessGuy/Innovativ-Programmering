#!/usr/bin/env python
# -*- coding: utf-8 -*-
import print_types, level_handler, objects
def totes_not_cheats_i_promise_pls_dont_look():
    return['death_thine_name_is_me',
           'avocado_cadaver',
           'cool_kidz_like_kyelfi',
           'police!_search_warrant',
           'stop_hiding_things_elmo',
           'if_you_cant_beat_them']

def not_cheating_i_swear(player_input, level, player, enemies):
    if player_input == 'death_thine_name_is_me': #invincible
        player.toggle_invinc()
        print_types.spoopy("You suddenly feel unkillable")
    elif player_input == 'avocado_cadaver': #killall
        enemies = []
        print_types.spoopy("Suddenly, everything arounds you falls down dead\n(Might take a few turns to buffer out)")
    elif player_input == 'cool_kidz_like_kyelfi': #20 keys
        for i in range(20):
            player.get_key()
        print_types.spoopy("Your pockets suddenly feel heavy")
    elif player_input == 'police!_search_warrant': #No doors
        for door in level_handler.get_objects(level, objects.Door):
            door.unlock()
    elif player_input == 'stop_hiding_things_elmo': #No secrets
        for secret in level_handler.get_objects(level, objects.Secret_door):
            secret.discover()
    elif player_input == 'if_you_cant_beat_them': #Player becomes enemy
        player.gave_up()
    return player, enemies