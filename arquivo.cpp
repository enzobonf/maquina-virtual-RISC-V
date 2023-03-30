
#include "arquivo.h"

using namespace std;

Arquivo::Arquivo(string nomeArq){
    this->nomeArq = nomeArq;
}

vector<string> Arquivo::lerInstrucoes(){
    string linha; std::ifstream fin(this->nomeArq);
    vector<string> instrucoes;

    while(getline(fin, linha)){
        if(linha.size() == 32){
            //cout << linha << '\n';
            instrucoes.push_back(linha);
        }
    }

    fin.close();
    return instrucoes;
}