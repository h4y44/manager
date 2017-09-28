TARGET = manager
CFLAGS = -Wall -c -D__DEBUG__ -D__THREAD_SAFE__

all:
	$(CC) $(CFLAGS) session.c -o session.o
	$(CC) $(CFLAGS) database.c -o database.o
	$(CC) $(CFLAGS) user.c -o user.o
	$(CC) $(CFLAGS) main.c -o main.o
	$(CC) $(CFLAGS) util.c -o util.o
	$(CC) $(CFLAGS) menu.c -o menu.o
	$(CC) *.o -o $(TARGET)

	@echo "Build done"

clean:
	$(RM) *.o
	@echo "Clean done!"

sweep:
	$(RM) *.o
	$(RM) $(TARGET)
	$(RM) *.dat
	@echo "Sweep done!"
