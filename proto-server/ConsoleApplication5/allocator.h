#pragma once
#include "exception.h"
typedef long sizeT;
template <typename T>
#define MEMORY_STEP 2
class Allocator
{
	T* ptr;
	sizeT allocatedSize;
	sizeT elementsInBuffer;
public:
	Allocator();
	Allocator(sizeT allocatedSize);
	Allocator(const Allocator& arr);
	~Allocator();
	const T& AtIndex(sizeT i) const { return ptr[i]; }
	T& AtIndex(sizeT i) { return ptr[i]; }
	const T* GetPointer() const;
	T* GetPointer();
	void PushBack(const T& data);
	void Resize();
	sizeT Count() { return elementsInBuffer; }
	sizeT Allocated() { return allocatedSize; }
	Allocator& operator = (const Allocator&);
};

template <typename T>
Allocator<T>::Allocator()
{
	allocatedSize = 0;
	ptr = NULL;
	elementsInBuffer = 0;
}

template <typename T>
Allocator<T>::Allocator(const Allocator<T>& arr)
{
	ptr = NULL;
	*this = arr;
}

template <typename T>
Allocator<T>::Allocator(sizeT sizeToAlloc)
{
	allocatedSize = sizeToAlloc;
	if (allocatedSize == 0)
	{
		ptr = NULL;
	}
	else
	{
		ptr = new T[allocatedSize];

		if (!ptr)
			ThrowException("cant allocate");
	}
	elementsInBuffer = 0;
}

template <typename T>
Allocator<T>::~Allocator()
{
	delete[] ptr;
}

template <typename T>
void Allocator<T>::PushBack(const T& data)
{
	if (elementsInBuffer == allocatedSize)
		Resize();
	ptr[elementsInBuffer++] = data;
}

template <typename T>
void Allocator<T>::Resize()
{
	if (allocatedSize == 0)
	{
		allocatedSize = 1;
		ptr = new T[allocatedSize];
		if (!ptr) ThrowException("err");
		return;
	}
	sizeT newSize = allocatedSize * MEMORY_STEP;
	T *oldPtr = ptr;
	T *newPtr = new T[newSize];
	if (!newPtr) ThrowException("Cant reallcate");
	for (int i = 0; i < allocatedSize; i++)
		newPtr[i] = oldPtr[i];
	delete[] oldPtr;
	ptr = newPtr;
	allocatedSize *= MEMORY_STEP;
}

template <typename T>
T *Allocator<T>::GetPointer()
{
	return ptr;
}

template <typename T>
const T* Allocator<T>::GetPointer() const
{
	return ptr;
}

template <typename T>
Allocator<T>& Allocator<T>::operator = (const Allocator<T>& arr)
{
	delete[] ptr;
	allocatedSize = arr.allocatedSize;
	elementsInBuffer = arr.elementsInBuffer;
	if (allocatedSize == 0)
	{
		ptr = NULL;
		return *this;
	}
	ptr = new T[allocatedSize];
	if (!ptr) ThrowException("Cant alloc");
	for (int i = 0; i < elementsInBuffer; i++)
		ptr[i] = arr.ptr[i];
	return *this;
}

typedef long sizeT;
template <typename T>
class Array
{
	Allocator<T> *allocator;
public:
	Array();
	Array(sizeT allocatedSize);
	Array(const Array& arr);
	~Array();
	void PushBack(const T& data);
	sizeT Count() const;
	sizeT Allocated() const;
	T* GetPointer();
	const T* GetPointer() const;
	const T& operator[] (sizeT i) const;
	T& operator[] (sizeT i);
	Array& operator = (const Array& arr);
	const T& AtIndex(sizeT i) const;
	T& AtIndex(sizeT i);
};


template <typename T>
Array<T>::Array()
{
	allocator = new Allocator<T>();
	if(!allocator) ThrowException("Cnat alloc");
}

template <typename T>
Array<T>::Array(const Array<T>& arr)
{
	*this = arr;
}

template <typename T>
Array<T>::Array(sizeT sizeToAlloc)
{
	allocator = new Allocator<T>(sizeToAlloc);
	if (!allocator) ThrowException("Error alloc");
}

template <typename T>
Array<T>::~Array()
{
	delete allocator;
}

template <typename T>
void Array<T>::PushBack(const T& data)
{
	allocator->PushBack(data);
}

//	ptr = newPtr;
//	allocatedSize *= MEMORY_STEP;
//}

template <typename T>
T *Array<T>::GetPointer()
{
	return allocator->GetPointer();
}


template <typename T>
const T* Array<T>::GetPointer() const
{
	return allocator->GetPointer();
}

template <typename T>
Array<T>& Array<T>::operator = (const Array<T>& arr)
{
	*allocator = *arr.allocator;
	return *this;
}

template <typename T>
const T& Array<T>::operator[] (sizeT i) const
{
	return allocator->AtIndex(i);
}

template <typename T>
const T& Array<T>::AtIndex(sizeT i) const
{
	return allocator->AtIndex(i);
}


template <typename T>
T& Array<T>::operator[] (sizeT i)
{
	return allocator->AtIndex(i);
}

template <typename T>
T& Array<T>::AtIndex(sizeT i)
{
	return allocator->AtIndex(i);
}

template <typename T>
sizeT Array<T>::Count() const
{
	return allocator->Count();
}


template <typename T>
sizeT Array<T>::Allocated() const
{
	return allocator->Allocated();
}

// TODO
//template <typename T>
//class String
//{
//	Allocator<char> allocator;
//
//};

