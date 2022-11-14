#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"
#include <iostream>
#include <vector>

enum Opcode {
  OP_CONSTANT,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_RETURN,
  OP_NEGATE,
};

/*
  Chunk is a class for code, which will be run using VM
*/

class Chunk {
private:
  std::vector<uint8_t> code;
  std::vector<int> lines;
  std::vector<Value> constants;

public:
  void writeChunk(uint8_t byte, int line);
  /* add value to to constant pool and return its position in pool */
  int addConstant(Value value);

  uint8_t get_code(int offset) const;

  Value get_constant(int offset) const;

  int get_code_size() const;

  int get_line(int offset) const;
};

#endif
