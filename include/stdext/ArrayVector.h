#ifndef STDEXT_ARRAY_VECTOR_H
#define STDEXT_ARRAY_VECTOR_H

#include <cstddef>
#include <iterator>
#include <initializer_list>

namespace stdext {

	/**
	 * Class ArrayVector, a ArrayVector is a container of elements of type
	 * @tparam T with a compile-time maximum size of @tparam N elements but
	 * which can vary the number of elements stored inside. Internally the
	 * ArrayVector stores the elements contiguously so they could be accessed
	 * with pointers and offsets. The ArrayVector won't do any allocation at
	 * runtime.
	 */
	template <typename T, std::size_t N>
	class ArrayVector
	{
	public:		// Nested types
		using size_type					= std::size_t;
		using difference_type			= std::ptrdiff_t;
		using value_type				= T;
		using pointer					= T*;
		using reference					= T&;
		using iterator					= T*;
		using const_iterator			= const T*;
		using reverse_iterator			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

	private:	// Attributes
		/** The array of memory where the elements will be stored */
		std::byte mData[N * sizeof(T)];

		/** The number of Elements added to the ArrayVector */
		size_type mNumElements;

	public:		// Functions
		/** Creates a new ArrayVector */
		ArrayVector() : mNumElements(0) {};
		ArrayVector(const ArrayVector& other);
		ArrayVector(ArrayVector&& other);

		/** Creates a new ArrayVector
		 *
		 * @param	numElements the initial number of elements stored inside
		 *			the ArrayVector. If it's larger than the maximum number of
		 *			elements the ArrayVector can store, it will be set to 0
		 * @param	value the initial value with which the new values will be
		 *			intialized */
		ArrayVector(size_type numElements, const T& value = T()) :
			mNumElements(0) { resize(numElements, value); };

		/** Creates a new ArrayVector
		 *
		 * @param	list the initial elements stored inside the ArrayVector
		 * @note	it will be stored up to N elements */
		ArrayVector(std::initializer_list<T> list);

		/** Creates a new ArrayVector
		 *
		 * @param	first the first iterator of the range to copy
		 * @param	last the past-the-end iterator of the range to copy */
		template <typename InputIterator>
		ArrayVector(InputIterator first, InputIterator last);

		/** Class destructor */
		~ArrayVector();

		/** Assignment operator */
		ArrayVector& operator=(const ArrayVector& other);
		ArrayVector& operator=(ArrayVector&& other);

		/** Returns the Element i of the ArrayVector
		 *
		 * @param	i the index of the Element
		 * @return	a reference to the Element */
		T& operator[](size_type i)
		{ return reinterpret_cast<T*>(mData)[i]; };

		/** Returns the Element i of the ArrayVector
		 *
		 * @param	i the index of the Element
		 * @return	a const reference to the Element */
		const T& operator[](size_type i) const
		{ return reinterpret_cast<const T*>(mData)[i]; };

		/** Compares the given ArrayVectors
		 *
		 * @param	lhs the first ArrayVector to compare
		 * @param	rhs the second ArrayVector to compare
		 * @return	true if both ArrayVector are equal, false otherwise */
		template <typename U, std::size_t N2>
		friend bool operator==(
			const ArrayVector<U, N2>& lhs, const ArrayVector<U, N2>& rhs
		);

		/** Compares the given ArrayVectors
		 *
		 * @param	lhs the first ArrayVector to compare
		 * @param	rhs the second ArrayVector to compare
		 * @return	true if both ArrayVector are different, false otherwise */
		template <typename U, std::size_t N2>
		friend bool operator!=(
			const ArrayVector<U, N2>& lhs, const ArrayVector<U, N2>& rhs
		);

		/** @return	the initial iterator of the ArrayVector */
		iterator begin()
		{ return reinterpret_cast<T*>(mData); };

		/** @return	the initial iterator of the ArrayVector */
		const_iterator begin() const
		{ return reinterpret_cast<const T*>(mData); };

		/** @return	the final iterator of the ArrayVector */
		iterator end()
		{ return reinterpret_cast<T*>(mData) + mNumElements; };

		/** @return	the final iterator of the ArrayVector */
		const_iterator end() const
		{ return reinterpret_cast<const T*>(mData) + mNumElements; };

		/** @return	the initial reverse iterator of the ArrayVector */
		reverse_iterator rbegin()
		{ return reinterpret_cast<T*>(mData) + mNumElements; };

		/** @return	the final reverse iterator of the ArrayVector */
		const_reverse_iterator rbegin() const
		{ return reinterpret_cast<const T*>(mData) + mNumElements; };

		/** @return	the final reverse iterator of the ArrayVector */
		reverse_iterator rend()
		{ return reinterpret_cast<T*>(mData); };

		/** @return	the initial reverse iterator of the ArrayVector */
		const_reverse_iterator rend() const
		{ return reinterpret_cast<const T*>(mData); };

		/** @return	the number of Elements in the ArrayVector */
		size_type size() const { return mNumElements; };

		/** @return	the maximum number of Elements that can be stored in the
		 *			ArrayVector */
		size_type capacity() const { return N; };

		/** @return	true if the ArrayVector has no Elements inside, false
		 *			otherwise */
		bool empty() const { return (mNumElements == 0); };

		/** @return	a reference to the first element in the ArrayVector
		 * @note	if the ArrayVector is empty, it causes undefined behavior */
		T& front()
		{ return reinterpret_cast<T*>(mData)[0]; };

		/** @return	a reference to the first element in the ArrayVector
		 * @note	if the ArrayVector is empty, it causes undefined behavior */
		const T& front() const
		{ return reinterpret_cast<const T*>(mData)[0]; };

		/** @return	a reference to the last element in the ArrayVector
		 * @note	if the ArrayVector is empty, it causes undefined behavior */
		T& back()
		{ return reinterpret_cast<T*>(mData)[mNumElements - 1]; };

		/** @return	a reference to the last element in the ArrayVector
		 * @note	if the ArrayVector is empty, it causes undefined behavior */
		const T& back() const
		{ return reinterpret_cast<const T*>(mData)[mNumElements - 1]; };

		/** @return	a pointer to the first element stored in the ArrayVector */
		T* data()
		{ return reinterpret_cast<T*>(mData); };

		/** @return	a pointer to the first element stored in the ArrayVector */
		const T* data() const
		{ return reinterpret_cast<const T*>(mData); };

		/** Removes all the elements in the ArrayVector */
		void clear();

		/** Changes the number of elements stored in the ArrayVector
		 *
		 * @param	numElements the new number of elements stored inside the
		 *			ArrayVector
		 * @param	value the initial value with which the new values will be
		 *			intialized
		 * @note	the new number of elements will be set to @tparam N if
		 *			numElements is larger */
		void resize(size_type numElements, const T& value = T());

		/** Adds a new Element at the back of the ArrayVector
		 *
		 * @param	element the Element to push at the back of the ArrayVector
		 * @note	if the ArrayVector is full, it causes undefined behavior */
		void push_back(const T& element);

		/** Adds a new Element at the back of the ArrayVector
		 *
		 * @param	args the arguments needed for calling the constructor of
		 *			the new Element
		 * @return	a reference to the new element
		 * @note	if the ArrayVector is full, it causes undefined behavior */
		template <typename... Args>
		T& emplace_back(Args&&... args);

		/** Removes the last Element of the ArrayVector
		 * @note	if the ArrayVector is empty, it causes undefined behavior */
		void pop_back();

		/** Inserts the given element into the ArrayVector at the position
		 * located at the given iterator
		 *
		 * @param	it an iterator to the position to insert the element
		 * @param	value the element to insert
		 * @note	the next stored elements will be displaced one position
		 *			right inside the ArrayVector */
		iterator insert(const_iterator it, const T& value);

		/** Inserts the given element into the ArrayVector at the position
		 * located at the given iterator
		 *
		 * @param	it an iterator to the position to insert the element
		 * @param	args the arguments needed for calling the constructor of
		 *			the new Element
		 * @note	the next stored elements will be displaced one position
		 *			right inside the ArrayVector */
		template <typename... Args>
		iterator emplace(const_iterator it, Args&&... args);

		/** Removes the element located at the given iterator from the
		 * ArrayVector
		 *
		 * @param	it an iterator to the Element
		 * @note	the next stored elements will be displaced one position
		 *			left inside the ArrayVector */
		iterator erase(const_iterator it);
	};

}

#include "ArrayVector.hpp"

#endif		// STDEXT_ARRAY_VECTOR_H
