#include "platform.h"

#define MAX_LINE 512
#define TOK_INC 16

char* get_input(const char* prompt)
{
    char* line = NULL;
    size_t size = 0;

    PRINT("%s", prompt);
    if (getline(&line, &size, stdin) == -1)
    {
        ASSERT(!feof(stdin), "PC: EOF.\n");
        ERROR("PC: Failed to read line.\n");
    }

    return line;
}

char** tokenize(char* str, const char* delim)
{
    int i = 0, max = TOK_INC;

    char** tokens = calloc(max, sizeof(char*));
    ASSERT(tokens != NULL, "PC: Failed to allocate memory.\n");
    char* token;
    char* saveptr = str;

    while((token = strtok_r(saveptr, delim, &saveptr)) != NULL)
    {
        tokens[i] = token;
        i++;

        if (i >= max)
        {
            const int new_max = max + TOK_INC;
            char** temp = realloc(tokens, new_max * sizeof(char*));
            ASSERT(temp != NULL, "PC: Failed to allocate memory.\n");
            tokens = temp;
            memset(tokens + i, 0, new_max - i);
            max = new_max;
        }
    }

    return tokens;
}

int tokens_length(char** tokens)
{
    int len = 0;
    if (!tokens) return -1;

    while(tokens[len] != NULL)
        len++;

    return len;
}

bool is_decimal(char c)
{
    return c >= '0' && c <= '9';
}

bool is_special(char c)
{
    return (c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
}

bool is_space(char c)
{
    return c == 32;
}

bool is_capital(char c)
{
    return c >= 'A' && c <= 'Z';
}

char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');
    return c;
}

bool str_contains_name(const char* str, char* name)
{
    bool contains_name = false;
    char** name_tokens = tokenize(name, " ");

    for (int i = 0; i < tokens_length(name_tokens); i++)
    {
        char* token = name_tokens[i];
        if (strlen(str) < strlen(token))
            continue;

        contains_name = true;

        for (int j = 0; j < strlen(token); j++)
        {
            if (to_lower(str[j]) != to_lower(token[j]))
            {
                contains_name = false;
                break;
            }
        }

        if (contains_name)
            return true;
    }

    free(name_tokens);
    return contains_name;
}

void check_password(char* name, char* password)
{
    bool has_space = false;
    bool has_special = false;
    bool has_number = false;
    bool has_capital = false;
    bool contains_name = false;

    if (strlen(password) < 12)
        WARN("Password should be longer than 12 characters\n");

    for (int i = 0; i < strlen(password); i++)
    {
        char ch = password[i];

        if (is_capital(ch))
            has_capital = true;

        if (is_space(ch))
            has_space = true;

        if (is_decimal(ch))
            has_number = true;

        if (is_special(ch))
            has_special = true;

        if (str_contains_name(&password[i], name))
            contains_name = true;
    }

    if (has_space)
        WARN("Password should not contain any spaces\n");
    if (!has_capital)
        WARN("Password should contain a capital letter\n");
    if (!has_number)
        WARN("Password should at least one number\n");
    if (!has_special)
        WARN("Password should at least one special character\n");
    if (contains_name)
        WARN("Password should not contain part of your name\n");

    if (!has_space && has_capital && has_number && has_special && !contains_name && strlen(password) >= 12)
        PRINT("Password looks good!\n");
}

int main(int argc, char* argv[])
{
    char* name = get_input("Enter your full name: ");
    name[strlen(name) - 1] = '\0'; // Ignore newline
    bool running = true;

    do
    {
        char* password = get_input("Enter your password: ");
        password[strlen(password) - 1] = '\0'; // Ignore newline

        check_password(name, password);

        do
        {
            char* input = get_input("Try another password? (y/n): ");
            input[strlen(input) - 1] = '\0'; // Ignore newline

            if (strcmp(input, "y") == 0)
                break;
            if (strcmp(input, "n") == 0)
            {
                running = false;
                break;
            }
        }
        while(true);
    }
    while (running);

    return 0;
}
