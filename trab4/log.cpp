#include <bits/stdc++.h>
#include "log.hpp"

using namespace std;

linha parse(string &s){
    int pos = 0;
    vector<string> info(7, "");
    for(int i = 1; i < s.size()-1; i++){
        while(i < s.size()-1 && s[i] != '|') if(s[i++] != ' ') info[pos] += s[i-1];
        ++pos;
    }
    return linha(info[0], info[1], info[2], info[3], info[4], info[5], info[6]);
}

vector<linha> ler_log(string nome_arquivo){
    vector<linha> linhas;
    string s;
    ifstream fin(nome_arquivo);
    while(getline(fin, s)) linhas.push_back(parse(s));
    fin.close();
    return linhas;
}
