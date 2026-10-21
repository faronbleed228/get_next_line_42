#include "get_next_line.h"
#include <stdio.h>

int main(void)
{
    char *line;

    line = get_next_line(STDIN_FILENO);
    while (line)
    {
        fputs(line, stdout);
        free(line);
        line = get_next_line(STDIN_FILENO);
    }
    return (0);
}
