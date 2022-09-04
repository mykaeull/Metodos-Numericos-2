#ifndef LOG_HPP
#define LOG_HPP

#include <bits/stdc++.h>
using namespace std;

struct linha{
    int num_seq, ts, tr;
    char operacao;
    string objeto, anterior, posterior;

    linha(){}

    linha(string a, string b, string c, string d, string e, string f, string g){
        num_seq = stoi(a);
        ts = stoi(b);
        tr = stoi(c.substr(1, c.size()-1));
        operacao = d[0];
        objeto = e;
        anterior = f;
        posterior = g;
    }

    void print(){
        cout << num_seq << " " << ts << " " << tr << " " << operacao << " " << objeto <<
        " " << anterior << " " << posterior << "\n";
    }
};

vector<linha> ler_log(string nome_arquivo);

#endif

