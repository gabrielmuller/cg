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

    Elemento<T> begin() {
        return *head;
    }

    Elemento<T> end() {
        return *head;
    }

private:
	Elemento<T>* head;
	int size;

};

#include "ListaEnc.tpp"

#endif /* LISTAENC_HPP */
