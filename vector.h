#ifndef VECTOR_H
#define VECTOR_H 
#define ALLOCSIZE 100 //count buf
#include <Arduino.h>


template<typename T>
void* operator new(size_t s, T* v) {
	return v;
}

template<typename T> struct Alloc {

	explicit Alloc() {};

	T allocbuf[ALLOCSIZE];
	T *allocp = allocbuf;

	T* allocate(int n)//returns a pointer to n
	{
		if (allocbuf + ALLOCSIZE - allocp >= n) {
			allocp += n;
			return allocp - n;
		}
		else
			return 0;
	}
	void deallocate(T* p, int n)//frees memory at p
	{
		if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
			allocp = p;
	}

	//construction/destruction
	void construct(T* p, const T& t) { new(p) T(t); }
	void destroy(T* p) { p->~T(); }
};

template<class T, class A = Alloc<T> >
class vector {

	A alloc;

	int sz;
	T* elem;
	int space;

	vector(const vector&);

public:
	vector() : sz(0), elem(0), space(0) {}
	vector(const int s) : sz(0) {
		reserve(s);
	}

	vector& operator=(const vector&);	//copy assignment

	~vector() {
		for (int i = 0; i < sz; ++i) alloc.destroy(&elem[i]);
	}

	T& operator[](int n) { return elem[n]; }
	const T& operator[](int n) const { return elem[n]; }

	int size() const { return sz; }
	int capacity() const { return space; }
	
	T& begin();
	int end();

	T& at(int n);
	const T& at(int n) const;

	void reserve(int newalloc);
	void push_back(const T& val);

};

template<class T, class A>
vector<T, A>& vector<T, A>::operator=(const vector& a) {
	if (this == &a) return *this;

	if (a.size() <= space) {	//enough space, no need for new allocation
		for (int i = 0; i < a.size(); ++i) elem[i] = a[i];
		sz = a.size();
		return *this;
	}

	T* p = alloc.allocate(a.size());		//get new memory 
	for (int i = 0; i < a.size(); ++i) {
		alloc.construct(&p[i], a[i]);	//copy
	}
	for (int i = 0; i < sz; ++i) alloc.destroy(&elem[i]);
	space = sz = a.size();
	elem = p;
	return *this;
}

template<class T, class A> void vector<T, A>::reserve(int newalloc) {
	if (newalloc <= space) return;		                    //never decrease space
	T* p = alloc.allocate(newalloc);
	for (int i = 0; i < sz; ++i) alloc.construct(&p[i], elem[i]);	//copy
	for (int i = 0; i < sz; ++i) alloc.destroy(&elem[i]);
	alloc.deallocate(elem, space);
	elem = p;
	space = newalloc;
}

template<class T, class A>
void vector<T, A>::push_back(const T& val) {
	if (space == 0) reserve(4);				//start small
	else if (sz == space) reserve(2 * space);
	alloc.construct(&elem[sz], val);
	++sz;
}

template <typename T, typename A>
T& vector<T, A>::begin()
{
	return this->elem[0];
}

template <typename T, typename A>
int vector<T, A>::end()
{
	return sizeof(this->elem[sz]);
}

template <typename T, typename A>
T& vector<T, A>::at(int n)
{
	if (n < 0 || this->sz <= n) throw ("Out of range");
	return this->elem[sz];
}

template <typename T, typename A>
const T& vector<T, A>::at(int n) const
{
	if (n < 0 || this->sz <= n) throw ("Out of range");
	return this->elem[n];
}
#endif
