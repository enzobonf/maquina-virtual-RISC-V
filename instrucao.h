#ifndef INSTRUCAO_H
#define INSTRUCAO_H

#include <string>

enum class TipoInstrucao { I, S, R, B };
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
};

class Instrucao {
public:
    Opcode opcode;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t rd;
    uint32_t funct3;
    uint32_t funct7;
    int32_t imm;

    TipoInstrucao tipo;

    Instrucao() = default;
    void parse(std::string str_instr);
};

int32_t signExtend(int32_t imm, uint8_t bits);

#endif // INSTRUCAO_H