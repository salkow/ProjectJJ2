##################################
#								 #
#	Emmanouil-Nikolaos Skouris   #
#								 #
##################################

CC := g++
FLAGS := -Wall -g3 -std=c++11 -pthread
ARGS :=
oFiles := $(patsubst %.cpp,%.o,$(wildcard classes/*.cpp)) # get all .cpp files and replace the extension with .o
oFileMain :=  $(patsubst %.cpp,%.o,$(wildcard *.cpp))
cppFileMain := $(wildcard *.cpp)
.PHONY: all

all: main test_runner
	./test_runner

test_runner: $(oFiles) $(wildcard tests/*.cpp) acutest.h test_runner.cpp
	$(CC) -o test_runner test_runner.cpp $(oFiles) $(FLAGS)

main: $(oFiles) main.o
	$(CC) -o $(patsubst %,%,$@) $^ $(FLAGS)

%.o: %.cpp %.hpp
	$(CC) $(FLAGS) -c -o $@ $<

main.o: main.cpp
	$(CC) $(FLAGS) -c -o $@ $<


# $(oFileMain): $(cppFileMain)
# 	$(CC) $(FLAGS) -c -o $@ $<



clean:
	rm classes/*.o tests/*.o *.o main

run:
	./main  $(ARGS)

run_tests:
	./test_runner
