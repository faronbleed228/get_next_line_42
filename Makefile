CC = cc
CFLAGS = -Wall -Wextra -Werror
SANITIZERS ?= -g -fsanitize=address,undefined -fno-pie -no-pie

test:
	@mkdir -p .tests
	@set -e; for size in 1 42 4096; do \
		$(CC) $(CFLAGS) $(SANITIZERS) -I. -DBUFFER_SIZE=$$size tests/regression.c get_next_line.c get_next_line_utils.c -Wl,--wrap=malloc -o .tests/gnl_$$size; \
		.tests/gnl_$$size; \
	done

demo:
	$(CC) $(CFLAGS) -I. -DBUFFER_SIZE=42 examples/read_lines.c get_next_line.c get_next_line_utils.c -o gnl

clean:
	rm -rf .tests gnl

.PHONY: test demo clean
