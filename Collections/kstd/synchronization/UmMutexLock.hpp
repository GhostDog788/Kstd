#pragma once
#ifndef _KERNEL_MODE
#include <windows.h>
#include <kstd/synchronization/Lock.hpp>

namespace kstd {

	class MutexLock : public Lock
	{
	public:
		MutexLock() = default;
		virtual void lock() {
			WaitForSingleObject(m_lock, INFINITE);
		}
		virtual void unlock() {
			if (m_lock) {
				ReleaseMutex(m_lock);
			}
		}
	private:
		HANDLE m_lock;
	};
}
#endif