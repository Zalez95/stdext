#ifndef STDEXT_RELEASE_VECTOR_H
#define STDEXT_RELEASE_VECTOR_H

#include <vector>

namespace stdext {

	/**
	 * Class ReleaseVector, it works as an usual vector of elements of type
	 * @tparam T but it also stores the removed elements instead of erasing
	 * them for reusing them later and preventing the old indices pointing to
	 * the vector from being invalidated. All the memory allocation will be
	 * done using the allocator @tparam A.
	 *
	 * @note	it doesn't prevent from pointer invalidations due to the
	 *			increment of the vector size with new allocations
	 */
	template <typename T, typename A = std::allocator<T>>
	class ReleaseVector
	{
	private:
		static_assert(std::is_constructible<T, T>::value,
			"result type must be constructible from input type");
	public:		// Nested types
		template <typename U, typename B>
		friend class ReleaseVector;

		/** Class PVIterator, it's the class used to iterate through the
		 * elements of a ReleaseVector */
		template <bool isConst>
		class PVIterator
		{
		public:		// Nested types
			template <bool isConst2>
			friend class PVIterator;

			using VectorType = std::conditional_t<isConst,
				const ReleaseVector, ReleaseVector
			>;

			using size_type			= typename VectorType::size_type;
			using difference_type	= long;
			using value_type		= std::conditional_t<isConst, const T, T>;
			using pointer			= value_type*;
			using reference			= value_type&;
			using iterator_category	= std::bidirectional_iterator_tag;

		private:	// Attributes
			/** A pointer to the vector to iterate */
			VectorType* mVector;

			/** The current position in the vector */
			size_type mIndex;

		public:		// Functions
			/** Creates a new PVIterator located at the initial valid position
			 * of the given ReleaseVector (begin)
			 *
			 * @param	vector a pointer to the vector to iterate */
			PVIterator(VectorType* vector);

			/** Creates a new PVIterator
			 *
			 * @param	vector a pointer to the vector to iterate
			 * @param	index the inital index of the iterator */
			PVIterator(VectorType* vector, size_type index) :
				mVector(vector), mIndex(index) {};

			/** Implicit conversion operator between const PVIterator and non
			 * const PVIterator
			 *
			 * @return	the new PVIterator with a different template const
			 *			type */
			operator PVIterator<!isConst>() const;

			/** @return	the index of the Element that the iterator is pointing
			 *			to */
			size_type getIndex() const { return mIndex; };

			/** @return	the index of the Element that the iterator is pointing
			 *			to */
			PVIterator& setIndex(size_type index);

			/** @return	a reference to the current Element that the iterator is
			 *			pointing to */
			reference operator*() const { return (*mVector)[mIndex]; };

			/** @return	a pointer to the current Element that the iterator is
			 *			pointing to */
			pointer operator->() const { return &(*mVector)[mIndex]; };

			/** Compares the given PVIterators
			 *
			 * @param	it1 the first iterator to compare
			 * @param	it2 the second iterator to compare
			 * @return	true if both iterators are equal, false otherwise */
			friend bool operator==(const PVIterator& it1, const PVIterator& it2)
			{ return it1.mVector == it2.mVector && it1.mIndex == it2.mIndex; };

			/** Compares the given PVIterators
			 *
			 * @param	it1 the first iterator to compare
			 * @param	it2 the second iterator to compare
			 * @return	true if both iterators are different, false otherwise */
			friend bool operator!=(const PVIterator& it1, const PVIterator& it2)
			{ return !(it1 == it2); };

			/** Preincrement operator
			 *
			 * @return	a reference to the current iterator after its
			 *			incrementation */
			PVIterator& operator++();

			/** Postincrement operator
			 *
			 * @return	a copy of the current iterator with the previous value
			 *			to the incrementation */
			PVIterator operator++(int);

			/** Predecrement operator
			 *
			 * @return	a reference to the current iterator after its
			 *			decrementation */
			PVIterator& operator--();

			/** Postdecrement operator
			 *
			 * @return	a copy of the current iterator with the previous value
			 *			to the decrementation */
			PVIterator operator--(int);
		};

		using size_type			= std::size_t;
		using value_type		= T;
		using pointer			= T*;
		using reference			= T&;
		using iterator			= PVIterator<false>;
		using const_iterator	= PVIterator<true>;

	private:	// Attributes
		/** A pointer to the Elements of the ReleaseVector */
		T* mElements;

		/** The number of Elements reserved in the ReleaseVector */
		size_type mCapacity;

		/** The index of the past-the-end Element of the ReleaseVector.
		 * @note the previous element to the end could have been released */
		size_type mEndIndex;

		/** The indices to the released Elements of the ReleaseVector */
		std::vector<size_type> mReleasedIndices;

		/** The allocator used for creating objects of type T */
		A mAllocator;

	public:		// Functions
		/** Creates a new ReleaseVector */
		ReleaseVector() : mElements(nullptr), mCapacity(0), mEndIndex(0) {};
		ReleaseVector(const ReleaseVector& other);
		ReleaseVector(ReleaseVector&& other);
		ReleaseVector(
			const T* elements, std::size_t capacity, std::size_t endIndex,
			const std::size_t* releasedIndices, std::size_t numReleasedIndices
		);

		/** Class destructor */
		~ReleaseVector();

		/** Assignment operator */
		ReleaseVector& operator=(const ReleaseVector& other);
		ReleaseVector& operator=(ReleaseVector&& other);

		/** Returns the Element i of the ReleaseVector
		 *
		 * @param	i the index of the Element
		 * @return	a reference to the Element */
		T& operator[](size_type i) { return mElements[i]; };

		/** Returns the Element i of the ReleaseVector
		 *
		 * @param	i the index of the Element
		 * @return	a const reference to the Element */
		const T& operator[](size_type i) const { return mElements[i]; };

		/** Compares the given ReleaseVectors
		 *
		 * @param	cv1 the first ReleaseVector to compare
		 * @param	cv2 the second ReleaseVector to compare
		 * @return	true if both ReleaseVector are equal, false otherwise */
		template <typename U>
		friend bool operator==(
			const ReleaseVector<U>& cv1, const ReleaseVector<U>& cv2
		);

		/** Compares the given ReleaseVectors
		 *
		 * @param	cv1 the first ReleaseVector to compare
		 * @param	cv2 the second ReleaseVector to compare
		 * @return	true if both ReleaseVector are different, false
		 *			otherwise */
		template <typename U>
		friend bool operator!=(
			const ReleaseVector<U>& cv1, const ReleaseVector<U>& cv2
		);

		/** @return	the initial iterator of the ReleaseVector */
		iterator begin() { return iterator(this); };

		/** @return	the initial iterator of the ReleaseVector */
		const_iterator begin() const { return const_iterator(this); };

		/** @return	the past the end iterator of the ReleaseVector */
		iterator end()
		{ return iterator(this, mEndIndex); };

		/** @return	the past the end iterator of the ReleaseVector */
		const_iterator end() const
		{ return const_iterator(this, mEndIndex); };

		/** @return	the number of Elements that can be added to the
		 *			ReleaseVector without reallocating it */
		size_type capacity() const { return mCapacity; };

		/** @return	the number of Elements in the ReleaseVector */
		size_type size() const
		{ return mEndIndex - mReleasedIndices.size(); };

		/** @return	true if the ReleaseVector has no Elements inside, false
		 *			otherwise */
		bool empty() const { return (size() == 0); };

		/** Changes the ReleaseVector capacity so it can be added up to the
		 * given elements without reallocating
		 *
		 * @param	n the new minimum capacity of the ReleaseVector */
		void reserve(std::size_t n);

		/** Removes all the elements in the ReleaseVector */
		void clear();

		/** @return	a pointer to the internal buffer of the ReleaseVector */
		T* data() { return mElements; };

		/** @return	a pointer to the internal buffer of the ReleaseVector */
		const T* data() const { return mElements; };

		/** @return	a pointer to the indices that has been released */
		const size_type* releasedIndices() const
		{ return mReleasedIndices.data(); };

		/** @return	the number of indices that has been released */
		size_type numReleasedIndices() const
		{ return mReleasedIndices.size(); };

		/** Creates a new Element in the vector or reuses an already released
		 * one
		 *
		 * @param	args the arguments needed for calling the constructor of
		 *			the new Element
		 * @return	an iterator to the Element */
		template <typename... Args>
		iterator emplace(Args&&... args);

		/** Removes the element located at the given iterator from the
		 * ReleaseVector
		 *
		 * @param	it an iterator to the Element
		 * @return	an iterator to the next Element
		 * @note	the element will be released instead of deleted so the
		 *			indices to the other elements won't need to be updated */
		iterator erase(const_iterator it);

		/** Checks if the Element given located at the given index is valid and
		 * active
		 *
		 * @param	i the index of the Element
		 * @return	true if is valid and active false otherwise */
		bool isActive(size_type i) const;

		/** Replicates the size and released elements of the given
		 * ReleaseVector into the current one, so they will have the same
		 * active indices
		 *
		 * @param	other the ReleaseVector to replicate
		 * @param	value the default value with which the elements will be
		 *			created
		 * @note	the elements currently stored in the vector will be removed
		 *			and the new ones will be default initialized */
		template <typename U>
		void replicate(const ReleaseVector<U>& other, const T& value = T());
	};

}

#include "ReleaseVector.hpp"

#endif		// STDEXT_RELEASE_VECTOR_H
