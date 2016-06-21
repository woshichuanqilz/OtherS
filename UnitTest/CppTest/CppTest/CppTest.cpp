// queue::push/pop
#include "stdafx.h"
#include <iostream>       // std::cin, std::cout
#include <queue>          // std::queue

int add(int a, int b)
{
	return a + b;
}

int main()
{
	// example of the queue
	std::queue<int> myqueue;
	int myint;

	std::cout << "Please enter some integers (enter 0 to end):\n";

	do {
		std::cin >> myint;
		myqueue.push(myint);
	} while (myint);

	std::cout << "myqueue contains: ";
	while (!myqueue.empty())
	{
		std::cout << ' ' << myqueue.front();
		myqueue.pop();
	}
	std::cout << '\n';

	// Example of the test.
	printf("the add result is %d\n", add(1, 3));

	system("pause");
	return 0;
}