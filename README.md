# get_next_line

*This project has been created as part of the 42 curriculum by ypopovyc.*

---

## Description

`get_next_line` is a C function that reads a file descriptor line by line, returning one line per call — including the newline character `\n` when present. The function can be called repeatedly to iterate through an entire file, and returns `NULL` when the file is exhausted or an error occurs.

The goal of the project is to understand static variables, dynamic memory management, and the challenges of buffered I/O in C.

---

## Algorithm

The function uses a **static remainder pointer** to persist leftover data between calls. Here is the logic step by step:

1. **Initialization** — If a remainder exists from a previous call, it is copied into a fresh buffer as the starting point. Otherwise an empty buffer of size `BUFFER_SIZE + 1` is allocated.

2. **Read loop** — `read()` fills the buffer from where it left off (`&buffer[ft_strlen(buffer)]`). After each read, the buffer is scanned for a newline character using `ft_strchr`.

3. **Newline found** — The portion up to and including `\n` is extracted and returned as the line. Everything after `\n` is saved into `remainder` for the next call via a fresh allocation. The buffer is then freed.

4. **Newline not found** — The buffer is grown using `ft_double_buffer`, which reallocates with extra space for the next `BUFFER_SIZE` bytes, copies the existing content, and frees the old block. The loop continues reading.

5. **End of file** — When `read()` returns 0, the loop exits. If the buffer contains data (last line without `\n`), it is returned directly. If the buffer is empty, `NULL` is returned.

**Why this approach?**
A static remainder avoids re-reading data already consumed from the file descriptor. Growing the buffer dynamically ensures correctness regardless of line length or `BUFFER_SIZE`. The design keeps a clear separation between accumulation (`ft_double_buffer`), extraction (`ft_return_line`), and state management (`remainder`).

---

## Instructions

### Compilation

The project is compiled with a custom `BUFFER_SIZE` passed as a preprocessor flag:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl
```

You can replace `42` with any positive integer to test different buffer sizes.

### Usage example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int   fd;
    char  *line;

    fd = open("file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	char	*line;

	while (1)
	{
		line = get_next_line(0);
		if (!line)
			break ;
		printf("%s", line);
		free(line);
	}
	return (0);
}
```

### Testing with valgrind

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c  -o gnl
valgrind --leak-check=full ./gnl
```

Test with multiple values of `BUFFER_SIZE` — especially `1`, `42`, and very large values.

---

## Resources

- [man 2 read](https://man7.org/linux/man-pages/man2/read.2.html) — POSIX read() system call documentation
- [man 3 malloc](https://man7.org/linux/man-pages/man3/malloc.3.html) — Dynamic memory allocation
- [Static variables in C — GeeksForGeeks](https://www.geeksforgeeks.org/static-variables-in-c/) — Explanation of static variable lifetime and scope
- [Valgrind user manual](https://valgrind.org/docs/manual/manual.html) — Memory error detection

### AI usage

Claude (claude.ai) was used during this project as a **guidance tool**. Specifically:

- Understanding why `free()` on a mid-block pointer causes invalid free errors
- Interpreting valgrind output to locate memory leaks and invalid reads/writes

Claude did not write any code directly. All implementation decisions and code were written by the student, with AI used to ask questions and verify understanding — similar to asking a peer for guidance.
