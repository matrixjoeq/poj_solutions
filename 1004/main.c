/*
 *==================================================================================
 * [Description]
 *
 * Larry graduated this year and finally has a job. He's making a lot of money, but 
 * somehow never seems to have enough. Larry has decided that he needs to grab hold
 * of his financial portfolio and solve his financing problems. The first step is to
 * figure out what's been going on with his money. Larry has his bank account 
 * statements and wants to see how much money he has. Help Larry by writing a 
 * program to take his closing balance from each of the past twelve months and 
 * calculate his average account balance.
 *
 * [Input]
 *
 * The input will be twelve lines. Each line will contain the closing balance of his
 * bank account for a particular month. Each number will be positive and displayed 
 * to the penny. No dollar sign will be included.
 *
 * [Output]
 *
 * The output will be a single number, the average (mean) of the closing balances 
 * for the twelve months. It will be rounded to the nearest penny, preceded 
 * immediately by a dollar sign, and followed by the end-of-line. There will be no
 * other spaces or characters in the output.
 *==================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

static void mean()
{
    double r = 0.0;
    double balance = 0.0;
    while (scanf("%lf", &balance) != EOF) {
        r += balance;
        //printf("balance = %.2lf, total = %.2lf\n", balance, r);
    }

    r /= 12;
    printf("$%.2lf\n", r);
}

int main()
{
    mean();
    return 0;
}

