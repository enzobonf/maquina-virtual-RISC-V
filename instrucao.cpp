#include "instrucao.h"
#include <cstdio>
#include <bitset>

using namespace std;

void Instrucao::parse(string str_instr) {
    uint32_t instr = std::bitset<32>(str_instr).to_ulong();
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
            rs1 = (instr >> 15) & 0b11111;
            rs2 = (instr >> 20) & 0b11111;
            funct7 = instr >> 25;
            break;
        case TipoInstrucao::B:
            funct3 = (instr >> 12) & 0b111;
            rs1 = (instr >> 15) & 0b11111;
            rs2 = (instr >> 20) & 0b11111;
            
            imm = ((instr >> 7) & 0b11110 |
                  ((instr >> 20 & 0b11111100000)) |
                  ((instr << 4 & 0b100000000000)) |
                  ((instr >> 19 & 0b1000000000000)));
                
            imm += (instr >> 31)
                        ? 0b11111111111111111110000000000000
                        : 0;

            break;
    }
}

int32_t signExtend(int32_t imm, uint8_t bits) {
    uint32_t shift = 32 - bits;
    int32_t extended = (imm << shift) >> shift;
    return extended;
}