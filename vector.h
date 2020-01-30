#pragma once
#ifndef VECTOR_H
#define VECTOR_H
#include <initializer_list>
#include <iterator>
#include <memory>

template <typename T, typename A = std::allocator<T>>
class vector
{
public: 
	explicit vector(int s);//default
	vector(std::initializer_list<T> lst);
	
	vector(vector&& arg);	//moving constructor
	vector operator=(vector&& arg);

	~vector() { delete[] elem; }

	T& operator[ ](int i) { return elem[i]; }

	int capacity() const { return space; }
	int size() const { return sz; }

	T* begin(vector<T> &x);
	T* end(vector<T>& x);

	void reserve(int newalloc); 
	void resize(int newsize, const T& val = T())
	{
		reserve(newsize);

		for (int i = sz; i < newsize; ++i)
			alloc.construct(&elem[i], val);
		for (int i = sz; i < newsize; ++i)
			alloc.destroy(&elem[i]);
		sz = newsize;
	}

	void push_back(const T& x);	

private:
	vector(const vector& arg);//copy constructors
	vector& operator=(const vector& arg);

	A alloc;
	int sz;	
	T* elem;	//pointer on elements
	int space;  //amount of elements + amount of "free memory"
	
};