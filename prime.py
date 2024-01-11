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
        self.value = value
        self.modulus = modulus
        self.R = 2**256  # R is a power of 2 just larger than modulus
        self.Rinv = pow(self.R, -1, modulus)  # Modular inverse of R modulo N
        self.Nprime = -pow(modulus, -1, self.R) % self.R  # -N^-1 mod R

        # Convert value to Montgomery form
        self.montgomery_value = (self.value * self.R) % self.modulus

    def __str__(self):
        return (f"Value: {hex(self.value)}\n"
                f"Montgomery Value: {hex(self.montgomery_value)}\n"
                f"Modulus: {hex(self.modulus)}\n"
                f"R: {hex(self.R)}\n"
                f"Rinv: {hex(self.Rinv)}\n"
                f"Nprime: {hex(self.Nprime)}")

# Example usage
montgomery_number = MontgomeryNumber(251, 131)
print(montgomery_number)

# Driver code
# a = 0x57e8fa0952c6cc76ef6c4ce9d3458852348b996a31a5af158c0f2f8ab026eb31
# b = 0x7d4579e9545cdbeec8371b03ac12b6861e261a8a373fb88dab547860cf71d5e5
# g, x, y = gcdExtended(b, a) 
# print(hex(x))
# print(hex(y % 2**512))
# print(generate_prime_number(2048))
