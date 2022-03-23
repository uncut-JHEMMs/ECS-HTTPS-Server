target = exec
objs = main.o
cflags = -c -Wall -g
cxxflags = -std=c++17
data_files = emails.xml

all: $(target)

$(target): $(objs)
	clang++ -o $(target) $(objs) -ljsoncpp -lhttpserver -pthread -lxml2 -lxmlsec1 $(shell pkg-config --cflags --libs openssl)

main.o: main.cpp server/*
	clang++ $(cflags) $(cxxflags) main.cpp $(pkg-config --cflags --libs openssl)

clean:
	rm -r *.o $(target) $(data_files)
