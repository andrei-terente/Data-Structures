// Copyright 2018 Terente Andrei-Alexandru
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

// using namespace std;

// <DoublyLinkedList NODE>
template <class T>
struct Node
{
    T data;
    Node<T> *next;
    Node<T> *prev;
    static Node<T>* getnode(T data) {
    	Node<T>* newNode = new Node<T>;
    	newNode->data = data;
    	newNode->prev = newNode->next = NULL;
    	return newNode;
    }
    
};
// </DoublyLinkedList NODE>

// <DEQUE IMPLEMENTATION>
template<class T>
class Deque
{
public:

	Node<T>* front;
	Node<T>* rear;
	int Size;

	Deque() {
		front = rear = NULL;
		Size = 0;
	}

	bool isEmpty() {
		return (front == NULL);
	}

	Node<T> *getRear() {
		return rear;
	}

	Node<T> *getFront() {
		return front;
	}

	int size() {
		return Size;
	}

	void addFront(T data) {
		Node<T>* newNode = Node<T>::getnode(data);
		if (newNode == NULL) {
			std::cout << "Overflow\n";
		} else {
			if (front == NULL) {
				front = rear = newNode;
			} else {
				newNode->next = front;
				front->prev = newNode;
				front = newNode;
			}
			Size++;
		}
	}

	void addRear(T data) {
		Node<T>* newNode = Node<T>::getnode(data);
		if (newNode == NULL) {
			std::cout << "Overflow\n";
		} else {
			if (front == NULL) {
				front = rear = newNode;
			} else {
				newNode->prev = rear;
				rear->next = newNode;
				rear = newNode; 
			}
			Size++;
		}

	}

	void deleteFront() {
		if (front == NULL) {
			std::cout << "Underflow\n";
		} else {
			Node<T>* temp = front;
			front = front->next;
			if (front == NULL) {
				rear = NULL;
			} else {
				front->prev = NULL;
			}
			delete temp;
			Size--;
		}
	}

	void deleteRear() {
		if (front == NULL) {
			std::cout << "Underflow\n";
		} else {
			Node<T>* temp = rear;
			rear = rear->prev;
			if (rear == NULL) {
				front = NULL;
			} else {
				rear->next = NULL;
			}
			delete temp;
			Size--;
		}
	}

	void erase() {
		rear = NULL;
		while (front != NULL) {
			Node<T>* temp = front;
			front = front->next;
			delete temp;
		}
		Size = 0;
	}

}; 
// </DEQUE IMPLEMENTATION>

// <STACK IMPLEMENTATION>
template <class T>
class Stack {
public:

	T* stackArray;
	int topLevel;
	int maxSize;
	static const int growthFactor = 2;

	Stack() {
		maxSize = 10;
		topLevel = 0;
		stackArray = new T[maxSize];
	}

	~Stack() {
		delete[] stackArray;
	}

	void grow() {
		maxSize *= growthFactor;
		T* auxArray = new T[maxSize];
		for (int i = 0; i < topLevel; ++i) {
			auxArray[i] = stackArray[i];
		}
		delete[] stackArray;
		stackArray = auxArray;
	}

	void push(T data) {
		if (topLevel == maxSize) {
			this->grow();
		}
		stackArray[topLevel++] = data;
	}

	void pop() {
		if (topLevel == 0) {
			std::cout << "Stack is empty\n";
		} else {
			topLevel--;
		}
	}

	T peek() {
		return stackArray[topLevel-1];
	}

	T* top() {
		return &stackArray[topLevel-1];
	}

	bool isEmpty() {
		return (topLevel == 0);
	}

};
// </STACK IMPLEMENTATION>

// <COMMAND CLASS>
class COMMAND {
public:
	char type;
	int robotID;
	int mapX, mapY;
	int boxes;
	int priority;
	int waitTime;
	bool isExecute;

	COMMAND() {
	}

	COMMAND(char type, int robotID, int mapX, int mapY, int boxes,
			int priority,int waitTime, bool isExecute) {
		this->type = type;
		this->robotID = robotID;
		this->mapX = mapX;
		this->mapY = mapY;
		this->boxes = boxes;
		this->priority = priority;
		this->waitTime = waitTime;
		this->isExecute = isExecute;
	}
	
	~COMMAND() {}

	COMMAND& operator=(COMMAND other) {
		type = other.type;
		robotID = other.robotID;
		mapX = other.mapX;
		mapY = other.mapY;
		boxes = other.boxes;
		priority = other.priority;
		waitTime = other.waitTime;
		isExecute = other.isExecute;
		return *this;
	}

	void setExecStatus(bool status) {
		this->isExecute = status;
	}

	void setWaitTime(int time) {
		this->waitTime = time;
	}

	void printCommand(std::ofstream &out) {
		if (type == 'G') {
			out << " GET " << mapX << " " << mapY << " " << boxes;
		} else {
			out << " DROP " << mapX << " " << mapY << " " << boxes;
		}
	}
};
// </COMMAND CLASS>

typedef struct {
	int nrBoxes = 0;
	Deque<COMMAND> commands;

	void increaseTime() {
		Node<COMMAND> *temp;
		temp = commands.front;
		if (!temp) {
			return;
		}
		while (temp) {
			temp->data.waitTime++;
			temp = temp->next;
		}
	}

	void printAll(std::ofstream &out) {
		Node<COMMAND> *temp;
		temp = commands.front;
		if (!temp) {
			return;
		}
		while (temp) {
			temp->data.printCommand(out);
			if (temp != commands.rear) {
				out << ";";
			}
			temp = temp->next;
		}

	}

} Robot;

int main() {

	std::string buffer;
	std::ifstream in;
	std::ofstream out;
	int n, m, nrRobots, rID, X, Y, boxNr, prrty;
	char type;
	Stack<COMMAND> history, exec_history;

	in.open("robots.in");
	out.open("robots.out");

	in >> buffer;
	nrRobots = std::stoi(buffer);
	// ROBOT ARRAY INIT USING GIVEN INPUT
	Robot robotArray[nrRobots];
	in >> buffer;
	n = std::stoi(buffer);
	in >> buffer;
	m = std::stoi(buffer);
	// MAP MATRIX INIT USING GIVEN INPUT
	int map[n][m];

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			in >> buffer;
			map[i][j] = std::stoi(buffer);
		}
	}

	// INPUT FILE LINE BY LINE PARSING
	while (in >> buffer) {
		// ADD_GET_BOX RobotID X Y NrBoxes Priority
		if (buffer == "ADD_GET_BOX") {
			// getting command parameters
			in >> buffer;
			rID = std::stoi(buffer);
			in >> buffer;
			X = std::stoi(buffer);
			in >> buffer;
			Y = std::stoi(buffer);
			in >> buffer;
			boxNr = std::stoi(buffer);
			in >> buffer;
			prrty = std::stoi(buffer);
			// creating auxiliary object
			COMMAND tempComm('G', rID, X, Y, boxNr, prrty, 1, false);
			// adding command to history stack
			history.push(tempComm);
			// increasing wait time for the other commands in robots' deques
			for (int i = 0; i < nrRobots; ++i) {
				robotArray[i].increaseTime();
			}
			// adding command to robot deque by priority
			if (!prrty) {
				robotArray[rID].commands.addFront(tempComm);
			} else {
				robotArray[rID].commands.addRear(tempComm);
			}
		}
		// ADD_DROP_BOX RobotID X Y NrBoxes Priority
		if (buffer == "ADD_DROP_BOX") {
			// getting command parameters
			in >> buffer;
			rID = std::stoi(buffer);
			in >> buffer;
			X = std::stoi(buffer);
			in >> buffer;
			Y = std::stoi(buffer);
			in >> buffer;
			boxNr = std::stoi(buffer);
			in >> buffer;
			prrty = std::stoi(buffer);
			COMMAND tempComm('D', rID, X, Y, boxNr, prrty, 1, false);
			// adding command to history stack
			history.push(tempComm);
			// increasing wait time for the other commands in robots' deques
			for (int i = 0; i < nrRobots; ++i) {
				robotArray[i].increaseTime();
			}
			// adding command to robot deque by priority
			if (!prrty) {
				robotArray[rID].commands.addFront(tempComm);
			} else {
				robotArray[rID].commands.addRear(tempComm);
			}

		}
		// EXECUTE RobotID
		if (buffer == "EXECUTE") {
			// getting robot id
			in >> buffer;
			rID = std::stoi(buffer);
			// checking if there are any commands in robot's deque
			if (robotArray[rID].commands.isEmpty()) {
				// printing message
				out << "EXECUTE: No command to execute\n";
				// increasing time
				for (int i = 0; i < nrRobots; ++i) {
				robotArray[i].increaseTime();
			}
				continue;
			}
			// getting parameters of command
			X = robotArray[rID].commands.getFront()->data.mapX;
			Y = robotArray[rID].commands.getFront()->data.mapY;
			boxNr = robotArray[rID].commands.getFront()->data.boxes;
			type = robotArray[rID].commands.getFront()->data.type;
			// moving boxes
			if (type == 'D') {
				// identifying the correct number of boxes
				// = min(boxes to be moved, boxes in robot's inventory)
				boxNr = boxNr > robotArray[rID].nrBoxes
						? robotArray[rID].nrBoxes : boxNr;
				map[X][Y] += boxNr;
				robotArray[rID].nrBoxes -= boxNr;
			} else if (type == 'G') {
				// identifying the correct number of boxes
				// = min(boxes to be moves, boxes in map[X][Y])
				boxNr = boxNr > map[X][Y] ? map[X][Y] : boxNr;
				map[X][Y] -= boxNr;
				robotArray[rID].nrBoxes += boxNr;
			}
			// updating command parameters
			robotArray[rID].commands.getFront()->data.boxes = boxNr;
			robotArray[rID].commands.getFront()->data.isExecute = true;
			// moving command to both history and exec_history stacks
			history.push(robotArray[rID].commands.getFront()->data);
			exec_history.push(robotArray[rID].commands.getFront()->data);
			// removing command from robot's deque
			robotArray[rID].commands.deleteFront();
			// increasing wait time for the other commands in robots' deques
			for (int i = 0; i < nrRobots; ++i) {
				robotArray[i].increaseTime();
			}
		}
		// LAST_EXECUTED_COMMAND
		if (buffer == "LAST_EXECUTED_COMMAND") {
			// increasing wait time for the other commands in robots' deques
			for (int i = 0; i < nrRobots; ++i) {
				robotArray[i].increaseTime();
			}
			// if exec_history is empty
			// printing message and moving to next the line in input

			if (exec_history.isEmpty()) {
				out << "LAST_EXECUTED_COMMAND: No command was executed\n";
				continue; 
			}
			// printing output
			out << "LAST_EXECUTED_COMMAND: " << exec_history.peek().robotID
				 << ":";
			exec_history.peek().printCommand(out);
			out << std::endl;
		}
		if (buffer == "HOW_MUCH_TIME") {
			// increasing wait time for the other commands in robots' deques
			for (int i = 0; i < nrRobots; ++i) {
				robotArray[i].increaseTime();
			}
			// if exec_history is empty
			// printing message and moving to the next line in input
			if (exec_history.isEmpty()) {
				out << "HOW_MUCH_TIME: No command was executed\n";
				continue;
			}
			// printing output
			out << "HOW_MUCH_TIME: " << exec_history.peek().waitTime << std::endl;
		}
		// HOW_MANY_BOXES RobotID
		if (buffer == "HOW_MANY_BOXES") {
			// increasing wait time for the other commands in robots' deques
			for (int i = 0; i < nrRobots; ++i) {
				robotArray[i].increaseTime();
			}
			in >> buffer;
			rID = std::stoi(buffer);
			out << "HOW_MANY_BOXES: " << robotArray[rID].nrBoxes << std::endl;
		}
		// PRINT_COMMANDS RobotID
		if (buffer == "PRINT_COMMANDS") {
			// increasing wait time for the other commands in robots' deques
			for (int i = 0; i < nrRobots; ++i) {
				robotArray[i].increaseTime();
			}
			in >> buffer;
			rID = std::stoi(buffer);
			if (robotArray[rID].commands.isEmpty()) {
				out << "PRINT_COMMANDS: No command found\n";
				continue;
			}
			out << "PRINT_COMMANDS: " << rID << ":";
			robotArray[rID].printAll(out);
			out << std::endl;
		}
		// UNDO
		if (buffer == "UNDO") {
			// increasing wait time for the other commands in robots' deques
			for (int i = 0; i < nrRobots; ++i) {
				robotArray[i].increaseTime();
			}
			// if there is no history
			if (history.isEmpty()) {
				// printing message and moving to next line
				out << "UNDO: No history\n";
				continue;
			}
			// getting parameters of last command given
			type = history.peek().type;
			rID = history.peek().robotID;
			X = history.peek().mapX;
			Y = history.peek().mapY;
			boxNr = history.peek().boxes;
			prrty = history.peek().priority;
			// in case last given command was an EXECUTE
			if (history.peek().isExecute) {
				// undoing box movement
				if (type == 'G') {
					map[X][Y] += boxNr;
					robotArray[rID].nrBoxes -= boxNr;
				} else if (type == 'D') {
					map[X][Y] -= boxNr;
					robotArray[rID].nrBoxes += boxNr;
				}
				// adding the ADD/GET command to the robot's deque
				history.top()->setExecStatus(false);
				history.top()->setWaitTime(1);
				robotArray[rID].commands.addFront(history.peek());
				
				// removing the EXECUTE from history
				history.pop();
				exec_history.pop();
			// in case last given command was an ADD/GET
			} else {
				// removing the ADD/GET command from deque
				if (prrty == 0) {
					robotArray[rID].commands.deleteFront();
				} else if (prrty == 1) {
					robotArray[rID].commands.deleteRear();
				}
				// removing the ADD/GET command from history
				history.pop();
			}
		}
	}

	// ROBOT QUEUES FREEING
	for (int i = 0; i < nrRobots; ++i) {
		robotArray[i].commands.erase();
	}

	// closing files
	in.close();
	out.close();


	return 0;
}
