#!/usr/bin/env python
# -*- coding: utf-8 -*-
from functools import reduce
from time import clock
from fractions import gcd
def uppg_3(num):
    answer = 1
    #For every loop, add all the numbers from the range that don't mod 0. While that list's lenth is larger than 0, loop
    while len([x for x in range(1, num+1) if answer % x != 0]) > 0:
        answer += 1
    return answer

def uppg_3_1(num):
    return reduce(lambda x, y: x*y, [get_highest_pow(x, num) for x in range(1, num+1) if is_prime(x)])

def is_prime(n):
    for m in range(2, n):
        if n % m == 0:
            return False
    return True

def get_highest_pow(x, num):
    highest = x
    for i in range(2, num):
        if (x ** i) > num:
            break
        highest = x ** i
    return highest

def lcm(numbers):
    return reduce(lambda x, y: (x * y)/gcd(x, y), numbers, 1)

number = 13
#Process 1 (while) breaks at 16 (16: ~6 seconds, 17: ~72 seconds)
#Process 3 (lcm) breaks at 218 (returns inf, then (>=219) chugs)
#Process 2 (pow) holds over 30.000 (~9 seconds)
#*************************************************************
pretime = clock()
print("PROCESS 1 ANSWER:", uppg_3(number))
posttime = clock()
final_time_1 = posttime - pretime
print("PROCESS TIME:", str(final_time_1), '\n')
#*************************************************************
pretime = clock()
print("PROCESS 2 ANSWER:", uppg_3_1(number))
posttime = clock()
final_time_2 = posttime - pretime
print("PROCESS TIME:", str(final_time_2), '\n')
#*************************************************************
pretime = clock()
print("PROCESS 3 ANSWER:", lcm(range(1, number+1)))
posttime = clock()
final_time_3 = posttime - pretime
print("PROCESS TIME:", str(final_time_3), '\n')
#*************************************************************
lst = [final_time_2, final_time_3]
if min(lst) == final_time_1:
    print("First process was the fastest")
    lst.remove(final_time_1)
    if min(lst) == final_time_2:
        print("Runner-up was the second process by", final_time_2 - final_time_1, "seconds")
    else:
        print("Runner-up was the third process by", final_time_3 - final_time_1, "seconds")
#**************************************************************
if min(lst) == final_time_2:
    print("Second process was the fastest")
    lst.remove(final_time_2)
    if min(lst) == final_time_3:
        print("Runner-up was the third process by", final_time_3 - final_time_2, "seconds")
    else:
        print("Runner-up was the first process by", final_time_1 - final_time_2, "seconds")
#**************************************************************
if min(lst) == final_time_3:
    print("Third process was the fastest")
    lst.remove(final_time_3)
    if min(lst) == final_time_2:
        print("Runner-up was the second process by", final_time_2 - final_time_3, "seconds")
    else:
        print("Runner-up was the first process by", final_time_1 - final_time_3, "seconds")
#**************************************************************