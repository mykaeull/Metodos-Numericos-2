#include <bits/stdc++.h>

using namespace std;

vector<vector<double>> A1 = {{5., 2., 1.},
                             {2., 3., 1.},
                             {1., 1., 2.}};

vector<vector<double>> A2 = {{-40., 8., 4., 2., 1.},
                             {8., -30., 12., 6., 2.},
                             {4., 12., 20., 1., 2.},
                             {2., 6., 1., 25., 4.},
                             {1., 2., 2., 4., 5.}};

vector<vector<double>> A3 = {{-14., 1., -2.},
                             {1., -1., 1.},
                             {-2., 1., -11.}};

vector<double> vnew1 = {1., 1., 1.}, vold1 = {0., 0., 0.};
vector<double> vnew2 = {1., 1., 1., 1., 1.}, vold2 = {0., 0., 0., 0., 0.};

double norm_of_vector(vector<double> v, int n)
{
    double comp = 0.; // comprimento do vetor

    for (int i = 0; i < n; i++)
    {
        comp += v[i] * v[i];
    }

    return sqrt(comp);
}

void norm_of_vector_2(vector<double> &v, int n)
{
    double m = v[n - 1];

    for (int i = 0; i < n; i++)
    {
        v[i] = v[i] / m;
    }
}

void unit_vector(vector<double> &v, int n)
{
    double sum;

    sum = norm_of_vector(v, n);
    for (int i = 0; i < n; i++)
    {
        v[i] /= sum;
    }
}

double dot_product(vector<double> v1, vector<double> v2, int n)
{ // produto escalar
    double sum = 0.;
    for (int i = 0; i < n; i++)
    {
        sum += v1[i] * v2[i];
    }
    return sum;
}

vector<double> successive_substitutions(vector<vector<double>> A, vector<double> v, int n)
{
    vector<double> x(n);
    double sum;

    for (int i = 0; i < n; i++)
    {
        sum = 0.;
        for (int j = 0; j < i; j++)
        {
            sum = sum + (A[i][j] * x[j]);
        }
        x[i] = (v[i] - sum) / A[i][i];
    }

    return x;
}

vector<double> retroactive_substitutions(vector<vector<double>> A, vector<double> v, int n)
{
    vector<double> x(n);
    double sum;

    for (int i = n - 1; i > -1; i--)
    {
        sum = 0.;
        for (int j = i + 1; j < n; j++)
        {
            sum = sum + (A[i][j] * x[j]);
        }
        x[i] = (v[i] - sum) / A[i][i];
    }

    return x;
}

vector<vector<double>> identity_matrix(int n)
{
    vector<vector<double>> M(n, vector<double>(n));

    for (int i = 0; i < n; i++)
    {
        M[i][i] = 1;
    }

    return M;
}

void LUdecomposition(vector<vector<double>> A, vector<vector<double>> &L, vector<vector<double>> &U, int n)
{
    double m;

    L = identity_matrix(n);
    U = A;

    for (int k = 0; k < n - 1; k++)
    {
        for (int i = k + 1; i < n; i++)
        {
            m = -U[i][k] / U[k][k];
            L[i][k] = -m;
            for (int j = k + 1; j < n; j++)
            {
                U[i][j] = m * U[k][j] + U[i][j];
            }
            U[i][k] = 0;
        }
    }
}

vector<double> solver_LU(vector<vector<double>> L, vector<vector<double>> U, vector<double> v, int n)
{
    vector<double> y, x;

    y = successive_substitutions(L, v, n);
    x = retroactive_substitutions(U, y, n);

    return x;
}

tuple<double, vector<double>, int> inverse_power_method(vector<vector<double>> A, vector<double> vnew, vector<double> vold, int n)
{
    double sum;
    double lamb_new = 1., lamb_old = 0., eps = 0.0000001;
    int step = 0;
    vector<vector<double>> L(n, vector<double>(n));
    vector<vector<double>> U(n, vector<double>(n));

    LUdecomposition(A, L, U, n);

    while (abs((lamb_new - lamb_old) / lamb_new) > eps)
    {
        step++;
        lamb_old = lamb_new;
        unit_vector(vnew, n);

        vold = vnew;

        vnew = solver_LU(L, U, vold, n);

        lamb_new = dot_product(vold, vnew, n);
        // cout << "lambda" << step << " = " << lamb_new << "\n";
    }

    return {1 / lamb_new, vnew, step};
}

vector<vector<double>> subtract_A_mi(vector<vector<double>> A, double mi, int n)
{
    vector<vector<double>> I;

    I = identity_matrix(n);

    for (int i = 0; i < n; i++)
    {
        I[i][i] = mi * I[i][i];
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = A[i][j] - I[i][j];
        }
    }

    return A;
}

tuple<double, vector<double>, int> power_with_displacement_method(vector<vector<double>> A, vector<double> vnew, vector<double> vold, double mi, int n)
{
    int step;
    double lambda;
    vector<double> v;

    A = subtract_A_mi(A, mi, n);
    tie(lambda, v, step) = inverse_power_method(A, vnew, vold, n);

    return {lambda + mi, v, step};
}

int main()
{
    cout << fixed << setprecision(6);
    int op = -1;
    int n, step;
    double lambda, mi;
    vector<double> v;
    // cout << "~METODO DA POTENCIA REGULAR~\n";
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
    cout << "3 - |-14, 1, -2|\n";
    cout << "    |1, -1, 1  |\n";
    cout << "    |-2, 1, 11 |\n";
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
    cout << "Entre com o valor de mi: ";
    cin >> mi;
    cout << "----------------------------------------------------------------------\n";

    if (op == 1)
    {
        n = 3;
        tie(lambda, v, step) = inverse_power_method(A1, vnew1, vold1, n);
        cout << "~METODO DA POTENCIA INVERSO~\n";
        cout << "{";
        for (int i = 0; i < n; i++)
        {
            i < (n - 1) ? cout << v[i] << ", " : cout << v[i];
        }
        cout << "}\n";
        cout << "lambda = " << lambda << "\n";
        cout << "quantidade de passos = " << step << "\n";
        cout << "----------------------------------------------------------------------\n";
        tie(lambda, v, step) = power_with_displacement_method(A1, vnew1, vold1, mi, n);
        cout << "~METODO DA POTENCIA COM DESLOCAMENTO~\n";
        cout << "{";
        for (int i = 0; i < n; i++)
        {
            i < (n - 1) ? cout << v[i] << ", " : cout << v[i];
        }
        cout << "}\n";
        cout << "lambda = " << lambda << "\n";
        cout << "quantidade de passos = " << step << "\n";
    }
    else if (op == 2)
    {
        n = 5;
        tie(lambda, v, step) = inverse_power_method(A2, vnew2, vold2, n);
        cout << "~METODO DA POTENCIA INVERSO~\n";
        cout << "{";
        for (int i = 0; i < n; i++)
        {
            i < (n - 1) ? cout << v[i] << ", " : cout << v[i];
        }
        cout << "}\n";
        cout << "lambda = " << lambda << "\n";
        cout << "quantidade de passos = " << step << "\n";
        cout << "----------------------------------------------------------------------\n";
        tie(lambda, v, step) = power_with_displacement_method(A2, vnew2, vold2, mi, n);
        norm_of_vector_2(v, n);
        cout << "~METODO DA POTENCIA COM DESLOCAMENTO~\n";
        cout << "{";
        for (int i = 0; i < n; i++)
        {
            i < (n - 1) ? cout << v[i] << ", " : cout << v[i];
        }
        cout << "}\n";
        cout << "lambda = " << lambda << "\n";
        cout << "quantidade de passos = " << step << "\n";
    }
    else if (op == 3)
    {
        n = 3;
        tie(lambda, v, step) = inverse_power_method(A3, vnew1, vold1, n);
        cout << "~METODO DA POTENCIA INVERSO~\n";
        cout << "{";
        for (int i = 0; i < n; i++)
        {
            i < (n - 1) ? cout << v[i] << ", " : cout << v[i];
        }
        cout << "}\n";
        cout << "lambda = " << lambda << "\n";
        cout << "quantidade de passos = " << step << "\n";
        cout << "----------------------------------------------------------------------\n";
        tie(lambda, v, step) = power_with_displacement_method(A3, vnew1, vold1, mi, n);
        cout << "~METODO DA POTENCIA COM DESLOCAMENTO~\n";
        cout << "{";
        for (int i = 0; i < n; i++)
        {
            i < (n - 1) ? cout << v[i] << ", " : cout << v[i];
        }
        cout << "}\n";
        cout << "lambda = " << lambda << "\n";
        cout << "quantidade de passos = " << step << "\n";
    }

    return 0;
}