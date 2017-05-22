from random import randint

def random_tree(max_depth=5, depth=0):
    math_signs = ['+', '-', '*', '/']
    tree = ()

    if (randint(0, 1) == 1 and depth < max_depth):
        tree = tree + (random_tree(max_depth, depth+1),)
    else:
        tree = tree + (str(randint(0, 9)),)

    tree = tree + (math_signs[randint(0, 3)],)

    if (randint(0, 1) == 1 and depth < max_depth):
        tree = tree + (random_tree(max_depth, depth+1),)
    else:
        tree = tree + (str(randint(0, 9)),)

    return tree

def pre_order_traverse(tree):
    print(tree[1], end="")
    if (type(tree[0]) == str):
        print(tree[0], end="")
    else:
        pre_order_traverse(tree[0])

    if (type(tree[2]) == str):
        print(tree[2], end="")
    else:
        pre_order_traverse(tree[2])

def post_order_traverse(tree):
    if (type(tree[0]) == str):
        print(tree[0], end="")
    else:
        post_order_traverse(tree[0])

    if (type(tree[2]) == str):
        print(tree[2], end="")
    else:
        post_order_traverse(tree[2])

    print(tree[1], end="")

def in_order_traverse(tree):
    if (type(tree[0]) == str):
        print(tree[0], end="")
    else:
        in_order_traverse(tree[0])

    print(tree[1], end="")

    if (type(tree[2]) == str):
        print(tree[2], end="")
    else:
        in_order_traverse(tree[2])

tree = random_tree(3)
print("TREE: ", end="")
print(tree, end="\n\n")

print("PRE-ORDER TRAVERSE:")
pre_order_traverse(tree)
print("\n")

print("POST-ORDER TRAVERSE:")
post_order_traverse(tree)
print("\n")

print("IN-ORDER TRAVERSE:")
in_order_traverse(tree)
print()