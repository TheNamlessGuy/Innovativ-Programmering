from uppg1 import newton
from math import ceil

def round_off(num, decimals):
    decimals = 10**decimals
    return ceil(num*decimals)/decimals

def total_newton(f, f_prime, x, rounding=10):
    prev = x
    x = newton(f, f_prime, x)
    if (round_off(x, rounding) == round_off(prev, rounding)):
        return round_off(x, rounding)
    else:
        return total_newton(f, f_prime, x, rounding)

if __name__ == "__main__":
    x = -1
    def f(x): return (x**3) - x + 1
    def f_prime(x): return (3*x**2) - 1
    
    print(total_newton(f, f_prime, x))
