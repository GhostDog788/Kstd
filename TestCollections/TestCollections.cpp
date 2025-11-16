#include <iostream>
#include <kstd/memory/move.hpp>
#include <kstd/collections/CircularBuffer.hpp>
#include <kstd/collections/String.hpp>
#include "Utils.hpp"

constexpr auto BUFFER_SIZE = 5;

int main()
{
	kstd::CircularBuffer<int> buffer(BUFFER_SIZE);
	for (int i = -10; i <= 15; ++i)
	{
		buffer.push(i);
		std::cout << serialize_circular_buffer(buffer) << std::endl;
	}

	// Move semantics test
	kstd::CircularBuffer<int> buffer2(kstd::move(buffer));
	std::cout << "Move constructor: " << serialize_circular_buffer(buffer2) << std::endl;
	buffer = kstd::move(buffer2);
	std::cout << "Move instance: " << serialize_circular_buffer(buffer) << std::endl;
	kstd::CircularBuffer<int> buffer3 = kstd::move(buffer);
	std::cout << "Move copy constructor: " << serialize_circular_buffer(buffer3) << std::endl;

	kstd::wstring str1;
	for (int i = 0; i < BUFFER_SIZE * 3; ++i)
	{
		str1.append(*L"X" + i);
	}
	std::wcout << L"String contents: " << str1.data() << L" Capacity: " << str1.capacity() << L" Size: " << str1.size() << std::endl;

	for (int i = 0; i < BUFFER_SIZE * 2; ++i)
	{
		str1.pop();
	}
	std::wcout << L"String contents: " << str1.data() << L" Capacity: " << str1.capacity() << L" Size: " << str1.size() << std::endl;
	str1 += kstd::wstring(L"Hello world!");
	std::wcout << L"String contents: " << str1.data() << L" Capacity: " << str1.capacity() << L" Size: " << str1.size() << std::endl;
}
