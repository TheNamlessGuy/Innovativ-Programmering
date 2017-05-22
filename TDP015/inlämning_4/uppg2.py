def get_node_amount(lines):
    # Calculates the amount of nodes in the "lines" parameter
    temp = set()
    for entry in lines:
        temp.add(entry[0])
        temp.add(entry[1])
    return len(temp)

def calc_values(mod, length):
    # Calculates the true/false values for a specific node.
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

def calc_all_values(node_array):
    # Makes use of the fact that the variables values follow a pattern
    # of doubling the window of True and False.
    # For example:
    #     If we have two variables (a, b), and calculate all their values, we
    #     can see that a will take on the pattern 0, 1, 0, 1
    #     while b will take on the pattern 0, 0, 1, 1
    #     thus doubling the window of 0's and 1's
    #
    # This is used to determine all of the possibilities where
    # a node is or is not part of a graph
    # (where the 0's and 1's represent true or false)
    mod = 1
    vals = {}

    for i in node_array:
        vals[i] = calc_values(mod, len(node_array))
        mod *= 2

    return_array = []
    for value in range(2 ** len(node_array)):
        temp = {}
        for name in node_array:
            temp[name] = vals[name][value]
        return_array.append(temp)

    return return_array

def subgraph(graph):
    # Calculates all the possible values of graphs,
    # and adds them to sets, which are returned.
    all_values = calc_all_values(graph[1])
    return_array = []

    for entry in all_values:
        temp = set()
        for name in graph[1]:
            if (entry[name]):
                temp.add(name)
        return_array.append((graph[0], temp))
    return return_array

lines = {(0, 1), (0, 4), (1, 2), (2, 3), (2, 4), (3, 0)}
g = (get_node_amount(lines), lines)

# Get all the subgraphs for variable "g"
subgraphs = subgraph(g)

# Print all of the subgraphs
for entry in subgraphs:
    if (entry[0] == 0):
        print("(0, EMPTY)") # This looks better
    else:
        print(entry)

# Print the amount of subgraphs available
print("Amount: ", end="")
print(len(subgraphs))
