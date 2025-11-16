#pragma once

namespace kstd {

	void* allocate_bytes(size_t size) {
		return new char[size];
	}

	void free_bytes(void* ptr) {
		delete[] static_cast<char*>(ptr);
	}

	template <typename T>
	T* allocate_array(size_t count) {
		return new T[count];
	}

	template <typename T>
	void free_array(T* ptr) {
		delete[] ptr;
	}
}
