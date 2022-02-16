target = exec
objs = main.o
cflags = -c -Wall -g
cxxflags = -std=c++17

all:$(target)
$(target): $(objs) 
	clang++ -o $(target) $(objs) -lhttpserver -ljsoncpp -pthread

main.o: main.cpp ThreadPool.h ThreadSafeQueue.h
	clang++ $(cflags) $(cxxflags) main.cpp

clean:
	rm -r *.o $(target)
