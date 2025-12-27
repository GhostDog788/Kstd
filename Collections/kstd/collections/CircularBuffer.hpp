#pragma once
#include "StaticBuffer.hpp"
#include <kstd/memory/UniquePointer.hpp>
#include <kstd/synchronization/Lock.hpp>

namespace kstd {

	template<typename T, unsigned int SIZE, typename ReadLock, typename WriteLock>
	class CircularBuffer : public StaticBuffer<T, SIZE>
	{
	public:
		explicit CircularBuffer(bool override_oldest = false) : StaticBuffer<T, SIZE>(), m_override_oldest(override_oldest) {
			m_read_lock = kstd::unique_ptr<kstd::Lock>(new ReadLock());
			m_write_lock = kstd::unique_ptr<kstd::Lock>(new WriteLock());
		}
		virtual ~CircularBuffer() = default;

		virtual bool push(const T& item) {
			// returns if the write succeeded
			kstd::LockGuard guard(m_write_lock.get());
			if ((m_write + 1) % SIZE == m_read) {
				if (!m_override_oldest) {
					if (!m_first_to_touch) return false;
					m_first_to_touch = false;
					this->m_buffer[m_write] = item;
					return true;
				}
				m_read = (m_read + 1) % SIZE;
			}
			m_first_to_touch = true;
			this->m_buffer[m_write] = item;
			m_write = (m_write + 1) % SIZE;
			return true;
		}

		virtual bool head(T& head) const {
			// returns if the read succeeded
			kstd::LockGuard guard(m_read_lock.get());
			if (m_read == m_write) return false;
			head = this->m_buffer[m_read];
			return true;
		}
		virtual bool pop(T& head) {
			// returns if the read succeeded
			kstd::LockGuard guard(m_read_lock.get());
			if (m_read == m_write) return false;
			head = this->m_buffer[m_read];
			m_read = (m_read + 1) % SIZE;
			return true;
		}

	private:
		size_t m_read = 0;
		size_t m_write = 0;
		bool m_override_oldest = false;
		bool m_first_to_touch = true;
		kstd::unique_ptr<kstd::Lock> m_read_lock;
		kstd::unique_ptr<kstd::Lock> m_write_lock;
	};
}
