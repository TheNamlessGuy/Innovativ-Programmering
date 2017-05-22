def list_to_set(lst):
    return_set = set()

    for entry in lst:
        temp = set()
        for entry2 in entry:
            temp.add(entry2)
        return_set.add(frozenset(temp))

    return return_set

def subsets(s):
    # Returns all subsets for inputted array
    # Very slow if length of array exceeds 20ish
    subset_array = [[]]

    for entry in s:
        array = []
        for subset in subset_array:
            array.append(subset + [entry])
        subset_array.extend(array)
    return list_to_set(subset_array)

array = list(range(1, 6))
subset_array = subsets(array)

print("ARRAY: ", end="")
print(array)
print("SUBSET ARRAY: ", end="")
print(sorted(subset_array, key=len))

if (len(subset_array) == 2 ** len(array)):
    print("WORKS!")
else:
    print("BROKEN!")
