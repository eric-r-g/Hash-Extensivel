#include "hash_extensivel.h"

Hash_Extensivel::Hash_Extensivel(){
    dir.prof_global = 0;
}

int Hash_Extensivel::get_PG(){
    return dir.prof_global;
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
        else cerr << "Erro ao criar arquivo: " << nome << endl;
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

    //imprimir linha do texto original
    if(qtde > 0){
        ifstream ArquivoTexto("bd-trab2 - dataset.csv");
        if(ArquivoTexto.is_open()){
            string LinhaTexto;
            int LinhaAtual = 0;
            while(getline(ArquivoTexto, LinhaTexto)){
                if(LinhaAtual == chave){
                    cout << "Chave: " << chave << ", Conteudo: " << LinhaTexto << endl;
                    break;
                }
                LinhaAtual++;
            }
            ArquivoTexto.close();
        }
        else cerr << "Erro ao abrir arquivo bd-trab2 - dataset.csv" << endl;
    }

    //escrever no out.txt (BUS:x/<quantidade de tuplas selecionadas>)
    arquivo_saida << "BUS:" << chave << "/" << qtde << "\n";
}


void Hash_Extensivel::inserir(int chave, ofstream& arquivo_saida){
    bool inserted = false;
    while(!inserted){
        int idx = calcular_hash(chave);

        string nome = dir.ponteiros[idx];
        ifstream arquivo(nome);

        if(arquivo.is_open()){
            //instancia o bucket e le os dados do txt
            bucket b; 
            arquivo >> b.prof_local;
            arquivo >> b.qtd_chaves;
            for(int i = 0; i < 5; i++) {
                arquivo >> b.chaves[i];
            }

            if(b.qtd_chaves < 5) { // existe espaço naquele bucket
                b.chaves[b.qtd_chaves] = chave;
                b.qtd_chaves++;
                inserted = true;

                ofstream arquivo(nome);

                if(arquivo.is_open()){
                    arquivo << b.prof_local << "\n";
                    arquivo << b.qtd_chaves << "\n";
                    for(int i = 0 ; i < 5 ; i++)
                        arquivo << -1 << " ";

                    arquivo.close();
                }
                else cerr << "Erro ao abrir arquivo: " << nome << endl;
            } else { 
                // aumenta a profundidade local
                if(b.prof_local == dir.prof_global){ 
                    // aumenta a profundidade global
                    dir.prof_global++;
                    int new_size = (1 << dir.prof_global);
                    
                    dir.ponteiros.resize(new_size);
                    for(int idx_pont = new_size / 2; idx_pont < new_size; idx_pont++){
                        dir.ponteiros[idx_pont] = dir.ponteiros[idx_pont - new_size / 2];
                    }
                }

                b.prof_local++;
                vector <int> idx_originais, idx_transferidos;
                for(int i = 0; i < b.qtd_chaves; i++){
                    if(b.chaves[i] & (1 << (b.prof_local - 1))) {
                        idx_transferidos.push_back(b.chaves[i]);
                    } else {
                        idx_originais.push_back(b.chaves[i]);
                    }
                }

                ofstream arquivo(nome);

                if(arquivo.is_open()){
                    arquivo << b.prof_local << "\n";
                    arquivo << idx_originais.size() << "\n";
                    for(int i = 0 ; i < 5 ; i++){
                        if(i < idx_originais.size()) arquivo << idx_originais[i] << " ";
                        else arquivo << -1 << " ";
                    }

                    arquivo.close();
                }
                else cerr << "Erro ao abrir arquivo: " << nome << endl;

                string new_nome = "bucket_" + to_string(idx + (1 << (b.prof_local - 1))) + ".txt";

                arquivo = ofstream(new_nome);

                if(arquivo.is_open()){
                    arquivo << b.prof_local << "\n";
                    arquivo << idx_transferidos.size() << "\n";
                    for(int i = 0 ; i < 5 ; i++){
                        if(i < idx_transferidos.size()) arquivo << idx_transferidos[i] << " ";
                        else arquivo << -1 << " ";
                    }

                    arquivo.close();
                }
                else cerr << "Erro ao abrir arquivo: " << nome << endl;
            }

            arquivo.close();
        } else {
            // cria o bucket necessario e faz as devidas mudanças
            ofstream arquivo(nome);

            if(arquivo.is_open()){
                arquivo << dir.prof_global << "\n";
                arquivo << 1 << "\n";
                arquivo << chave << " ";
                for(int i = 1 ; i < 5 ; i++){
                    arquivo << -1 << " ";
                }

                arquivo.close();
            }
            else cerr << "Erro ao abrir arquivo: " << nome << endl;
        }
    }
    arquivo_saida << "INC:" << chave << "/<" << ">\n";
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
    arquivo_saida << "REM:" << chave << "/" << removidas << "," << dir.prof_global << "," << b.prof_local << "\n";
}

