#!/usr/bin/env python
# -*- coding: utf-8 -*-
import deck as deck_handler
import card as card_handler
def solitaire_keystream(length_of_key, deck):
    if deck_handler.deck_is_valid(deck):
        deck = deck_handler.shuffle_deck(deck, len(deck[1])-1) #Shuffles the deck
        times_to_loop = 0
        key = ""
        while (times_to_loop < length_of_key):
            deck = deck_handler.move_jokers(deck) #Moves the jokers according to point 2 and 3
            splitDeckA, deck = deck_handler.split_deck_by_index(deck, deck_handler.get_topmost_joker_index(deck))
            #Splits the deck up to the topmost joker
            deck, splitDeckB = deck_handler.split_deck_by_index_backwards(deck, deck_handler.get_bottommost_joker_index(deck))
            #Splits the deck by the bottommost joker
            deck[1].extend(splitDeckB[1]) #Add the middle split into the bottom split
            deck[1].extend(splitDeckA[1]) #Add the top split into the other splits
            bottom_card_value = card_handler.get_value(deck[1][len(deck[1])-1])
            #Gets the value of the bottom card
            cards_to_move = deck_handler.get_x_cards(deck, bottom_card_value)
            #Gets as many cards as the value of the bottom card
            deck = deck_handler.remove_list_of_cards_from_deck(deck, cards_to_move)
            #Removes the cards in cards_to_move from the deck
            deck = deck_handler.add_list_of_cards_at_index(deck, cards_to_move, len(deck[1])-2)
            #Adds the cards in cards_to_move into the deck right above the bottom card
            top_card_value = card_handler.get_value(deck[1][0])
            #Gets the value of the top card
            if not isinstance(top_card_value, str):
                letter_value = card_handler.get_value(deck[1][top_card_value+1])
                if not isinstance(letter_value, str):
                    #Gets the value of the card below the top card as many cards as top_card_value's value
                    key += get_letter(letter_value)
                    times_to_loop += 1
        return key

def get_letter(value):
    value += 64
    return_value = chr(value)
    return return_value

def TEST(x):
    for i in range(x):
        key = solitaire_keystream(20, deck_handler.create_deck())
        print("KEY IS", key)
