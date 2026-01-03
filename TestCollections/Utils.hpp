#include <string>
#include <kstd/collections/CircularBuffer.hpp>
#include <kstd/synchronization/UmMutexLock.hpp>

template<typename T, unsigned int SIZE>
std::string serialize_circular_buffer(kstd::CircularBuffer<T, SIZE, kstd::MutexLock, kstd::MutexLock>& buffer)
{
	std::string result;
	result += '[';
	T item;
	for (size_t i = 0; i < buffer.size(); ++i)
	{
		if (!buffer.pop(item)) break;
		if (i != 0) result += " ";
		result += std::to_string(item);
	}
	result += ']';
	return result;
}
