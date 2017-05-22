#!/usr/bin/env python
# -*- coding: utf-8 -*-
def create_deck():
    """Returns a list starting with the word 'deck'
    and ending with a list of the cards"""
    import card as card_handler
    return_value = ['deck']
    deck = []
    for i in range(1, 14): 
        """13 because there are 13 values,
        range 1-14 because create_card returns None for value 0"""
        for j in range(4): #The 4 suits
            deck.append(card_handler.create_card(i, j))
    deck.append(['Joker', 'A'])
    deck.append(['Joker', 'B'])
    return_value.append(deck)
    return return_value

def pick_card(deck):
    import random
    if deck_is_valid(deck):
        rand_card_index = random.randrange(len(deck[1]))-1
        return deck[1][rand_card_index]
    return None

def insert_card(card, deck):
    return insert_card_by_index(card, deck, 0)

def insert_card_by_index(card, deck, index):
    import card as card_handler
    if deck_is_valid(deck):
        if card_handler.card_is_valid(card):
            deck[1].insert(index, card)
            return deck
    return deck
    
                    
def deck_is_valid(deck):
    if isinstance(deck, list):
        if len(deck) == 2:
            if deck[0] == 'deck':
                if isinstance(deck[1], list):
                    return True
    return False

def get_card_index(deck, card):
    import card as card_handler
    if deck_is_valid(deck):
        if card_handler.card_is_valid(card):
            return deck[1].index(card)
    return None

def get_card_by_index(deck, index):
    return deck[1][index]

def move_card(deck, card, new_index):
    import card as card_handler
    if deck_is_valid(deck):
        if card_handler.card_is_valid(card):
            if new_index < len(deck[1]) or new_index > 0:
                old_index = get_card_index(deck, card)
                deck[1].insert(new_index, deck[1].pop(old_index))
                return deck
    return None

def remove_card(deck, index):
    del deck[1][index]
    return deck

def get_x_cards(deck, x):
    returnList = []
    for i in range(x):
        returnList.append(deck[1][i])
    return returnList

def remove_list_of_cards_from_deck(deck, card_list):
    for i in range(len(card_list)):
        deck[1].remove(card_list[i])
    return deck

def add_list_of_cards_at_index(deck, card_list, index):
    returnDeck = deck[:]
    for i in range(len(card_list)):
        returnDeck = insert_card_by_index(card_list[i], deck, index)
    return returnDeck

def shuffle_deck(deck, looptimes):
    if not (looptimes > len(deck[1])):
        import random
        moved_cards = []
        number_of_loops = 0
        while (number_of_loops < looptimes):
            random_card = pick_card(deck)
            if not random_card in moved_cards:
                moved_cards.append(random_card)
                rand_new_index = random.randint(0, len(deck[1]))
                deck = move_card(deck, random_card, rand_new_index)
                number_of_loops += 1
        return deck
    return None

def get_topmost_joker_index(deck):
    for i in range(len(deck[1])):
        if deck[1][i] == ['Joker', 'A'] or deck[1][i] == ['Joker', 'B']:
            return i
    return None

def get_bottommost_joker_index(deck):
    for i in range(len(deck[1])-1, 0, -1):
        if deck[1][i] == ['Joker', 'A'] or deck[1][i] == ['Joker', 'B']:
            return i
    return None

def split_deck_by_index(deck, index):
    splitDeck = deck[:]
    splitDeck[1] = deck[1][:index]
    deck[1] = deck[1][index:]
    return splitDeck, deck

def split_deck_by_index_backwards(deck, index):
    splitDeck = deck[:]
    index += 1
    splitDeck[1] = deck[1][index:]
    deck[1] = deck[1][:index]
    return splitDeck, deck

def move_jokers(deck):
    joker_a_index = get_card_index(deck, ['Joker', 'A'])
    joker_a_new_index = joker_a_index + 1
    joker_b_index = get_card_index(deck, ['Joker', 'B'])
    joker_b_new_index = joker_b_index + 2
    if joker_a_index == (len(deck[1])-1):
        joker_a_new_index = 1
    if joker_b_index == (len(deck[1])-1):
        joker_b_new_index = 2
    if joker_b_index == (len(deck[1])-2):
        joker_b_new_index = 1
    deck = move_card(deck, ['Joker', 'A'], joker_a_new_index)
    #Moves Joker A down one step
    deck = move_card(deck, ['Joker', 'B'], joker_b_new_index)
    #Moves Joker B down 2 steps
    return deck
