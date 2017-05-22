from uppg2 import total_newton

def f(x): return (x**5) - 5
def f_prime(x): return (5*x**4)
x = 1

print("Fifth root of 5: ", end="")
print(total_newton(f, f_prime, x))
