/*
 *=======================================================================================
 * [Description]
 *
 * Businesses like to have memorable telephone numbers. One way to make a telephone
 * number memorable is to have it spell a memorable word or phrase. For example, 
 * you can call the University of Waterloo by dialing the memorable TUT-GLOP. 
 * Sometimes only part of the number is used to spell a word. When you get back to 
 * your hotel tonight you can order a pizza from Gino's by dialing 310-GINO. 
 * Another way to make a telephone number memorable is to group the digits in a 
 * memorable way. You could order your pizza from Pizza Hut by calling their 
 * ``three tens'' number 3-10-10-10. 
 *
 * The standard form of a telephone number is seven decimal digits with a hyphen
 * between the third and fourth digits (e.g. 888-1200). The keypad of a phone 
 * supplies the mapping of letters to numbers, as follows: 
 *
 * A, B, and C map to 2 
 * D, E, and F map to 3 
 * G, H, and I map to 4 
 * J, K, and L map to 5 
 * M, N, and O map to 6 
 * P, R, and S map to 7 
 * T, U, and V map to 8 
 * W, X, and Y map to 9 
 *
 * There is no mapping for Q or Z. Hyphens are not dialed, and can be added and 
 * removed as necessary. The standard form of TUT-GLOP is 888-4567, the standard
 * form of 310-GINO is 310-4466, and the standard form of 3-10-10-10 is 310-1010. 
 *
 * Two telephone numbers are equivalent if they have the same standard form. 
 * (They dial the same number.) 
 *
 * Your company is compiling a directory of telephone numbers from local businesses.
 * As part of the quality control process you want to check that no two (or more)
 * businesses in the directory have the same telephone number. 
 *
 * [Input]
 *
 * The input will consist of one case. The first line of the input specifies the
 * number of telephone numbers in the directory (up to 100,000) as a positive integer
 * alone on the line. The remaining lines list the telephone numbers in the directory,
 * with each number alone on a line. Each telephone number consists of a string composed
 * of decimal digits, uppercase letters (excluding Q and Z) and hyphens. 
 * Exactly seven of the characters in the string will be digits or letters. 
 *
 *
 * [Output]
 * Generate a line of output for each telephone number that appears more than once in
 * any form. The line should give the telephone number in standard form, followed by a
 * space, followed by the number of times the telephone number appears in the directory.
 * Arrange the output lines by telephone number in ascending lexicographical order. 
 * If there are no duplicates in the input print the line: 
 *
 * No duplicates. 
 *=======================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#define SAFE_RELEASE(x) \
    do { \
        if ((x)) { \
            free((x)); \
            (x) = 0; \
        } \
    } while (0)

typedef struct __dup_num {
    uint32_t number;
    uint32_t count;
} dup_num_t;

typedef struct __slist {
    struct __slist* next;
    void* data;
} slist_t;

static int compare(const void* x, const void* y)
{
    const uint32_t* _x = (const uint32_t*)x;
    const uint32_t* _y = (const uint32_t*)y;

    return (*_x < *_y ? -1 : 
            *_x == *_y ?  0 : 1);
}

static void output_duplicated_numbers(slist_t* list)
{
    if (!list) {
        printf("No duplicates\n");
        return;
    }

    slist_t* l = list;
    while (l) {
        dup_num_t* p = (dup_num_t*)(l->data);
        assert(p && "duplicate number is NULL");
        printf("%03u-%04u %u\n", p->number / 10000, p->number % 10000, p->count);
        l = l->next;
    }
}

static uint32_t normalize_number(char* str_number)
{
    if (!str_number) return 0;

    char str_normal[8] = { '\0' };
    char* p = str_number;
    char* s = str_normal;
    while (p < (str_number + strlen(str_number))) {
        switch (*p) {
            case '-' :
            {
                break;
            }
            case 'A' :
            case 'B' :
            case 'C' :
            {
                *s++ = '2';
                break;
            }
            case 'D' :
            case 'E' :
            case 'F' :
            {
                *s++ = '3';
                break;
            }
            case 'G' :
            case 'H' :
            case 'I' :
            {
                *s++ = '4';
                break;
            }
            case 'J' :
            case 'K' :
            case 'L' :
            {
                *s++ = '5';
                break;
            }
            case 'M' :
            case 'N' :
            case 'O' :
            {
                *s++ = '6';
                break;
            }
            case 'P' :
            case 'R' :
            case 'S' :
            {
                *s++ = '7';
                break;
            }
            case 'T' :
            case 'U' :
            case 'V' :
            {
                *s++ = '8';
                break;
            }
            case 'W' :
            case 'X' :
            case 'Y' :
            {
                *s++ = '9';
                break;
            }
            default :
            {
                if (*p >= '0' && *p <= '9')
                    *s++ = *p;
                else
                    assert(0 && "illegal character detected");
                break;
            }
        }
        ++p;
    }
    *s = '\0';
    uint32_t r = (uint32_t)atoi(str_normal);
    return r;
}

static slist_t* create_node(size_t data_size)
{
    slist_t* node = (slist_t*) malloc(sizeof(slist_t));
    assert(node && "Failed to allocate memory for list node");

    node->next = 0;
    node->data = malloc(data_size);
    assert(node->data && "Failed to allocate memory for list node data");

    return node;
}

static slist_t* pick_duplicated_numbers(uint32_t* numbers, size_t count)
{
    if (!numbers || count == 0) return 0;

    uint32_t* p = numbers;
    uint32_t cur = *p++;
    slist_t* list = 0;
    slist_t* tail = list;
    while (p < (numbers + count)) {
        if (cur  == *p) {
            if (tail) {
                if (((dup_num_t*)(tail->data))->number == cur)
                    ++(((dup_num_t*)(tail->data))->count);
                else {
                    slist_t* node = create_node(sizeof(dup_num_t));
                    tail->next = node;
                    tail = tail->next;
                    ((dup_num_t*)(tail->data))->number = cur;
                    ((dup_num_t*)(tail->data))->count = 2;
                }
            }
            else {
                tail = create_node(sizeof(dup_num_t));
                ((dup_num_t*)(tail->data))->number = cur;
                ((dup_num_t*)(tail->data))->count = 2;
                list = tail;
            }
        }
        else {
            cur = *p;
        }
        ++p;
    }

    return list;
}

static void dump_numbers(uint32_t* numbers, size_t count)
{
    printf("Numbers:\n");
    while (count--) {
        printf("%d\n", *numbers++);
    }
    printf("\n");
}

static void check_phone_numbers()
{
    uint32_t num_of_lines = 0;
    scanf("%u", &num_of_lines);

    if (num_of_lines < 2) {
        output_duplicated_numbers(0);
        return;
    }

    uint32_t* numbers = (uint32_t*)malloc(sizeof(uint32_t) * num_of_lines);
    assert(numbers && "Failed to allocate memory for input numbers");
    memset(numbers, 0, sizeof(uint32_t) * num_of_lines);

    char str_number[16] = { '\0' };
    uint32_t* p = numbers;
    size_t count = 0;
    while (scanf("%s", str_number) != EOF && count < num_of_lines) {
        *p++ = normalize_number(str_number);
        ++count;
    }

    //dump_numbers(numbers, count);
    qsort(numbers, count, sizeof(uint32_t), compare);
    //dump_numbers(numbers, count);
    slist_t* dup_nums = pick_duplicated_numbers(numbers, count);
    output_duplicated_numbers(dup_nums);

    SAFE_RELEASE(numbers);
    slist_t* l = dup_nums;
    while (l) {
        dup_num_t* p = (dup_num_t*)(l->data);
        SAFE_RELEASE(p);
        slist_t* cur = l;
        l = l->next;
        SAFE_RELEASE(cur);
    }
}

int main()
{
    check_phone_numbers();
    return 0;
}

