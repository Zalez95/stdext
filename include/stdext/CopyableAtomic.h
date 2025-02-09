#ifndef STDEXT_COPYABLE_ATOMIC_H
#define STDEXT_COPYABLE_ATOMIC_H

#include <atomic>

namespace stdext {

	/**
	 * Class CopyableAtomic, it's a @see std::atomic that allows to copy
	 * its values of type @tparam T. This allows us to use the atomic class
	 * inside standard containers.
	 * @note the copy operation isn't atomic, so other synchronization
	 * mechanisms should be used when this operation is performed.
	 */
	template <typename T>
	class CopyableAtomic : public std::atomic<T>
	{
	public:		// Functions
		/** Class constructor */
		CopyableAtomic() = default;
		CopyableAtomic(const T& value) : std::atomic<T>(value) {};
		CopyableAtomic(const std::atomic<T>& value) :
			std::atomic<T>(value.load()) {};
		CopyableAtomic(const CopyableAtomic& other) :
			std::atomic<T>(other.load()) {};

		/** Class destructor */
		~CopyableAtomic() = default;

		/** Assignment operator */
		CopyableAtomic& operator=(const T& value);
		CopyableAtomic& operator=(const std::atomic<T>& value);
		CopyableAtomic& operator=(const CopyableAtomic& other);
	};


	template <typename T>
	CopyableAtomic<T>& CopyableAtomic<T>::operator=(const T& value)
	{
		this->store(value);
		return *this;
	}


	template <typename T>
	CopyableAtomic<T>& CopyableAtomic<T>::operator=(const std::atomic<T>& value)
	{
		this->store(value.load());
		return *this;
	}


	template <typename T>
	CopyableAtomic<T>& CopyableAtomic<T>::operator=(const CopyableAtomic& other)
	{
		this->store(other.load());
		return *this;
	}

}

#endif		// STDEXT_COPYABLE_ATOMIC_H
