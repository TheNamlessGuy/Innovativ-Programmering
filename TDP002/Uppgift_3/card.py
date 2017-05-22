#!/usr/bin/env python
# -*- coding: utf-8 -*-
def create_card(value, suit):
    card = [value, suit]
    return card

def get_value(card):
    if card_is_valid(card):
        return card[0]
    return None

def get_suit(card):
    if card_is_valid(card):
        return card[1]
    return None

def display_card(card):
    if card_is_valid(card):
        return str(card[0]) + " of " + get_suit_name(card[1])
    return None

def get_suit_name(suitInt):
    return {0: "Hearts",
            1: "Diamonds",
            2: "Spades",
            3: "Clubs"
    }[suitInt]

def card_is_valid(card):
    if isinstance(card, list):
        if len(card) == 2:
            return True
    return False
