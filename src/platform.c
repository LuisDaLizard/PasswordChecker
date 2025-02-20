#include "platform.h"


int64_t getline(char** line, size_t* len, FILE* fp)
{
    if (!line || !len)
    {
        WARN("PC: Bad arguments.\n");
        return -1;
    }

    if (!fp)
    {
        WARN("PC: Bad file pointer.\n");
        return -1;
    }

    char chunk[128];

    if (!*line)
    {
        *len = sizeof(chunk);
        if ((*line = malloc(*len)) == NULL)
        {
            WARN("PC: Failed to allocate memory.\n");
            return -1;
        }
    }

    (*line)[0] = '\0';

    while(fgets(chunk, sizeof(chunk), fp) != NULL)
    {
        if (*len - strlen(*line) < sizeof(chunk))
        {
            *len *= 2;
            if ((*line = realloc(*line, *len)) == NULL)
            {
                WARN("PC: Failed to allocate memory.\n");
                free(line);
                return -1;
            }
        }

        strcat(*line, chunk);

        if ((*line)[strlen(*line) - 1] == '\n')
            return strlen(*line);
    }

    return -1;
}