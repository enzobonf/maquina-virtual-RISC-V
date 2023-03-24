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
    uint32_t mask = 0xFFFFFFFF >> (32 - bits);
    if (imm & (1 << (bits - 1))) {
        // O valor imediato é negativo, precisamos preencher com 1's.
        return imm | ~mask;
    } else {
        // O valor imediato é positivo, preenchemos com 0's.
        return imm & mask;
    }
}

enum class TipoInstrucao { I, S, R, B };

enum class InstrucaoR {
    ADD = 0b000,
    SUB = 0b010,
    AND = 0b111,
    OR = 0b110,
};

class Instrucao {
public:
    Opcode opcode;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t rd;
    uint32_t imm;
    uint32_t funct3;
    uint32_t funct7;

    TipoInstrucao tipo;

    void parse(uint32_t instr) {

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
                tipo = TipoInstrucao::S;
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
                cout << funct3 << endl;
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
    int mem[1024];
    int pc;

    MaquinaVirtual(){
        for(int i = 0; i < 20; i++) reg[i] = 0;
        for(int i = 0; i < 256; i++) mem[i] = 0;
    }

    void run(string str_instr){
        uint32_t num = std::bitset<32>(str_instr).to_ulong();
        Instrucao instr;
        instr.parse(num);

        switch (instr.tipo) {
            case TipoInstrucao::R:
                this->executeR(instr);
                break;
            case TipoInstrucao::I:
                this->executeI(instr);
                break;
            default:
                this->executeR(instr);
        }
    }

    void printRegistradores(){
        for(int i = 0; i < 20; i++){
            printf("reg[%d] = %d\n", i, reg[i]);
        }
    }
private:
    void executeR(Instrucao instr){
        switch(instr.funct3){
            case 0b000:
                if(instr.funct7 == 0){
                    reg[instr.rd] = reg[instr.rs1] + reg[instr.rs2];
                    printf("reg[%d] = reg[%d] + reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
                }
                else{
                    reg[instr.rd] = reg[instr.rs1] - reg[instr.rs2];
                    printf("reg[%d] = reg[%d] - reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
                }
                break;

            case 0b111:
                reg[instr.rd] = reg[instr.rs1] & reg[instr.rs2];
                printf("reg[%d] = reg[%d] & reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
                break;

            case 0b110:
                reg[instr.rd] = reg[instr.rs1] | reg[instr.rs2];
                printf("reg[%d] = reg[%d] | reg[%d]\n", instr.rd, instr.rs1, instr.rs2);
                break;
        }

        pc += 4;
        printf("funct3 = %d | rd = %d | rs1 = %d | rs2 = %d\n", instr.funct3, instr.rd, instr.rs1, instr.rs2);
    }

    void executeI(Instrucao instr){
        if(instr.funct3 == 0){
            reg[instr.rd] = reg[instr.rs1] + instr.imm;
            printf("reg[%d] = reg[%d] + %d\n", instr.rd, instr.rs1, instr.imm);
        }

        pc += 4;
        printRegistradores();
    }
    
};

int main(){

    string operation = "00000000001100000000001010010011";
    
    MaquinaVirtual maquina;
    maquina.run(operation);


}