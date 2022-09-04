#include <bits/stdc++.h>

using namespace std;

double f1(double x){
	return 1/cbrt(x*x);
}

double f2(double x){
	return 1/sqrt(4-x*x);
}

double f3(double x){
	return 1/sqrt(x);
}

double xs_simples(double a, double b, double s){
	return (a+b)/2.0 + ((b-a)/2.0)*tanh(s);	
}

double xs_dupla(double a, double b, double s){
	return (a+b)/2.0 + ((b-a)/2.0)*tanh((M_PI/2.0)*sinh(s));	
}

double jacobiano_simples(double a, double b, double s){
	return ((b-a)/2.0)*(1/(cosh(s)*cosh(s)));
}

double jacobiano_dupla(double a, double b, double s){
	return ((b-a)/2.0)*((M_PI/2.0)*(cosh(s)/(cosh((M_PI/2.0)*sinh(s))*cosh((M_PI/2.0)*sinh(s)))));
}

double fs_simples(double a, double b, double s, double (*f)(double)){
	return f(xs_simples(a, b, s))*jacobiano_simples(a, b, s);
}

double fs_dupla(double a, double b, double s, double (*f)(double)){
	return f(xs_dupla(a, b, s))*jacobiano_dupla(a, b, s);
}

double fechada(double xi, double xf, double a, double b, int f_op, int fs_op) {
    double h = (xf-xi) / 4;
    vector<double> s = {7.0, 32.0, 12.0, 32.0, 7.0};
    double sum = 0.0;
    auto f = f_op == 1 ? f1 : f_op == 2 ? f2 : f3;
    auto fs = fs_op == 1 ? fs_simples : fs_dupla;

    for(int i = 0; i < (int)s.size(); i++) {
        sum += fs(a, b, xi+i*h, f)*s[i];
    }
    return (2*h*sum)/45;
}

double aberta(double xi, double xf, double a, double b, int f_op, int fs_op) {
    double h = (xf-xi) / 6;
    vector<double> s = {11.0, -14.0, 26.0, -14.0, 11.0};
    double sum = 0.0;
    auto f = f_op == 1 ? f1 : f_op == 2 ? f2 : f3;
    auto fs = fs_op == 1 ? fs_simples : fs_dupla;

    for(int i = 0; i < (int)s.size(); i++) {
        sum += fs(a, b, ((xi+h)+i*h), f)*s[i];
    }
    return (3*h*sum)/10;
}

double particionado(double xi, double xf, double aa, double bb, int n, int filosofia, int f_op, int fs_op) {
    double dx = (xf-xi)/n, sum = 0.0;
    for(int i = 0; i < n; i++){
	    double a = xi+i*dx;		
		double b = a+dx;
		sum += filosofia == 1 ? fechada(a, b, aa, bb, f_op, fs_op) : aberta(a, b, aa, bb, f_op, fs_op);
	}
    return sum;
}

int main() {
    int f_op = -1;
    double a, b, c;
    cout << "1 - (1/cbrt(x^2))\n";
	cout << "2 - (1/sqrt(4-x^2))\n";
    cout << "3 - (1/sqrt(x))\n";
	while(f_op == -1){
		cout << "Escolha a funcao que deseja integrar: ";
		cin >> f_op;
		if(f_op < 1 || f_op > 3){
			f_op = -1;
			cout << "Digite uma opcao valida!\n";
		}
	}
    cout << "Entre com o intervalo de integracao [a, b]:\n";
	cout << "a = "; cin >> a;
	cout << "b = "; cin >> b;

    cout << "1 - Exponencial simples" << endl;
    cout << "2 - Exponencial dupla" << endl;
    int fs_op = -1;
    while(fs_op == -1){
	    cout << "Escolha qual exponencial deseja usar: ";
	    cin >> fs_op;
	    if(fs_op < 1 || fs_op > 2){
		    fs_op = -1;
		    cout << "Escolha uma opcao valida." << endl;
	    }
    }
    cout << "Entre com o intervalo finito c" << endl;
    cout << "c = "; cin >> c;
    cout << "1 - Filosofia Fechada" << endl;
    cout << "2 - Filosofia Aberta" << endl;
    int op_filosofia = -1;
    while(op_filosofia == -1){
	    cout << "Escolha qual filosofia de Newton-Coutes deseja usar: ";
	    cin >> op_filosofia;
	    if(op_filosofia < 1 || op_filosofia > 2){
		    op_filosofia = -1;
		    cout << "Escolha uma opcao valida." << endl;
	    }
    }
    int n;
    cout << "Entre o numero de intervalos que deseja particionar: ";
    cin >> n;
    cout << "-----------------------------------------------------------------------------\n";

    cout << "Resultado sem a condicao de tolerancia: " << particionado(-c, c, a, b, n, op_filosofia, f_op, fs_op) << endl;

    n = 1;
    double erro, int_atual, int_antiga = particionado(-c, c, a, b, n, op_filosofia, f_op, fs_op);
    int cont = 0;
    do {
        n *= 2;
        int_atual = particionado(-c, c, a, b, n, op_filosofia, f_op, fs_op);
        erro = abs((int_atual - int_antiga) / int_atual);
        int_antiga = int_atual;
        ++cont;
    } while(erro > 0.000001);

    cout << "Tolerancia usada: 10^-6" << endl;
    cout << "Resultado com a condicao de tolerancia: " << int_atual << endl;
    cout << "Quantidade de iteracoes: " << cont << endl;

    return 0;
}