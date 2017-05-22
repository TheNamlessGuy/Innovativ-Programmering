from uppg3_classes import *

a = Var("a")
b = Var("b")

neg_a = Neg(a)
neg_b = Neg(b)

conj = Conj(a, b)

print("***** CONJ WITH DIRECT VARS *****")

print("True && True: ", end="")
print(conj.value({"a": True, "b": True}), end="\n\n")

print("True && False: ", end="")
print(conj.value({"a": True, "b": False}), end="\n\n")

print("False && True: ", end="")
print(conj.value({"a": False, "b": True}), end="\n\n")

print("False && False: ", end="")
print(conj.value({"a": False, "b": False}), end="\n\n")

conj = Conj(neg_a, b)

print("***** CONJ WITH ONE NEG *****")

print("!True && True: ", end="")
print(conj.value({"a": True, "b": True}), end="\n\n")

print("!True && False: ", end="")
print(conj.value({"a": True, "b": False}), end="\n\n")

print("!False && True: ", end="")
print(conj.value({"a": False, "b": True}), end="\n\n")

print("!False && False: ", end="")
print(conj.value({"a": False, "b": False}), end="\n\n")

disc = Disc(a, b)

print("***** DISC WITH DIRECT VARS *****")

print("True || True: ", end="")
print(disc.value({"a": True, "b": True}), end="\n\n")

print("True || False: ", end="")
print(disc.value({"a": True, "b": False}), end="\n\n")

print("False || True: ", end="")
print(disc.value({"a": False, "b": True}), end="\n\n")

print("False || False: ", end="")
print(disc.value({"a": False, "b": False}), end="\n\n")

disc = Disc(a, neg_b)

print("***** DISC WITH ONE NEG *****")

print("True || !True: ", end="")
print(disc.value({"a": True, "b": True}), end="\n\n")

print("True || !False: ", end="")
print(disc.value({"a": True, "b": False}), end="\n\n")

print("False || !True: ", end="")
print(disc.value({"a": False, "b": True}), end="\n\n")

print("False || !False: ", end="")
print(disc.value({"a": False, "b": False}), end="\n\n")

conj = Conj(neg_a, b)
disc = Disc(a, conj)

print("***** DISC WITH ONE CONJ *****")

print("True || (!True && True): ", end="")
print(disc.value({"a": True, "b": True}), end="\n\n")

print("True || (!True && False): ", end="")
print(disc.value({"a": True, "b": False}), end="\n\n")

print("False || (!False && True): ", end="")
print(disc.value({"a": False, "b": True}), end="\n\n")

print("False || (!False && False): ", end="")
print(disc.value({"a": False, "b": False}), end="\n\n")

impl = Impl(a, b)

print("***** IMPL WITH DIRECT VARS *****")

print("True -> True: ", end="")
print(impl.value({"a": True, "b": True}), end="\n\n")

print("True -> False: ", end="")
print(impl.value({"a": True, "b": False}), end="\n\n")

print("False -> True: ", end="")
print(impl.value({"a": False, "b": True}), end="\n\n")

print("False -> False: ", end="")
print(impl.value({"a": False, "b": False}), end="\n\n")

equi = Equi(a, b)

print("***** EQUI WITH DIRECT VARS *****")

print("True <-> True: ", end="")
print(equi.value({"a": True, "b": True}), end="\n\n")

print("True <-> False: ", end="")
print(equi.value({"a": True, "b": False}), end="\n\n")

print("False <-> True: ", end="")
print(equi.value({"a": False, "b": True}), end="\n\n")

print("False <-> False: ", end="")
print(equi.value({"a": False, "b": False}), end="\n\n")
