CC=g++
EXEC=tema1
FLAGS= -std=c++11

build: $(EXEC)
$(EXEC): tema1.o
	$(CC) $(FLAGS) $^ -o $(EXEC)
tema1.o: tema1.cpp
	$(CC) $(FLAGS) -c $^
run: $(EXEC)
	./$(EXEC)
clean:
	rm -f tema1.o $(EXEC) 
