#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <bitset>
#include <cstdio>

using namespace std;

struct Estado {
    std::vector<int> reg; // registradores
    std::vector<int> mem; // memória
    int pc;               // contador de programa
};

enum class Opcode {
    ADD = 51,
    SUB = 51,
    AND = 51,
    OR = 51,
    ADDI = 19,
    LW = 3,
    SW = 35,
    BEQ = 99,
    BNE = 99,
    //UNKNOWN
};

int32_t signExtend(int32_t imm, uint8_t bits) {
    //uint32_t mask = 0xFFFFFFFF >> (32 - bits);
    uint32_t shift = 32 - bits;
    int32_t extended = (imm << shift) >> shift;
    return extended;
}

enum class TipoInstrucao { I, S, R, B };

class Instrucao {
public:
    Opcode opcode;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t rd;
    uint32_t funct3;
    uint32_t funct7;
    uint32_t imm;

    TipoInstrucao tipo;

    void parse(string str_instr) {
        uint32_t instr = std::bitset<32>(str_instr).to_ulong();
        //cout << "instr binário: " << bitset<32>(instr) << endl; // imprimir o valor binário de funct3
        // Extrai os campos da instrução
        opcode = static_cast<Opcode>(instr & 0x7f);
        switch(opcode){
            case Opcode::ADD:
                tipo = TipoInstrucao::R;
                break;

            case Opcode::ADDI:
            case Opcode::LW:
                tipo = TipoInstrucao::I;
                break;

            case Opcode::BNE:
                tipo = TipoInstrucao::B;
                break;
            
            case Opcode::SW:
                tipo = TipoInstrucao::S;
                break;

            default:
                tipo = TipoInstrucao::I;
        }

        switch (tipo) {
            case TipoInstrucao::I:
                rd = (instr >> 7) & 0b11111;
                funct3 = (instr >> 12) & 0b111;
                rs1 = (instr >> 15) & 0b11111;
                imm = signExtend(instr >> 20, 12);
                break;
            case TipoInstrucao::S:
                funct3 = (instr >> 12) & 0b111;
                rs1 = (instr >> 15) & 0b11111;
                rs2 = (instr >> 20) & 0b11111;
                imm = signExtend((instr >> 7) & 0b11111, 5) |
                       (signExtend(instr >> 25, 7) << 5);
                break;
            case TipoInstrucao::R:
                rd = (instr >> 7) & 0b11111;
                funct3 = (instr >> 12) & 0b111;
                //cout << "funct3 binário: " << bitset<3>(funct3) << endl; // imprimir o valor binário de funct3
                rs1 = (instr >> 15) & 0b11111;
                rs2 = (instr >> 20) & 0b11111;
                funct7 = instr >> 25;
                //cout << "funct7: " << funct3 << '\n';
                break;
            case TipoInstrucao::B:
                funct3 = (instr >> 12) & 0b111;
                rs1 = (instr >> 15) & 0b11111;
                rs2 = (instr >> 20) & 0b11111;
                imm = signExtend((instr >> 7) & 0b11111, 5) |
                       (signExtend((instr >> 25) & 0b111111, 6) << 5) |
                       (signExtend(instr >> 31, 1) << 11);
                break;
        }
    }
};

class MaquinaVirtual {
public:
    /* std::vector<int> reg; // registradores
    std::vector<int> mem; // memória */

    int reg[32];
    int mem[256];
    uint32_t pc;
    vector<string> instrucoes;

    MaquinaVirtual(vector<string> instrucoes){
        for(int i = 0; i < 20; i++) reg[i] = 0;
        for(int i = 0; i < 256; i++) mem[i] = 0;

        this->instrucoes = instrucoes;
        pc = 0;
    }

    void run(){
        while((pc / 4) < instrucoes.size()){
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
        }

        printRegistradores();
    }

    void printRegistradores(){
        for(int i = 0; i < 20; i++){
            printf("reg[%d] = %d\n", i, reg[i]);
        }
        printf("pc %u\n", pc);
    }
private:
    void executeR(Instrucao instr){
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

        //printf("funct3 = %d | rd = %d | rs1 = %d | rs2 = %d\n", instr.funct3, instr.rd, instr.rs1, instr.rs2);
    }

    void executeI(Instrucao instr){
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

    void executeS(Instrucao instr){
        if(instr.funct3 == 0b010){
            mem[instr.imm + reg[instr.rs1]] = reg[instr.rd];
            printf("sw -> mem[%d] = reg[%d]\n", instr.imm + reg[instr.rs1], instr.rs2);
        }
    }

    void executeB(Instrucao instr){
        printf("imm: %d | rs2 = %u | rs1 = %u | funct3 = %u\n", instr.imm, instr.rs2, instr.rs1, instr.funct3);
        if(instr.funct3 == 0){
            if(reg[instr.rs2] == reg[instr.rs1]){
                pc += (int)(instr.imm / 4);
                //printf("beq -> pc + %d\n", (instr.imm / ));
            }
            else{
                pc += 4;
            }
        }
    }
};

int main(){

    string operation = "00000000001100000000001010010011";
    vector<string> instrucoes = 
    {/* "00000000001100000000001010010011", "00000000101000000000001100010011", 
    "00000001010000000000010100010011", "00000000010101010010000000100011", 
    "00000000011001010010001000100011", "00000000000001010010010110000011", 
    "00000000010001010010011000000011", "00000000110001011000011100110011", 
    "01000000101101100000011110110011", "01000000110001011000100000110011", 
    "00000000101101100111100010110011", "00000000110001011110100100110011", */"11111111000010000000101011100011"};
    
    MaquinaVirtual *maquina = new MaquinaVirtual(instrucoes); 
    maquina->run();


}