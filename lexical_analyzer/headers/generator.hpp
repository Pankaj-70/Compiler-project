#ifndef __GENERATOR_H
#define __GENERATOR_H
#include<fstream>
#include "parser.hpp"

class Generator{
    AST_NODE *ROOT;
    std::ofstream assemblyFile;
    std::stringstream sectionData;
    std::stringstream sectionText;
    
    public:
    Generator(AST_NODE *ROOT, std::string fileName) {
        this->ROOT = ROOT;
        assemblyFile.open(fileName + ".asm");
    }

    void writeSections() {
        assemblyFile << sectionData.str();
        assemblyFile << sectionText.str();

        assemblyFile.close();
    }

    std::string generateReturn(AST_NODE * STATEMENT) {
        if(!STATEMENT -> CHILD -> VALUE) {
            std::cout << "[!] Generator error, provide something to return" << std::endl;
            exit(1);
        }
        std::stringstream codeBuffer;
        codeBuffer << "\nmov rax , 60\n";
        codeBuffer << "mov rdi , " << *STATEMENT -> CHILD -> VALUE << std::endl;
        codeBuffer << "syscall" << std::endl; 

        return codeBuffer.str();
    }

    void generate() {
        sectionData << "section .data\n\n";
        sectionText << "section .text\nglobal _start\n_start: \n";
        for(AST_NODE *CURRENT: ROOT -> SUB_STATEMENTS) {
            switch(CURRENT -> NODE_TYPE) {
                case NODE_RETURN: {sectionText << generateReturn(CURRENT); break;}
                default: {std::cout << "The unknown node is: " << nodeToString(CURRENT -> NODE_TYPE); break;}
            }
        }
        

        writeSections();
    }
};

#endif