#!/usr/bin/env python
# -*- coding: utf-8 -*-
import deck_keygen as deck_handler
import card_keygen as card_handler
#from card_keygen import *
def solitaire_keystream(length_of_key, deck):
    if deck_handler.deck_is_valid(deck):
        times_to_loop = 0
        key = ""
        while (times_to_loop < length_of_key):
            deck = deck_handler.move_jokers(deck) #Moves the jokers according to point 2
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
            top_card_value = card_handler.get_suit(deck[1][0])
            #Gets the value of the top card
            if not isinstance(top_card_value, str):
                letter_value = card_handler.get_value(deck[1][top_card_value+1])
                #Gets the value of the card below the top card as many cards as top_card_value's value
                key += get_letter(letter_value)
                times_to_loop += 1
        return key

def solitaire_encrypt(message, input_deck):
    if deck_handler.deck_is_valid(input_deck):
        deck = input_deck[:]
        deck[1] = input_deck[1][:]
        message = convert_message(message)
        key = solitaire_keystream(len(message), deck)
        message = convert_to_numbers(message)
        key = convert_to_numbers(key)
        final_value = add_two_lists_of_numbers(message, key)
        final_value = convert_to_letters(final_value)
        return final_value

def solitaire_decrypt(secret_message, deck):
    if deck_handler.deck_is_valid(deck):
        secret_message = convert_to_numbers(secret_message)
        key = solitaire_keystream(len(secret_message), deck)
        key = convert_to_numbers(key)
        final_value = subtract_two_lists_of_numbers(secret_message, key)
        final_value = convert_to_letters(final_value)
        return final_value
    

def get_letter(value):
    value += 64
    return_value = chr(value)
    return return_value

def get_value(letter):
    return_value = ord(letter)
    return_value -= 64
    return return_value

def convert_message(message):
    message = message.upper()
    if not message.isalpha():
        chars_to_remove = []
        for i in range(len(message)):
            if not message[i].isalnum():
                chars_to_remove.append(message[i])
        for i in range(len(chars_to_remove)):
            message = message.replace(chars_to_remove[i], "")
    return message

def convert_to_numbers(text):
    return_list = []
    for i in range(len(text)):
        return_list.append(get_value(text[i]))
    return return_list

def convert_to_letters(number_list):
    return_string = ""
    for i in range(len(number_list)):
        return_string += get_letter(number_list[i])
    return return_string

def add_two_lists_of_numbers(list_one, list_two):
    if len(list_one) == len(list_two):
        return_list = []
        for i in range(len(list_one)):
            sum_value = list_one[i] + list_two[i]
            if sum_value > 26:
                sum_value -= 26
            return_list.append(sum_value)
        return return_list

def subtract_two_lists_of_numbers(list_one, list_two):
    if len(list_one) == len(list_two):
        return_list = []
        for i in range(len(list_one)):
            sum_value = list_one[i] - list_two[i]
            if sum_value < 0:
                sum_value += 26
            return_list.append(sum_value)
        return return_list

def TEST(message, x):
    for i in range(x):
        deck = deck_handler.create_deck()
        sm = solitaire_encrypt(message, deck)
        dm = solitaire_decrypt(sm, deck)
        print("ENCRYPT NR", i, "|", sm)
        print("DECRYPT NR", i, "|", dm)
        if not message.upper() == dm:
            print("ERROR")
            print("ERROR")
            print("ERROR")
            print("ERROR")
            print("ERROR")
        else:
            print(i, "WAS SUCCESSFUL")
