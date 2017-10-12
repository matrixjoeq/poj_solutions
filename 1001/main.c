#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef unsigned short digit;
typedef unsigned long twodigits;

typedef struct __big_number_t {
    size_t num_of_digits;
    digit digits[1];
} big_number_t;

static big_number_t* create_big_number(size_t size)
{
    if (size <= 0) return 0;

    big_number_t* bn = (big_number_t*) malloc(
        sizeof(big_number_t) + sizeof(digit) * (size - 1));
    if (!bn) return 0;

    bn->num_of_digits = size;
    memset(bn->digits, 0, sizeof(digit) * size);
    return bn;
}

static big_number_t* bn_mul(big_number_t* x, big_number_t* y)
{
    if (!x || !y) return 0;
    if (x->num_of_digits == 0 || y->num_of_digits == 0) return 0;

    size_t size = x->num_of_digits + y->num_of_digits;
    big_number_t* z = create_big_number(size);
    if (!z) return 0;

    size_t i;
    for (i = 0; i < x->num_of_digits; ++i) {
        twodigits carry;
        twodigits f = x->digits[i];
        digit* pz = z->digits + i;
        digit* py = y->digits;
        digit* py_end = py + y->num_of_digits;

        while (py < py_end) {
            carry += ((*pz) + (*py) * (f));
            ++py;

        }
    }

    return z;
}

int64_t fast_pow(int64_t base, int64_t power)
{
    if (power == 0) return 1;
    if (power == 1 || base == 0 || base == 1) return base;

    int64_t r = 1;
    while (power != 0) {
        if (power & 1) {
            r *= base;
        }
        base *= base;
        power /= 2;
    }
    return r;
}

void print_result(double exp)
{
    long decimal = 0;
    double fraction = 0.0;
    if ((exp <= (double)(-1.0)) || (exp >= (double)(1.0))) {
        decimal = (long)exp;
    }

    if (exp != (double)decimal) {
        fraction = exp - decimal;
    }

    if (decimal == 0 && fraction == (double)0.0) {
        printf("\n");
    }
    else {
        if (decimal == 0) {
        }
        else if (fraction == (double)0.0) {
        }
        else {
        }
    }
}

void calc_pow()
{
    char str_base[7] = { '\0' };
    int decimal = 0, fraction = 0;
    int power = 0;

    while (scanf("%s %d", str_base, &power) != EOF) {
        decimal = atoi(str_base);
        int64_t exp = fast_pow(decimal, power);
        //print_result(exp);
        printf("%lld\n", exp);
    }
}

int main()
{
    calc_pow();
    return 0;
}

