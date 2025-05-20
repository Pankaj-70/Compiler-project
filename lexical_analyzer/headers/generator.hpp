#ifndef __GENERATOR_H
#define __GENERATOR_H
#include<fstream>
#include<vector>
#include "parser.hpp"

class Generator{
    AST_NODE *ROOT;
    std::ofstream assemblyFile;
    std::stringstream sectionData;
    std::stringstream sectionText;
    int stringReferenceCounter;
    std::vector<std::string *>stringReferenceNumber;
    
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

    int lookup(std::string *DATA) {
        int counter = 0;
        for(std::string *TEMP: stringReferenceNumber) {
            if(*TEMP == *DATA) {
                return counter;
            }
            counter++;
        }
        return -1;
    }

    void sectionDataDefine(std::string *NEW_ENTRY,int referenceNumber) {
        std::cout<<"dfhsld";
        sectionData << "SRef" + referenceNumber;
        sectionData << " : \n";
        sectionData << "db \"" + *NEW_ENTRY + "\"\n   ";
        sectionData << "\n";
        sectionData << "SRef" + referenceNumber;
        sectionData << "_L : equ $ -SRef" + referenceNumber;
        sectionData << "\n\n";
    }
    void generatePrint(AST_NODE * STATEMENT)
    {
        if(!STATEMENT -> CHILD -> VALUE) {
            std::cout << "[!] Generator error, no string is linked with print" << std::endl;
            exit(1);
        }
        int referenceNumber = lookup(STATEMENT->CHILD->VALUE);
        if(referenceNumber == -1) {
            sectionDataDefine(STATEMENT->CHILD->VALUE, referenceNumber);
            referenceNumber = stringReferenceCounter;
            stringReferenceCounter++;
        }
        sectionText << "mov rax , 1\n";
        sectionText << "mov rdi , 1\n";
        sectionText << "mov rsi , SRef" + referenceNumber;
        sectionText << "\nmov rdx , SRef" + referenceNumber;
        sectionText << "_L\nsyscall\n\n";
    }
    void generate() {
        stringReferenceCounter = 0;
        sectionData << "section .data\n\n";
        sectionText << "section .text\nglobal _start\n_start: \n";
        for(AST_NODE *CURRENT: ROOT -> SUB_STATEMENTS) {
            switch(CURRENT -> NODE_TYPE) {
                case NODE_RETURN: {sectionText << generateReturn(CURRENT); break;}
                case NODE_PRINT: {generatePrint(CURRENT); break;}
                default: {std::cout << "The unknown node is: " << nodeToString(CURRENT -> NODE_TYPE); break;}
            }
        }
        

        writeSections();
    }
};

#endif