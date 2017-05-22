from uppg3_classes import Var, Neg, Conj

def calc_values(mod, length):
    length = 2 ** length
    amount = 0
    bool_val = False
    return_array = []
    
    while (amount < length):
        for i in range(mod):
            return_array.append(bool_val)
            amount += 1
        bool_val = not bool_val
    return return_array


def calc_all_values(name_array):
    # Makes use of the fact that the variables values follow a pattern
    # of doubling the window of True and False.
    # For example:
    #     If we have two variables (a, b), and calculate all their values, we
    #     can see that a will take on the pattern 0, 1, 0, 1
    #     while b will take on the pattern 0, 0, 1, 1
    #     thus doubling the window of 0's and 1's
    # This is of course not the only truth, but it is true nonetheless

    mod = 1
    vals = {}

    for i in name_array:
        vals[i] = calc_values(mod, len(name_array))
        mod *= 2

    return_array = []
    for value in range(2 ** len(name_array)):
        temp = {}
        for name in name_array:
            temp[name] = vals[name][value]
        return_array.append(temp)
    
    return return_array

def can_get_true(network):
    val_array = calc_all_values(network.get_vars())
    
    for entry in val_array:
        print(entry, end=" => ")
        print(network.value(entry))
        if (network.value(entry) == True):
            print("Network can get a value of True!")
            return
    print ("Network can only get a value of False!")

if __name__ == "__main__":
    c1 = Conj(Var("a"), Var("b"))
    c2 = Conj(Var("c"), Neg(c1))

    network = Conj(c2, Var("d")) # (c && !(a and b)) && d

    can_get_true(network)
