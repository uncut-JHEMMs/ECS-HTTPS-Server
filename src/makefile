target = exec
objs = main.o
cflags = -c -Wall -g
cxxflags = -std=c++17
data_files = emails.xml

all: $(target)

$(target): $(objs)
	clang++ -o $(target) $(objs) -ljsoncpp -lhttpserver -pthread

main.o: main.cpp server/*
	clang++ $(cflags) $(cxxflags) main.cpp 

clean:
	rm -r *.o $(target) $(data_files)
