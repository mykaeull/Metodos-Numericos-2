#include <bits/stdc++.h>
#include "operacoes.hpp"
// #include "log.hpp"

using namespace std;


map<string, string> banco;
unordered_set<int> rollbacked, commited, undo_list;

void rollback(vector<linha> &linhas, int transacao, int linha_rollback){
    for(int i = linha_rollback-1; i >= 0; i--){
        if(linhas[i].tr != transacao) continue;
        if(linhas[i].operacao == 'w')
            banco[linhas[i].objeto] = linhas[i].anterior; 
        else if(linhas[i].operacao == 'r' && !banco.count(linhas[i].objeto))
            banco[linhas[i].objeto] = linhas[i].anterior; 
    }
}

void obter_estado_transacao(vector<linha> &linhas){
    for(int i = 0; i < linhas.size(); i++){
        linha l = linhas[i];
        if(l.operacao == 'a'){
            rollback(linhas, l.tr, i);
            rollbacked.insert(l.tr);
        }else if(l.operacao == 'c'){
            commited.insert(l.tr);
        }
    }
}

void undo(vector<linha> &linhas){
    for(int i = linhas.size()-1; i >= 0; i--){
        linha l = linhas[i];
        //se a transação sofrer rollback ou for comittada ela n deve sofrer undo
        if(rollbacked.count(l.tr) || commited.count(l.tr)) 
            continue;
        undo_list.insert(l.tr);
        if(l.operacao == 'w')
            banco[l.objeto] = l.anterior;
        else if(l.operacao == 'r' && !banco.count(l.objeto))
            banco[l.objeto] = l.anterior; 
            
    }
}

void redo(vector<linha> &linhas){
    for(int i = 0; i < linhas.size(); i++){
        linha l = linhas[i];
        //se a transação não foi comittada ela n deve sofrer redo
        if(!commited.count(l.tr)) continue;
        if(l.operacao == 'w')
            banco[l.objeto] = l.posterior;
        else if(l.operacao == 'r' && !banco.count(l.objeto))
            banco[l.objeto] = l.anterior; 
    }
}

void recuperar_apos_falha(vector<linha> &linhas){
    obter_estado_transacao(linhas);
    undo(linhas);
    redo(linhas);
}

void listar_rollbacks(){
    cout << "ROLL: ";
    for(int x:rollbacked)
        cout << "T" << x << " ";
    cout << "\n";
}

void listar_redos(){
    cout << "REDO: ";
    for(int x:commited)
        cout << "T" << x << " ";
    cout << "\n";
}

void listar_undos(){
    cout << "UNDO: ";
    for(int x:undo_list)
        cout << "T" << x << " ";
    cout << "\n";
}

void printar_banco(){
    listar_rollbacks();
    listar_undos();
    listar_redos();
    for(auto par:banco) 
        cout << par.first << " = " << par.second << "\n";
}
