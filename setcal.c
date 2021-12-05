#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//_______DEFINITIONS/CONSTANTS__________________________________________________________________________________
#define DEFAULT_LINE_SIZE 30
const char *BANNED_STRINGS[19] = {"complement", "union", "intersect", "minus", "subseteq", "subset", "equals", "reflexive", "symmetric", "antisymmetric", "transitive", "function", "domain", "codomain", "injective", "surjective", "bijective", "true", "false"};
const int NUM_BANNED_STRINGS = 19;
const char VALID_LINE_IDENTIFIERS[4] = {'U', 'S', 'C', 'R'};
const int NUM_VALID_LINE_IDENTIFIERS = 4;
const char *SINGULAR_COMMANDS[10] = {"empty", "card", "complement", "reflexive", "symmetric", "antisymmetric", "transitive", "function", "domain", "codomain"};
const int NUM_SINGULAR_COMMANDS = 10;
const char *BINARY_COMMANDS[6] = {"union", "intersect", "minus", "subseteq", "subset", "equals"};
const int NUM_BINARY_COMMANDS = 6;
const char *TERTIARY_COMMANDS[3] = {"injective", "surjective", "bijective"};
const int NUM_TERTIARY_COMMANDS = 3;
//------STRUCTS-FOR-SETS------------------------------------------------------------------------------
typedef struct
{
    int size;
    int row;
    char **items;
} Set;

//------STRUCRS-FOR-SESSIONS-----------------------------------------------------------------------------
typedef struct
{
    char *left_val;
    char *right_val;
} Session_pair;

typedef struct
{
    int size;
    int row;
    Session_pair *pairs;
} Session;

//______FUNCTION_HEADERS_________________________________________________________________________________
//------UTILS--------------------------------------------------------------------------------------------
// TODO
char **my_split(char string[], char separator, int line_length, int *result_len);
// returns 1 if is end of file, else returns 0
int readline(FILE *file, char *line);
// creates array of lines from file, returns number of lines
int process_file(const char filename[], char **lines_array);
// TODO
void process_rows(char *lines_array[], int line_count, int *is_comman_error);
// TODO
void process_operation(char command_name[], char *lines_array[], int first_line_num, int second_line_num, int third_line_num, int *is_command_error);
void remove_all_chars(char *str, char c);
//------VALIDATORS----------------------------------------------------------------------------------------
// Returns 1 if input is correct, otherwise 0
int validate_user_input(int argc, char const *argv[]);
void subval_universe_chars_max_len(char **universe_array, int universe_size, int *is_error);
int subval_valid_line_identifier(char identifier, int *is_error, int line_num);
void subval_banned_strings(char **splitted_line, int num_items, int *is_error, int line_num);
void subval_char_type(char **splitted_line, int num_items, int *is_error, int line_num);
void subval_same_values_set(char **splitted_line, int num_items, int *is_error, int line_num);
void subval_same_values_session(char **splitted_line, int num_items, int *is_error, int line_num);
void subval_values_from_universe(char **splitted_line, int num_items, int *is_error, char **universe_array, int universe_size, int line_num);
int subval_get_num_params(char *command);
int validate_lines(char *lines_array[], int line_count);
int validate_singular_command(const char first_type, int first_line_number, char *lines_array[]);
int validate_binary_command(const char first_type, int first_line_number, const char second_type, int second_line_number, char *lines_array[]);
int validate_terciary_command(const char first_type, int first_line_number, const char second_type, int second_line_number, const char third_type, int third_line_number, char *lines_array[]);

//------SET-HELPERS------
void set_ctor_from_line_string(Set *set, char *string, int row);
void set_ctor(Set *set, char **items, int row, int items_len);
void set_print_from_array(char **items, int items_count);
void set_dtor(Set *set);
void *set_copy(Set *src, Set *dst);

//------SET-COMMANDS------
int is_empty_com(int first_line_num, char *lines_array[], int *is_command_error);
int card_com(int first_line_num, char *lines_array[], int *is_command_error);
int complement_com(int first_line_num, char *lines_array[], int *is_command_error);
int union_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error);
int intersect_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error);
int minus_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error);
int subseteq_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error);
int subset_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error);
int equals_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error);

//------SESSION-HELPERS-------
void session_init(Session *session, int row);
void session_append(Session *session, Session_pair *pair);
void session_pair_ctor(Session_pair *pair, char *left_val, char *right_val);
void session_pair_dtor(Session_pair *pair);
void session_ctor_from_line_string(Session *session, char *string, int row);
void session_ctor(Session *session, Session_pair *pairs, int row, int size);
void session_print(Session session);
void session_dtor(Session *session);

//------SESSION-COMMANDS------
int reflexive_com(int first_line_num, char *lines_array[], int *is_command_error);
int symmetric_com(int first_line_num, char *lines_array[], int *is_command_error);
int antisymmetric_com(int first_line_num, char *lines_array[], int *is_command_error);
int transitive_com(int first_line_num, char *lines_array[], int *is_command_error);
int function_com(int first_line_num, char *lines_array[], int *is_command_error);
int domain_com(int first_line_num, char *lines_array[], int *is_command_error);
int codomain_com(int first_line_num, char *lines_array[], int *is_command_error);
int injective_com(int first_line_num, int second_line_num, int third_line_num, char *lines_array[], int *is_command_error);
int surjective_com(int first_line_num, int second_line_num, int third_line_num, char *lines_array[], int *is_command_error);
int bijective_com(int first_line_num, int second_line_num, int third_line_num, char *lines_array[], int *is_command_error);

//_______________MAIN_CODE____________________________________________________________________________________

int main(int argc, char const *argv[])
{

    if (validate_user_input(argc, argv))
    {
        char *lines_array[1000];
        int is_command_error = 0;
        // int line_count = process_file("file.txt", lines_array);
        int line_count = process_file(argv[1], lines_array);
        if (line_count == -1) return 1;
        if (validate_lines(lines_array, line_count))
        {
            process_rows(lines_array, line_count, &is_command_error);
            for (int i = 0; i < line_count; i++)
            {
                free(lines_array[i]);
            }
            if (is_command_error)
            {
                return 1;
            }
            return 0;
        }
    }

    return 1;
}

//-------UTILS-------------------------------------------------------------------------------------------------
// returns result, it is fully dynamic allocated
char **my_split(char string[], char separator, int line_length, int *result_len)
{
    int subst_start = 0;  // where should creation of the substring start
    int substs_count = 0; // how many substrings I have (index in result)
    char *substr;
    char **result = malloc(sizeof(char **));
    substr = (char *)malloc(1); // dummy malloc
    for (int i = 0; i <= line_length; i++)
    {
        if ((string[i] == separator) || (string[i] == '\0'))
        {
            substr = (char *)realloc(substr, (i - subst_start) + 1);      // reallocing dummy with size of string +1 for \0 char
            strncpy(substr, &string[subst_start], i - subst_start);       // giving right value to substring
            substr[i - subst_start] = '\0';                               // adding 0 to end of substring
            result[substs_count] = (char *)malloc((i - subst_start) + 1); // allocating result size
            strncpy(result[substs_count], substr, (i - subst_start) + 1); // giving right value to result
            subst_start = i + 1;
            substs_count++;
            result = realloc(result, sizeof(char **) * (substs_count + 1)); // increase space of result
        }
    }
    *result_len = substs_count; //withdrawing length of array
    free(substr);
    return result;
}

// returns 1 if is end of file, else returns 0
int readline(FILE *file, char *line)
{
    int output_length = DEFAULT_LINE_SIZE;
    char ch = getc(file);
    int c_index = 0;
    while ((ch != '\n') && (ch != EOF))
    {
        if (c_index == output_length - 1)
        {
            output_length *= 2;
            line = (char *)realloc(line, output_length);
        }
        line[c_index] = ch;

        c_index++;
        ch = getc(file);
    }
    line[c_index] = '\0';

    if (ch == '\n')
    {
        return 0;
    }
    return 1;
}

// creates array of lines from file, returns line count
int process_file(const char filename[], char **lines_array)
{
    FILE *file;
    int line_count;
    file = fopen(filename, "r");
    for (int i = 0; i < 1002; i++)
    {
        if (i < 1001) {
            lines_array[i] = (char *)malloc(DEFAULT_LINE_SIZE);
            if (readline(file, lines_array[i]))
            {
                line_count = i;
                break;
            }
        }
        else {
            fprintf(stderr, "Soubor nesmi obsahovat vic jak 1000 radku");
            return -1;
        }
    }

    fclose(file);
    return line_count;
}
void process_rows(char *lines_array[], int line_count, int *is_command_error)
{
    for (int i = 0; i < line_count; i++)
    {
        if (lines_array[i][0] == 'C')
        {
            int items_len = 0;
            char **output_com = my_split(lines_array[i], ' ', strlen(lines_array[i]), &items_len);
            output_com = realloc(output_com, 5 * sizeof(char *));
            if (items_len == 3)
            {
                output_com[3] = malloc(sizeof(char *));
                output_com[3] = "_";
                output_com[4] = malloc(sizeof(char *));
                output_com[4] = "_";
            }
            else if (items_len == 4)
            {
                output_com[4] = malloc(sizeof(char *));
                output_com[4] = "_";
            }
            process_operation(output_com[1], lines_array, atoi(output_com[2]), atoi(output_com[3]), atoi(output_com[4]), is_command_error);

            for (int i = 0; i < items_len; i++)
            {
                free(output_com[i]);
            }
            free(output_com);
        }
        else
        {
            printf("%s\n", lines_array[i]);
        }
    }
}

// execute proper function when line contains command
void process_operation(char command_name[], char *lines_array[], int first_line_num, int second_line_num, int third_line_num, int *is_command_error)
{
    //SETS
    if (!strcmp(command_name, "empty"))
    {
        is_empty_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "card"))
    {
        card_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "complement"))
    {
        complement_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "union"))
    {
        union_com(first_line_num, second_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "intersect"))
    {
        intersect_com(first_line_num, second_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "minus"))
    {
        minus_com(first_line_num, second_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "subseteq"))
    {
        subseteq_com(first_line_num, second_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "subset"))
    {
        subset_com(first_line_num, second_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "equals"))
    {
        equals_com(first_line_num, second_line_num, lines_array, is_command_error);
    }
    //SESSIONS
    else if (!strcmp(command_name, "reflexive"))
    {
        reflexive_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "symmetric"))
    {
        symmetric_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "antisymmetric"))
    {
        antisymmetric_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "transitive"))
    {
        transitive_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "function"))
    {
        function_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "domain"))
    {
        domain_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "codomain"))
    {
        codomain_com(first_line_num, lines_array, is_command_error);
    }
    else if (!strcmp(command_name, "injective"))
    {
        if (injective_com(first_line_num, second_line_num, third_line_num, lines_array, is_command_error) == 1)
        {
            printf("true\n");
        }
        else
            printf("false\n");
    }
    else if (!strcmp(command_name, "surjective"))
    {
        if (surjective_com(first_line_num, second_line_num, third_line_num, lines_array, is_command_error) == 1)
        {
            printf("true\n");
        }
        else
            printf("false\n");
    }
    else if (!strcmp(command_name, "bijective"))
    {
        if (bijective_com(first_line_num, second_line_num, third_line_num, lines_array, is_command_error) == 1)
        {
            printf("true\n");
        }
        else
            printf("false\n");
    }
    else
    {
        fprintf(stderr, "Chyba vstupu, spatny nazev fce\n"); // TODO - ukoncit program protoze spatny vstup
        *is_command_error = 1;
    }
}
void remove_all_chars(char *str, char c)
{
    char *pr = str, *pw = str;
    while (*pr)
    {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

//------VALIDATORS------
// Returns 1 if input is correct, otherwise 0
int validate_user_input(int argc, char const *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "Prilis mnoho zadanych argumentu!\n");
        return 0;
    }
    if (argc < 2)
    {
        fprintf(stderr, "Prilis malo zadanych argumentu!\n");
        return 0;
    }
    char buffer[4];
    for (int i = 4; i > 0; i--)
    {
        buffer[4 - i] = argv[1][strlen(argv[1]) - i];
    }
    if (!strcmp(buffer, ".txt") && strlen(argv[1]) > 4)
        return 1;
    fprintf(stderr, "Spatne zadany argument!\n");
    return 0;
}
void subval_universe_chars_max_len(char **universe_array, int universe_size, int *is_error)
{
    for (int i = 1; i < universe_size; i++)
    {
        if (strlen(universe_array[i]) > 30)
        {
            *is_error = 1;
            fprintf(stderr, "Chyba na radku 1: Hodnota v universe '%s' je prilis dlouha \n", universe_array[i]);
        }
    }
}

int subval_valid_line_identifier(char identifier, int *is_error, int line_num)
{
    int is_valid = 0;
    for (int i = 0; i < NUM_VALID_LINE_IDENTIFIERS; i++)
    {
        if (identifier == VALID_LINE_IDENTIFIERS[i])
        {
            is_valid = 1;
        }
    }
    if (is_valid)
    {
        return 1;
    }
    *is_error = 1;
    fprintf(stderr, "Chyba na radku %d: Neplatna hodnota identifikatoru radku '%c' \n", line_num, identifier);
    return 0;
}

void subval_banned_strings(char **splitted_line, int num_items, int *is_error, int line_num)
{
    for (int i = 1; i < num_items; i++)
    {
        for (int j = 0; j < NUM_BANNED_STRINGS; j++)
        {
            if (!strcmp(splitted_line[i], BANNED_STRINGS[j]))
            {
                *is_error = 1;
                fprintf(stderr, "Chyba na radku %d: Bylo pouzito zakazane slovo: '%s' \n", line_num, splitted_line[i]);
            }
        }
    }
}

void subval_char_type(char **splitted_line, int num_items, int *is_error, int line_num)
{
    for (int i = 1; i < num_items; i++)
    {
        for (int j = 0; j < (int)strlen(splitted_line[i]) - 1; j++)
        {
            if (!(
                    (splitted_line[i][j] >= 'a' && splitted_line[i][j] <= 'z') ||
                    (splitted_line[i][j] >= 'A' && splitted_line[i][j] <= 'Z')))
            {
                *is_error = 1;
                fprintf(stderr, "Chyba na radku %d: Prvky musi byt retezce pouze z malych nebo velkych pismen abecedy.\n", line_num);
            }
        }
    }
}

void subval_same_values_set(char **splitted_line, int num_items, int *is_error, int line_num)
{
    for (int i = 1; i < num_items; i++)
    {
        for (int j = i + 1; j < num_items; j++)
        {
            if (!strcmp(splitted_line[i], splitted_line[j]))
            {
                *is_error = 1;
                if (!strcmp(splitted_line[0], "U"))
                {
                    fprintf(stderr, "Chyba na radku %d: Vsechny prvky univerza musi byt ruzne.\n", line_num);
                }
                else
                {
                    fprintf(stderr, "Chyba na radku %d: Vsechny prvky mnoziny musi byt ruzne.\n", line_num);
                }
            }
        }
    }
}

void subval_same_values_session(char **splitted_line, int num_items, int *is_error, int line_num)
{
    for (int i = 1; i < num_items; i += 2)
    {
        for (int j = i + 2; j < num_items; j += 2)
        {
            if (!(strcmp(splitted_line[i], splitted_line[j])) && !(strcmp(splitted_line[i + 1], splitted_line[j + 1])))
            {
                *is_error = 1;
                fprintf(stderr, "Chyba na radku %d: Vsechny prvky relace musi byt ruzne.\n", line_num);
            }
        }
    }
}

void subval_values_from_universe(char **splitted_line, int num_items, int *is_error, char **universe_array, int universe_size, int line_num)
{
    int is_in_uni;
    for (int i = 1; i < num_items; i++)
    {
        is_in_uni = 0;
        for (int j = 1; j < universe_size; j++)
        {
            printf("%s - %s\n", universe_array[j], splitted_line[i]);

            if (!strcmp(splitted_line[i], universe_array[j]))
            {
                is_in_uni = 1;
                break;
            }
        }
        if (!is_in_uni)
        {
            *is_error = 1;
            fprintf(stderr, "Chyba na radku %d: Prvek %s neni z universe, num %d\n", line_num, splitted_line[i], i);
        }
    }
}
int subval_get_num_params(char *command)
{
    int num_params = 0;
    for (int i = 0; i < NUM_SINGULAR_COMMANDS; i++)
    {
        if (!strcmp(command, SINGULAR_COMMANDS[i]))
        {
            num_params = 1;
            break;
        }
    }
    if (!num_params)
    {
        for (int i = 0; i < NUM_BINARY_COMMANDS; i++)
        {
            if (!strcmp(command, BINARY_COMMANDS[i]))
            {
                num_params = 2;
                break;
            }
        }
    }
    if (!num_params)
    {
        for (int i = 0; i < NUM_TERTIARY_COMMANDS; i++)
        {
            if (!strcmp(command, TERTIARY_COMMANDS[i]))
            {
                num_params = 3;
                break;
            }
        }
    }
    return num_params;
}

// Validates lines from file
int validate_lines(char *lines_array[], int line_count)
{
    int is_error = 0;
    int num_items;
    char *working_string;
    working_string = malloc(sizeof(char *));

    if (lines_array[0][0] != 'U')
    {
        is_error = 1;
        fprintf(stderr, "Prvni radek musi byt universe");
    }
    else
    {
        int universe_size;
        char *universe_string = malloc((strlen(lines_array[0]) + 1) * sizeof(char *));
        strcpy(universe_string, lines_array[0]);
        char **universe_array = my_split(universe_string, ' ', strlen(universe_string), &universe_size);
        // lengt of values in universe
        subval_universe_chars_max_len(universe_array, universe_size, &is_error);
        for (int i = 0; i < line_count; i++)
        {
            working_string = realloc(working_string, (strlen(lines_array[i]) + 1) * sizeof(char *));
            strcpy(working_string, lines_array[i]);

            if (working_string[0] == 'R')
            {
                remove_all_chars(working_string, '(');
                remove_all_chars(working_string, ')');
            }

            char **splitted_line = my_split(working_string, ' ', strlen(working_string), &num_items);
            if (subval_valid_line_identifier(working_string[0], &is_error, i + 1))
            {
                if (strcmp(splitted_line[0], "C"))
                {
                    // compares whole line with banned strings
                    subval_banned_strings(splitted_line, num_items, &is_error, i + 1);
                    // chechs if all chars are big or small letters
                    subval_char_type(splitted_line, num_items, &is_error, i + 1);
                    // checks for duplicates
                    if (strcmp(splitted_line[0], "R"))
                    {
                        subval_same_values_set(splitted_line, num_items, &is_error, i + 1);
                    }
                    else
                    {
                        subval_same_values_session(splitted_line, num_items, &is_error, i + 1);
                    }
                    // TEMPORARLY DISABLED DUE TO UNFIXABLE BUG ON MERLIN
                    // if not universe, checks if all values are from universe
                    // if (strcmp(splitted_line[0], "U"))
                    // {
                    //     subval_values_from_universe(splitted_line, num_items, &is_error, universe_array, universe_size, i + 1);
                    // }
                }
                else
                {
                    // validates valid command input and number of params
                    int num_params = subval_get_num_params(splitted_line[1]);
                    if (num_params == 0)
                    {
                        is_error = 1;
                        fprintf(stderr, "Chyba na radku %d: Neznamy prikaz: '%s'", i + 1, splitted_line[1]);
                    }
                    else if (num_params == 1)
                    {
                        if (num_items != 3)
                        {
                            is_error = 1;
                            fprintf(stderr, "Chyba na radku %d: Nespravny pocet argumentu u prikazu: '%s'", i + 1, splitted_line[1]);
                        }
                    }
                    else if (num_params == 2)
                    {
                        if (num_items != 4)
                        {
                            is_error = 1;
                            fprintf(stderr, "Chyba na radku %d: Nespravny pocet argumentu u prikazu: '%s'", i + 1, splitted_line[1]);
                        }
                    }
                    else if (num_params == 3)
                    {
                        if (num_items != 5)
                        {
                            is_error = 1;
                            fprintf(stderr, "Chyba na radku %d: Nespravny pocet argumentu u prikazu: '%s'", i + 1, splitted_line[1]);
                        }
                    }
                }
            }
            for (int i = 0; i < num_items; i++)
            {
                free(splitted_line[i]);
            }
            free(splitted_line);
        }
        free(universe_string);
        for (int i = 0; i < universe_size; i++)
        {
            free(universe_array[i]);
        }
        free(universe_array);
    }
    working_string = NULL;
    free(working_string);

    if (is_error)
    {
        return 0;
    }
    return 1;
}

int validate_singular_command(const char first_type, int first_line_number, char *lines_array[])
{

    if (
        first_type == lines_array[first_line_number - 1][0])
    {
        return 1;
    }
    return 0;
}
int validate_binary_command(const char first_type, int first_line_number, const char second_type, int second_line_number, char *lines_array[])
{
    if (
        (first_type == lines_array[first_line_number - 1][0]) &&
        (second_type == lines_array[second_line_number - 1][0]))
    {
        return 1;
    }
    return 0;
}
int validate_terciary_command(const char first_type, int first_line_number, const char second_type, int second_line_number, const char third_type, int third_line_number, char *lines_array[])
{
    if (
        (first_type == lines_array[first_line_number - 1][0]) &&
        (second_type == lines_array[second_line_number - 1][0]) &&
        (third_type == lines_array[third_line_number - 1][0]))
    {
        return 1;
    }
    return 0;
}

//------SET-HELPERS------
void set_ctor_from_line_string(Set *set, char *string, int row)
{
    int items_count = 0;
    char **splitted_line = my_split(string, ' ', strlen(string), &items_count);
    set->row = row;
    set->size = 0;
    set->items = malloc(items_count * sizeof(char *));
    for (int i = 1; i < items_count; i++)
    {
        set->items[i - 1] = malloc(strlen(splitted_line[i]) + 1);
        strcpy(set->items[i - 1], splitted_line[i]);
        set->size = i;
    }
    for (int i = 0; i < items_count; i++)
    {
        free(splitted_line[i]);
    }
    free(splitted_line);
}
void set_ctor(Set *set, char **items, int row, int items_len)
{
    set->row = row;
    set->items = malloc(items_len * sizeof(char *));
    for (int i = 0; i < items_len; i++)
    {
        set->items[i] = malloc(strlen(items[i]) + 1);
        strcpy(set->items[i], items[i]);
        set->size = i + 1;
    }
}
void set_print_from_array(char **items, int items_count)
{
    printf("S ");
    if (items_count > 0)
    {
        for (int i = 0; i < items_count; i++)
        {
            if (i == items_count - 1)
            {
                printf("%s\n", items[i]);
            }
            else
                printf("%s ", items[i]);
        }
    }
    else
        printf("\n");
}
void set_dtor(Set *set)
{
    set->row = 0;
    for (int i = 0; i < set->size; i++)
    {
        free(set->items[i]);
    }
    free(set->items);
    set->items = NULL;
    set->size = 0;
}
void *set_copy(Set *src, Set *dst)
{
    set_ctor(dst, src->items, src->row, src->size);
    return dst;
}

//------SET-COMMANDS------
int is_empty_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('S', first_line_num, lines_array))
    {
        Set set;
        set_ctor_from_line_string(&set, lines_array[first_line_num - 1], first_line_num);
        if (set.size == 0)
            printf("true\n");
        else
            printf("false\n");
        set_dtor(&set);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz empty \n");
        *is_command_error = 1;
        return 1;
    }
}
int card_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('S', first_line_num, lines_array))
    {

        Set set;
        set_ctor_from_line_string(&set, lines_array[first_line_num - 1], first_line_num);
        printf("%d\n", set.size);
        set_dtor(&set);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz card \n");
        *is_command_error = 1;
        return 1;
    }
}
int complement_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('S', first_line_num, lines_array))
    {
        Set set;
        Set universe;
        set_ctor_from_line_string(&set, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&universe, lines_array[0], 1);
        int items_count_comp = 0;
        int is_identical = 0;
        char **comp_arr = malloc(sizeof(char *));
        for (int i = 0; i < universe.size; i++)
        {
            for (int j = 0; j < set.size; j++)
            {
                if (!strcmp(universe.items[i], set.items[j]))
                {
                    is_identical = 1;
                    break;
                }
            }
            if (is_identical == 0)
            {
                comp_arr = realloc(comp_arr, (items_count_comp + 1) * sizeof(char *));
                comp_arr[items_count_comp] = malloc(strlen(universe.items[i]) + 1);
                strcpy(comp_arr[items_count_comp], universe.items[i]);
                items_count_comp++;
            }
            is_identical = 0;
        }
        set_print_from_array(comp_arr, items_count_comp);
        set_dtor(&set);
        set_dtor(&universe);
        for (int i = 0; i < items_count_comp; i++)
        {
            free(comp_arr[i]);
        }
        free(comp_arr);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz complement \n");
        *is_command_error = 1;
        return 1;
    }
}
int union_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_binary_command('S', first_line_num, 'S', second_line_num, lines_array))
    {
        Set first_set;
        Set second_set;
        set_ctor_from_line_string(&first_set, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&second_set, lines_array[second_line_num - 1], second_line_num);
        char **uni_array = malloc(sizeof(char *));
        int items_count_uni = 0, is_identical = 0;
        for (int i = 0; i < first_set.size; i++)
        {
            uni_array = realloc(uni_array, (items_count_uni + 1) * sizeof(char *));
            uni_array[items_count_uni] = malloc(strlen(first_set.items[i]) + 1);
            strcpy(uni_array[items_count_uni], first_set.items[i]);
            items_count_uni++;
        }

        for (int i = 0; i < second_set.size; i++)
        {
            for (int j = 0; j < items_count_uni; j++)
            {
                if (!strcmp(second_set.items[i], uni_array[j]))
                {
                    is_identical = 1;
                    break;
                }
            }
            if (is_identical == 0)
            {
                uni_array = realloc(uni_array, (items_count_uni + 1) * sizeof(char *));
                uni_array[items_count_uni] = malloc(strlen(second_set.items[i]) + 1);
                strcpy(uni_array[items_count_uni], second_set.items[i]);
                items_count_uni++;
            }
            is_identical = 0;
        }
        set_print_from_array(uni_array, items_count_uni);
        set_dtor(&first_set);
        set_dtor(&second_set);
        for (int i = 0; i < items_count_uni; i++)
        {
            free(uni_array[i]);
        }
        free(uni_array);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatne parametry pro prikaz union \n");
        *is_command_error = 1;
        return 1;
    }
}
int intersect_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_binary_command('S', first_line_num, 'S', second_line_num, lines_array))
    {
        Set first_set;
        Set second_set;
        set_ctor_from_line_string(&first_set, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&second_set, lines_array[second_line_num - 1], second_line_num);
        char **inter_array = malloc(sizeof(char *));
        int items_count_inter = 0, is_identical = 0;
        for (int i = 0; i < first_set.size; i++)
        {
            for (int j = 0; j < second_set.size; j++)
            {
                if (!strcmp(first_set.items[i], second_set.items[j]))
                {
                    is_identical = 1;
                    break;
                }
            }
            if (is_identical == 1)
            {
                inter_array = realloc(inter_array, (items_count_inter + 1) * sizeof(char *));
                inter_array[items_count_inter] = malloc(strlen(first_set.items[i]) + 1);
                strcpy(inter_array[items_count_inter], first_set.items[i]);
                items_count_inter++;
            }
            is_identical = 0;
        }
        set_print_from_array(inter_array, items_count_inter);
        set_dtor(&first_set);
        set_dtor(&second_set);
        for (int i = 0; i < items_count_inter; i++)
        {
            free(inter_array[i]);
        }
        free(inter_array);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatne parametry pro prikaz intersect \n");
        *is_command_error = 1;
        return 1;
    }
}
int minus_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_binary_command('S', first_line_num, 'S', second_line_num, lines_array))
    {
        Set first_set;
        Set second_set;
        set_ctor_from_line_string(&first_set, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&second_set, lines_array[second_line_num - 1], second_line_num);
        char **min_array = malloc(sizeof(char *));
        int items_count_min = 0, is_identical = 0;
        for (int i = 0; i < first_set.size; i++)
        {
            for (int j = 0; j < second_set.size; j++)
            {
                if (!strcmp(first_set.items[i], second_set.items[j]))
                {
                    is_identical = 1;
                    break;
                }
            }
            if (is_identical == 0)
            {
                min_array = realloc(min_array, (items_count_min + 1) * sizeof(char *));
                min_array[items_count_min] = malloc(strlen(first_set.items[i]) + 1);
                strcpy(min_array[items_count_min], first_set.items[i]);
                items_count_min++;
            }
            is_identical = 0;
        }
        set_print_from_array(min_array, items_count_min);
        set_dtor(&first_set);
        set_dtor(&second_set);
        for (int i = 0; i < items_count_min; i++)
        {
            free(min_array[i]);
        }
        free(min_array);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatne parametry pro prikaz minus \n");
        *is_command_error = 1;
        return 1;
    }
}
int subseteq_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_binary_command('S', first_line_num, 'S', second_line_num, lines_array))
    {
        Set first_set;
        Set second_set;
        set_ctor_from_line_string(&first_set, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&second_set, lines_array[second_line_num - 1], second_line_num);
        int is_identical = 0;
        for (int i = 0; i < first_set.size; i++)
        {
            for (int j = 0; j < second_set.size; j++)
            {
                if (!strcmp(first_set.items[i], second_set.items[j]))
                {
                    is_identical = 1;
                    break;
                }
            }
            if (is_identical == 0)
            {
                printf("false\n");
                set_dtor(&first_set);
                set_dtor(&second_set);
                return 0;
            }
            is_identical = 0;
        }
        set_dtor(&first_set);
        set_dtor(&second_set);
        printf("true\n");
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatne parametry pro prikaz subseteq \n");
        *is_command_error = 1;
        return 1;
    }
}
int subset_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_binary_command('S', first_line_num, 'S', second_line_num, lines_array))
    {
        Set first_set;
        Set second_set;
        set_ctor_from_line_string(&first_set, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&second_set, lines_array[second_line_num - 1], second_line_num);
        int is_identical = 0;
        if (first_set.size != second_set.size)
        {
            for (int i = 0; i < first_set.size; i++)
            {
                for (int j = 0; j < second_set.size; j++)
                {
                    if (!strcmp(first_set.items[i], second_set.items[j]))
                    {
                        is_identical = 1;
                        break;
                    }
                }
                if (is_identical == 0)
                {
                    printf("false\n");
                    set_dtor(&first_set);
                    set_dtor(&second_set);
                    return 0;
                }
                is_identical = 0;
            }
            printf("true\n");
            set_dtor(&first_set);
            set_dtor(&second_set);
            return 0;
        }
        else {
            printf("false\n");
            set_dtor(&first_set);
            set_dtor(&second_set);
            return 0;
        }
    }
    else
    {
        fprintf(stderr, "Neplatne parametry pro prikaz subset \n");
        *is_command_error = 1;
        return 1;
    }
}
int equals_com(int first_line_num, int second_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_binary_command('S', first_line_num, 'S', second_line_num, lines_array))
    {
        Set first_set;
        Set second_set;
        set_ctor_from_line_string(&first_set, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&second_set, lines_array[second_line_num - 1], second_line_num);
        int is_identical = 0;
        if (first_set.size == second_set.size)
        {
            for (int i = 0; i < first_set.size; i++)
            {
                for (int j = 0; j < second_set.size; j++)
                {
                    if (!strcmp(first_set.items[i], second_set.items[j]))
                    {
                        is_identical = 1;
                        break;
                    }
                }
                if (is_identical == 0)
                {
                    printf("false\n");
                    set_dtor(&first_set);
                    set_dtor(&second_set);
                    return 0;
                }
                is_identical = 0;
            }
            set_dtor(&first_set);
            set_dtor(&second_set);
            printf("true\n");
            return 0;
        }
        else
            printf("false\n");
        set_dtor(&first_set);
        set_dtor(&second_set);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatne parametry pro prikaz equals \n");
        *is_command_error = 1;
        return 1;
    }
}

//------SESSION-HELPERS-------
void session_init(Session *session, int row)
{
    session->row = row;
    session->size = 0;
    session->pairs = NULL;
}
void session_append(Session *session, Session_pair *pair)
{
    session->size += 1;
    session->pairs = realloc(session->pairs, session->size * sizeof(Session_pair));
    session->pairs[session->size - 1].left_val = malloc(strlen(pair->left_val) * sizeof(char *));
    strcpy(session->pairs[session->size - 1].left_val, pair->left_val);
    session->pairs[session->size - 1].right_val = malloc(strlen(pair->right_val) * sizeof(char *));
    strcpy(session->pairs[session->size - 1].right_val, pair->right_val);
}
void session_pair_ctor(Session_pair *pair, char *left_val, char *right_val)
{
    pair->left_val = malloc(strlen(left_val) * sizeof(char *));
    pair->right_val = malloc(strlen(right_val) * sizeof(char *));
    pair->left_val = left_val;
    pair->right_val = right_val;
}
void session_pair_dtor(Session_pair *pair)
{
    free(pair->left_val);
    free(pair->right_val);
}
void session_ctor_from_line_string(Session *session, char *string, int row)
{
    session_init(session, row);
    int items_count = 0;
    char *working_string;
    working_string = malloc(strlen(string) * sizeof(char *));
    strcpy(working_string, string);
    remove_all_chars(working_string, ')');
    remove_all_chars(working_string, '(');
    remove_all_chars(working_string, 'R');

    char **splitted_line = my_split(working_string, ' ', strlen(working_string), &items_count);
    Session_pair tmp_pair;
    for (int i = 1; i < items_count; i += 2)
    {
        session_pair_ctor(&tmp_pair, splitted_line[i], splitted_line[i + 1]);
        session_append(session, &tmp_pair);
    }

    session_pair_dtor(&tmp_pair);
}
void session_ctor(Session *session, Session_pair *pairs, int row, int size)
{
    session->row = row;
    for (int i = 0; i < size; i++)
    {
        session_append(session, &(pairs[i]));
    }
}
void session_print(Session session)
{
    printf("R ");
    for (int i = 0; i < session.size; i++)
    {
        printf("(%s %s) ", session.pairs[i].left_val, session.pairs[i].right_val);
    }
    printf("\n");
}
void session_dtor(Session *session)
{
    for (int i = 0; i < session->size; i++)
    {
        session_pair_dtor(&(session->pairs[i]));
    }
    free(session->pairs);
}

//------SESSION-COMMANDS------
int reflexive_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('R', first_line_num, lines_array))
    {

        Session session;
        Set universe;
        session_ctor_from_line_string(&session, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&universe, lines_array[0], 1);
        int is_reflexive = 0;

        for (int i = 0; i < session.size; i++)
        {
            for (int j = 0; j < session.size; j++)
            {
                if (!strcmp(universe.items[i], session.pairs[j].left_val) && !strcmp(universe.items[i], session.pairs[j].right_val))
                {
                    is_reflexive = 1;
                    break;
                }
            }
            if (!is_reflexive)
            {
                printf("false\n");
                set_dtor(&universe);
                session_dtor(&session);
                return 0;
            }
            is_reflexive = 0;
        }
        printf("true\n");
        set_dtor(&universe);
        session_dtor(&session);
        return 0;
    }

    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz reflexive \n");
        *is_command_error = 1;
        return 1;
    }
}
int symmetric_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('R', first_line_num, lines_array))
    {
        Session session;
        session_ctor_from_line_string(&session, lines_array[first_line_num - 1], first_line_num);
        int is_inverse = 0;
        for (int i = 0; i < session.size; i++)
        {
            for (int j = 0; j < session.size; j++)
            {
                if (!strcmp(session.pairs[i].left_val, session.pairs[j].right_val) && !strcmp(session.pairs[i].right_val, session.pairs[j].left_val))
                {
                    is_inverse = 1;
                    break;
                }
            }
            if (!is_inverse)
            {
                session_dtor(&session);
                printf("false\n");
                return 0;
            }
            is_inverse = 0;
        }
        session_dtor(&session);
        printf("true\n");
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz symetric \n");
        *is_command_error = 1;
        return 1;
    }
}
int antisymmetric_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('R', first_line_num, lines_array))
    {

        Session session;
        session_ctor_from_line_string(&session, lines_array[first_line_num - 1], first_line_num);
        int is_inverse = 1;
        for (int i = 0; i < session.size; i++)
        {
            for (int j = 0; j < session.size; j++)
            {
                if (!strcmp(session.pairs[i].left_val, session.pairs[j].right_val) && !strcmp(session.pairs[i].right_val, session.pairs[j].left_val) && strcmp(session.pairs[i].right_val, session.pairs[i].left_val) && strcmp(session.pairs[j].right_val, session.pairs[j].left_val))
                {
                    is_inverse = 0;
                    break;
                }
            }
            if (!is_inverse)
            {
                session_dtor(&session);
                printf("false\n");
                return 0;
            }
            is_inverse = 1;
        }
        session_dtor(&session);
        printf("true\n");
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz antisymetric \n");
        *is_command_error = 1;
        return 1;
    }
}
int transitive_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('R', first_line_num, lines_array))
    {
        Session session;
        session_ctor_from_line_string(&session, lines_array[first_line_num - 1], first_line_num);
        int is_identical = 0;
        for (int i = 0; i < session.size; i++)
        {
            for (int j = 0; j < session.size; j++)
            {
                if (!strcmp(session.pairs[i].right_val, session.pairs[j].left_val))
                {
                    for (int k = 0; k < session.size; k++)
                    {
                        if (!strcmp(session.pairs[k].left_val, session.pairs[i].left_val) && !strcmp(session.pairs[k].right_val, session.pairs[j].right_val))
                        {
                            is_identical = 1;
                            break;
                        }
                    }
                    if (!is_identical)
                    {
                        session_dtor(&session);
                        printf("false\n");
                        return 0;
                    }
                    is_identical = 0;
                    break;
                }
            }
        }
        session_dtor(&session);
        printf("true\n");
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz transitive \n");
        *is_command_error = 1;
        return 1;
    }
}
int function_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('R', first_line_num, lines_array))
    {
        Session session;
        session_ctor_from_line_string(&session, lines_array[first_line_num - 1], first_line_num);
        for (int i = 0; i < session.size; i++)
        {
            for (int j = i + 1; j < session.size; j++)
            {
                if (!strcmp(session.pairs[i].left_val, session.pairs[j].left_val))
                {
                    session_dtor(&session);
                    printf("false\n");
                    return 0;
                }
            }
        }
        session_dtor(&session);
        printf("true\n");
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz function \n");
        *is_command_error = 1;
        return 1;
    }
}
int domain_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('R', first_line_num, lines_array))
    {
        Session session;
        session_ctor_from_line_string(&session, lines_array[first_line_num - 1], first_line_num);
        char **domain_array = malloc(sizeof(char *));
        int item_count = 0, is_in_arr = 0;
        for (int i = 0; i < session.size; i++)
        {
            for (int j = 0; j < item_count; j++)
            {
                if (!strcmp(session.pairs[i].left_val, domain_array[j]))
                {
                    is_in_arr = 1;
                    break;
                }
            }
            if (is_in_arr != 1)
            {
                domain_array = realloc(domain_array, (item_count + 1) * sizeof(char *));
                domain_array[item_count] = malloc(strlen(session.pairs[i].left_val) + 1);
                strcpy(domain_array[item_count], session.pairs[i].left_val);
                item_count++;
            }
            is_in_arr = 0;
        }
        set_print_from_array(domain_array, item_count);
        for (int i = 0; i < item_count; i++)
        {
            free(domain_array[i]);
        }
        free(domain_array);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz domain \n");
        *is_command_error = 1;
        return 1;
    }
}
int codomain_com(int first_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_singular_command('R', first_line_num, lines_array))
    {

        Session session;
        session_ctor_from_line_string(&session, lines_array[first_line_num - 1], first_line_num);
        char **codomain_array = malloc(sizeof(char *));
        int item_count = 0, is_in_arr = 0;
        for (int i = 0; i < session.size; i++)
        {
            for (int j = 0; j < item_count; j++)
            {
                if (!strcmp(session.pairs[i].right_val, codomain_array[j]))
                {
                    is_in_arr = 1;
                    break;
                }
            }
            if (is_in_arr != 1)
            {
                codomain_array = realloc(codomain_array, (item_count + 1) * sizeof(char *));
                codomain_array[item_count] = malloc(strlen(session.pairs[i].right_val) + 1);
                strcpy(codomain_array[item_count], session.pairs[i].right_val);
                item_count++;
            }
            is_in_arr = 0;
        }
        set_print_from_array(codomain_array, item_count);
        for (int i = 0; i < item_count; i++)
        {
            free(codomain_array[i]);
        }
        free(codomain_array);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatny parametr pro prikaz codomain \n");
        *is_command_error = 1;
        return 1;
    }
}
int injective_com(int first_line_num, int second_line_num, int third_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_terciary_command('R', first_line_num, 'S', second_line_num, 'S', third_line_num, lines_array))
    {
        Session session;
        Set first_set;
        Set second_set;
        session_ctor_from_line_string(&session, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&first_set, lines_array[second_line_num - 1], second_line_num);
        set_ctor_from_line_string(&second_set, lines_array[third_line_num - 1], third_line_num);
        int is_valid_first = 0, is_valid_second = 0, is_repeated_first = 0, is_repeated_second = 0;
        //validates if size of first_set is smaller or equal to size of second set, because we have to use every element from first_set, but only once
        if (first_set.size <= second_set.size)
        {
            for (int i = 0; i < session.size; i++)
            {
                //validates if left values in relation contains only values from first_set
                for (int j = 0; j < first_set.size; j++)
                {
                    if (!strcmp(first_set.items[j], session.pairs[i].left_val))
                    {
                        is_valid_first = 1;
                        break;
                    }
                }
                if (!is_valid_first)
                {
                    session_dtor(&session);
                    set_dtor(&first_set);
                    set_dtor(&second_set);
                    return 0;
                }
                is_valid_first = 0;
                //validates if right values in relation contains only values from second_set
                for (int k = 0; k < second_set.size; k++)
                {
                    if (!strcmp(second_set.items[k], session.pairs[i].right_val))
                    {
                        is_valid_second = 1;
                        break;
                    }
                }
                if (!is_valid_second)
                {
                    session_dtor(&session);
                    set_dtor(&first_set);
                    set_dtor(&second_set);
                    return 0;
                }
                is_valid_second = 0;
            }

            //validates if size of first set is equal to size of relation, because we have to use every element in first_set, but only once
            if (first_set.size == session.size)
            {
                //main loop through relation
                for (int i = 0; i < session.size; i++)
                {
                    //loop to check if there is every element on left position of every relation used only once
                    for (int j = 0; j < session.size; j++)
                    {
                        if (!strcmp(session.pairs[j].left_val, session.pairs[i].left_val) && (j != i))
                        {
                            is_repeated_first = 1;
                            break;
                        }
                    }
                    if (is_repeated_first)
                    {
                        session_dtor(&session);
                        set_dtor(&first_set);
                        set_dtor(&second_set);
                        return 0;
                    }
                    is_repeated_first = 0;

                    //loop to check if there is element on right position of every relation used only once (it doesn't have to be used every element)
                    for (int k = 0; k < session.size; k++)
                    {
                        if (!strcmp(session.pairs[k].right_val, session.pairs[i].right_val) && (k != i))
                        {
                            is_repeated_second = 1;
                            break;
                        }
                    }
                    if (is_repeated_second)
                    {
                        session_dtor(&session);
                        set_dtor(&first_set);
                        set_dtor(&second_set);
                        return 0;
                    }
                    is_repeated_second = 0;
                }
                //if we can iterate through whole relation without error, it is injective -> print of true
                session_dtor(&session);
                set_dtor(&first_set);
                set_dtor(&second_set);
                return 1;
            }
            else {
                session_dtor(&session);
                set_dtor(&first_set);
                set_dtor(&second_set);
                return 0;
            }
        }
        session_dtor(&session);
        set_dtor(&first_set);
        set_dtor(&second_set);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatne parametry pro prikaz injective \n");
        *is_command_error = 1;
        return 2;
    }
}
int surjective_com(int first_line_num, int second_line_num, int third_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_terciary_command('R', first_line_num, 'S', second_line_num, 'S', third_line_num, lines_array))
    {
        Session session;
        Set first_set;
        Set second_set;
        char **surjective_array = malloc(sizeof(char *));
        session_ctor_from_line_string(&session, lines_array[first_line_num - 1], first_line_num);
        set_ctor_from_line_string(&first_set, lines_array[second_line_num - 1], second_line_num);
        set_ctor_from_line_string(&second_set, lines_array[third_line_num - 1], third_line_num);
        int is_valid_first = 0, is_valid_second = 0, is_repeated_first = 0, item_count = 0, is_in_arr = 0;
        // size of first set has to be bigger or equal than size of second set, because relation has to be function
        // we are capable to create surjective representation by using only one element from first set and all elements from second set, but this case wouldn't be function
        if (first_set.size >= second_set.size)
        {
            //main loop through relation to check if relation is valid
            for (int i = 0; i < session.size; i++)
            {
                //loop to check if left values from relation are all from first set
                for (int j = 0; j < first_set.size; j++)
                {
                    if (!strcmp(first_set.items[j], session.pairs[i].left_val))
                    {
                        is_valid_first = 1;
                        break;
                    }
                }
                if (!is_valid_first)
                {
                    session_dtor(&session);
                    set_dtor(&first_set);
                    set_dtor(&second_set);
                    return 0;
                }
                is_valid_first = 0;

                //loop to check if right values from relation are all from second set
                for (int k = 0; k < second_set.size; k++)
                {
                    if (!strcmp(second_set.items[k], session.pairs[i].right_val))
                    {
                        is_valid_second = 1;
                        break;
                    }
                }
                if (!is_valid_second)
                {
                    session_dtor(&session);
                    set_dtor(&first_set);
                    set_dtor(&second_set);
                    return 0;
                }
                is_valid_second = 0;
            }

            //start of checking if relation is surjective
            for (int i = 0; i < session.size; i++)
            {
                //another loop to check if relation is function, left element from relation cannot be repeated
                for (int j = 0; j < session.size; j++)
                {
                    if (!strcmp(session.pairs[j].left_val, session.pairs[i].left_val) && (j != i))
                    {
                        is_repeated_first = 1;
                        break;
                    }
                }
                if (is_repeated_first)
                {
                    session_dtor(&session);
                    set_dtor(&first_set);
                    set_dtor(&second_set);
                    return 0;
                }
                is_repeated_first = 0;

                //loop to append every character from right value to temporary array, but only once
                for (int k = 0; k < item_count; k++)
                {
                    if (!strcmp(surjective_array[k], session.pairs[i].right_val))
                    {
                        is_in_arr = 1;
                        break;
                    }
                }
                if (is_in_arr != 1)
                {
                    surjective_array = realloc(surjective_array, (item_count + 1) * sizeof(char *));
                    surjective_array[item_count] = malloc(strlen(session.pairs[i].right_val) + 1);
                    strcpy(surjective_array[item_count], session.pairs[i].right_val);
                    item_count++;
                }
                is_in_arr = 0;
            }
            //if length of array is equal to size of second set the relation must be surjective, because every element from secod set has to be used
            if (item_count == second_set.size)
            {
                session_dtor(&session);
                set_dtor(&first_set);
                set_dtor(&second_set);
                return 1;
            }
        }
        session_dtor(&session);
        set_dtor(&first_set);
        set_dtor(&second_set);
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatne parametry pro prikaz surjective \n");
        *is_command_error = 1;
        return 2;
    }
}
int bijective_com(int first_line_num, int second_line_num, int third_line_num, char *lines_array[], int *is_command_error)
{
    if (validate_terciary_command('R', first_line_num, 'S', second_line_num, 'S', third_line_num, lines_array))
    {
        if (injective_com(first_line_num, second_line_num, third_line_num, lines_array, is_command_error) == 1 && surjective_com(first_line_num, second_line_num, third_line_num, lines_array, is_command_error) == 1)
        {
            return 1;
        }
        else
            return 0;
        return 0;
    }
    else
    {
        fprintf(stderr, "Neplatne parametry pro prikaz bijective \n");
        *is_command_error = 1;
        return 2;
    }
}
