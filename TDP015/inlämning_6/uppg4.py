from uppg2 import total_newton

def f(x): return (2*x**2) - (3*x) + 1
def f_prime(x): return (4*x) - 3

x = 0
print("x = 0: ", end="")
print(total_newton(f, f_prime, x))

x = 1
print("x = 1: ", end="")
print(total_newton(f, f_prime, x))
