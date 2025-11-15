#include <iostream>
#include <string>
#include "CircularBuffer.h"

constexpr auto BUFFER_SIZE = 5;

template<typename T>
std::string serialize_circular_buffer(CircularBuffer<T>& buffer)
{
	std::string result;
	result += '[';
	T item;
	for (size_t i = 0; i < buffer.size(); ++i)
	{
		if (!buffer.at(i, item)) break;
		if (i != 0) result += " ";
		result += std::to_string(item);
	}
	result += ']';
	return result;
}

int main()
{
	CircularBuffer<int> buffer(BUFFER_SIZE);
	for (int i = -10; i <= 15; ++i)
	{
		buffer.push(i);
		std::cout << serialize_circular_buffer(buffer) << std::endl;
	}
}
