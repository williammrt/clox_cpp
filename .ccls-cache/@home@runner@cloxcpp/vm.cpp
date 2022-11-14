#include "vm.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
#include "chunk.h"
#include <iostream>

extern bool DEBUG_TRACE_EXECUTION;

#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double b = pop();                                                          \
    double a = pop();                                                          \
    push(a op b);                                                              \
  } while (false)

VM::VM(Chunk chunk)
    : chunk{chunk}, ip{0}, DEBUG{DEBUG_TRACE_EXECUTION}, STACK_MAX{256} {}

InterpretResult VM::run() {
  uint8_t instruction;
  while (true) {
    if (DEBUG) {
      std::cout << "          ";
      for (const Value &value : stack) {
        std::cout << "[ " << value << " ]";
      }
      std::cout << "\n";
      disassembleInstruction(chunk, ip);
    }
    instruction = read_byte();
    switch (instruction) {
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBTRACT:
      BINARY_OP(-);
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    case OP_NEGATE:
      // push(-pop());
      stack.back() *= -1;
      break;
    case OP_RETURN: {
      printValue(pop());
      std::cout << "\n";
      return InterpretResult::INTERPRET_OK;
    }
    case OP_CONSTANT: {
      Value constant = read_constant();
      push(constant);
      // printValue(constant);
      // std::cout << "\n";
      break;
    }
    }
  }
}

uint8_t VM::read_byte() {
  ip += 1;
  return chunk.get_code(ip - 1);
}

Value VM::read_constant() { return chunk.get_constant(read_byte()); }

void VM::push(Value value) { stack.push_back(value); }

Value VM::pop() {
  Value tmp = stack.back();
  stack.pop_back();
  return tmp;
}

InterpretResult interpret(std::string source) {
  Chunk chunk;
  if (!compile(std::move(source), chunk)) {
    return InterpretResult::INTERPRET_COMPILE_ERROR;
  }
  VM vm(std::move(chunk));
  InterpretResult result = vm.run();
  return result;
}
