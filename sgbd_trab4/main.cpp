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

set<int> commitadas, rollback;
set<string> dados_bloqueados;
map<string, string> banco;

void escrever_redo(linha &l){
    //Garantindo que todas as transações que sofreram rollback ou 
    //commit não vão realizar mais nenhuma operação de escrita
    if(commitadas.count(l.tr) || rollback.count(l.tr)) return;
    //Fazendo operação no banco
    banco[l.objeto] = l.posterior;
}

void escrever_undo(linha &l){
    //Caso o objeto tenha sido alterado por uma ação posterior 
    //a essa ação de desfazer, podemos desconsiderar tal ação 
    //dado que o que conta é o último estado do objeto
    if(dados_bloqueados.count(l.objeto)) return;
    //Caso este dado não esteja bloqueado então ele dever sofrer 
    //undo, dado que nenhuma operação até então usou aquele dado 
    //para escrita
    banco[l.objeto] = l.anterior;
}

void forward(vector<linha> &linhas){
    for(linha l:linhas){
        if(l.operacao == 'w'){
            escrever_redo(l);
        }else if(l.operacao == 'a'){
            rollback.insert(l.tr);
        }else if(l.operacao == 'c'){
            commitadas.insert(l.tr); 
        }        
    }
}

void backward(vector<linha> &linhas){
    for(int i = linhas.size()-1; i >= 0; i--){
        linha l = linhas[i];
        if(l.operacao == 'w' && !commitadas.count(l.tr)){
            //Escreveu mas não foi commitada, logo undo
            escrever_undo(l);
        }else if(l.operacao == 'w'){
            //Escreveu e foi commitada, logo a bloqueamos o objeto
            //para ele não sofrer undo por causa de outra transação
            dados_bloqueados.insert(l.objeto);
        }
    }
}

void printarBanco(){
    for(auto par:banco)
        cout << par.first << " = " << par.second << "\n";
}

int main(){
    vector<linha> linhas = ler_log("in.txt");
    forward(linhas);
    backward(linhas);
    printarBanco();
    return 0;
}
