#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#ifndef ARQUIVO_H
#define ARQUIVO_H

class Arquivo {
    private:
        string nomeArq;
    public:
        Arquivo(string nomeArq);
        vector<string> lerInstrucoes();
};

#endif