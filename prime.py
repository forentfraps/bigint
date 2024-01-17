from random import randrange, getrandbits 
from math import sqrt
def is_prime(n, k=128):
    """ Test if a number is prime        Args:
            n -- int -- the number to test
            k -- int -- the number of tests to do        return True if n is prime
    """
    # find r and s
    s = 0
    r = n - 1
    while r & 1 == 0:
        s += 1
        r //= 2
    print(r, s)
    # do k tests
    for _ in range(k):
        a = randrange(2, n - 1)
        x = pow(a, r, n)
        if x != 1 and x != n - 1:
            j = 1
            while j < s and x != n - 1:
                x = pow(x, 2, n)
                if x == 1:
                    return False
                j += 1
            if x != n - 1:
                return False
    return True
def generate_prime_candidate(length):
    """ Generate an odd integer randomly        Args:
            length -- int -- the length of the number to generate, in bits        return a integer
    """
    # generate random bits
    p = getrandbits(length)
    # apply a mask to set MSB and LSB to 1
    p |= (1 << length - 1) | 1
    return p
def generate_prime_number(length=1024):
    """ Generate a prime        Args:
            length -- int -- length of the prime to generate, in          bits        return a prime
    """
    p = 4
    # keep generating while the primality test fail
    while not is_prime(p, 128):
        p = generate_prime_candidate(length)
    return p
def gcdExtended(a, b):
    # Base Case
    if a == 0 :
        return b,0,1
    gcd,x1,y1 = gcdExtended(b%a, a)

    # Update x and y using results of recursive
    # call
    x = y1 - (b//a) * x1
    y = x1

    return gcd,x,y

class MontgomeryNumber:
    def __init__(self, value, modulus):
        self.modulus = modulus
        self.R = 2 ** 64
        self.Rinv = pow(self.R, -1, modulus)  # Modular inverse of R modulo N
        self.Nprime = -pow(modulus, -1, self.R) % self.R  # -N^-1 mod R

        # Convert value to Montgomery form
        self.montgomery_value = (value * self.R) % self.modulus

    def montgomery_reduce(self, T):
        """Performs the Montgomery Reduction on T."""
        print("value to reduce")
        print(hex(T))
        m = (T * self.Nprime) % self.R
        print("m")
        print(hex(m))
        t = ( m * self.modulus)
        print("multiplication")
        print(hex(t))
        print("addition")
        t += T
        print(hex(t))
        print("t small before mod")
        print(hex(t))
        t >>= 64
        print("t after modr")
        print(hex(t))
        if t >= self.modulus:
            print("path is taken to reduce")
            t -= self.modulus
        return t

    def __str__(self):
        return (f"Value in Montgomery Form: {hex(self.montgomery_value)}\n")

def montgomery_multiply(a, b):
    """Multiply two Montgomery Numbers."""
    T = a.montgomery_value * b.montgomery_value
    print("Temp multiplication is:")
    print(hex(T))

    return a.montgomery_reduce(T)
    # return a
# Example usage
# m1 = MontgomeryNumber(251, 131)
# m2 = MontgomeryNumber(115, 131)
# print(m1)
# print(m2)
# print(hex(montgomery_multiply(m1, m2).montgomery_value))

# Driver code
# a = 0x57e8fa0952c6cc76ef6c4ce9d3458852348b996a31a5af158c0f2f8ab026eb31
# b = 0x7d4579e9545cdbeec8371b03ac12b6861e261a8a373fb88dab547860cf71d5e5
# g, x, y = gcdExtended(b, a) 
# print(hex(x))
# print(hex(y % 2**512))
# print(generate_prime_number(2048))
a = 0x0000000000000000c103f70ea9efd2ab
b = 0x00000000000000007e3ee438fb2c71eb
c = 0x00000000000000006eccd7dadc525e3f

m1 = MontgomeryNumber(a, c)
m2 = MontgomeryNumber(b, c)
# print (hex(pow(a, b, c)))
print(m1)
print(m2)

m3 = montgomery_multiply(m1, m2)
print("multiplication in montgom")
print(hex(m3))

print(hex(m1.montgomery_reduce(m3)))