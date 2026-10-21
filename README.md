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

2. **Read loop** — `read()` fills the buffer from where it left off (`&buffer[ft_strlen(buffer)]`). Before each read, the accumulated buffer is scanned for a newline character using `ft_strchr`.

3. **Newline found** — The portion up to and including `\n` is extracted and returned as the line. Everything after `\n` is saved into `remainder` for the next call via a fresh allocation. The buffer is then freed.

4. **Newline not found** — The buffer is grown using `ft_double_buffer`, which reallocates with extra space for the next `BUFFER_SIZE` bytes, copies the existing content, and frees the old block. The loop continues reading.

5. **End of file** — When `read()` returns 0, the loop exits. If the buffer contains data (last line without `\n`), it is returned directly. If the buffer is empty, `NULL` is returned.

**Why this approach?**
A static remainder avoids re-reading data already consumed from the file descriptor. Growing the buffer dynamically ensures correctness regardless of line length or `BUFFER_SIZE`. The design keeps a clear separation between accumulation (`ft_double_buffer`), extraction (`ft_return_line`), and state management (`remainder`).

---

## Instructions

### Compile and run

```sh
make demo
printf 'first\nsecond\n' | ./gnl
cc -Wall -Wextra -Werror -I. -DBUFFER_SIZE=42 examples/read_lines.c get_next_line.c get_next_line_utils.c -o gnl
```

The checked-in [demo](examples/read_lines.c) reads standard input.
To embed the function, compile both source files with your application's main.

### Public regressions

```sh
make test
```

On Linux/GCC this builds ASan/UBSan checks at BUFFER_SIZE 1/42/4096 using
a linker malloc wrapper. Tests cover a long line, empty line, final line
without a newline, repeated EOF, buffered lines in an open pipe, read errors,
descriptor reuse and allocation failures. For a compiler without sanitizer
support use `make test SANITIZERS=`.

Only one descriptor stream is supported at a time: this mandatory-version
implementation shares a single static remainder. Drain to EOF before switching
streams. It does not implement the multi-fd bonus. A read error clears the
remainder. Callers own and must free every returned line.

The 2026-10-21 Linux run passed ASan/UBSan with leak detection disabled because
LeakSanitizer could not inspect process threads in that environment; a separate
leak checker was not run.

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

Maintenance fixes and public regression tests were prepared with OpenAI Codex on 2026-10-21.
