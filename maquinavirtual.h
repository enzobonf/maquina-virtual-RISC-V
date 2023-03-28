#ifndef MAQUINAVIRTUAL_H
#define MAQUINAVIRTUAL_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdio>
#include "instrucao.h"

using namespace std;

class MaquinaVirtual {
public:
    MaquinaVirtual(vector<string> instrucoes);
    void run();
private:
    void executeR(Instrucao instr);
    void executeI(Instrucao instr);
    void executeS(Instrucao instr);
    void executeB(Instrucao instr);

    void printRegistradoresEMemoria();

    int reg[32];
    int mem[32];
    int pc;
    vector<string> instrucoes;
};

#endif
