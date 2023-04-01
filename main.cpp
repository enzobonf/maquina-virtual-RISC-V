#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <bitset>
#include <cstdio>
#include "arquivo.h"
#include "maquinavirtual.h"

using namespace std;

int main(){

    string nomeArq;

    cout << "\nTrabalho de Organizacao e Arquitetura de Computadores\n" << "Enzo Bonfante\n\n";
    cout << "Digite o nome do arquivo das instrucoes: ";
    cin >> nomeArq;
    cin.ignore(); // limpa o buffer do cin

    Arquivo *arq = new Arquivo(nomeArq);
    auto instrucoes = arq->lerInstrucoes();

    MaquinaVirtual *maquina = new MaquinaVirtual(instrucoes); 
    maquina->run();

    cout << "Pressione enter para sair do programa..." << '\n';
    getchar();

}