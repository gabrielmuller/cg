#ifndef LISTAENC_HPP
#define LISTAENC_HPP

#include <cstdint>
#include <stdexcept>
#include "Elemento.hpp"

template<typename T>
class ListaEnc {

public:
	ListaEnc();
	~ListaEnc();
	// inicio
	void adicionaNoInicio(const T& dado);
	T retiraDoInicio();
	void eliminaDoInicio();
	// posicao
	void adicionaNaPosicao(const T& dado, int pos);
	int posicao(const T& dado) const;
	T* posicaoMem(const T& dado) const;
	bool contem(const T& dado);
	T retiraDaPosicao(int pos);
	//fim
	void adiciona(const T& dado);
	T retira();
	// especifico
	T retiraEspecifico(const T& dado);
	void adicionaEmOrdem(const T& data);
	bool listaVazia() const;
	bool igual(T dado1, T dado2);
	bool maior(T dado1, T dado2);
	bool menor(T dado1, T dado2);
	void destroiLista();

    Elemento<T>* begin() {
        return head;
    }

    Elemento<T>* end() {
        return nullptr;
    }

private:
	Elemento<T>* head;
	int size;

};

//#include "ListaEnc.tpp"

template<typename T>
ListaEnc<T>::ListaEnc() {
    size = 0;
    head = nullptr;
}

template<typename T>
ListaEnc<T>::~ListaEnc() {
    destroiLista();
}

template<typename T>
void ListaEnc<T>::adicionaNoInicio(const T& dado) {
    Elemento<T>* added = new Elemento<T>(dado, head);
    if (added == nullptr)
        throw std::out_of_range("Full");
    head = added;
    ++size;

}

template<typename T>
T ListaEnc<T>::retiraDoInicio() {
	if (listaVazia())
        throw std::out_of_range("List is empty");

    Elemento<T> *out = head;
    T back = out->getInfo();
    head = out->getProximo();
    --size;
    delete out;
    return back;
}

template<typename T>
void ListaEnc<T>::eliminaDoInicio() {
	if (listaVazia())
        throw std::out_of_range("List is empty");

    Elemento<T> *out = head;
    T back = out->getInfo();
    head = out->getProximo();
    --size;
    delete out;
}

template<typename T>
void ListaEnc<T>::adicionaNaPosicao(const T& dado, int pos) {
	if (pos > size)
        throw std::out_of_range("Index out of range");
    if (pos == 0) {
        adicionaNoInicio(dado);
    } else {
        Elemento<T> *added = new Elemento<T>(dado, added);
        if (added == nullptr)
            throw std::out_of_range("List is full");

        Elemento<T> *previous = head;
        for (std::size_t i = 0; i < (pos-1); i++)
            previous = previous->getProximo();

        added->setProximo(previous->getProximo());
        previous->setProximo(added);
        ++size;
    }
}

template<typename T>
int ListaEnc<T>::posicao(const T& dado) const {
	if (listaVazia())
        throw std::out_of_range("List is empty");

    Elemento<T> *current = head;
    for (std::size_t i = 0; i < size; ++i) {
        if (current->getInfo() == dado) return i;
        current = current->getProximo();
    }
    return size;
}

template<typename T>
T* ListaEnc<T>::posicaoMem(const T& dado) const {
	if (listaVazia())
        throw std::out_of_range("List is empty");

    Elemento<T> *current = head;
    for (std::size_t i = 0; i < size; ++i) {
        if (current->getInfo() == dado) return current;
        current = current->getProximo();
    }
    return nullptr;
}

template<typename T>
bool ListaEnc<T>::contem(const T& dado) {
	return posicao(dado) != size;
}

template<typename T>
T ListaEnc<T>::retiraDaPosicao(int pos) {
	if (listaVazia())
        throw std::out_of_range("List is empty");
    if (pos >= size)
        throw std::out_of_range("Index out of range");
    if (pos == 0)
        return retiraDoInicio();

    Elemento<T> *previous, *out;
    previous = head;
    for (std::size_t i = 0; i < (pos-1); ++i)
        previous = previous->getProximo();
    out = previous->getProximo();
    T back = out->getInfo();
    previous->setProximo(out->getProximo());
    --size;
    delete out;
    return back;
}

template<typename T>
void ListaEnc<T>::adiciona(const T& dado) {
	adicionaNaPosicao(dado, size);
}

template<typename T>
T ListaEnc<T>::retira() {
	retiraDaPosicao(size-1);
}

template<typename T>
T ListaEnc<T>::retiraEspecifico(const T& dado) {
	if (listaVazia())
        throw std::out_of_range("List is empty");
    retira(posicao(dado));
}

template<typename T>
void ListaEnc<T>::adicionaEmOrdem(const T& data) {
	if (listaVazia()) {
        adicionaNoInicio(data);
    } else {
        Elemento<T> *current = head;

        std::size_t index = 0;
        while ((current->getProximo() != nullptr) && (data > current->getInfo())) {
            current = current->getProximo();
            ++index;
        }

        if (data > current->getInfo())
            adicionaNaPosicao(data, index+1);
        else
            adicionaNaPosicao(data, index);
    }
}

template<typename T>
bool ListaEnc<T>::listaVazia() const {
	return size == 0;
}

template<typename T>
bool ListaEnc<T>::igual(T dado1, T dado2) {
	return dado1 == dado2;
}

template<typename T>
bool ListaEnc<T>::maior(T dado1, T dado2) {
	return dado1 > dado2;
}

template<typename T>
bool ListaEnc<T>::menor(T dado1, T dado2) {
	return dado1 < dado2;
}

template<typename T>
void ListaEnc<T>::destroiLista() {
	while (!listaVazia())
        retiraDoInicio();
    size = 0;
    head = nullptr;
}

#endif /* LISTAENC_HPP */
