#ifndef VECTOR_H
#define VECTOR_H
#include <Arduino.h>

template<typename T>
class vector
{
	private:
		T* elem;
		int scale;
	public:
		explicit vector(int s);
		~vector(){delete[] elem;}
		T& operator[](int i);
		const T& operator[](int i) const;
		int size()const{return scale}
}


#endif