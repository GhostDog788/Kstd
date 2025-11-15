#include <iostream>
#include "Utils.hpp"
#include "CircularBuffer.h"

constexpr auto BUFFER_SIZE = 5;

int main()
{
	CircularBuffer<int> buffer(BUFFER_SIZE);
	for (int i = -10; i <= 15; ++i)
	{
		buffer.push(i);
		std::cout << serialize_circular_buffer(buffer) << std::endl;
	}

	// Move semantics test
	CircularBuffer<int> buffer2(std::move(buffer));
	std::cout << "Move constructor: " << serialize_circular_buffer(buffer2) << std::endl;
	buffer = std::move(buffer2);
	std::cout << "Move instance: " << serialize_circular_buffer(buffer) << std::endl;
	CircularBuffer<int> buffer3 = std::move(buffer);
	std::cout << "Move copy constructor: " << serialize_circular_buffer(buffer3) << std::endl;
}
