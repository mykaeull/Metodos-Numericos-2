#include <bits/stdc++.h>

using namespace std;

double f(double x)
{
    return 10 - (2. * sin(8 * x) * exp(-1 * powf(x, 2.5)));
    double func = (sin(2 * x) + 4 * x * x + 3 * x);
    return func * func;
}

double two_point(double xi, double xf)
{
    double sum = 0.0;
    double alfa_1 = -(sqrt(1.0 / 3.0));
    double alfa_2 = (sqrt(1.0 / 3.0));
    double x_alfa_1 = ((xi + xf) / 2.) + (((xf - xi) / 2.) * alfa_1);
    double x_alfa_2 = (((xi + xf) / 2.) + ((xf - xi) / 2.) * alfa_2);
    double x_alfa[2] = {x_alfa_1, x_alfa_2};
    double w1 = 1;
    double w2 = 1;
    double w[2] = {w1, w2};
    for (int i = 0; i < 2; i++)
    {
        sum += f(x_alfa[i]) * w[i];
    }

    return ((xf - xi) / 2.) * sum;
}

double three_point(double xi, double xf)
{
    double sum = 0.0;
    double alfa_1 = -(sqrt(3.0 / 5.0));
    double alfa_2 = 0.0;
    double alfa_3 = (sqrt(3.0 / 5.0));
    double x_alfa_1 = ((xi + xf) / 2.) + (((xf - xi) / 2.) * alfa_1);
    double x_alfa_2 = (((xi + xf) / 2.) + ((xf - xi) / 2.) * alfa_2);
    double x_alfa_3 = (((xi + xf) / 2.) + ((xf - xi) / 2.) * alfa_3);
    double x_alfa[3] = {x_alfa_1, x_alfa_2, x_alfa_3};
    double w1 = 5.0 / 9.0;
    double w2 = 8.0 / 9.0;
    double w3 = w1;
    double w[3] = {w1, w2, w3};
    for (int i = 0; i < 3; i++)
    {
        sum += f(x_alfa[i]) * w[i];
    }

    return ((xf - xi) / 2) * sum;
}

double four_point(double xi, double xf)
{
    double sum = 0.0;
    double alfa_1 = -0.8611;
    double alfa_2 = -0.3399;
    double alfa_3 = 0.3399;
    double alfa_4 = 0.8611;
    double x_alfa_1 = ((xi + xf) / 2.) + (((xf - xi) / 2.) * alfa_1);
    double x_alfa_2 = (((xi + xf) / 2.) + ((xf - xi) / 2.) * alfa_2);
    double x_alfa_3 = (((xi + xf) / 2.) + ((xf - xi) / 2.) * alfa_3);
    double x_alfa_4 = (((xi + xf) / 2.) + ((xf - xi) / 2.) * alfa_4);
    double x_alfa[4] = {x_alfa_1, x_alfa_2, x_alfa_3, x_alfa_4};
    double w1 = 0.3478;
    double w2 = 0.6521;
    double w3 = w2;
    double w4 = w1;
    double w[4] = {w1, w2, w3, w4};
    for (int i = 0; i < 4; i++)
    {
        sum += f(x_alfa[i]) * w[i];
    }

    return ((xf - xi) / 2) * sum;
}

double particionado(double xi, double xf, int n, int op)
{
    double dx = (xf - xi) / n, sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        double a = xi + i * dx;
        double b = a + dx;
        if (op == 1)
        {
            sum += two_point(a, b);
        }
        else if (op == 2)
        {
            sum += three_point(a, b);
        }
        else if (op == 3)
        {
            sum += four_point(a, b);
        }
    }

    return sum;
}

int main()
{
    cout << fixed << setprecision(6);
    double xi, xf;
    int op = -1;
    int n;
    cout << "~Gauss Legendre~" << endl;
    cout << "Funcao integrada: (sen(2x) + 4x^2 + 3x)^2" << endl;
    cout << "Tolerancia: 10^-6" << endl;
    cout << "Entre com o intervalo de integracao xi e xf" << endl;
    // cout << "xi = ";
    // cin >> xi;
    // cout << "xf = ";
    // cin >> xf;
    xi = 0;
    xf = M_PI;
    cout << "1 - 2 pontos" << endl;
    cout << "2 - 3 pontos" << endl;
    cout << "3 - 4 pontos" << endl;
    while (op == -1)
    {
        cout << "Escolha uma opcao: ";
        cin >> op;
        if (op < 1 || op > 3)
        {
            op = -1;
            cout << "Escolha uma opcao valida." << endl;
        }
    }
    cout << "Entre o numero de intervalos que deseja particionar: ";
    cin >> n;

    cout << "Resultado sem a condicao de tolerancia: " << particionado(xi, xf, n, op) << endl;

    n = 1;
    double erro, int_atual, int_antiga = particionado(xi, xf, n, op);
    int cont = 0;
    do
    {
        n *= 2;
        int_atual = particionado(xi, xf, n, op);
        erro = abs((int_atual - int_antiga) / int_atual);
        int_antiga = int_atual;
        ++cont;
    } while (erro > 0.00000001);

    cout << "Resultado com a condicao de tolerancia: " << int_atual << endl;
    cout << "Quantidade de iteracoes: " << cont << endl;

    return 0;
}