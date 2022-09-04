#include <bits/stdc++.h>

using namespace std;

double f(double x)
{
    return 10 - (2. * sin(8 * x) * exp(-1 * powf(x, 2.5)));
    double func = (sin(2 * x) + 4 * x * x + 3 * x);
    return func * func;
}

double fechada(double xi, double xf)
{
    double h = (xf - xi) / 4.;
    vector<double> s = {7.0, 32.0, 12.0, 32.0, 7.0};
    double sum = 0.0;
    for (int i = 0; i < (int)s.size(); i++)
    {
        sum += f(xi + i * h) * s[i];
    }
    return (2 * h * sum) / 45.;
}

double aberta(double xi, double xf)
{
    double h = (xf - xi) / 6.;
    vector<double> s = {11.0, -14.0, 26.0, -14.0, 11.0};
    double sum = 0.0;
    for (int i = 0; i < (int)s.size(); i++)
    {
        sum += f((xi + h) + i * h) * s[i];
    }
    return (3 * h * sum) / 10.;
}

double particionado(double xi, double xf, int n, int filosofia)
{
    double dx = (xf - xi) / n, sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        double a = xi + i * dx;
        double b = a + dx;
        sum += filosofia == 1 ? fechada(a, b) : aberta(a, b);
    }
    return sum;
}

int main()
{
    cout << fixed << setprecision(6);
    double xi, xf;
    cout << "Polinomio de substituicao de grau 4" << endl;
    cout << "Funcao integrada: (sen(2x) + 4x^2 + 3x)^2" << endl;
    cout << "Tolerancia: 10^-6" << endl;
    cout << "Entre com o intervalo de integracao xi e xf" << endl;
    // cout << "xi = ";
    // cin >> xi;
    xi = 0;
    // cout << "xf = ";
    // cin >> xf;
    xf = M_PI;
    cout << "1 - Filosofia Fechada" << endl;
    cout << "2 - Filosofia Aberta" << endl;
    int op_filosofia = -1;
    while (op_filosofia == -1)
    {
        cout << "Escolha qual filosofia deseja usar: ";
        cin >> op_filosofia;
        if (op_filosofia < 1 || op_filosofia > 2)
        {
            op_filosofia = -1;
            cout << "Escolha uma opção válida." << endl;
        }
    }
    int n;
    cout << "Entre o numero de intervalos que deseja particionar:";
    cin >> n;

    cout << "Resultado sem a condicao de tolerancia: " << particionado(xi, xf, n, op_filosofia) << endl;

    n = 10;
    double erro, int_atual, int_antiga = particionado(xi, xf, n, op_filosofia);
    int cont = 0;
    do
    {
        n *= 2;
        int_atual = particionado(xi, xf, n, op_filosofia);
        erro = abs((int_atual - int_antiga) / int_atual);
        int_antiga = int_atual;
        ++cont;
    } while (erro > 0.000000001);

    cout << "Resultado com a condicao de tolerancia: " << int_atual << endl;
    cout << "Quantidade de iteracoes: " << cont << endl;

    return 0;
}