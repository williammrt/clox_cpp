#include "compiler.h"
#include "chunk.h"
#include "common.h"
#include "scanner.h"
#include <iomanip>
#include <iostream>
#include <string>

Parser::Parser(Scannar &scannar, Chunk& chunk))
    : scannar{scannar}, chunk{chunk}, Error{false}, panicMode{false}, current{}, previous{} {}

void Parser::advance() {
  previous = current;
  while (true) {
    current = scannar.scanToken();
    // != ??
    if (current.type != TokenType::ERROR) {
      break;
    }
    errorAtCurrent(current.lexeme);
  }
}

void Parser::comsume(TokenType type, std::string message) {
  if (current.type == type) {
    advance();
    return;
  }
  errorAtCurrent(message);
}

void Parser::emitByte(uint8_t byte) { chunk.writeChunk(byte, previous.line); }
void Parser::emitBytes(uint8_t byte1, uint8_t byte2) {
  chunk.writeChunk(byte1, previous.line);
  chunk.writeChunk(byte2, previous.line);
}
void Parser::endCompiler() { emitByte(OP_RETURN); }

void Parser::binary() {
  Tokentype operatorType = previous.type;
  
}

void Parser::grouping() {
  expression();
  consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
}

uint8_t Parser::makeConstant(Value value) {
  int constant = chunk.addConstant(value);
  if (constant > 256) { // UINT8_MAX
    error("Too many constants in one chunk.");
    return 0;
  }
  return static_cast<uint8_t>(constant);
}
void Parser::emitConstant(Value value) {
  emitBytes(OP_CONSTANT, makeConstant(value));
}
void Parser::number() {
  double value = stod(previous.lexeme);
  emitConstant(value);
}

void Parser::unary() {
  TokenType operatorType = previous.type;
  parsePrecedence(Precedence::UNARY);
  switch (operatorType) {
    case TokenType::MINUS:
      emitByte(OP_NEGATE); 
      break;
    default:
      return; // unreachable
  }
  
}

void Parser::expression() {
  parsePrecedence(Precedence::ASSIGNMENT);
}
/* Do I really need this wrapper in Class?? */
void Parser::errorAtCurrent(std::string message) { errorAt(current, message); }

void Parser::error(std::string &message) { errorAt(previous, message); }

void Parser::errorAt(Token token, std::string message) {
  if (panicMode) {
    return;
  }
  panicMode = true;
  std::cerr << "[line " << token.line << "] Error";
  if (token.type == TokenType::EOF_T) {
    std::cerr << " at end";
  } else if (token.type == TokenType::ERROR) {
    // Do nothing
  } else {
    std::cerr << " at '" << token.lexeme << "'";
  }
  std::cerr << ": " << message << "\n";
  Error = true;
}

bool Parser::hasError() const { return Error; }

bool compile(std::string source, Chunk &chunk) {
  Scannar scannar(move(source));
  Parser parser(scannar);
  parser.advance();
  parser.expression();
  parser.consume(TokenType::EOF_T, "Expect end of expression.");
  return parser.hasError();
  /*
  int line = -1;
  while (true) {
    Token token = scannar.scanToken();
    if (token.line != line) {
      std::cout << std::setw(4) << token.line << " ";
      line = token.line;
    } else {
      std::cout << "   | ";
    }
    std::cout << std::setw(2) << token.type << " '" << token.lexeme << "'\n";
    if (token.type == TokenType::EOF_T) {
      break;
    }
  }
  */
}