#include "ShaderParser.h"

BaseParser::BaseParser(const std::string& str)
  : str_(str)
  , p_(str.c_str())
  , line_index_(1)
  , char_index_begin_(1)
  , char_index_end_(1)
  , lexer_()
{
}

TokenType BaseParser::GetTokenType()
{
  return this->lexer_.GetTokenType(this->GetChar());
}

void BaseParser::SkipSpace()
{
  TokenType type = this->GetTokenType();
  while (type == TokenType::kSpace || type == TokenType::kEnter)
  {
    this->NextChar();
    type = this->GetTokenType();
  }
}

TokenType BaseParser::CheckNextToken(bool skip_space)
{
  if (skip_space)
  {
    this->SkipSpace();
  }
  return this->GetTokenType();
}

void BaseParser::GetToken(TokenType type, bool skip_space)
{
  this->BeginParse(skip_space);
  if (this->GetTokenType() != type)
  {
    this->ThrowTokenError();
    return;
  }
  this->NextChar();
}

void BaseParser::EatToken(bool skip_space)
{
  if (skip_space)
  {
    this->SkipSpace();
  }
  this->NextChar();
}

void BaseParser::ParseText(std::string* dest)
{
  this->BeginParse();
  (*dest).clear();
  this->GetToken(TokenType::kTextParen);
  while (this->CheckNextToken() != TokenType::kTextParen)
  {
    (*dest) += this->GetChar();
    this->EatToken(false);
  }
  this->EatToken();
}

void BaseParser::ParseIdentifier(std::string* dest)
{
  this->BeginParse();
  (*dest).clear();
  TokenType next_token = this->CheckNextToken();
  if (next_token != TokenType::kAlphabet && next_token != TokenType::kUnderBar)
  {
    this->ThrowTokenError();
    return;
  }
  do
  {
    (*dest) += this->GetChar();
    this->EatToken(false);
    next_token = this->CheckNextToken(false);
  } while (next_token == TokenType::kAlphabet || next_token == TokenType::kUnderBar || next_token == TokenType::kNumber);
}

void BaseParser::ParseSpecialIdentifier(std::string* dest)
{
  this->BeginParse();
  (*dest).clear();
  TokenType next_token = this->CheckNextToken();
  if (next_token != TokenType::kAlphabet && next_token != TokenType::kUnderBar && next_token != TokenType::kNumber)
  {
    this->ThrowTokenError();
    return;
  }
  do
  {
    (*dest) += this->GetChar();
    this->EatToken(false);
    next_token = this->CheckNextToken(false);
  } while (next_token == TokenType::kAlphabet || next_token == TokenType::kUnderBar || next_token == TokenType::kNumber);
}

void BaseParser::ParseInt(T_FLOAT* dest)
{
  this->BeginParse();
  bool minus = false;
  TokenType next_token = this->CheckNextToken();
  // 符号部分の処理
  if (next_token == TokenType::kPlus)
  {
    this->EatToken(false);
    next_token = this->CheckNextToken();
  }
  else if (next_token == TokenType::kMinus)
  {
    minus = true;
    this->EatToken(false);
    next_token = this->CheckNextToken();
  }

  std::string str = minus ? "-" : "";
  if (next_token != TokenType::kNumber)
  {
    this->ThrowTokenError();
    return;
  }
  do
  {
    str += this->GetChar();
    this->EatToken(false);
    next_token = this->CheckNextToken();
  } while (next_token == TokenType::kNumber);
  (*dest) = (T_FLOAT)std::strtol(str.c_str(), NULL, 10);
}

void BaseParser::ParseFloat(T_FLOAT* dest)
{
  this->BeginParse();
  bool minus = false;
  TokenType next_token = this->CheckNextToken();
  // 符号部分の処理
  if (next_token == TokenType::kPlus)
  {
    this->EatToken(false);
    next_token = this->CheckNextToken();
  }
  else if (next_token == TokenType::kMinus)
  {
    minus = true;
    this->EatToken(false);
    next_token = this->CheckNextToken();
  }
  std::string sign = minus ? "-" : "";
  // 整数部の処理
  std::string integer = "";
  if (next_token == TokenType::kNumber)
  {
    do
    {
      integer += this->GetChar();
      this->EatToken(false);
      next_token = this->CheckNextToken();
    } while (next_token == TokenType::kNumber);
  }
  // 小数部の処理
  std::string fractional = "";
  if (next_token == TokenType::kDot)
  {
    this->EatToken(false);
    next_token = this->CheckNextToken();
    if (next_token == TokenType::kNumber)
    {
      do
      {
        fractional += this->GetChar();
        this->EatToken(false);
        next_token = this->CheckNextToken();
      } while (next_token == TokenType::kNumber);
    }
  }
  (*dest) = std::strtof((sign + integer + "." + fractional).c_str(), NULL);
}

void BaseParser::GetText(const std::string& end_simbol, std::string* dest)
{
  this->BeginParse();
  while ((*dest).find(end_simbol) == std::string::npos)
  {
    (*dest) += this->GetChar();
    this->NextChar();
  }
  (*dest).replace((*dest).length() - end_simbol.length(), end_simbol.length(), "");
}

void BaseParser::NextChar()
{
  TokenType token_type = this->GetTokenType();
  if (token_type == TokenType::kEOF)
  {
    this->ThrowEofError();
    return;
  }
  ++this->char_index_end_;
  if (token_type == TokenType::kEnter)
  {
    ++this->line_index_;
    this->char_index_begin_ = 0;
    this->char_index_end_ = 0;
  }
  ++this->p_;
}

char BaseParser::GetChar()
{
  return *this->p_;
}

void BaseParser::BeginParse(bool skip_space)
{
  if (skip_space)
  {
    this->SkipSpace();
  }
  this->char_index_begin_ = this->char_index_end_;
}

void BaseParser::ThrowError(const std::string& message)
{
  throw ParseException("(" + std::to_string(this->line_index_) + "," + std::to_string(this->char_index_begin_) + "): Parse Error: " + message);
}

void BaseParser::ThrowIdentifierError(const std::string& type, const std::string& identifier)
{
  this->ThrowError("予期せぬ" + type + "\"" + identifier + "\"が検出されました");
}

void BaseParser::ThrowTokenError()
{
  if (this->GetTokenType() == TokenType::kEOF)
  {
    this->ThrowEofError();
    return;
  }
  std::string message = std::string();
  message += "予期せぬトークン\'";
  message += this->GetChar();
  message += "\'が検出されました";
  this->ThrowError(message);
}

void BaseParser::ThrowEofError()
{
  this->ThrowError("予期せぬEOFが検出されました");
}
