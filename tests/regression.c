#include "get_next_line.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

void *__real_malloc(size_t size);
static int fail_at = -1;
static int allocations;
void *__wrap_malloc(size_t size)
{
    if (fail_at == allocations++)
        return NULL;
    return __real_malloc(size);
}
static void expect(int fd, const char *text)
{
    char *line = get_next_line(fd);
    assert(line && strcmp(line, text) == 0);
    free(line);
}
int main(void)
{
    int fds[2], fd, i;
    char *line;
    FILE *file = tmpfile();
    assert(file);
    fputs("\n", file);
    for (i = 0; i < 9000; i++)
        fputc('x', file);
    fputs("\nlast", file);
    fflush(file);
    rewind(file);
    fd = fileno(file);
    expect(fd, "\n");
    line = get_next_line(fd);
    assert(line && strlen(line) == 9001 && line[9000] == '\n');
    free(line);
    expect(fd, "last");
    assert(get_next_line(fd) == NULL);
    assert(get_next_line(fd) == NULL);
    fclose(file);
    assert(pipe(fds) == 0);
    assert(write(fds[1], "a\nb\n", 4) == 4);
    alarm(3);
    expect(fds[0], "a\n");
    expect(fds[0], "b\n"); /* Writer stays open: must not read again. */
    alarm(0);
    close(fds[1]);
    assert(get_next_line(fds[0]) == NULL);
    close(fds[0]);
    file = tmpfile();
    assert(file);
    fputs("a\npartial", file);
    fflush(file);
    rewind(file);
    expect(fileno(file), "a\n");
    fd = fileno(file);
    fclose(file);
    assert(get_next_line(fd) == NULL); /* read error clears remainder */
    fd = open("/dev/null", O_RDONLY);
    assert(fd >= 0 && get_next_line(fd) == NULL);
    close(fd);
    for (i = 0; i < 20; i++) {
        file = tmpfile();
        assert(file);
        fputs("abc\ndef\n", file);
        fflush(file);
        rewind(file);
        allocations = 0;
        fail_at = i;
        line = get_next_line(fileno(file));
        free(line);
        fail_at = -1;
        while ((line = get_next_line(fileno(file))))
            free(line);
        fclose(file);
    }
    puts("GNL file, pipe, read-error and allocation-failure checks passed");
}
