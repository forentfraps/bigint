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
        self.R = 2 ** (3072)
        self.Rinv = pow(self.R, -1, modulus)  # Modular inverse of R modulo N
        self.Nprime = -pow(modulus, -1, self.R) % self.R  # -N^-1 mod R

        # Convert value to Montgomery form
        self.montgomery_value = (value * self.R) % self.modulus

    def montgomery_reduce(self, T):
        """Performs the Montgomery Reduction on T."""
        # print("[R] value to reduce")
        # print(hex(T))
        m = (T * self.Nprime) % self.R
        # print("[R] m")
        # print(hex(m))
        t = ( m * self.modulus)
        # print("[R] multiplication")
        # print(hex(t))
        # print("[R] addition")
        t += T
        # print(hex(t))
        # print("[R] t small before mod")
        # print(hex(t))
        t >>= (3072)
        # print("[R] t after modr")
        # print(hex(t))
        if t >= self.modulus:
            # print("path is taken to reduce")
            t -= self.modulus
        return t

    def __str__(self):
        return (f"Value in Montgomery Form: {hex(self.montgomery_value)}\n")

def montgomery_multiply(a, b):
    """Multiply two Montgomery Numbers."""
    # print("[M] Multiplying this")
    # print(hex(a.montgomery_value))
    # print("[M] and this")
    # print(hex(b.montgomery_value))
    T = a.montgomery_value * b.montgomery_value
    # print("[M] Temp multiplication is:")
    # print(hex(T))

    return a.montgomery_reduce(T)

def montgomery_exponentiation(base, exponent, modulus):
    # Convert the base to Montgomery form
    mont_base = MontgomeryNumber(base, modulus)

    # Initialize the result in Montgomery form (Montgomery representation of 1)
    mont_result = MontgomeryNumber(1, modulus)

    # Iterate over the bits of the exponent
    for bit in bin(exponent)[2:]:  # [2:] to skip the '0b' prefix
        mont_result.montgomery_value = montgomery_multiply(mont_result, mont_result)

        if bit == '1':
            # Montgomery multiply if the bit is 1
            mont_result.montgomery_value = montgomery_multiply(mont_result, mont_base)
        # Square the base for the next iteration

    # Convert the result back to standard form
    return mont_result.montgomery_reduce(mont_result.montgomery_value)

    # return a
# Example usage
# m1 = MontgomeryNumber(251, 133)
# m2 = MontgomeryNumber(107, 133)
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
a = 0x70279d2b681320c46019eb2aa4d25fa6caa293185ef7e4403ee1f1781fe897c6fe99fbd21695e32ba838374174be2694ee1862eca3d4ae0de48aed245f48873ca557226d33434401b66125d7fbd1232628c4ccc084c3c34cc82a4c440e0507fcf491de6ce5a91c7327195993d77152c1fe337435cdcd1e15e009c544c6dddebcac4f2f601b5ae1b50675e7903aeadb2649aebc4a64141142d0f458e7df3da37d15515290914202794d10f56a4900351135d722bcd61f5e03c13f28b57a442c66
b = 0x3b9239880a5dc06e4723d03f4d815239c7c61277f3d2ef8c02770cc3ea0edb702ef62f0aa2d53d2dbe281997932d2fd77505926137174a7ddcce6a2554820ad3b12a8a4cfa7f8dcd01659a49c75fb23cffb43e49a65e409e38dffb673cb8845e77b2c7ab1533f8decf496f54373ed84a0746ee767fb60e390653d5f7848f746540d28eda22e9d68eabdffb41a31c95f91d5431ee2b386527e2c33c8f890cc891e9157b7b27c89999306602662d40e6e6a420084661c215548946c0479940670a
c = 0x0604b84a1b7805f6b910fab15913a43aa14833f1db2d61a7e6c86ba6c5d4fa1a27b5028f0f4acb55974c8634a9712603c4e8a2edbfdc5f7ff25ac611a7cf0d94ef3728fca7f39cf449023b670deeb06dcd763c820df06287ecd54ec76d656f5146c4cc87d5ae1b79daa277d195e63d644415f0c8b40a62fd4e93f2aeafd63febc17f5708208b32a56677ed94f0b97f851b7f7b28322aecbaefadf0070887a32d7542044f557491ea7a82c788c6c9cd035baeb009defd0edcb5806a72f347371b
# a = 251
# b = 107
# c = 133


m1 = MontgomeryNumber(a, c)
m2 = MontgomeryNumber(b, c)


print(m1)
print(m2)

# m3 = montgomery_multiply(m1, m2)
# print("multiplication in montgom")
# print(hex(m3))

print(hex(montgomery_exponentiation(a, b, c)))
print (hex(pow(a, b, c)))