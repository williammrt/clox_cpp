#include "debug.h"
#include "value.h"
#include <iomanip>
#include <iostream>
/*
  main() calls disassembleChunk() to treat all code
  disassembleChunk() calls disassembleInstruction() to treat with individual
  opcode disassembleInstruction() calls *Instruction() to deal with specific
  opcode
  */

void disassembleChunk(const Chunk &chunk, const std::string &name) {
  std::cout << "== " << name << " ==\n";
  for (int offset = 0; offset < chunk.get_code_size();) {
    offset = disassembleInstruction(chunk, offset);
  }
}

static int constantInstruction(const std::string &name, const Chunk &chunk,
                               int offset) {
  uint8_t constant_pos = chunk.get_code(offset + 1);
  std::cout << std::left << std::setfill(' ') << std::setw(16) << name << " "
            << std::right << std::setw(4) << static_cast<int>(constant_pos)
            << " '";
  // static cast here because cout << uint8_t will print char instead of int
  printValue(chunk.get_constant(constant_pos));
  std::cout << "'\n";
  return offset + 2;
}

static int simpleInstruction(const std::string &name, int offset) {
  std::cout << name << "\n";
  return offset + 1;
}

int disassembleInstruction(const Chunk &chunk, int offset) {
  std::cout << std::right << std::setfill('0') << std::setw(4) << offset << " ";
  // about setting precision:
  // https://stackoverflow.com/questions/530614/print-leading-zeros-with-c-output-operator

  if (offset > 0 && (chunk.get_line(offset) == chunk.get_line(offset - 1))) {
    std::cout << "   | ";
  } else {
    std::cout << std::right << std::setfill(' ') << std::setw(4)
              << chunk.get_line(offset) << " ";
  }
  uint8_t instruction = chunk.get_code(offset);
  switch (instruction) {
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", chunk, offset);
  case OP_ADD:
    return simpleInstruction("OP_ADD", offset);
  case OP_SUBTRACT:
    return simpleInstruction("OP_SUBTRACT", offset);
  case OP_MULTIPLY:
    return simpleInstruction("OP_MULTIPLY", offset);
  case OP_DIVIDE:
    return simpleInstruction("OP_DIVIDE", offset);
  case OP_NEGATE:
    return simpleInstruction("OP_NEGATE", offset);
  case Opcode::OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  default:
    std::cout << "Unknown opcode " << instruction
              << "\n"; // in book it print out opcode in byte
    return offset + 1;
  }
}