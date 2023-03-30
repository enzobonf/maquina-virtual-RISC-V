#include "instrucao.h"
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
            imm = signExtend((instr >> 7) & 0b11111, 5) |
                    (signExtend((instr >> 25) & 0b111111, 6) << 5) |
                    (signExtend(instr >> 31, 1) << 11);
            break;
    }
}

int32_t signExtend(int32_t imm, uint8_t bits) {
    //uint32_t mask = 0xFFFFFFFF >> (32 - bits);
    uint32_t shift = 32 - bits;
    int32_t extended = (imm << shift) >> shift;
    return extended;
}