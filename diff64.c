/*
 * Differentiate two numbers to see which bits are different
 *
 * Written by: Gaspar Kolumban
 * Date: 2019-03-16
 */


/* **** Includes **** */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

/* **** Defines **** */

#define MAX_COMPARE_LENGTH        (64u)
#define DEFAULT_RESULT_LENGTH     (32u)
#define ELEMENT_LENGTH            (3u)

#if DEFAULT_RESULT_LENGTH > MAX_COMPARE_LENGTH
#error You greedy bastard
#endif

#define ERROR_OPERAND_INVALID         (1)
#define ERROR_RESULT_LENGTH_INVALID   (2)

/* **** Static declarations **** */

static void print_help(const char *command);
static uint64_t parse_number(const char *number);
static uint32_t parse_result_length(const char *number);
static uint32_t get_bit(const uint64_t number, const uint32_t index);
static void differentiate(const uint64_t left_number, const uint64_t right_number, const uint32_t result_length);

/* **** Static functions **** */

static void print_help(const char *command)
{
    printf("Usage: %s <operand 1> <operand 2> [result length]\n\n", command);
    printf("All values can be given as decimal, hexadecimal or octal values.\n");
    printf("Result length is a number between 1 and %u.\n\n", MAX_COMPARE_LENGTH);
    printf("Example: %s 0x1337 0x1338 15\n", command);
}

static uint64_t parse_number(const char *number)
{
    const uint64_t retval = strtoul(number, NULL, 0);

    if (ERANGE == errno)
    {
        printf("Number is out of range: %s\n", number);
        exit(ERROR_OPERAND_INVALID);
    }

    return retval;
}

static uint32_t parse_result_length(const char *number)
{
    const uint32_t result_length = parse_number(number);

    if ((MAX_COMPARE_LENGTH < result_length) || (result_length == 0u))
    {
        printf("Invalid result length: %d\n", result_length);
        exit(ERROR_RESULT_LENGTH_INVALID);
    }

    return result_length;
}

static uint32_t get_bit(const uint64_t number, const uint32_t index)
{
    return ((number >> index) & 1uLL);
}

static void differentiate(const uint64_t left_number, const uint64_t right_number, const uint32_t result_length)
{
    char legend_result[(MAX_COMPARE_LENGTH * ELEMENT_LENGTH) + 1u] = {0};
    char upper_result[(MAX_COMPARE_LENGTH * ELEMENT_LENGTH) + 1u]  = {0};
    char middle_result[(MAX_COMPARE_LENGTH * ELEMENT_LENGTH) + 1u] = {0};
    char lower_result[(MAX_COMPARE_LENGTH * ELEMENT_LENGTH) + 1u]  = {0};

    for (int32_t i = (result_length - 1), j = 0; i >= 0; --i, ++j)
    {
        const uint32_t result_index = j * ELEMENT_LENGTH;
        const uint32_t write_size   = ELEMENT_LENGTH + 1u; // snprintf will add null termination
        const uint32_t left_bit     = get_bit(left_number, i);
        const uint32_t right_bit    = get_bit(right_number, i);

        snprintf(&(legend_result[result_index]), write_size, "%*u", ELEMENT_LENGTH, i);
        snprintf(&(upper_result[result_index]),  write_size, "%*u", ELEMENT_LENGTH, left_bit);
        snprintf(&(middle_result[result_index]), write_size, "%*c", ELEMENT_LENGTH, (left_bit != right_bit) ? '|' : ' ');
        snprintf(&(lower_result[result_index]),  write_size, "%*u", ELEMENT_LENGTH, right_bit);
    }

    printf("%s\n", legend_result);
    printf("%s\n", upper_result);
    printf("%s\n", middle_result);
    printf("%s\n", lower_result);
}

/* **** Main **** */

int main(const int argc, const char *argv[])
{
    if (3 == argc)
    {
        differentiate(parse_number(argv[1]), parse_number(argv[2]), DEFAULT_RESULT_LENGTH);
    }
    else if (4 == argc)
    {
        differentiate(parse_number(argv[1]), parse_number(argv[2]), parse_result_length(argv[3]));
    }
    else
    {
        print_help(argv[0]);
    }

    return 0;
}
