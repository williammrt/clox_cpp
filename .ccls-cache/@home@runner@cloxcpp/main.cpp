#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

constexpr bool DEBUG_TRACE_EXECUTION = true;

/* line by line execution */
static void repl();
/* given a filename in cmd, run that */
static void runFile(const char *fileName);

int main(int argc, char **argv) {
  /*
  Chunk chunk;

  int constant_pos = chunk.addConstant(1.2);
  chunk.writeChunk(OP_CONSTANT, 123);
  chunk.writeChunk(constant_pos,
                   123); // this pos should be able to be converted to byte...

  constant_pos = chunk.addConstant(3.4);
  chunk.writeChunk(OP_CONSTANT, 123);
  chunk.writeChunk(constant_pos, 123);

  chunk.writeChunk(OP_ADD, 123);

  constant_pos = chunk.addConstant(5.6);
  chunk.writeChunk(OP_CONSTANT, 123);
  chunk.writeChunk(constant_pos, 123);

  chunk.writeChunk(OP_DIVIDE, 123);

  chunk.writeChunk(OP_NEGATE, 123);
  chunk.writeChunk(OP_RETURN, 123);
  // disassembleChunk(chunk, "test chunk");

  VM vm(std::move(chunk), DEBUG_TRACE_EXECUTION);

  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    std::cerr << "Usage: clox [path]\n";
    exit(64);
  }

  vm.run();
  */
  runFile("test.txt");
  return 0;
}

static void repl() {
  std::string line;
  while (true) {
    std::cout << "> ";
    if (!getline(std::cin, line)) {
      std::cout << "\n";
      break;
    }
    interpret(line);
  }
}

static void runFile(const char *fileName) {
  std::ifstream inFile(fileName);
  if (!inFile.is_open()) {
    std::cerr << R"(Could not open file \)" << fileName << R"(\.)"
              << "\n";
    exit(74);
  }
  std::stringstream buffer;
  buffer << inFile.rdbuf();
  if (!buffer.good()) {
    std::cerr << "Something wrong in converting file to string stream\n";
    exit(74);
  }
  InterpretResult result = interpret(std::move(buffer.str()));
  inFile.close();
  if (result == InterpretResult::INTERPRET_COMPILE_ERROR) {
    exit(65);
  }
  if (result == InterpretResult::INTERPRET_RUNTIME_ERROR) {
    exit(70);
  }
}