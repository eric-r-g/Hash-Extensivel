#ifndef HASH_EXTENSIVEL_H
#define HASH_EXTENSIVEL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

struct bucket{
    int prof_local;
    int chaves[5];
    int qtd_chaves;
};

struct diretorio{
    int prof_global;
    vector<string> ponteiros; //guardar os nomes dos arquivos dos buckets
};

class Hash_Extensivel{
    public:
        Hash_Extensivel();

        int calcular_hash(int chave); //retornar os PG bits menos significativos
        void inicializar_diretorio(int pg_inicial); //recebe a PG e cria os buckets (.txt)
        int get_PG();

        void buscar(int chave, ofstream& arquivo_saida);
        void inserir(int chave, ofstream& arquivo_saida);
        void remover(int chave, ofstream& arquivo_saida);
    
    private:
        diretorio dir;
};

#endif