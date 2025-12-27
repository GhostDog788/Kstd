#pragma once
#ifdef _KERNEL_MODE
#include <ntddk.h>
#include <kstd/synchronization/BaseGuard.hpp>

namespace kstd {

	class SpinLock
	{
	public:
		SpinLock() {
			KeInitializeSpinLock(&m_lock);
		}
		void dpcLock() {
			KeAcquireSpinLockAtDpcLevel(&m_lock);
		}
		void dpcUnlock() {
			KeReleaseSpinLockFromDpcLevel(&m_lock);
		}
		void passiveLock(KIRQL& old_irql) {
			KeAcquireSpinLock(&m_lock, &old_irql);
		}
		void passiveUnlock(KIRQL& old_irql) {
			KeReleaseSpinLock(&m_lock, old_irql);
		}
	private:
		KSPIN_LOCK m_lock;
	};

	class SpinLockGuard : public BaseGuard<SpinLock>
	{
	public:
		SpinLockGuard(SpinLock& lock) : BaseGuard<SpinLock>(lock) {
			m_dpc_level = KeGetCurrentIrql() >= DISPATCH_LEVEL;
			_lock();
		};
		~SpinLockGuard() { _unlock(); }
	protected:
		bool m_dpc_level = false;
		KIRQL m_old_irql;

		virtual void _lock() override {
			if (m_dpc_level) {
				m_lock->dpcLock();
			}
			else {
				m_lock->passiveLock(m_old_irql);
			}
		}
		virtual void _unlock() override {
			if (m_lock) {
				if (m_dpc_level) {
					m_lock->dpcUnlock();
				}
				else {
					m_lock->passiveUnlock(m_old_irql);
				}
				m_lock = nullptr;
			}
		}
	};
}
#endif