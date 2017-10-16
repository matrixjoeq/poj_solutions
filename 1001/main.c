/*
 *==================================================================================
 * Description
 *
 * Problems involving the computation of exact values of very large magnitude 
 * and precision are common. For example, the computation of the national debt
 * is a taxing experience for many computer systems. 
 *
 * This problem requires that you write a program to compute the exact value of
 * Rn where R is a real number ( 0.0 < R < 99.999 ) and n is an integer such 
 * that 0 < n <= 25.
 *
 * Input
 *
 * The input will consist of a set of pairs of values for R and n. The R value 
 * will occupy columns 1 through 6, and the n value will be in columns 8 and 9.
 *
 * Output
 *
 * The output will consist of one line for each line of input giving the exact
 * value of R^n. Leading zeros should be suppressed in the output. Insignificant
 * trailing zeros must not be printed. Don't print the decimal point if the result
 * is an integer.
 *==================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#define SAFE_RELEASE(x) \
    do { \
        if ((x)) { \
            free((x)); \
            (x) = 0; \
        } \
    } while (0)

#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

typedef unsigned short digit;
typedef unsigned long twodigits;

typedef struct __big_number_t {
    size_t num_of_digits;
    size_t point_shift; // float point shift counting from back to front 
                        // (e.g. for 1.01, shift = 2)
    bool negative;
    digit digits[1];
} big_number_t;

static void bn_print(big_number_t* bn)
{
    if (!bn) return;

    size_t size = max(bn->num_of_digits, bn->point_shift) + (bn->point_shift ? 1 : 0);
    char* repr = (char*) malloc(size + 1);
    if (!repr) return;

    size_t i = bn->num_of_digits;
    char* p = repr;

    if (bn->num_of_digits < bn->point_shift) {
        *p++ = '.';
        size_t num_of_heading_zero = bn->point_shift - bn->num_of_digits;
        while (num_of_heading_zero-- > 0) *p++ = '0';
        while (i > 0) {
            *p++ = '0' + bn->digits[i - 1];
            --i;
        }
    }
    else {
        while (i > 0) {
            *p++ = '0' + bn->digits[i - 1];
            --i;
            if (i == bn->point_shift) *p++ = '.';
        }
    }

    // remove trailing zeros for fraction part
    if (bn->point_shift) {
        while (*--p == '0');
        if (*p == '.') --p;
        ++p;
    }
    *p = '\0';
    printf("%s%s\n", bn->negative ? "-" : "", repr);

    SAFE_RELEASE(repr);
}

static big_number_t* create_big_number(size_t size)
{
    if (size <= 0) return 0;

    big_number_t* bn = (big_number_t*) malloc(
        sizeof(big_number_t) + sizeof(digit) * (size - 1));
    if (!bn) return 0;

    bn->num_of_digits = size;
    bn->point_shift = 0;
    bn->negative = false;
    memset(bn->digits, 0, sizeof(digit) * size);
    return bn;
}

static big_number_t* create_big_number_from_long(long n, size_t shift)
{
    if (n == 0) {
        return create_big_number(1);
    }

    bool negative = (n < 0);
    n = abs(n);
    size_t size = (size_t)(floor(log10(n)) + 1);
    big_number_t* bn = create_big_number(size);
    if (!bn) return 0;

    bn->negative = negative;
    bn->point_shift = shift;
    digit* p = bn->digits;
    size_t i = size;
    while (i > 0) {
        assert(p < (bn->digits + size));
        *p++ = n % 10;
        n /= 10;
        --i;
    }

    return bn;
}

static big_number_t* bn_copy(big_number_t* x)
{
    if (!x) return 0;
    
    big_number_t* bn = create_big_number(x->num_of_digits);
    if (!bn) return 0;

    bn->negative = x->negative;
    bn->point_shift = x->point_shift;
    memcpy(bn->digits, x->digits, sizeof(digit) * bn->num_of_digits);
    return bn;
}

static bool bn_is_zero(big_number_t* x)
{
    if (!x) return false;
    return (x->num_of_digits == 1 && x->digits[0] == 0);
}

static big_number_t* bn_mul(big_number_t* x, big_number_t* y)
{
    if (!x || !y) return 0;
    if (x->num_of_digits == 0 || y->num_of_digits == 0) return 0;
    if (bn_is_zero(x) || bn_is_zero(y)) return create_big_number_from_long((long)0, 0);

    size_t size = x->num_of_digits + y->num_of_digits;
    big_number_t* z = create_big_number(size);
    if (!z) return 0;

    size_t i;
    for (i = 0; i < x->num_of_digits; ++i) {
        twodigits carry = 0;
        twodigits f = x->digits[i];
        digit* pz = z->digits + i;
        digit* py = y->digits;
        digit* py_end = py + y->num_of_digits;

        while (py < py_end) {
            carry += *pz + *py++ * f;
            *pz++ = (digit)(carry % 10);
            carry /= 10;
        }

        if (carry) {
            *pz += (digit)(carry);
        }
    }

    // remove heading zeros
    i = size;
    while (i > 0 && z->digits[i - 1] == 0) {
        --i;
    }
    if (i != size) {
        z->num_of_digits = i;
    }

    // setting new shift
    z->point_shift = x->point_shift + y->point_shift;

    return z;
}

static big_number_t* bn_pow(big_number_t* base, long power)
{
    if (!base) return 0;
    if (power == 0) return create_big_number_from_long((long)1, 0);
    if (power == 1) return bn_copy(base);

    big_number_t* b = bn_copy(base);
    big_number_t* r = create_big_number_from_long((long)1, 0);
    while (power != 0) {
        if (power & 1) {
            big_number_t* tmp = bn_mul(r, b);
            SAFE_RELEASE(r);
            r = tmp;
        }
        big_number_t* tmp = bn_mul(b, b);
        SAFE_RELEASE(b);
        b = tmp;
        power /= 2;
    }
    SAFE_RELEASE(b);
    return r;
}

static void conv_str_to_decimal(char* str, int* decimal, size_t* shift)
{
    if (!str || !decimal || !shift) return;

    char* str_decimal = (char*) malloc(strlen(str) + 1);
    if (!str_decimal) return;

    memset(str_decimal, 0, strlen(str) + 1);
    char* c = str;
    char* p = str_decimal;
    while (c < (str + strlen(str))) {
        if (*c == '.') {
            *shift = strlen(str) - (c - str) - 1;
        }
        else {
            *p++ = *c;
        }
        ++c;
    }
    *decimal = atoi(str_decimal);
    SAFE_RELEASE(str_decimal);
}

static void calc_pow()
{
    char str_base[7] = { '\0' };
    int decimal = 0;
    size_t shift = 0;
    int power = 0;

    while (scanf("%s %d", str_base, &power) != EOF) {
        conv_str_to_decimal(str_base, &decimal, &shift);
        big_number_t* bn = create_big_number_from_long((long)decimal, shift);
        big_number_t* exp = bn_pow(bn, power);
        bn_print(exp);
        SAFE_RELEASE(bn);
        SAFE_RELEASE(exp);
    }
}

int main()
{
    calc_pow();
    return 0;
}

