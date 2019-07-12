CC = g++ -std=c++11
CFLAGS = -Wall
LDFLAGS = -lGL -lGLU -lglut

sim: *.cpp
	$(CC) $(CFLAGS) -g $? $(LDFLAGS) -o ../bin/debug/$@

release: *.cpp
	$(CC) $(CFLAGS) -O2 $? $(LDFLAGS) -o ../bin/release/sim
