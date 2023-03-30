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

    Arquivo *arq = new Arquivo("teste3.txt");
    auto instrucoes = arq->lerInstrucoes();

    cout << instrucoes.size() << '\n';

    MaquinaVirtual *maquina = new MaquinaVirtual(instrucoes); 
    maquina->run();


}