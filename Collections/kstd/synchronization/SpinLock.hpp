#pragma once
#ifdef _KERNEL_MODE
#include <ntddk.h>
#include <kstd/synchronization/Lock.hpp>

namespace kstd {

	class SpinLock : public Lock
	{
	public:
		SpinLock() {
			KeInitializeSpinLock(&m_lock);
		}
		virtual void lock() {
			if (KeGetCurrentIrql() >= DISPATCH_LEVEL) {
				dpcLock();
			}
			else {
				passiveLock(m_old_irql);
			}
		}
		virtual void unlock() {
			if (m_dpc_level) {
				dpcUnlock();
			}
			else {
				passiveUnlock(m_old_irql);
			}
		}
	private:
		KSPIN_LOCK m_lock;
		bool m_dpc_level;
		KIRQL m_old_irql;

		void dpcLock() {
			KeAcquireSpinLockAtDpcLevel(&m_lock);
			m_dpc_level = true;
		}
		void dpcUnlock() {
			KeReleaseSpinLockFromDpcLevel(&m_lock);
		}
		void passiveLock(KIRQL& old_irql) {
			KeAcquireSpinLock(&m_lock, &old_irql);
			m_dpc_level = false;
		}
		void passiveUnlock(KIRQL& old_irql) {
			KeReleaseSpinLock(&m_lock, old_irql);
		}
	};
}
#endif