CC = g++
CFLAGS = -Wall
LDFLAGS = -lGL -lGLU -lglut

sim: *.cpp
	$(CC) $(CFLAGS) -g $? $(LDFLAGS) -o ../bin/debug/$@

release: *.cpp
	$(CC) $(CFLAGS) $? $(LDFLAGS) -o ../bin/release/sim
