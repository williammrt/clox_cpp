#include "chunk.h"

void Chunk::writeChunk(uint8_t byte, int line) {
  code.push_back(byte);
  lines.push_back(line);
}

/* add value to to constant pool and return its position in pool */
int Chunk::addConstant(Value value) {
  constants.push_back(value);
  return constants.size() - 1;
}

uint8_t Chunk::get_code(int offset) const { return code[offset]; }

Value Chunk::get_constant(int offset) const { return constants[offset]; }

int Chunk::get_code_size() const { return code.size(); }

int Chunk::get_line(int offset) const { return lines[offset]; }