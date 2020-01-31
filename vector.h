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
	vector() :sz{0}, space{0}, elem{nullptr} {}

	explicit vector(const int s) :sz{ s }, space{ s }, elem{ new T[s] } {for (int i = 0; i < s; ++i)elem[i] = 0;}

	vector(std::initializer_list<T> lst);
	
	vector(vector&& arg);	//moving constructor
	vector operator=(vector&& arg);

	~vector() { delete[] elem; }

	T& operator[](int i) { return elem[i]; }

	vector& operator=(const vector& arg); //copy assignment

	int capacity() const { return space; }
	int size() const { return sz; }

	T* begin(vector<T> &x);
	T* end(vector<T>& x);

	void reserve(int newalloc); 
	
	void push_back(const T& x);	

private:
	vector(const vector& arg);//copy constructors
	void resize(int newsize, const T& val = T());

	A alloc;
	T* elem;	//pointer on elements
	int sz;	
	int space;  //amount of elements + amount of "free memory"
	
};

template <typename T, typename A>
vector<T, A>::vector(std::initializer_list<T> lst):sz{ static_cast<int>(lst.size()) }, elem{ new T[lst.size()] }, space{ sz }
{
	std::copy(lst.begin(), lst.end(), elem);
}
template <typename T, typename A>
vector<T, A>::vector(const vector& arg) : sz{ arg.sz }, elem{ new T[arg.sz] }, space{ sz }
{
	std::copy(arg.elem, arg.elem + arg.sz, elem);
}

template <typename T, typename A>
vector<T, A>::vector(vector&& arg) :
	sz{ arg.sz }, elem{ arg.elem }, space{ sz }
{
	arg.sz = 0;
	arg.elem = nullptr;
}

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator=(const vector& arg){
	if (this == &arg)
		return *this;
	if (arg.sz <= space)
	{
		std::copy(arg.elem, arg.elem + arg.sz, elem);
		sz = arg.sz;
		return *this;
	}

	T* p = new T[arg.size()];
	std::copy(arg.elem, arg.elem + arg.sz, p);
	delete[] elem;
	elem = p;
	space = sz = arg.sz;
	return *this;
}

template <typename T, typename A>
vector<T, A> vector<T, A>::operator=(vector&& arg){
	delete[] elem;
	elem = arg.elem;
	arg.elem = nullptr;
	sz = arg.sz;
	arg.sz = 0;
	return *this;
}

template <typename T, typename A>
void vector<T, A>::reserve(int newalloc){
	if (newalloc <= space) return;
	T* p = alloc.allocate(newalloc);
	for (int i = 0; i < sz; ++i)
		alloc.construct(&p[i], elem[i]);
	for (int i = 0; i < sz; ++i)
		alloc.detroy(&elem[i]);
	alloc.deallocate(elem, space);
	elem = p;
	space = newalloc;
}

template <typename T, typename A>
void vector<T, A>::resize(int newsize, const T& val){
	reserve(newsize);
	for (int i = sz; i < newsize; ++i)
		alloc.construct(&elem[i], val);
	for (int i = sz; i < newsize; ++i)
		alloc.destroy(&elem[i]);
	sz = newsize;
}
