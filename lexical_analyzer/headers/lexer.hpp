#ifndef __LEXER_HPP
#define __LEXER_HPP
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

enum type
{
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PARENTH,
    TOKEN_RIGHT_PARENTH,
    TOKEN_KEYWORD,
    TOKEN_EOF,
    TOKEN_QUOTES,
    TOKEN_STRING
};

struct Token
{
    enum type TYPE;
    std::string VALUE;
};

class Lexer
{
private:
    std::string source;
    int cursor;
    char curr;
    int size;
    int lineNumber;
    int characterNumber;
    std::vector<std::string> keywords = {"print", "return"};

public:
    Lexer(std::string sourceCode)
    {
        source = sourceCode;
        cursor = 0;
        curr = sourceCode[cursor];
        size = source.length();
        lineNumber = 1;
        characterNumber = 1;
    }

    char advance()
    {
        if (cursor < size)
        {
            char temp = curr;
            cursor++;
            characterNumber++;
            curr = cursor < size ? source[cursor] : '\0';
            return temp;
        }
        else
        {
            return '\0';
        }
    }

    char peek(int offset)
    {
        if (offset + cursor < size)
        {
            return source[offset + cursor];
        }
        return '\0';
    }

    void checkAndSkip()
    {
        while (curr == ' ' || curr == '\n' || curr == '\t' || curr == '\r')
        {
            if (curr == '\n')
            {
                lineNumber++;
                characterNumber = 0;
            }
            advance();
        }
    }

    Token *tokenizeID_KEYWORD()
    {
        std::stringstream buffer;
        buffer << curr;
        advance();
        while (isalnum(curr) || curr == '_')
        {
            buffer << curr;
            advance();
        }
        Token *newToken = new Token;
        newToken->TYPE = std::find(keywords.begin(), keywords.end(), buffer.str()) != keywords.end() ? TOKEN_KEYWORD : TOKEN_ID;
        newToken->VALUE = buffer.str();
        return newToken;
    }

    Token *tokenizeINT()
    {
        std::stringstream buffer;
        while (isdigit(curr))
        {
            buffer << curr;
            advance();
        }
        Token *newToken = new Token();
        newToken->TYPE = TOKEN_INT;
        newToken->VALUE = buffer.str();
        return newToken;
    }

    Token *tokenizeString()
    {
        std::stringstream buffer;
        while (curr != '"')
        {
            if (curr == '\0')
            {
                std::cout << "[!]Lexer Error: Missing quote";
                exit(1);
            }
            buffer << curr;
            advance();
        }
        Token *newToken = new Token();
        newToken->TYPE = TOKEN_STRING;
        newToken->VALUE = buffer.str();
        return newToken;
    }

    Token *tokenizeSPECIAL(enum type TYPE)
    {
        Token *newToken = new Token();
        newToken->TYPE = TYPE;
        newToken->VALUE = std::string(1, curr);
        advance();
        return newToken;
    }

    std::string typeToString(enum type TYPE)
    {
        std::string str;
        switch (TYPE)
        {
        case TOKEN_ID:
            str = "Identifier";
            break;
        case TOKEN_INT:
            str = "Integer";
            break;
        case TOKEN_EQUALS:
            str = "Equals To";
            break;
        case TOKEN_SEMICOLON:
            str = "Semicolon";
            break;
        case TOKEN_LEFT_PARENTH:
            str = "Left Parenthesis";
            break;
        case TOKEN_RIGHT_PARENTH:
            str = "Right Parenthesis";
            break;
        case TOKEN_KEYWORD:
            str = "Keyword";
            break;
        case TOKEN_EOF:
            str = "EOF";
            break;
        }
        return str;
    }
    std::vector<Token *> tokenize()
    {
        std::vector<Token *> tokens;
        bool notEOF = true;
        while (cursor < size && notEOF)
        {
            checkAndSkip();
            if (isalpha(curr) || curr == '_') // LOGIC FOR IDENTIFIERS
            {
                tokens.push_back(tokenizeID_KEYWORD());
            }
            else if (isdigit(curr))
            {
                tokens.push_back(tokenizeINT());
            }
            else
            {
                switch (curr)
                {
                case ';':
                    tokens.push_back(tokenizeSPECIAL(TOKEN_SEMICOLON));
                    break;
                case '(':
                    tokens.push_back(tokenizeSPECIAL(TOKEN_LEFT_PARENTH));
                    break;
                case ')':
                    tokens.push_back(tokenizeSPECIAL(TOKEN_RIGHT_PARENTH));
                    break;
                case '=':
                    tokens.push_back(tokenizeSPECIAL(TOKEN_EQUALS));
                    break;
                case '"':
                    tokens.push_back(tokenizeSPECIAL(TOKEN_QUOTES));
                    tokens.push_back(tokenizeString());
                    tokens.push_back(tokenizeSPECIAL(TOKEN_QUOTES));
                    break;
                case '~':
                    while(curr != '\n' && curr != '\0')
                    {
                        advance();
                    }
                    break;
                default:
                    std::cout << "[!] Lexer Error: Token '" << curr << "' is unidentified" << std::endl;
                    std::cout << "Error occured at line number: " << lineNumber << " and character number: " << characterNumber << std::endl;
                    exit(1);
                }
            }
        }
        return tokens;
    }
};
#endif