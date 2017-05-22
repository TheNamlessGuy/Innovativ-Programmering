from uppg3_classes import *

from uppg4 import calc_all_values

beer = Var("Beer")
fish = Var("Fish")
ice_cream = Var("Ice cream")

expr_A = Impl(Neg(beer), fish) # !beer -> fish
expr_B = Impl(Conj(beer, fish), Neg(ice_cream)) # (beer && fish) -> !ice_cream
expr_C = Impl(Disc(ice_cream, Neg(beer)), Neg(fish)) # (ice_cream || !beer) -> !fish

complex = Conj(Conj(expr_A, expr_B), expr_C) # (expr_A && expr_B) && expr_C
simple = Conj(Neg(Conj(fish, ice_cream)), beer) # !(fish && ice_cream) && beer

val_array = calc_all_values(["Beer", "Fish", "Ice cream"])

print("Complex, Simple")
print("-----------------")
for entry in val_array:
    print(complex.value(entry), end=", ")
    print(simple.value(entry))

    if not Equi(complex, simple).value(entry):
        print("\nExpressions are not equivalent")
        exit(0)

print("\nExpressions are equivalent")