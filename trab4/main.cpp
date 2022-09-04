#include <bits/stdc++.h>
#include "log.hpp"
#include "operacoes.hpp"

using namespace std;

int main(){
    vector<linha> linhas = ler_log("in.txt");
    recuperar_apos_falha(linhas);
    printar_banco();
    return 0;
}
