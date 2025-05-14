#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "headers\lexer.hpp"
#include "headers\parser.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Please pass the file for compilation" << std::endl;
        return 0;
    }
    std::ifstream input(argv[1]);
    std::stringstream buffer;
    char temp;
    std::string sourceCode;
    while (input.get(temp))
    {
        buffer << temp;
    }
    sourceCode = buffer.str();
    sourceCode.append("\0");
    std::cout << "The source code is:\n"
              << sourceCode << std::endl;

    // LEXER
    Lexer lexer(sourceCode);
    int cnt = 0;
    std::vector<Token *> tokens = lexer.tokenize();
    if (tokens.back()->TYPE != TOKEN_EOF)
    {
        Token *newToken = new Token();
        newToken->TYPE = TOKEN_EOF;
        tokens.push_back(newToken);
    }
    for (Token *temp : tokens)
    {
        cnt++;
        std::cout << cnt << "-> " << temp->VALUE << " " << lexer.typeToString(temp->TYPE) << std::endl;
    }

    std::cout << "Lexical Analysis Performed" << std::endl;

    // PARSER
    Parser parser(tokens);
    AST_NODE *ROOT = parser.parse();
    std::cout << "Parsing successfully perform" << std::endl;
    std::cout << "Root size " << ROOT->SUB_STATEMENTS.size();
    return 0;
}