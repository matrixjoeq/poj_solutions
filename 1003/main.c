/*
 *==================================================================================
 * [Description]
 *
 * How far can you make a stack of cards overhang a table? If you have one card, 
 * you can create a maximum overhang of half a card length. (We're assuming that
 * the cards must be perpendicular to the table.) With two cards you can make the
 * top card overhang the bottom one by half a card length, and the bottom one 
 * overhang the table by a third of a card length, for a total maximum overhang 
 * of 1/2 + 1/3 = 5/6 card lengths. In general you can make n cards overhang by 
 * 1/2 + 1/3 + 1/4 + ... + 1/(n + 1) card lengths, where the top card overhangs 
 * the second by 1/2, the second overhangs tha third by 1/3, the third overhangs
 * the fourth by 1/4, etc., and the bottom card overhangs the table by 1/(n + 1).
 * This is illustrated in the figure below.
 *
 * [Input]
 * 
 * The input consists of one or more test cases, followed by a line containing the
 * number 0.00 that signals the end of the input. Each test case is a single line
 * containing a positive floating-point number c whose value is at least 0.01 and
 * at most 5.20; c will contain exactly three digits.
 *
 * [Output]
 *
 * For each test case, output the minimum number of cards necessary to achieve an 
 * overhang of at least c card lengths. Use the exact output format shown in the 
 * examples.
 *==================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

static void output_result(uint32_t n)
{
    printf("%u card(s)\n", n);
}

static double calculate(uint32_t n)
{
    double r = 0.0;
    for (uint32_t i = 1; i <= n; ++i) {
        r += 1 / (i + 1);
    }
    
    return r;
}

static uint32_t conv_str_to_uint(char s[4])
{
    s[1] = s[2];
    s[2] = s[3];
    s[3] = '\0';
    uint32_t r = (uint32_t) atoi(s);
    return r;
}

static void guess_card_number()
{
    char str_length[4] = { '\0' };
    while (1) {
        if (scanf("%s", str_length) == EOF) break;
        if (strcmp(str_length, "0.00") == 0) break;
        
        uint32_t length = conv_str_to_uint(str_length);
        if (length <= 50) {
            // minimum length is 0.5 for one card, so any number not greater than 0.5 must be one card
            output_result(1u);
        }
        else {
            // take a guess since 2 cards
            uint32_t guess = 2u;
            uint32_t prev_guess = 1u;
            
            while (1) {
                uint32_t guess_length = (uint32_t)(calculate(guess) * 100);
                
            }
        }
    }
}

int main()
{
    guess_card_number();
    return 0;
}
