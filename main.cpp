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

    /* string operation = "00000000001100000000001010010011";
    vector<string> instrucoes = 
    {"00000000001100000000001010010011", "00000000101000000000001100010011", 
    "00000001010000000000010100010011", "00000000010101010010000000100011", 
    "00000000011001010010001000100011", "00000000000001010010010110000011", 
    "00000000010001010010011000000011", "00000000110001011000011100110011", 
    "01000000101101100000011110110011", "01000000110001011000100000110011", 
    "00000000101101100111100010110011", "00000000110001011110100100110011","00000001001010001000100001100011"}; */

    Arquivo *arq = new Arquivo("teste1.txt");
    auto instrucoes = arq->lerInstrucoes();

    cout << instrucoes.size() << '\n';

    MaquinaVirtual *maquina = new MaquinaVirtual(instrucoes); 
    maquina->run();


}