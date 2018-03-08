#ifndef ELEMENTO_HPP
#define ELEMENTO_HPP

#include <iterator>
#include <cstddef>

template<typename T>
class Elemento : std::iterator<std::input_iterator_tag,
    T, std::nullptr_t, T*, T&> {
private:
	T *info;
	Elemento<T>* _next{nullptr};

public:
	Elemento(const T& info, Elemento<T>* next) : 
		info(new T(info)), _next(next) 
	{}

	Elemento(const T& info) : 
		info(new T(info)) 
	{}

	~Elemento()
	{
		delete info;
	}

	Elemento<T>* getProximo() const
	{
		return _next;
	}

	T getInfo() const
	{
		return *info;
	}

	void setProximo(Elemento<T>* next)
	{
		_next = next;
	}

    Elemento<T> operator++() { return *_next;}
    T& operator*() { return *info; }
    bool operator!=(Elemento<T> e) { info == e.info; }
    


};

#endif
