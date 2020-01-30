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