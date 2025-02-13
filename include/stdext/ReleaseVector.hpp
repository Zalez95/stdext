#ifndef STDEXT_RELEASE_VECTOR_HPP
#define STDEXT_RELEASE_VECTOR_HPP

#include <algorithm>

namespace stdext {

	template <typename T, typename A>
	ReleaseVector<T, A>::ReleaseVector(const ReleaseVector& other) :
		mElements(nullptr), mCapacity(0),
		mEndIndex(other.mEndIndex), mReleasedIndices(other.mReleasedIndices)
	{
		reserve(other.mCapacity);
		for (auto it = other.begin(); it != other.end(); ++it) {
			new (&mElements[it.getIndex()]) T(*it);
		}
	}


	template <typename T, typename A>
	ReleaseVector<T, A>::ReleaseVector(ReleaseVector&& other) :
		mElements(other.mElements), mCapacity(other.mCapacity),
		mEndIndex(other.mEndIndex), mReleasedIndices(std::move(other.mReleasedIndices))
	{
		other.mElements = nullptr;
		other.mCapacity = 0;
		other.mEndIndex = 0;
	}


	template <typename T, typename A>
	ReleaseVector<T, A>::ReleaseVector(
		const T* elements, std::size_t capacity, std::size_t size,
		const std::size_t* releasedIndices, std::size_t numReleasedIndices
	) : mElements(nullptr), mCapacity(0), mEndIndex(0)
	{
		reserve(capacity);
		mEndIndex = size + numReleasedIndices;
		std::copy(elements, elements + mEndIndex, mElements);
		std::copy(releasedIndices, releasedIndices + numReleasedIndices, std::back_inserter(mReleasedIndices));
	}


	template <typename T, typename A>
	ReleaseVector<T, A>::~ReleaseVector()
	{
		clear();
		mAllocator.deallocate(mElements, mCapacity);
	}


	template <typename T, typename A>
	ReleaseVector<T, A>& ReleaseVector<T, A>::operator=(const ReleaseVector& other)
	{
		clear();

		mEndIndex = other.mEndIndex;
		mReleasedIndices = other.mReleasedIndices;
		reserve(other.mCapacity);
		for (auto it = other.begin(); it != other.end(); ++it) {
			new (&mElements[it.getIndex()]) T(*it);
		}

		return *this;
	}


	template <typename T, typename A>
	ReleaseVector<T, A>& ReleaseVector<T, A>::operator=(ReleaseVector&& other)
	{
		clear();

		mElements = other.mElements;
		mCapacity = other.mCapacity;
		mEndIndex = other.mEndIndex;
		mReleasedIndices = std::move(other.mReleasedIndices);

		other.mElements = nullptr;
		other.mCapacity = 0;
		other.mEndIndex = 0;

		return *this;
	}


	template <typename T, typename A>
	bool operator==(const ReleaseVector<T, A>& cv1, const ReleaseVector<T, A>& cv2)
	{
		return (cv1.mElements == cv2.mElements)
			&& (cv1.mCapacity == cv2.mCapacity)
			&& (cv1.mEndIndex == cv2.mEndIndex)
			&& (cv1.mReleasedIndices == cv2.mReleasedIndices);
	}


	template <typename T, typename A>
	bool operator!=(const ReleaseVector<T, A>& cv1, const ReleaseVector<T, A>& cv2)
	{
		return !(cv1 == cv2);
	}


	template <typename T, typename A>
	void ReleaseVector<T, A>::reserve(std::size_t n)
	{
		if (n > mCapacity) {
			T* buffer = mAllocator.allocate(n);
			if (mCapacity > 0) {
				for (auto it = begin(); it != end(); ++it) {
					new (&buffer[it.getIndex()]) T(std::move(*it));
					(*it).~T();
				}
				mAllocator.deallocate(mElements, mCapacity);
			}

			mElements = buffer;
			mCapacity = n;
			mReleasedIndices.reserve(n);
		}
	}


	template <typename T, typename A>
	void ReleaseVector<T, A>::clear()
	{
		for (auto it = begin(); it != end();) {
			it = erase(it);
		}
	}


	template <typename T, typename A>
	template <typename... Args>
	typename ReleaseVector<T, A>::iterator ReleaseVector<T, A>::emplace(Args&&... args)
	{
		size_type index;
		if (mReleasedIndices.empty()) {
			if (mCapacity == 0) {
				reserve(1);
			}
			else if (mEndIndex + 1 > mCapacity) {
				reserve(2 * mCapacity);
			}
			index = mEndIndex++;
		}
		else {
			index = mReleasedIndices.back();
			mReleasedIndices.pop_back();
		}

		new (&mElements[index]) T(std::forward<Args>(args)...);
		return iterator(this, index);
	}


	template <typename T, typename A>
	typename ReleaseVector<T, A>::iterator ReleaseVector<T, A>::erase(const_iterator it)
	{
		iterator ret = it;
		++ret;

		size_type index = it.getIndex();
		if (isActive(index)) {
			mElements[index].~T();
			mReleasedIndices.push_back(index);
		}

		return ret;
	}


	template <typename T, typename A>
	bool ReleaseVector<T, A>::isActive(size_type i) const
	{
		return (i < mEndIndex)
			&& (std::find(mReleasedIndices.begin(), mReleasedIndices.end(), i) == mReleasedIndices.end());
	}


	template <typename T, typename A>
	template <typename U>
	void ReleaseVector<T, A>::replicate(const ReleaseVector<U>& other, const T& value)
	{
		clear();

		reserve(other.mCapacity);
		mEndIndex = other.mEndIndex;
		mReleasedIndices = other.mReleasedIndices;
		for (auto it = begin(); it != end(); ++it) {
			new (&(*it)) T(value);
		}
	}


	template <typename T, typename A>
	template <bool isConst>
	ReleaseVector<T, A>::PVIterator<isConst>::PVIterator(VectorType* vector) :
		mVector(vector), mIndex(0)
	{
		if (!mVector->isActive(mIndex) && (mVector->mEndIndex > 0)) {
			operator++();
		}
	}


	template <typename T, typename A>
	template <bool isConst>
	ReleaseVector<T, A>::PVIterator<isConst>::operator
		ReleaseVector<T, A>::PVIterator<!isConst>() const
	{
		PVIterator<!isConst> ret(nullptr, mIndex);

		if constexpr (isConst) {
			ret.mVector = const_cast<ReleaseVector*>(mVector);
		}
		else {
			ret.mVector = mVector;
		}

		return ret;
	}


	template <typename T, typename A>
	template <bool isConst>
	typename ReleaseVector<T, A>::template PVIterator<isConst>&
		ReleaseVector<T, A>::PVIterator<isConst>::setIndex(size_type index)
	{
		mIndex = index;
		return *this;
	}


	template <typename T, typename A>
	template <bool isConst>
	typename ReleaseVector<T, A>::template PVIterator<isConst>&
		ReleaseVector<T, A>::PVIterator<isConst>::operator++()
	{
		do {
			++mIndex;
		}
		while (!mVector->isActive(mIndex) && (mIndex < mVector->mEndIndex));

		return *this;
	}


	template <typename T, typename A>
	template <bool isConst>
	typename ReleaseVector<T, A>::template PVIterator<isConst>
		ReleaseVector<T, A>::PVIterator<isConst>::operator++(int)
	{
		PVIterator ret(*this);
		operator++();
		return ret;
	}


	template <typename T, typename A>
	template <bool isConst>
	typename ReleaseVector<T, A>::template PVIterator<isConst>&
		ReleaseVector<T, A>::PVIterator<isConst>::operator--()
	{
		do {
			--mIndex;
		}
		while (!mVector->isActive(mIndex) && (mIndex < mVector->mEndIndex));

		return *this;
	}


	template <typename T, typename A>
	template <bool isConst>
	typename ReleaseVector<T, A>::template PVIterator<isConst>
		ReleaseVector<T, A>::PVIterator<isConst>::operator--(int)
	{
		PVIterator ret(*this);
		operator--();
		return ret;
	}

}

#endif		// STDEXT_RELEASE_VECTOR_HPP
