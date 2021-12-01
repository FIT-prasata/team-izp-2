#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//_______________DEFINITIONS__________________
#define DEFAULT_LINE_SIZE 30

//------STRUCTS-FOR-SETS
typedef struct
{
    int size;
    int row;
    char **items;
} Set;

typedef struct
{
    char *left_val;
    char *right_val;
} Session_pair;

typedef struct
{
    int size;
    int row;
    Session_pair *pair;
} Session;
//struct for Array of Sets
typedef struct
{
    int len;
    Set *data;
} SetArray;

//_______________FUNCTION_HEADERS__________________
//------UTILS------
// TODO
char **my_split(char line[], char separator, int line_length, int *result_len);
// returns 1 if is end of file, else returns 0
int readline(FILE *file, char *line);
// creates array of lines from file
void process_file(char filename[], char **result);
// TODO
void process_rows(char *lines[], SetArray *setar);
// TODO
void process_operation(char command_name[], char *lines[], int line1, int line2);
//------VALIDATORS------
// Returns 1 if input is correct, otherwise 0
int validate_user_input(int argc, char const *argv[]);

//------SET-HELPERS------
void set_ctor_string(Set *s, char *string, int row);
void set_ctor(Set *s, char **items, int row, int items_len);
void set_print(char **items, int items_count);
void set_dtor(Set *s);
void setarray_ctor(SetArray *setar);
void *setarray_inc(SetArray *setar);
void *set_copy(Set *src, Set *dst);
void *set_array_append(SetArray *setar, Set *s);

//------SET-OPERATIONS------
int empty_com(int line1, char *lines[]);
int card_com(int line1, char *lines[]);
int complement_com(int line1, char *lines[]);
int union_com(int line1, int line2, char *lines[]);
int intersect_com(int line1, int line2, char *lines[]);
int minus_com(int line1, int line2, char *lines[]);
int subseteq_com(int line1, int line2, char *lines[]);
int subset_com(int line1, int line2, char *lines[]);
int equals_com(int line1, int line2, char *lines[]);

//------RELATIONS-OPERATIONS------
int reflexive_com(int line1, char *lines[]);
int symmetric_com(int line1, char *lines[]);
int antisymmetric_com(int line1, char *lines[]);
int transitive_com(int line1, char *lines[]);
int function_com(int line1, char *lines[]);
int domain_com(int line1, char *lines[]);
int codomain_com(int line1, char *lines[]);
int injective_com(int line1, char *lines[]);
int surjective_com(int line1, char *lines[]);
int bijective_com(int line1, char *lines[]);

//_______________MAIN_CODE___________________________________

int main(int argc, char const *argv[])
{
    SetArray setar;
    setarray_ctor(&setar);
    char *lines[1000];
    char filename[10] = "file.txt";
    process_file(filename, lines);
    process_rows(lines, &setar);
    return 0;
}

//-------UTILS------
// returns result, it is fully dynamic allocated
char **my_split(char line[], char separator, int line_length, int *result_len)
{
    int subst_start = 0;  // where should creation of the substring start
    int substs_count = 0; // how many substrings I have (index in result)
    char *substr;
    char **result = malloc(sizeof(char *));
    substr = (char *)malloc(1); // dummy malloc
    for (int i = 0; i <= line_length; i++)
    {
        if ((line[i] == separator) || (line[i] == '\0'))
        {
            substr = (char *)realloc(substr, (i - subst_start) + 1);      // reallocing dummy with size of string +1 for \0 char
            strncpy(substr, &line[subst_start], i - subst_start);         // giving right value to substring
            substr[i - subst_start] = '\0';                               // adding 0 to end of substring
            result[substs_count] = (char *)malloc((i - subst_start) + 1); // allocating result size
            strncpy(result[substs_count], substr, (i - subst_start) + 1); // giving right value to result
            subst_start = i + 1;
            substs_count++;
            result = realloc(result, sizeof(char *) * (substs_count + 1)); // increase space of result
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

// creates array of lines from file
void process_file(char filename[], char **result)
{
    FILE *file;
    file = fopen(filename, "r");
    for (int i = 0; i < 1000; i++)
    {
        result[i] = (char *)malloc(DEFAULT_LINE_SIZE);
        if (readline(file, result[i]))
        {
            break;
        }
    }
}

void process_rows(char *lines[], SetArray *setar)
{
    for (int i = 0; i < 1000; i++)
    {
        if (lines[i][0] == 'C')
        {
            int items_len = 0;
            char **output_com = my_split(lines[i], ' ', strlen(lines[i]), &items_len);
            for (int i = 0; i < items_len; i++)
                if (items_len == 3)
                    output_com[3] = "_";
            process_operation(output_com[1], lines, atoi(output_com[2]), atoi(output_com[3]));

            for (int i = 0; i < items_len; i++)
            {
                free(output_com[i]);
            }
            free(output_com);
        }
        else
        {
            printf("%s\n", lines[i]);
        }
    }
}

// execute proper function when line contains command
void process_operation(char command_name[], char *lines[], int line1, int line2)
{
    //SETS
    if (!strcmp(command_name, "empty"))
    {
        empty_com(line1, lines);
    }
    else if (!strcmp(command_name, "card"))
    {
        card_com(line1, lines);
    }
    else if (!strcmp(command_name, "complement"))
    {
        complement_com(line1, lines);
    }
    else if (!strcmp(command_name, "union"))
    {
        union_com(line1, line2, lines);
    }
    else if (!strcmp(command_name, "intersect"))
    {
        intersect_com(line1, line2, lines);
    }
    else if (!strcmp(command_name, "minus"))
    {
        minus_com(line1, line2, lines);
    }
    else if (!strcmp(command_name, "subseteq"))
    {
        subseteq_com(line1, line2, lines);
    }
    else if (!strcmp(command_name, "subset"))
    {
        subset_com(line1, line2, lines);
    }
    else if (!strcmp(command_name, "equals"))
    {
        equals_com(line1, line2, lines);
    }
    //RELATIONS
    else if (!strcmp(command_name, "reflexive"))
    {
        reflexive_com(line1, lines);
    }
    else if (!strcmp(command_name, "symmetric"))
    {
        symmetric_com(line1, lines);
    }
    else if (!strcmp(command_name, "antisymmetric"))
    {
        antisymmetric_com(line1, lines);
    }
    else if (!strcmp(command_name, "transitive"))
    {
        transitive_com(line1, lines);
    }
    else if (!strcmp(command_name, "function"))
    {
        function_com(line1, lines);
    }
    else if (!strcmp(command_name, "domain"))
    {
        domain_com(line1, lines);
    }
    else if (!strcmp(command_name, "codomain"))
    {
        codomain_com(line1, lines);
    }
    else if (!strcmp(command_name, "injective"))
    {
        injective_com(line1, lines);
    }
    else if (!strcmp(command_name, "surjective"))
    {
        surjective_com(line1, lines);
    }
    else if (!strcmp(command_name, "bijective"))
    {
        bijective_com(line1, lines);
    }
    else
    {
        fprintf(stderr, "Chyba vstupu, spatny nazev fce\n"); // TODO - ukoncit program protoze spatny vstup
    }
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
// Validates lines from file
bool validate_lines(char lines[])
{
    // array of banned strings
    char *cmd_arr[19] = {"complement", "union", "intersect", "minus", "subseteq", "subset", "equals", "reflexive", "symmetric", "antisymmetric", "transitive", "function", "domain", "codomain", "injective", "surjective", "bijective", "true", "false"};
    int items_len = 0;
    // converts line to array
    char **line_arr = my_split(lines, ' ', strlen(lines), &items_len);
    // UNIVERSE AND SET VALIDATIONS
    if (strcmp(line_arr[0], "U") == 0 || strcmp(line_arr[0], "S") == 0)
    {
        for (int i = 1; i < items_len; i++)
        {
            for (int j = 0; j < 19; j++)
            {
                // comparison with banned strings
                if (strcmp(line_arr[i], cmd_arr[j]) == 0)
                {
                    fprintf(stderr, "Prvky se nesmi jmenovat stejne jako prikazy!\n");
                    return false;
                }
            }
            // char type validation
            for (int j = 0; j < strlen(line_arr[i]); j++)
            {
                if (!((line_arr[i][j] >= 'a' && line_arr[i][j] <= 'z') || (line_arr[i][j] >= 'A' && line_arr[i][j] <= 'Z')))
                {
                    fprintf(stderr, "Prvky musi byt retezce pouze z malych nebo velkych pismen abecedy!\n");
                    return false;
                }
            }
            // checks if there are no same values
            for (int j = 1; j < items_len; j++)
            {
                if (i != j)
                {
                    if (strcmp(line_arr[i], line_arr[j]) == 0)
                    {
                        if (*line_arr[0] == 'U')
                        {
                            fprintf(stderr, "Vsechny prvky universa musi byt ruzne!\n");
                            return false;
                        }
                        if (*line_arr[0] == 'S')
                        {
                            fprintf(stderr, "Vsechny prvky mnoziny musi byt ruzne!\n");
                            return false;
                        }
                    }
                }
            }
        }
    }
    // COMMAND VALIDATIONS
    else if (strcmp(line_arr[0], "C") == 0)
    {
        // checks if has accurate number of items
        if (items_len > 4 || items_len < 3)
        {
            fprintf(stderr, "Spatne zadany prikaz!\n");
            return false;
        }
        // validation of correctly written command
        bool is_cmd = false;
        for (int i = 0; i < 17; i++)
        {
            if (strcmp(line_arr[1], cmd_arr[i]) == 0)
            {
                is_cmd = true;
                break;
            }
        }
        if (is_cmd == false)
        {
            fprintf(stderr, "Spatne zadany prikaz!\n");
            return false;
        }
        // one line number is necessary after command
        if (items_len == 3)
        {
            for (int i = 0; i < strlen(line_arr[2]); i++)
            {
                if (!(line_arr[2][i] >= '0' && line_arr[2][i] <= '9'))
                {
                    fprintf(stderr, "Spatne zadany prikaz!\n");
                    return false;
                }
            }
        }
        // two line numbers are necessary after command
        if (items_len == 4)
        {
            for (int i = 0; i < strlen(line_arr[2]); i++)
            {
                if (!(line_arr[2][i] >= '0' && line_arr[2][i] <= '9'))
                {
                    fprintf(stderr, "Spatne zadany prikaz!\n");
                    return false;
                }
            }
            for (int i = 0; i < strlen(line_arr[3]); i++)
            {
                if (!(line_arr[3][i] >= '0' && line_arr[3][i] <= '9'))
                {
                    fprintf(stderr, "Spatne zadany prikaz!\n");
                    return false;
                }
            }
        }
    }
    // RELATIONS VALIDATION
    else if (strcmp(line_arr[0], "R") == 0)
    {
        // parenthesis removal
        int move = 0;
        for (int i = 1; i < items_len; i++)
        {
            for (int j = 0; j < strlen(line_arr[i]); j++)
            {
                if (line_arr[i][j] == ')')
                {
                    line_arr[i][j] = '\0';
                }
                if (line_arr[i][j] == '(')
                {
                    move++;
                }
                line_arr[i][j] = line_arr[i][j + move];
            }
            move = 0;
        }
        // check for banned strings
        for (int i = 1; i < items_len; i++)
        {
            for (int j = 0; j < 19; j++)
            {
                if (strcmp(line_arr[i], cmd_arr[j]) == 0)
                {
                    fprintf(stderr, "Prvky se nesmi jmenovat stejne jako prikazy!\n");
                    return false;
                }
            }
            // char type validation
            for (size_t j = 0; j < strlen(line_arr[i]); j++)
            {
                if (!((line_arr[i][j] >= 'a' && line_arr[i][j] <= 'z') || (line_arr[i][j] >= 'A' && line_arr[i][j] <= 'Z')))
                {
                    fprintf(stderr, "Prvky musi byt retezce pouze z malych nebo velkych pismen abecedy!\n");
                    return false;
                }
            }
        }
        // checks if there are no same pairs
        for (int i = 1; i < items_len; i += 2)
        {
            for (int j = 1; j < items_len; j += 2)
            {
                if (i != j)
                {
                    if (strcmp(line_arr[i], line_arr[j]) == 0 && strcmp(line_arr[i + 1], line_arr[j + 1]) == 0)
                    {
                        fprintf(stderr, "Relace nesmi obsahovat vice stejnych dvojic!\n");
                        return false;
                    }
                }
            }
        }
    }
    // INVALID LINE IDENTIFIER
    else
    {
        fprintf(stderr, "Spatne zadany identifikator radku!\n");
        return false;
    }
    return true;
}
//------SET-HELPERS------
void set_ctor_string(Set *s, char *string, int row)
{
    int items_count = 0;
    char **output_set = my_split(string, ' ', strlen(string), &items_count);
    s->row = row;
    s->size = 0;
    s->items = malloc(items_count * sizeof(char *));
    for (int i = 1; i < items_count; i++)
    {
        s->items[i - 1] = malloc(strlen(output_set[i]) + 1);
        strcpy(s->items[i - 1], output_set[i]);
        s->size = i;
    }
    for (int i = 0; i < items_count; i++)
    {
        free(output_set[i]);
    }
    free(output_set);
}

void set_ctor(Set *s, char **items, int row, int items_len)
{
    s->row = row;
    s->items = malloc(items_len * sizeof(char *));
    for (int i = 0; i < items_len; i++)
    {
        s->items[i] = malloc(strlen(items[i]) + 1);
        strcpy(s->items[i], items[i]);
        s->size = i + 1;
    }
}

void set_print(char **items, int items_count)
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
        printf("{}\n");
}

void set_dtor(Set *s)
{
    s->row = 0;
    for (int i = 0; i < s->size; i++)
    {
        free(s->items[i]);
    }
    free(s->items);
    s->items = NULL;
    s->size = 0;
}

void setarray_ctor(SetArray *setar)
{
    setar->len = 0;
    setar->data = NULL;
}

void *setarray_inc(SetArray *setar)
{
    void *p = realloc(setar->data, (setar->len + 1) * sizeof(Set));
    if (p == NULL)
        return NULL;
    setar->len++;
    return setar->data = p;
}

void *set_copy(Set *src, Set *dst)
{
    set_ctor(dst, src->items, src->row, src->size);
    return dst;
}

void *set_array_append(SetArray *setar, Set *s)
{
    if (setarray_inc(setar))
        return set_copy(s, &setar->data[setar->len - 1]); //musím použít jinou fci než set_ctor, protože ta je přizpůsobená na to, aby to předělala z toho stringu (prakticky jen trochu upravím ten creator, ale se správnýma indexama)
    else
        return NULL;
}

//------SET-OPERATIONS
int empty_com(int line1, char *lines[])
{
    Set s;
    set_ctor_string(&s, lines[line1 - 1], line1);
    if (s.size == 0)
        printf("true");
    else
        printf("false");
    set_dtor(&s);
    return 0;
}
int card_com(int line1, char *lines[])
{
    Set s;
    set_ctor_string(&s, lines[line1 - 1], line1);
    printf("%d\n", s.size);
    set_dtor(&s);
    return 0;
}
int complement_com(int line1, char *lines[])
{
    Set s;
    Set univerzum;
    set_ctor_string(&s, lines[line1 - 1], line1);
    set_ctor_string(&univerzum, lines[0], 1);
    int items_count_comp = 0;
    int is_identical = 0;
    char **comp_arr = malloc(sizeof(char *));
    for (int i = 0; i < univerzum.size; i++)
    {
        for (int j = 0; j < s.size; j++)
        {
            if (!strcmp(univerzum.items[i], s.items[j]))
            {
                is_identical = 1;
                break;
            }
        }
        if (is_identical == 0)
        {
            comp_arr = realloc(comp_arr, (items_count_comp + 1) * sizeof(char *));
            comp_arr[items_count_comp] = malloc(strlen(univerzum.items[i]) + 1);
            strcpy(comp_arr[items_count_comp], univerzum.items[i]);
            items_count_comp++;
        }
        is_identical = 0;
    }
    set_print(comp_arr, items_count_comp);
    set_dtor(&s);
    set_dtor(&univerzum);
    for (int i = 0; i < items_count_comp; i++)
    {
        free(comp_arr[i]);
    }
    free(comp_arr);
    return 0;
}
int union_com(int line1, int line2, char *lines[])
{
    Set first_set;
    Set second_set;
    set_ctor_string(&first_set, lines[line1 - 1], line1);
    set_ctor_string(&second_set, lines[line2 - 1], line2);
    char **uni_arr = malloc(sizeof(char *));
    int items_count_uni = 0, is_identical = 0;
    for (int i = 0; i < first_set.size; i++)
    {
        uni_arr = realloc(uni_arr, (items_count_uni + 1) * sizeof(char *));
        uni_arr[items_count_uni] = malloc(strlen(first_set.items[i]) + 1);
        strcpy(uni_arr[items_count_uni], first_set.items[i]);
        items_count_uni++;
    }

    for (int i = 0; i < second_set.size; i++)
    {
        for (int j = 0; j < items_count_uni; j++)
        {
            if (!strcmp(second_set.items[i], uni_arr[j]))
            {
                is_identical = 1;
                break;
            }
        }
        if (is_identical == 0)
        {
            uni_arr = realloc(uni_arr, (items_count_uni + 1) * sizeof(char *));
            uni_arr[items_count_uni] = malloc(strlen(second_set.items[i]) + 1);
            strcpy(uni_arr[items_count_uni], second_set.items[i]);
            items_count_uni++;
        }
        is_identical = 0;
    }
    set_print(uni_arr, items_count_uni);
    set_dtor(&first_set);
    set_dtor(&second_set);
    for (int i = 0; i < items_count_uni; i++)
    {
        free(uni_arr[i]);
    }
    free(uni_arr);
    return 0;
}
int intersect_com(int line1, int line2, char *lines[])
{
    Set first_set;
    Set second_set;
    set_ctor_string(&first_set, lines[line1 - 1], line1);
    set_ctor_string(&second_set, lines[line2 - 1], line2);
    char **inter_arr = malloc(sizeof(char *));
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
            inter_arr = realloc(inter_arr, (items_count_inter + 1) * sizeof(char *));
            inter_arr[items_count_inter] = malloc(strlen(first_set.items[i]) + 1);
            strcpy(inter_arr[items_count_inter], first_set.items[i]);
            items_count_inter++;
        }
        is_identical = 0;
    }
    set_print(inter_arr, items_count_inter);
    set_dtor(&first_set);
    set_dtor(&second_set);
    for (int i = 0; i < items_count_inter; i++)
    {
        free(inter_arr[i]);
    }
    free(inter_arr);
    return 0;
}
int minus_com(int line1, int line2, char *lines[])
{
    Set first_set;
    Set second_set;
    set_ctor_string(&first_set, lines[line1 - 1], line1);
    set_ctor_string(&second_set, lines[line2 - 1], line2);
    char **min_arr = malloc(sizeof(char *));
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
            min_arr = realloc(min_arr, (items_count_min + 1) * sizeof(char *));
            min_arr[items_count_min] = malloc(strlen(first_set.items[i]) + 1);
            strcpy(min_arr[items_count_min], first_set.items[i]);
            items_count_min++;
        }
        is_identical = 0;
    }
    set_print(min_arr, items_count_min);
    set_dtor(&first_set);
    set_dtor(&second_set);
    for (int i = 0; i < items_count_min; i++)
    {
        free(min_arr[i]);
    }
    free(min_arr);
    return 0;
}
int subseteq_com(int line1, int line2, char *lines[])
{
    Set first_set;
    Set second_set;
    set_ctor_string(&first_set, lines[line1 - 1], line1);
    set_ctor_string(&second_set, lines[line2 - 1], line2);
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
    return 1;
}
int subset_com(int line1, int line2, char *lines[])
{
    if (subseteq_com(line1, line2, lines) && !equals_com(line1, line2, lines))
        printf("true\n");
    else
        printf("false\n");
    return 0;
}
int equals_com(int line1, int line2, char *lines[])
{
    Set first_set;
    Set second_set;
    set_ctor_string(&first_set, lines[line1 - 1], line1);
    set_ctor_string(&second_set, lines[line2 - 1], line2);
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
        return 1;
    }
    else
        printf("false\n");
    set_dtor(&first_set);
    set_dtor(&second_set);
    return 0;
}

//------RELATION-OPERATIONS
int reflexive_com(int line1, char *lines[])
{
    printf("reflexive\n");
    return 0;
}
int symmetric_com(int line1, char *lines[])
{
    printf("symetric\n");
    return 0;
}
int antisymmetric_com(int line1, char *lines[])
{
    printf("antisymmetric\n");
    return 0;
}
int transitive_com(int line1, char *lines[])
{
    printf("transitive\n");
    return 0;
}
int function_com(int line1, char *lines[])
{
    printf("function\n");
    return 0;
}
int domain_com(int line1, char *lines[])
{
    printf("domain\n");
    return 0;
}
int codomain_com(int line1, char *lines[])
{
    printf("codomain\n");
    return 0;
}
int injective_com(int line1, char *lines[])
{
    printf("injective\n");
    return 0;
}
int surjective_com(int line1, char *lines[])
{
    printf("surjective\n");
    return 0;
}
int bijective_com(int line1, char *lines[])
{
    printf("bijective\n");
    return 0;
}
