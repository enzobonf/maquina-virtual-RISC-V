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

        printRegistradores();
        printMemoria();
        cout << "PC: " << pc << '\n';
    }

    cout << "\nInstrucoes terminadas\n";
}

void MaquinaVirtual::executeR(Instrucao instr){
    this->Branch = 0;
    this->MemRead = 0;
    this->MemWrite = 0;
    this->MemToReg = 0;
    this->RegWrite = 1;
    this->ALUOp = 2;
    this->ALUSrc = 0;

    this->printControl();

    switch(instr.funct3){
        case 0b000:
            if(instr.funct7 == 0){ // ADD
                reg[instr.rd] = reg[instr.rs1] + reg[instr.rs2];
                printf("add -> reg[%d] = reg[%d] + reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
            }
            else{ // SUB
                reg[instr.rd] = reg[instr.rs1] - reg[instr.rs2];
                printf("sub -> reg[%d] = reg[%d] - reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
            }
            break;

        case 0b111: // AND
            reg[instr.rd] = reg[instr.rs1] & reg[instr.rs2];
            printf("and -> reg[%d] = reg[%d] & reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
            break;

        case 0b110: // OR
            reg[instr.rd] = reg[instr.rs1] | reg[instr.rs2];
            printf("or -> reg[%d] = reg[%d] | reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
            break;
    }
}

void MaquinaVirtual::executeI(Instrucao instr){
    if(instr.funct3 == 0){ // ADDI
        this->Branch = 0;
        this->MemRead = 0;
        this->MemWrite = 0;
        this->MemToReg = 0;
        this->RegWrite = 1;
        this->ALUOp = 0;
        this->ALUSrc = 1;

        this->printControl();

        reg[instr.rd] = reg[instr.rs1] + instr.imm;
        printf("addi -> reg[%d] = reg[%d] + %d\n", instr.rd, instr.rs1, instr.imm);
    }
    else{ // LW
        this->Branch = 0;
        this->MemRead = 1;
        this->MemWrite = 0;
        this->MemToReg = 1;
        this->RegWrite = 1;
        this->ALUOp = 0;
        this->ALUSrc = 1;

        this->printControl();

        reg[instr.rd] = mem[instr.imm + reg[instr.rs1]];
        printf("lw -> reg[%d] = mem[%d]\n", instr.rd, instr.imm + reg[instr.rs1]);
    }
}

void MaquinaVirtual::executeS(Instrucao instr){
    if(instr.funct3 == 0b010){ // SW
        this->Branch = 0;
        this->MemRead = 0;
        this->MemWrite = 1;
        this->MemToReg = 0;
        this->RegWrite = 0;
        this->ALUOp = 0;
        this->ALUSrc = 1;

        this->printControl();

        mem[instr.imm + reg[instr.rs1]] = reg[instr.rs2];
        printf("sw -> mem[%d] = reg[%d]\n", instr.imm + reg[instr.rs1], instr.rs2);
    }
}

void MaquinaVirtual::executeB(Instrucao instr){

    this->Branch = 1;
    this->MemRead = 0;
    this->MemWrite = 0;
    this->MemToReg = 0;
    this->RegWrite = 0;
    this->ALUOp = 1;
    this->ALUSrc = 0;

    this->printControl();

    printf("imm: %d\n", instr.imm);
    if(instr.funct3 == 0){ // BEQ
        int equal = reg[instr.rs2] == reg[instr.rs1];
        printf("beq -> (reg[%d] == reg[%d]) = %d\n", instr.rs2, instr.rs1, equal);
        if(equal){
            printf("pc + %d\n", instr.imm);
            pc += instr.imm;
        }
        else{
            pc += 4;
        }
    }
    else{ // BNE
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

void MaquinaVirtual::printRegistradores(){
    cout << '\n'; int i;
    for(int i = 0; i < 32; i++){
        if(reg[i])
            printf("reg[%2d] = %2d\n", i, reg[i]);
    }
    cout << '\n';
}

void MaquinaVirtual::printMemoria(){; int i;
    for(int i = 0; i < 32; i++){
        if(mem[i])
            printf("mem[%2d] = %2d\n", i, mem[i]);
    }
    cout << '\n';
}

void MaquinaVirtual::printControl(){
    printf("\nBranch: %d\n", this->Branch);
    printf("MemRead: %d\n", this->MemRead);
    printf("MemWrite: %d\n", this->MemWrite);
    printf("MemToReg: %d\n", this->MemToReg);
    printf("RegWrite: %d\n", this->RegWrite);
    printf("ALUOp: %d\n", this->ALUOp);
    printf("ALUSrc: %d\n\n", this->ALUSrc);
}
