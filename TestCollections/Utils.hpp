#include <string>
#include <kstd/collections/CircularBuffer.hpp>

template<typename T>
std::string serialize_circular_buffer(kstd::CircularBuffer<T>& buffer)
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
