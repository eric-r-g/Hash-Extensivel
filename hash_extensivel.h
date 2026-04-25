#ifndef HASH_EXTENSIVEL_H
#define HASH_EXTENSIVEL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct bucket{
    int prof_local;
    int chaves[5]; //array com as entradas
    int qtd_chaves;
};

struct diretorio{
    int prof_global;
    vector<string> ponteiros; //guardar os nomes dos arquivos dos buckets
};

class Hash_Extensivel{
    public:
        Hash_Extensivel();

        int calcular_hash(int chave);
        void inicializar_diretorio(int pg_inicial);

        void buscar(int chave, ofstream& arquivoSaida);
        void inserir(int chave, ofstream& arquivoSaida);
        void remover(int chave, ofstream& arquivoSaida);
    
    private:
        diretorio dir;
};

#endif