#include "hash_extensivel.h"

int main() {
    Hash_Extensivel hash;

    ifstream entrada("in.txt");
    ofstream saida("out.txt");

    if (!entrada.is_open()) {
        cerr << "Erro ao abrir o ficheiro in.txt" << endl;
        return 1;
    }
    if (!saida.is_open()) {
        cerr << "Erro ao criar o ficheiro out.txt" << endl;
        return 0;
    }

    string linha;
    //pegar a primeira linha e inicializar o hash
    //PG/<profundidade global inicial>
    getline(entrada, linha);
    saida << linha << "\n";
    int pg = stoi(linha.substr(3));
    hash.inicializar_diretorio(pg);

    while(getline(entrada, linha)){
        if(linha[0] == 'I'){
            int chave = stoi(linha.substr(4));
            hash.inserir(chave, saida);
        }
        else if(linha[0] == 'R'){
            int chave = stoi(linha.substr(4));
            hash.inserir(chave, saida);
        }
        else{
            int chave = stoi(linha.substr(5));
            hash.inserir(chave, saida);
        }
    }
    //P:/<profundidade global final>
    saida << "P:/" << hash.get_PG() << "\n"; 

    entrada.close();
    saida.close();

    cout << "Operacoes Realizadas!" << endl;
    return 0;
}