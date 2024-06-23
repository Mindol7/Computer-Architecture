CC = g++ -O2
OBJS = addr.o alu.o BranchPredictor.o fr.o latch.o stage.o VerifyPredict.o main.o alg.o cache.o
TARGET = main

clean:
	rm -f *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

addr.o: addr.cpp addr.hpp
	$(CC) -c addr.cpp

alu.o: alu.cpp alu.hpp
	$(CC) -c alu.cpp

BranchPredictor.o: BranchPredictor.cpp BranchPredictor.hpp
	$(CC) -c BranchPredictor.cpp

fr.o: fr.cpp fr.hpp
	$(CC) -c fr.cpp

latch.o: latch.cpp latch.hpp
	$(CC) -c latch.cpp

stage.o: stage.cpp stage.hpp
	$(CC) -c stage.cpp

VerifyPredict.o: VerifyPredict.cpp VerifyPredict.hpp
	$(CC) -c VerifyPredict.cpp

alg.o: alg.cpp alg.hpp
	$(CC) -c alg.cpp

cache.o: cache.cpp cache.hpp
	$(CC) -c cache.cpp

main.o: main.cpp addr.hpp alu.hpp base.hpp BranchPredictor.hpp fr.hpp latch.hpp stage.hpp VerifyPredict.hpp alg.hpp cache.hpp
	$(CC) -c main.cpp
