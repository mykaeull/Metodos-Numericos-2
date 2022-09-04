#include <bits/stdc++.h>

using namespace std;

vector<vector<double>> A1 = {{5., 2., 1.},
                             {2., 3., 1.},
                             {1., 1., 2.}};

vector<vector<double>> A2 = {{40., 8., 4., 2., 1.},
                             {8., 30., 12., 6., 2.},
                             {4., 12., 20., 1., 2.},
                             {2., 6., 1., 25., 4.},
                             {1., 2., 2., 4., 5.}};

vector<vector<double>> A3 = {{2 / 3., 1 / 3., 1 / 3.},
                             {1 / 3., 4 / 3., 1 / 3.},
                             {1 / 3., 1 / 3., 2 / 3.}};

double vnew1[3] = {1., 1., 1.}, vold1[3] = {0., 0., 0.};
double vnew2[5] = {1., 1., 1., 1., 1.}, vold2[5] = {0., 0., 0., 0., 0.};

double norm_of_vector(double *v, int n)
{
    double comp = 0.; // comprimento do vetor

    for (int i = 0; i < n; i++)
    {
        comp += v[i] * v[i];
    }

    return sqrt(comp);
}

void unit_vector(double *v, int n)
{
    double sum;

    sum = norm_of_vector(v, n);
    for (int i = 0; i < n; i++)
    {
        v[i] /= sum;
    }
}

double dot_product(double *v1, double *v2, int n)
{
    double sum = 0.;
    for (int i = 0; i < n; i++)
    {
        sum += v1[i] * v2[i];
    }
    return sum;
}

void regular_power_method(vector<vector<double>> A, double *vnew, double *vold, int n)
{
    double sum;
    double lamb_new = 1., lamb_old = 0., eps = 0.000001;
    int step = 0;

    while (abs((lamb_new - lamb_old) / lamb_new) > eps)
    {
        step++;
        lamb_old = lamb_new;
        unit_vector(vnew, n);

        for (int i = 0; i < n; i++)
        {
            vold[i] = vnew[i];
        }

        cout << "{";

        for (int i = 0; i < n; i++)
        {
            i < (n - 1) ? cout << vold[i] << ", " : cout << vold[i];
        }
        cout << "}\n";

        for (int i = 0; i < n; i++)
        {
            sum = 0.;
            for (int j = 0; j < n; j++)
            {
                sum += A[i][j] * vold[j];
            }
            vnew[i] = sum;
        }

        lamb_new = dot_product(vold, vnew, n);
        cout << "lambda" << step << " = " << lamb_new << "\n";
    }
}

int main()
{
    int op = -1;
    int n;
    cout << "~METODO DA POTENCIA REGULAR~\n";
    cout << "1 - |5, 2, 1|\n";
    cout << "    |2, 3, 1|\n";
    cout << "    |1, 1, 2|\n";
    cout << endl;
    cout << "2 - |40, 8, 4, 2, 1 |\n";
    cout << "    |8, 30, 12, 6, 2|\n";
    cout << "    |4, 12, 20, 1, 2|\n";
    cout << "    |2, 6, 1, 25, 4 |\n";
    cout << "    |1, 2, 2, 4, 5  |\n";
    cout << endl;
    cout << "3 - |2/3, 1/3, 1/3|\n";
    cout << "    |1/3, 4/3, 1/3|\n";
    cout << "    |1/3, 1/3, 2/3|\n";
    cout << endl;
    while (op == -1)
    {
        cout << "Escolha a matriz que deseja encontrar o autovalor e o autovetor: ";
        cin >> op;
        if (op < 1 || op > 3)
        {
            op = -1;
            cout << "Digite uma opcao valida!\n";
        }
    }
    cout << "----------------------------------------------------------------------\n";

    if (op == 1)
    {
        n = 3;
        regular_power_method(A1, vnew1, vold1, n);
    }
    else if (op == 2)
    {
        n = 5;
        regular_power_method(A2, vnew2, vold2, n);
    }
    else if (op == 3)
    {
        n = 3;
        regular_power_method(A3, vnew1, vold1, n);
    }

    return 0;
}