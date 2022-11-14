#include <iostream>
#include <string>

#include "common.h"
#include "scanner.h"

static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c) { return c >= '0' && c <= '9'; }

static Token makeToken(TokenType type, const Scannar &scannar) {
  return Token(type, scannar.get_line(), scannar.getLexeme());
}
static Token errorToken(std::string message, int line) {
  return Token(TokenType::ERROR, line, message);
}

Token::Token(TokenType type, int line, std::string lexeme)
    : type{type}, line{line}, lexeme{lexeme} {}

Token::Token(): type{TokenType::NULL_T}, line{-1}, lexeme{""} {}

Scannar::Scannar(std::string source)
    : source{source}, start_pos{0}, current_pos{0}, line{1} {
  std::cout << source << "\n";
}

bool Scannar::isAtEnd() const { return current_pos == source.size(); }

char Scannar::peek() const { return source[current_pos]; }

char Scannar::peekNext() const {
  if (isAtEnd()) {
    return '\0';
  }
  return source[current_pos + 1];
}

void Scannar::skipWhitespace() {
  char c;
  while (true) {
    c = peek();
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;
    case '\n':
      line += 1;
      advance();
      break;
    case '/':
      if (peekNext() == '/') {
        // A comment goes until the end of the line.
        while (peek() != '\n' && !isAtEnd()) {
          advance();
        };
      } else {
        return;
      }
      break;
    default:
      return;
    }
  }
}

TokenType Scannar::checkKeyword(int start, int length, const std::string &rest,
                                TokenType type) const {
  if (current_pos - start_pos == start + length &&
      source.substr(start_pos + start, length) == rest) {
    return type;
  }

  return TokenType::IDENTIFIER;
}

TokenType Scannar::identifierType() {
  switch (source[start_pos]) {
  case 'a':
    return checkKeyword(1, 2, "nd", TokenType::AND);
  case 'c':
    return checkKeyword(1, 4, "lass", TokenType::CLASS);
  case 'e':
    return checkKeyword(1, 3, "lse", TokenType::ELSE);
  case 'f':
    if (current_pos - start_pos > 1) {
      switch (source[start_pos + 1]) {
      case 'a':
        return checkKeyword(2, 3, "lse", TokenType::FALSE);
      case 'o':
        return checkKeyword(2, 1, "r", TokenType::FOR);
      case 'u':
        return checkKeyword(2, 1, "n", TokenType::FUN);
      }
    }
    break;
  case 'i':
    return checkKeyword(1, 1, "f", TokenType::IF);
  case 'n':
    return checkKeyword(1, 2, "il", TokenType::NIL);
  case 'o':
    return checkKeyword(1, 1, "r", TokenType::OR);
  case 'p':
    return checkKeyword(1, 4, "rint", TokenType::PRINT);
  case 'r':
    return checkKeyword(1, 5, "eturn", TokenType::RETURN);
  case 's':
    return checkKeyword(1, 4, "uper", TokenType::SUPER);
  case 't':
    if (current_pos - start_pos > 1) {
      switch (source[start_pos + 1]) {
      case 'h':
        return checkKeyword(2, 2, "is", TokenType::THIS);
      case 'r':
        return checkKeyword(2, 2, "ue", TokenType::TRUE);
      }
    }
    break;
  case 'v':
    return checkKeyword(1, 2, "ar", TokenType::VAR);
  case 'w':
    return checkKeyword(1, 4, "hile", TokenType::WHILE);
  }
  return TokenType::IDENTIFIER;
}

Token Scannar::identifier() {
  while (isAlpha(peek()) || isDigit(peek()))
    advance();
  return makeToken(identifierType(), *this);
}

Token Scannar::number() {
  while (isDigit(peek())) {
    advance();
  }

  // Look for a fractional part.
  if (peek() == '.' && isDigit(peekNext())) {
    // Consume the ".".
    advance();

    while (isDigit(peek())) {
      advance();
    }
  }

  return makeToken(TokenType::NUMBER, *this);
}

Token Scannar::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') {
      line += 1;
    }
    advance();
  }

  if (isAtEnd()) {
    return errorToken("Unterminated string.", get_line());
  }

  advance();
  return makeToken(TokenType::STRING, *this);
}
Token Scannar::scanToken() {
  skipWhitespace();
  start_pos = current_pos;
  if (isAtEnd()) {
    return makeToken(TokenType::EOF_T, *this);
  }
  char c = advance();
  if (isAlpha(c)) {
    return identifier();
  }
  if (isDigit(c)) {
    return number();
  }
  switch (c) {
  case '(':
    return makeToken(TokenType::LEFT_PAREN, *this);
  case ')':
    return makeToken(TokenType::RIGHT_PAREN, *this);
  case '{':
    return makeToken(TokenType::LEFT_BRACE, *this);
  case '}':
    return makeToken(TokenType::RIGHT_BRACE, *this);
  case ';':
    return makeToken(TokenType::SEMICOLON, *this);
  case ',':
    return makeToken(TokenType::COMMA, *this);
  case '.':
    return makeToken(TokenType::DOT, *this);
  case '-':
    return makeToken(TokenType::MINUS, *this);
  case '+':
    return makeToken(TokenType::PLUS, *this);
  case '/':
    return makeToken(TokenType::SLASH, *this);
  case '*':
    return makeToken(TokenType::STAR, *this);
  case '!':
    return makeToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG,
                     *this);
  case '=':
    return makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL,
                     *this);
  case '<':
    return makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS,
                     *this);
  case '>':
    return makeToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER,
                     *this);
  }
  return errorToken("Unexpected character.", get_line());
}

int Scannar::get_line() const { return line; }

std::string Scannar::getLexeme() const {
  return move(source.substr(start_pos, current_pos - start_pos));
}

char Scannar::advance() {
  char tmp = source[current_pos];
  current_pos += 1;
  return tmp;
}

bool Scannar::match(char expected) {
  if (isAtEnd()) {
    return false;
  }
  if (source[current_pos] != expected) {
    return false;
  }
  current_pos += 1;
  return true;
}