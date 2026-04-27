# Trabalho 2 de Banco de Dados - Índice Hash Extensível

**Dupla:** 
- Eric Rodrigues Gomes
- Paulo Eduardo Conrado Marinho

## Descrição do Projeto
Esse projeto implementa um indide Hash Extensivel em C++ que realiza operações de busca, inserção e remoção
de registro de um banco de dados. O projeto é Composto de uma classe principal, o hash_extensivel.cpp que
implementa as funcionalidades do indice Hash e pela main que atua como comunicação entre a entrada e a classe.

A estrutura utiliza o identificador único da linha (`LinhaNum`) como chave de busca e RID e os buckets são mantidos
como arquivos .txt na pasta buckets

## Estrutura de arquivos
Antes de executar o programa, garanta que que o banco de dados e o arquivo in.txt está na pasta principal, que 
o executavel está na página src e que exista uma pasta buckets. É de suma importancia que essa estrutura seja respeitada
para o bom funcionamento do código.

## Como compilar e executar
Para compilar o projeto, abra o terminal na pasta src e execute o comando 
> g++ main.cpp hash_extensivel.cpp -o hash

Após isso, execute o programa gerado:
No Linux/macOS:
> ./hash

No Windows:
> hash.exe

o programa vai ler o in.txt e gerá um arquivo out.txt com as respostas devidas, além de escrever no terminal qualquer aviso
ou mensagem adicional.