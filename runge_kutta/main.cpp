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
    pair<double, double> S_barra_zero;
    pair<double, double> S_barra_meio;
    pair<double, double> S_barra_um;

    S_barra_zero = F_S_t(S.first);                                                        // F1
    S_barra_meio = F_S_t(S.first + (delta / 2.) * S_barra_zero.first);                    // F2
    S_barra_um = F_S_t(S.first + delta * (-S_barra_zero.first + 2 * S_barra_meio.first)); // F3

    double vi = S.first + delta * ((1 / 6. * S_barra_zero.first) + (4 / 6. * S_barra_meio.first) + (1 / 6. * S_barra_um.first));
    double yi = S.second + delta * ((1 / 6. * S_barra_zero.second) + (4 / 6. * S_barra_meio.second) + (1 / 6. * S_barra_um.second));

    return {(vi), (yi)};
}

void printar_linha(pair<double, double> S, double delta)
{
    pair<double, double> Si; // Estado(v1, y1)
    double v, y_max = 0., t = 0., t_max;
    double y;

    Si = metodo_runge_kutta(S, delta);
    y = Si.second;
    t += delta;
    y_max = Si.second;

    while (y > 0)
    {
        Si = metodo_runge_kutta(Si, delta);
        t += delta;
        if (Si.second > y_max)
        {
            y_max = Si.second;
            t_max = t;
        }
        y = Si.second;
        v = Si.first;
    }

    cout << "delta_t(s) = " << delta << "\n";
    cout << "altura_max(m) = " << y_max << "\n";
    cout << "tempo_altura_max(s) = " << t_max << "\n";
    cout << "tempo_queda_mar(s) = " << t << "\n";
    cout << "velocidade_impacto_mar(m/s) = " << v << "\n"; // -1 * v
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
