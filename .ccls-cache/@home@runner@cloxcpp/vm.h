#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"
#include <string>

enum class InterpretResult {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
};

/*
  VM run code from class Chunk
  */
class VM {
private:
  bool DEBUG;
  Chunk chunk;
  int ip; // instruction pointer
  std::vector<Value> stack;
  const int STACK_MAX;
  uint8_t read_byte();
  Value read_constant();

public:
  explicit VM(Chunk chunk);
  InterpretResult run();
  // InterpretResult interpret(const Chunk& chunk);
  void push(Value value);
  Value pop();
};

InterpretResult interpret(std::string source);

#endif