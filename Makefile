FLAGS = -std=c++11
C = g++
all : mkdirs main LoadBalancer Worker Presenter   

mkdirs:
	@mkdir -p build
	@mkdir -p bin
	@mkdir -p namedPipe

main : src/main.cpp 
	$(C) $(FLAGS) src/main.cpp -o main


Worker : build/Worker.o build/Util.o
	$(C) $(FLAGS) build/Worker.o build/Util.o -o bin/Worker
	
build/Worker.o :  src/Worker.cpp src/Worker.h
	$(C) $(FLAGS) -c src/Worker.cpp -o build/Worker.o


Presenter : build/Presenter.o build/Util.o
	$(C) $(FLAGS) build/Presenter.o build/Util.o -o bin/Presenter

build/Presenter.o :  src/Presenter.cpp src/Presenter.h
	$(C) $(FLAGS) -c src/Presenter.cpp -o build/Presenter.o


LoadBalancer : build/LoadBalancer.o build/Util.o

	$(C) $(FLAGS) build/LoadBalancer.o build/Util.o -o bin/LoadBalancer

build/LoadBalancer.o : src/LoadBalancer.cpp src/LoadBalancer.h
	$(C) $(FLAGS) -c src/LoadBalancer.cpp -o build/LoadBalancer.o


build/Util.o : src/Util.cpp src/Util.h
	$(C) $(FLAGS) -c src/Util.cpp -o build/Util.o

clean : 
	rm -f -r build
	rm -f -r bin
	rm -f -r namedPipe
	rm -f main
