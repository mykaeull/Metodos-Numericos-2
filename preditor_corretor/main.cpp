#include <bits/stdc++.h>

using namespace std;

double k = 0.9, m = 2., g = 10.;
// double delta = 0.1;

pair<double, double> F_S_t(double v)
{
    return {(-g - ((k / m) * v)), v};
}

pair<double, double> metodo_runge_kutta(pair<double, double> S, double delta)
{
    pair<double, double> S_um;
    pair<double, double> S_dois;
    pair<double, double> S_tres;
    pair<double, double> S_quatro;

    S_um = F_S_t(S.first);                                 // F1
    S_dois = F_S_t(S.first + (delta / 2.) * S_um.first);   // F2
    S_tres = F_S_t(S.first + (delta / 2.) * S_dois.first); // F3
    S_quatro = F_S_t(S.first + delta * S_tres.first);      // F4

    double vi = S.first + delta * ((1 / 6. * S_um.first) + (2 / 6. * S_dois.first) + (2 / 6. * S_tres.first) + (1 / 6 * S_quatro.first));
    double yi = S.second + delta * ((1 / 6. * S_um.second) + (2 / 6. * S_dois.second) + (2 / 6. * S_tres.second) + (1 / 6 * S_quatro.second));

    return {(vi), (yi)};
}

vector<pair<double, double>> inicializacao(pair<double, double> S, double delta)
{ // É preciso 4 passos
    vector<pair<double, double>> estados(4);

    estados[0] = S;

    for (int i = 1; i < 4; i++)
    {
        estados[i] = metodo_runge_kutta(estados[i - 1], delta);
    }

    return estados;
}

pair<double, double> metodo_predicao_correcao(vector<pair<double, double>> estados_passados, double delta)
{
    pair<double, double> S_um;
    pair<double, double> S_dois;
    pair<double, double> S_tres;
    pair<double, double> S_quatro;
    pair<double, double> S_pred;
    double v_predicao, y_predicao;
    double v_correcao, y_correcao;

    S_um = F_S_t(estados_passados[0].first);
    S_dois = F_S_t(estados_passados[1].first);
    S_tres = F_S_t(estados_passados[2].first);
    S_quatro = F_S_t(estados_passados[3].first);

    v_predicao = estados_passados[3].first + (delta / 24.) * (-9 * S_um.first + 33 * S_dois.first - 59 * S_tres.first + 55 * S_quatro.first);
    y_predicao = estados_passados[3].second + (delta / 24.) * (-9 * S_um.second + 33 * S_dois.second - 59 * S_tres.second + 55 * S_quatro.second);

    S_pred = F_S_t(v_predicao);

    v_correcao = estados_passados[3].first + (delta / 24.) * (S_dois.first - 5 * S_tres.first + 19 * S_quatro.first + 9 * S_pred.first);
    y_correcao = estados_passados[3].second + (delta / 24.) * (S_dois.second - 5 * S_tres.second + 19 * S_quatro.second + 9 * S_pred.second);

    return {(v_correcao), (y_correcao)};
}

void printar_linha(pair<double, double> S, double delta)
{
    pair<double, double> Si; // Estado(v1, y1)
    double v, y_max = 0., t = 0., t_max;
    double y;
    vector<pair<double, double>> estados_passados;
    vector<pair<double, double>> aux(4);

    estados_passados = inicializacao(S, delta);

    Si = metodo_predicao_correcao(estados_passados, delta);
    for (int i = 0; i < 3; i++)
    {
        aux[i] = estados_passados[i + 1];
    }
    aux[3] = Si;
    y = Si.second;
    t += delta;
    y_max = Si.second;

    while (y > 0)
    {
        Si = metodo_predicao_correcao(estados_passados, delta);
        t += delta;
        if (Si.second > y_max)
        {
            y_max = Si.second;
            t_max = t;
        }
        for (int i = 0; i < 3; i++)
        {
            aux[i] = estados_passados[i + 1];
        }
        aux[3] = Si;
        estados_passados = aux;
        y = Si.second;
        v = Si.first;
    }

    cout << "delta_t(s) = " << delta << "\n";
    cout << "altura_max(m) = " << y_max << "\n";
    cout << "tempo_altura_max(s) = " << t_max << "\n";
    cout << "tempo_queda_mar(s) = " << t << "\n";
    cout << "velocidade_impacto_mar(m/s) = " << v << "\n";
}

int main()
{
    cout << fixed << setprecision(6);
    pair<double, double> S; // Estado(v0, y0)
    S.first = 5.;           // v0
    S.second = 200.;        // y0

    printar_linha(S, 0.1);
    cout << "------------------------------------------\n";
    printar_linha(S, 0.01);
    cout << "------------------------------------------\n";
    printar_linha(S, 0.001);
    cout << "------------------------------------------\n";
    printar_linha(S, 0.0001);

    return 0;
}
