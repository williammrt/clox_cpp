#include "chunk.h"
#include "scanner.h"
#include <string>
#ifndef clox_compiler_h
#define clox_compiler_h

enum class Precedence {
  NONE,
  ASSIGNMENT, // =
  OR,         // or
  AND,        // and
  EQUALITY,   // == !=
  COMPARISON, // < > <= >=
  TERM,       // + -
  FACTOR,     // * /
  UNARY,      // ! -
  CALL,       // . ()
  PRIMARY
};

class Parser {
private:
  Scannar &scannar;
  Chunk &chunk;
  Token current;
  Token previous;
  bool Error;
  bool panicMode;

public:
  Parser(Scannar& scannar, Chunk& chunk));
  void advance();
  void comsume(TokenType type, std::string mesage);
  void emitByte(uint8_t byte);
  void emitBytes(uint8_t byte1, uint8_t byte2);
  void endCompiler();
  void binary();
  void grouping();
  uint8_t makeConstant(Value value);
  void emitConstant(Value value);
  void number();
  void unary();
  void parsePrecedence(Precedence precedence);
  void expression();
  void errorAtCurrent(std::string mesage);
  void error(std::string &mesage);
  void errorAt(Token token, std::string mesage);
  bool hasError() const;
};

bool compile(std::string source, Chunk &chunk);

#endif
