#include "maquinavirtual.h"

using namespace std;

MaquinaVirtual::MaquinaVirtual(vector<string> instrucoes){
    for(int i = 0; i < 32; i++) reg[i] = 0;
    for(int i = 0; i < 32; i++) mem[i] = 0;

    this->instrucoes = instrucoes;
    pc = 0;
}

void MaquinaVirtual::run(){
    while((pc / 4) < instrucoes.size() && pc >= 0){
        cout << "Pressione enter para rodar a proxima instrucao" << '\n';
        getchar();

        Instrucao instr;
        instr.parse(instrucoes[pc / 4]);

        switch (instr.tipo) {
            case TipoInstrucao::R:
                this->executeR(instr);
                pc += 4;
                break;
            case TipoInstrucao::I:
                this->executeI(instr);
                pc += 4;
                break;
            case TipoInstrucao::S:
                this->executeS(instr);
                pc += 4;
                break;
            case TipoInstrucao::B:
                this->executeB(instr);
                break;
        }

        cout << "pc: " << pc << '\n';
    }

    printRegistradoresEMemoria();
}

void MaquinaVirtual::executeR(Instrucao instr){
    switch(instr.funct3){
        case 0b000:
            if(instr.funct7 == 0){
                reg[instr.rd] = reg[instr.rs1] + reg[instr.rs2];
                printf("add -> reg[%d] = reg[%d] + reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
            }
            else{
                reg[instr.rd] = reg[instr.rs1] - reg[instr.rs2];
                printf("sub -> reg[%d] = reg[%d] - reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
            }
            break;

        case 0b111:
            reg[instr.rd] = reg[instr.rs1] & reg[instr.rs2];
            printf("and -> reg[%d] = reg[%d] & reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
            break;

        case 0b110:
            reg[instr.rd] = reg[instr.rs1] | reg[instr.rs2];
            printf("or -> reg[%d] = reg[%d] | reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
            break;
    }
}

void MaquinaVirtual::executeI(Instrucao instr){
    //printf("imm: %d | rs1 = %d | funct3 = %d | rd = %d\n", instr.imm, instr.rs1, instr.funct3, instr.rd);
    if(instr.funct3 == 0){
        reg[instr.rd] = reg[instr.rs1] + instr.imm;
        printf("addi -> reg[%d] = reg[%d] + %d\n", instr.rd, instr.rs1, instr.imm);
    }
    else{
        reg[instr.rd] = mem[instr.imm + reg[instr.rs1]];
        printf("lw -> reg[%d] = mem[%d]\n", instr.rd, instr.imm + reg[instr.rs1]);
    }
}

void MaquinaVirtual::executeS(Instrucao instr){
    if(instr.funct3 == 0b010){
        mem[instr.imm + reg[instr.rs1]] = reg[instr.rs2];
        printf("sw -> mem[%d] = reg[%d]\n", instr.imm + reg[instr.rs1], instr.rs2);
    }
}

void MaquinaVirtual::executeB(Instrucao instr){
    printf("imm: %d | rs2 = %u | rs1 = %u | funct3 = %u\n", instr.imm, instr.rs2, instr.rs1, instr.funct3);
    if(instr.funct3 == 0){
        int notequal = reg[instr.rs2] == reg[instr.rs1];
        printf("beq -> (reg[%d] == reg[%d]) = %d\n", instr.rs2, instr.rs1, notequal);
        if(notequal){
            printf("pc + %d\n", instr.imm);
            pc += instr.imm;
        }
        else{
            pc += 4;
        }
    }
    else{
        int not_equal = reg[instr.rs2] != reg[instr.rs1];
        printf("bne -> (reg[%d] != reg[%d]) = %d\n", instr.rs2, instr.rs1, not_equal);
        if(not_equal){
            pc += instr.imm;
        }
        else{
            pc += 4;
        }
    }
}

void MaquinaVirtual::printRegistradoresEMemoria(){
    cout << '\n';
    for(int i = 0; i < 32; i++){
        printf("reg[%2d] = %2d || mem[%d] = %d\n\n", i, reg[i], i, mem[i]);
    }
}
