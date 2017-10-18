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
#include <string.h>
#include <assert.h>

static void output_result(uint32_t n)
{
    printf("%u card(s)\n", n);
}

static double calculate(uint32_t n)
{
    double r = 0.0;
    for (uint32_t i = 1; i <= n; ++i) {
        r += 1.0 / (i + 1);
    }
    
    return r;
}

static void guess_card_number()
{
    char str_length[4] = { '\0' };
    while (1) {
        if (scanf("%s", str_length) == EOF) break;
        if (strcmp(str_length, "0.00") == 0) break;
        
        double length = atof(str_length);
        if (length <= 0.5) {
            // minimum length is 0.5 for one card
            // so any number no greater than 0.5 must be one card
            output_result(1);
        }
        else {
            // take a guess since 2 cards
            uint32_t guess = 2;
            uint32_t prev_guess = 1;
            
            while (1) {
                double guess_length = calculate(guess);
                if (guess_length == length) {
                    break;
                }
                else if (guess_length < length) {
                    prev_guess = guess;
                    guess *= 2;
                }
                else {
                    while (--guess > prev_guess) {
                        guess_length = calculate(guess);
                        if (guess_length < length) {
                            ++guess;
                            break;
                        }
                        else if (guess_length == length) {
                            break;
                        }
                        else {
                            continue;
                        }
                    }

                    if (guess == prev_guess) ++guess;

                    break;
                }
            }
            output_result(guess);
        }
    }
}

int main()
{
    guess_card_number();
    return 0;
}

