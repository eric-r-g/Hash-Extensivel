#include "hash_extensivel.h"

Hash_Extensivel::Hash_Extensivel(){
    dir.prof_global = 0;
}

void Hash_Extensivel::inicializar_diretorio(int PG){
    dir.prof_global = PG;
    int num_buckets = pow(2, PG);
    dir.ponteiros.resize(num_buckets);

    //criar os arquivos dos buckets
    for(int i = 0 ; i < num_buckets ; i++){
        string nome = "bucket_" + to_string(i+1) + ".txt";
        dir.ponteiros[i] = nome;

        ofstream arquivo(nome);

        /* A estrutura eh:
        1 linha: profundidade local (inicialmente igual a profundidade global)
        2 linha: quatidade de chaves (inicialmente 0)
        3 linha: 5 chaves separadas por espaco (inicialmente -1)
        */
        if(arquivo.is_open()){
            arquivo << PG << "\n";
            arquivo << 0 << "\n";
            for(int i = 0 ; i < 5 ; i++)
                arquivo << -1 << " ";

            arquivo.close();
        }
        else cerr << "Erro ao abrir arquivo: " << nome << endl;
    }
}

int Hash_Extensivel::calcular_hash(int chave){
    //deve retornar os PG bits menos significativos
    //equivalente a fazer o resto por 2^PG
    int div = pow(2, dir.prof_global);
    return chave % div;
}

void Hash_Extensivel::buscar(int chave, ofstream& arquivo_saida){
    //encontrar e abrir o arquivo do bucket
    int idx = calcular_hash(chave);
    string nome = dir.ponteiros[idx];

    ifstream arquivo(nome);

    int qtde = 0; //quantidade de tuplas selecionadas (teoricamente eh pra dar so 1 ou 0)
    if(arquivo.is_open()){
        //instancia o bucket e le os dados do txt
        bucket b; 
        arquivo >> b.prof_local;
        arquivo >> b.qtd_chaves;
        for(int i = 0; i < 5; i++) {
            arquivo >> b.chaves[i];
        }

        //procura a chave no array do bucket
        for(int i = 0 ; i < b.qtd_chaves ; i++)
            if(b.chaves[i] == chave) qtde++;

        arquivo.close();
    }
    else cerr << "Erro ao abrir arquivo" << nome << endl;

    //escrever no out.txt (BUS:x/<quantidade de tuplas selecionadas>)
    arquivo_saida << "BUS:" << chave << "/<" << qtde << ">\n";
}

void Hash_Extensivel::remover(int chave, ofstream& arquivo_saida){
    //encontrar e abrir o arquivo do bucket
    int idx = calcular_hash(chave);
    string nome = dir.ponteiros[idx];

    ifstream arquivo(nome);

    //carregar na memoria
    bucket b;
    if(arquivo.is_open()){
        arquivo >> b.prof_local >> b.qtd_chaves;
        for(int i = 0 ; i < 5 ; i++) arquivo >> b.chaves[i];
        arquivo.close();
    }
    else cerr << "Erro ao abrir arquivo" << nome << endl;

    //procurar e remover as tuplas com a chave
    int removidas = 0;
    for(int i = 0 ; i < b.qtd_chaves ; i++){
        if(b.chaves[i] == chave){
            //mudar a posicao das chaves no array (1 pra tras)
            for(int j = i ; j < b.qtd_chaves - 1 ; j++)
                b.chaves[j] = b.chaves[j+1];
            b.chaves[b.qtd_chaves - 1] = -1; //resetar a ultima

            b.qtd_chaves--;
            removidas++;
        }
    }

    //salvar o bucket no disco (sobrescrever o outro arquivo)
    ofstream arquivo_novo(nome);
    if(arquivo_novo.is_open()){
            arquivo_novo << b.prof_local << "\n";
            arquivo_novo << b.qtd_chaves << "\n";
            for(int i = 0 ; i < 5 ; i++)
                arquivo_novo << b.chaves[i] << " ";

            arquivo_novo.close();
        }
    else cerr << "Erro ao tentar sobrescrever: " << nome << endl;

    //escrever no out.txt REM:x/<qtd de tuplas removidas>,<profundidade global>,<profundidade local>
    arquivo_saida << "REM:" << chave << "/<" << removidas << ">,<" << dir.prof_global << ">,<" << b.prof_local << ">\n";
}