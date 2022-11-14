#ifndef clox_scanner_h
#define clox_scanner_h

#include <string>

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,
  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,
  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FOR,
  FUN,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  ERROR,
  EOF_T,
  NULL_T 
};

class Token {
public:
  TokenType type;
  int line;
  std::string lexeme;

  /* I want an explicit constructor, but that may cause problem in Parser */
  explicit Token(TokenType type, int line, std::string lexeme);
  explicit Token();
};

class Scannar {
private:
  int start_pos;
  int current_pos;
  int line;
  std::string source;

public:
  explicit Scannar(std::string source);
  bool isAtEnd() const;
  char peek() const;
  char peekNext() const;
  void skipWhitespace();
  TokenType checkKeyword(int start, int length, const std::string& rest, TokenType type) const;
  TokenType identifierType();
  Token identifier();
  Token number();
  Token string();
  /* outer function call this to get tokens */
  Token scanToken();
  int get_line() const;
  std::string getLexeme() const;
  char advance();
  bool match(char expected);
};

#endif