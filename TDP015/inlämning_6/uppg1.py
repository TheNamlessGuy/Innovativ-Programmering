def newton(f, f_prime, x):
    return x - (f(x) / f_prime(x))

if __name__ == "__main__":
    def f(x): return (x ** 2) - 2
    def f_prime(x): return (2 * x)

    #answer = 1
    answer = 1.5
    for i in range(5):
        answer = newton(f,f_prime,answer)
        print(answer)
