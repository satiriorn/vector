#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <Arduino.h>
using namespace std;

template <typename T, typename A>
struct Foundation
{
	using size_type = size_t;
	A a;
	T* elem;
	size_type sz;
	size_type space;

	Foundation(A a, size_type n): a{ a }, elem{ a.allocate(n) }, sz{ n }, space{ n }{}

	~Foundation() {a.deallocate(elem, space);}
};

template<typename T, typename A = allocator<T>>
class vector : private Foundation<T, A>
{
public:
	~vector();

	using size_type = typename Foundation<T, A>::size_type;

	explicit vector(size_type n, T d = T(), A a = A());

	vector(initializer_list<T> lst, const A& a = A());

	vector(const A& a = A()) : Foundation<T, A>(a, 0) {}

	vector(const vector& arg);
	vector& operator= (const vector& v);

	vector(vector&& a);
	vector& operator= (vector&& a);

	T& at(size_type n);
	const T& at(size_type n) const;

	T& operator[] (size_type n);
	const T& operator[] (size_type n) const;

	size_type size() const { return this->sz; }

	size_type capacity() const { return this->space; }

	T& begin();

	int end();

	void reserve(size_type newalloc);

	void resize(size_type& newsize, T val = T());

	void push_back(const T& d);
};

template <typename T, typename A>
vector<T, A>::~vector()
{
	for (size_type i = 0; i < this->sz; ++i)
		this->a.destroy(this->elem + i);
}

template <typename T, typename A>
vector<T, A>::vector(size_type n, T d, A a)
	:Foundation<T, A>(a, n)
{
	for (size_type i = 0; i < n; ++i)
		this->a.construct(&this->elem[i], d);
}

template <typename T, typename A>
vector<T, A>::vector(initializer_list<T> lst, const A& a)
	:Foundation<T, A>(a, lst.size())
{
	for (size_type i = 0; i < lst.size(); ++i)
		this->a.construct(&this->elem[i], *(lst.begin() + i));
}

template <typename T, typename A>
vector<T, A>::vector(const vector& arg)
	:Foundation<T, A>(arg.a, arg.sz)
{
	for (size_type i = 0; i < arg.sz; ++i)
		this->a.construct(&this->elem[i], arg[i]);
}

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator=(const vector<T, A>& arg)
{
	if (this == &arg)
		return *this;
	Foundation<T, A> b(this->a, arg.sz);
	uninitialized_copy(&arg[0], &arg[arg.size()], b.elem);
	swap(this->elem, b.elem);
	swap(this->sz, b.sz);
	swap(this->space, b.space);
	for (size_type i = 0; i < b.sz; ++i)
		b.a.destroy(b.elem + i);//free memory used for vector before
	return *this;
}

template <typename T, typename A>
vector<T, A>::vector(vector&& arg):Foundation<T, A>(arg.a, arg.sz)
{
	arg.sz = 0;
	arg.elem = nullptr;
	arg.space = 0;
	std::cout << endl << "yes" << endl << endl;
}

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator=(vector<T, A>&& arg)
{
	swap(this->elem, arg.elem);
	swap(this->sz, arg.sz);
	swap(this->space, arg.space);
	delete[] arg.elem;
	std::cout << endl << "yes" << endl << endl;
	return *this;
}

template <typename T, typename A>
void vector<T, A>::reserve(size_type newalloc)
{
	if (newalloc <= this->space) return;
	Foundation<T, A> b(this->a, newalloc);
	uninitialized_copy(&this->elem[0], &this->elem[this->sz], b.elem);
	for (size_type i = 0; i < this->sz; ++i)
	{
		this->a.destroy(&this->elem[i]);
	}
	b.sz = this->sz;
	swap(this->elem, b.elem);
	swap(this->sz, b.sz);
	swap(this->space, b.space);
}

template <typename T, typename A>
void vector<T, A>::push_back(const T& val)
{
	if (!(this->space))
		reserve(8);
	else if (this->sz == this->space)
		reserve(2 * this->space);
	this->a.construct(&this->elem[this->sz], val);
	++this->sz;
}

template <typename T, typename A>
void vector<T, A>::resize(size_type& newsize, T val)
{
	reserve(newsize);
	for (size_type i = this->sz; i < newsize; ++i) this->a.construct(&this->elem[i], val);
	for (size_type i = newsize; i < this->sz; ++i) this->a.destroy(&this->elem[i]);
	this->sz = newsize;
}

template <typename T, typename A>
T& vector<T, A>::at(size_type n)
{
	if (n < 0 || this->sz <= n) throw out_of_range("Out of range");
	return this->elem[n];
}

template <typename T, typename A>
const T& vector<T, A>::at(size_type n) const
{
	if (n < 0 || this->sz <= n) throw out_of_range("Out of range");
	return this->elem[n];
}

template <typename T, typename A>
T& vector<T, A>::operator[] (size_type n)
{
	return this->elem[n];
}

template <typename T, typename A>
const T& vector<T, A>::operator[] (size_type n) const
{
	return this->elem[n];
}

template <typename T, typename A>
T& vector<T, A>::begin()
{
	return this->elem[0];
}

template <typename T, typename A>
int vector<T, A>::end()
{
	return sizeof(this->elem);
}
#endif