#include <bits/stdc++.h>

using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;

void mover_linha(Matrix &a, Vector &b, int i, int j)
{
    int n = a[0].size();
    for (int k = 0; k < n; k++)
        swap(a[i][k], a[j][k]);
    swap(b[i], b[j]);
}

void eliminacao_gauss(Matrix &a, Vector &b)
{
    int m = a.size(), n = a[0].size();
    for (int k = 0, id = -1; k < m - 1; k++)
    {
        double bigger = 0.0;
        for (int i = k; i < m; i++)
        {
            if (abs(a[i][k]) > bigger)
            {
                bigger = abs(a[i][k]);
                id = i;
            }
        }
        mover_linha(a, b, id, k);
        for (int i = k + 1; i < m; i++)
        {
            double c = a[i][k] / a[k][k];
            for (int j = k + 1; j < n; j++)
            {
                a[i][j] = a[i][j] - a[k][j] * c;
            }
            b[i] = b[i] - b[k] * c;
        }
    }
}

Vector triangular_superior(Matrix &a, Vector &b)
{
    int m = a.size(), n = a[0].size();
    Vector x;
    x.assign(n, -1);
    x[n - 1] = b[n - 1] / a[n - 1][n - 1];
    for (int i = m - 2; i >= 0; i--)
    {
        double s = 0;
        for (int j = i + 1; j < n; j++)
            s += a[i][j] * x[j];
        x[i] = (b[i] - s) / a[i][i];
    }
    return x;
}

Vector solve_eliminacao_gauss(Matrix &a, Vector &b)
{
    eliminacao_gauss(a, b);
    return triangular_superior(a, b);
}

pair<Matrix, Vector> gerar_sistema_linear_r1(Vector contorno, Vector mascara)
{
    Matrix matriz_coeficientes;
    Vector termos_independentes;
    for (int i = 1, ii = 0; i < contorno.size() - 1; i++, ii++)
    {
        Vector equacao(contorno.size() - 2);
        bool tem_termo_independente = false;
        if (i == contorno.size() - 2)
        {
            tem_termo_independente = true;
            termos_independentes.push_back(-1 * mascara.back() * contorno.back());
        }
        for (int k = 0, j = ii; k < mascara.size(); k++, j++)
            if (j > 0 && j < contorno.size() - 1)
                equacao[j - 1] = mascara[k];
        if (i == 1)
        {
            tem_termo_independente = true;
            termos_independentes.push_back(-1 * mascara[0] * contorno[0]);
        }
        if (!tem_termo_independente)
            termos_independentes.push_back(0.0);
        matriz_coeficientes.push_back(equacao);
    }
    return {matriz_coeficientes, termos_independentes};
}

Vector pvc1_aproximado(int n)
{
    Vector contorno = {0};
    for (int i = 0; i < n; i++)
        contorno.push_back(0.0);
    contorno.push_back(1);
    Vector mascara = {((n + 1.0) * (n + 1)), (-(2.0 * (n + 1) * (n + 1) + 1)), ((n + 1.0) * (n + 1))};

    Matrix M;
    Vector b;
    tie(M, b) = gerar_sistema_linear_r1(contorno, mascara);
    return solve_eliminacao_gauss(M, b);
}

double funcao_exata(double x)
{
    return (exp(-x) - exp(x)) / (exp(-1) - exp(1));
}

Vector pvc1_exato(int n)
{
    double inicio = 0.0, fim = 1.0;
    double step = (fim - inicio) / (n + 1);
    double pos_atual = step;

    Vector valores_exatos;
    for (int i = 0; i < n; i++, pos_atual += step)
        valores_exatos.push_back(funcao_exata(pos_atual));
    return valores_exatos;
}

int dx[5] = {0, -1, 0, 1, 0};
int dy[5] = {-1, 0, 1, 0, 0};

bool in(int i, int j, int n, int m)
{
    return (i >= 0 && i < n && j >= 0 && j < m);
}

double contorno_valor(int i, int j, int n, int m, array<double, 4> contorno)
{
    if (j == -1)
        return contorno[0];
    if (i == -1)
        return contorno[1];
    if (j == m)
        return contorno[2];
    return contorno[3];
}

pair<Matrix, Vector> gerar_sistema_linear_r2(Matrix indices, Vector mascara, array<double, 4> contorno)
{
    int n = indices.size(), m = indices[0].size();
    Matrix matriz_coeficientes;
    Vector termos_independentes(n * m, 4.0);
    for (int i = n - 1, c = 0; i >= 0; i--)
    {
        for (int j = 0; j < m; j++, c++)
        {
            Vector equacao(n * m, 0.0);
            for (int k = 0; k < 5; k++)
            {
                int ni = i + dx[k], nj = j + dy[k];
                if (in(ni, nj, n, m))
                {
                    equacao[(int)indices[ni][nj]] = mascara[k];
                }
                else
                {
                }
            }
            matriz_coeficientes.push_back(equacao);
        }
    }
    return {matriz_coeficientes, termos_independentes};
}

Vector pvc2_aproximado(int n, int m)
{
    Matrix indices(n, Vector(m));
    for (int i = n - 1, c = 0; i >= 0; i--)
        for (int j = 0; j < m; j++)
            indices[i][j] = c++;

    Vector mascara = {(n + 1.0) * (n + 1), (m + 1.0) * (m + 1), (n + 1.0) * (n + 1), (m + 1.0) * (m + 1), -2.0 * (((n + 1) * (n + 1)) + ((m + 1) * (m + 1)))};

    array<double, 4> contorno = {0.0, 0.0, 0.0, 0.0};

    Matrix M;
    Vector b;
    tie(M, b) = gerar_sistema_linear_r2(indices, mascara, contorno);
    return solve_eliminacao_gauss(M, b);
}

double funcao_exata2(double x)
{
    return (exp(-x) - exp(x)) / (exp(-1) - exp(1));
}

Vector pvc2_exato(int n)
{
    double inicio = 0.0, fim = 1.0;
    double step = (fim - inicio) / (n + 1);
    double pos_atual = step;

    Vector valores_exatos;
    for (int i = 0; i < n; i++, pos_atual += step)
        valores_exatos.push_back(funcao_exata2(pos_atual));
    return valores_exatos;
}

void printar_pvc(vector<double> sol_aproximada, vector<double> sol_exata)
{
    for (int i = 0; i < sol_aproximada.size(); i++)
    {
        cout << i + 1 << " - solucao aproximada: " << sol_aproximada[i] << " | "
             << "solucao exata: " << sol_exata[i] << " | "
             << "erro: " << 100 * abs(((sol_exata[i] - sol_aproximada[i]) / sol_exata[i])) << "%\n";
    }
}

int main()
{
    cout << fixed << setprecision(6);
    int n = 3;

    cout << "PVC1\n";
    vector<double> sol1_aproximada = pvc1_aproximado(n);
    vector<double> sol1_exata = pvc1_exato(n);
    printar_pvc(sol1_aproximada, sol1_exata);

    // cout << "----------------------------------------------------------------------------\n";

    // cout << "PVC2\n";
    // vector<double> sol2_aproximada = pvc2_aproximado(n, n);
    // printar_pvc(sol2_aproximada, sol2_aproximada);

    return 0;
}
