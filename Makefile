CC=gcc
CFLAGS=-Wall
TARGETS=main.o
FILES=main.c include/queue.c include/helpers.c include/insts.c include/ram.c workers/clock.c workers/timer.c workers/process_gen.c workers/sched.c

main.o:
	$(CC) $< $(CFLAGS) -o $@ $(FILES) -lm -lpthread

clean:
	rm $(TARGETS)
