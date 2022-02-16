target = exec
objs = main.o
cflags = -c -Wall -g
cxxflags = -std=c++17

all:$(target)
<<<<<<< HEAD:server/makefile
$(target): $(objs) 
	clang++ -o $(target) $(objs) -lhttpserver -ljsoncpp -pthread
=======
$(target): $(objs)
	clang++ -o $(target) $(objs) -ljsoncpp -lhttpserver -pthread
>>>>>>> origin/development:src/server/makefile

main.o: main.cpp ThreadPool.h ThreadSafeQueue.h
	clang++ $(cflags) $(cxxflags) main.cpp

clean:
	rm -r *.o $(target) log
