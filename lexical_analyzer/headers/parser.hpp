#ifndef PARSER_H
#define PARSER_H
#include "lexer.hpp"
#include <vector>

enum NODE_TYPE
{
    NODE_ROOT,
    NODE_VARIABLE,
    NODE_RETURN,
    NODE_PRINT,
    NODE_INT,
    NODE_STRING
};

std::string nodeToString(enum NODE_TYPE TYPE) {
    switch(TYPE) {
        case NODE_ROOT: {return "NODE_ROOT";}
        case NODE_STRING: {return "NODE_STRING";}
        case NODE_INT: {return "NODE_INT";}
        case NODE_PRINT: {return "NODE_PRINT";}
        case NODE_VARIABLE: {return "NODE_VARIABLE";}
        case NODE_RETURN: {return "NODE_RETURN";}
        default : {return "Unrecognized node";}
    }
}

typedef struct AST_NODE
{
    enum NODE_TYPE NODE_TYPE;
    std::string *VALUE;
    AST_NODE *CHILD;
    std::vector<AST_NODE *> SUB_STATEMENTS;
} AST_NODE;

class Parser
{
private:
    std::vector<Token *> parserTokens;
    Token *current;
    int limit;
    int index;

public:
    Parser(std::vector<Token *> tokens)
    {
        parserTokens = tokens;
        index = 0;
        current = tokens.at(index);
        limit = tokens.size();
    }

    Token *proceed(enum type TYPE)
    {
        if (current->TYPE != TYPE)
        {
            std::cout << "[4] Syntax Error" << std::endl;
            exit(1);
        }
        index++;
        current = parserTokens.at(index);
        return current;
    }

    AST_NODE *parseINT()
    {
        if (current->TYPE != TOKEN_INT)
        {
            std::cout << "[2] Syntax Error" << std::endl;
            exit(1);
        }
        AST_NODE *newNode = new AST_NODE();
        newNode->NODE_TYPE = NODE_INT;
        newNode->VALUE = &current->VALUE;
        proceed(TOKEN_INT);
        return newNode;
    }

    AST_NODE *parseID()
    {
        std::string *buffer = &current->VALUE;
        proceed(TOKEN_ID);
        proceed(TOKEN_EQUALS);

        AST_NODE *newNode = new AST_NODE();
        newNode->NODE_TYPE = NODE_VARIABLE;
        newNode->CHILD = parseINT();

        return newNode;
    }

    AST_NODE *parseRETURN()
    {
        proceed(TOKEN_KEYWORD);
        AST_NODE *newNode = new AST_NODE();
        newNode->NODE_TYPE = NODE_RETURN;
        newNode->CHILD = parseINT();
        return newNode;
    }

    AST_NODE *parseSTRING()
    {
        if (current->TYPE != TOKEN_STRING)
        {
            std::cout << "[!] Parser error, print doesn't have any string attached with it";
            exit(1);
        }
        AST_NODE *newNode = new AST_NODE();
        newNode->NODE_TYPE = NODE_STRING;
        newNode->VALUE = &current->VALUE;

        proceed(TOKEN_STRING);
        return newNode;
    }

    AST_NODE *parsePRINT()
    {
        proceed(TOKEN_KEYWORD);
        proceed(TOKEN_LEFT_PARENTH);
        proceed(TOKEN_QUOTES);

        AST_NODE *newNode = new AST_NODE();
        newNode->NODE_TYPE = NODE_PRINT;
        newNode->CHILD = parseSTRING();

        proceed(TOKEN_QUOTES);
        proceed(TOKEN_RIGHT_PARENTH);
        return newNode;
    }

    AST_NODE *parseKEYWORD()
    {
        if (current->VALUE == "return")
        {
            return parseRETURN();
        }
        else if (current->VALUE == "print")
        {
            return parsePRINT();
        }
        else
        {
            std::cout << "[1] Syntax Error" << std::endl;
            exit(1);
        }
    }

    AST_NODE *parse()
    {
        AST_NODE *ROOT = new AST_NODE();
        ROOT->NODE_TYPE = NODE_ROOT;

        while (current->TYPE != TOKEN_EOF)
        {
            switch (current->TYPE)
            {
            case TOKEN_ID:
                ROOT->SUB_STATEMENTS.push_back(parseID());
                break;
            case TOKEN_KEYWORD:
                ROOT->SUB_STATEMENTS.push_back(parseKEYWORD());
                break;

            default:
                std::cout << "[3] Syntax Error" << std::endl;
                exit(1);
                break;
            }
            proceed(TOKEN_SEMICOLON);
        }
        return ROOT;
    }
};
#endif