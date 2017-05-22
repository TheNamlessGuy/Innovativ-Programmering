#!/usr/bin/env python
# -*- coding: utf-8 -*-
import collision_handler
import print_types
import level_handler
class Player:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.dead = False
        self.keys = 0
        self.won = False
        self.invincible = False
        self.given_up = False

    def __str__(self):
        return "['player', " + str(self.x) + ", " + str(self.y) + "]"

    def move(self, move_dict, level):
        if collision_handler.player_can_move_to_loc(move_dict, self,  level):
            self.x += move_dict["x"]
            self.y += move_dict["y"]
            icon = level_handler.get_icon_at_loc(level, self.x, self.y, taboo=[Player])
            if not icon is None:
                if type(icon) == Key:
                    self.get_key()
                    icon.pick_up()
                if type(icon) == Win_area:
                    self.win()

    def toggle_invinc(self):
        self.invincible = not self.invincible

    def gave_up(self):
        self.given_up = not self.given_up

    def has_given_up(self):
        return self.given_up

    def kill(self):
        if not self.invincible:
            self.dead = True

    def isdead(self):
        return self.dead

    def win(self):
        self.won = True

    def has_won(self):
        return self.won

    def get_loc(self):
        return {"x": self.x, "y": self.y}

    def get_key(self):
        self.keys += 1

    def use_key(self):
        self.keys -= 1

    def get_key_count(self):
        return self.keys

class Enemy:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return "['enemy', " + str(self.x) + ", " + str(self.y) + "]"

    def move(self, player, level):
        if not player.has_given_up():
            if (self.x > player.get_loc()["x"]) and collision_handler.enemy_can_move_to_loc({"x": -1, "y": 0}, self, level):
                self.x -= 1
            elif (self.x < player.get_loc()["x"]) and collision_handler.enemy_can_move_to_loc({"x": 1, "y": 0}, self, level):
                self.x += 1
            elif (self.y > player.get_loc()["y"]) and collision_handler.enemy_can_move_to_loc({"x": 0, "y": -1}, self, level):
                self.y -= 1
            elif (self.y < player.get_loc()["y"]) and collision_handler.enemy_can_move_to_loc({"x": 0, "y": 1}, self, level):
                self.y += 1
            if self.x == player.get_loc()["x"] and self.y == player.get_loc()["y"]:
                player.kill()

    def get_loc(self):
        return {"x": self.x, "y": self.y}

class Wall:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return "['wall', " + str(self.x) + ", " + str(self.y) + "]"

    def get_loc(self):
        return {"x": self.x, "y": self.y}

class Door:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.unlocked = False

    def __str__(self):
        return "['door', " + str(self.x) + ", " + str(self.y) + "]"

    def get_loc(self):
        return {"x": self.x, "y": self.y}

    def unlock(self):
        self.unlocked = True

    def is_unlocked(self):
        return self.unlocked

class Key:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.picked_up = False

    def __str__(self):
        return "['key', " + str(self.x) + ", " + str(self.y) + "]"

    def get_loc(self):
        return {"x": self.x, "y": self.y}

    def pick_up(self):
        self.picked_up = True

    def is_picked_up(self):
        return self.picked_up

class Win_area:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return "['win_area', " + str(self.x) + ", " + str(self.y) + "]"

    def get_loc(self):
        return {"x": self.x, "y": self.y}

class Secret_door:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.discovered = False

    def __str__(self):
        return "['secret_door', " + str(self.x) + ", " + str(self.y) + "]"

    def get_loc(self):
        return {"x": self.x, "y": self.y}

    def discover(self):
        self.discovered = True

    def is_discovered(self):
        return self.discovered
