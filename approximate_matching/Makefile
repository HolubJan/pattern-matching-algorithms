CC=gcc
#CFLAGS=-g -lm
CFLAGS=-g
CPPFLAG=

% : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@


.PHONY: clean
clean:
	rm -f *.o *~
