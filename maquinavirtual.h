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
    int reg[32];
    int mem[32];
    int pc;

    int Branch;
    int MemRead;
    int MemWrite;
    int MemToReg;
    int RegWrite;
    int ALUOp;
    int ALUSrc;

    vector<string> instrucoes;
    
    void executeR(Instrucao instr);
    void executeI(Instrucao instr);
    void executeS(Instrucao instr);
    void executeB(Instrucao instr);

    void printRegistradores();
    void printMemoria();
    void printControl();
};

#endif
