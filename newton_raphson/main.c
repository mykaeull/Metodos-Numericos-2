// Disciplina: Métodos Computacionais
// Professor: Adilson da Silva
// Tutor: Alfredo João dos Santos Neto
// Aluno: Ricardo Sérgio Duarte 
// Matrícula: 01308230
// Data: 05/08/2022

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct tuple Tuple;

struct tuple
{
	int k;
	double x;
};

double funcao(double x)
{
	return pow(x, 4) + 12.6 * pow(x, 3) - 155.7 * pow(x, 2) - 678.6 * x + 1863;
}

double derivada(double x)
{
	return 4 * pow(x, 3) + 37.8 * pow(x, 2) - 311.4 * x - 678.6;
}

double intervalo_escolhido(int op)
{
	if (op == 1)
	{
		return -17.;
	}
	if (op == 2)
	{
		return -4.;
	}
	if (op == 3)
	{
		return 3.;
	}
	return 10.;
}

Tuple newton_raphson(double x0, double epsilon)
{
	double x1;
	double fx;
	double derx;
	double k = 0;

	fx = funcao(x0);
	derx = derivada(x0);

	x1 = x0 - (fx / derx);

	while (fabs(funcao(x1)) > epsilon || fabs(x0 - x1) > epsilon)
	{
		k += 1;
		x0 = x1;

		fx = funcao(x0);
		derx = derivada(x0);

		x1 = x0 - (fx / derx);
	}

	Tuple t = {k, x1};

	return t;
}

int main()
{
	double x0;
	double epsilon = 0.0001;
	int op = -1;
	Tuple t;

	printf("~Newton-Raphson~\n");
	printf("Funcao utilizada: 4x^4 + 12.6x^3 - 155.7x^2 - 678.6x + 1863\n\n");

	printf("1 - A[-19, -17]\n");
	printf("2 - B[-6, -4]\n");
	printf("3 - C[0, 3]\n");
	printf("4 - D[8, 10]\n");
	while (op == -1)
	{
		printf("Escolha a opcao de intervalo que deseja usar: ");
		scanf("%d", &op);
		if (op < 1 || op > 4)
		{
			op = -1;
			printf("Opcao invalida.");
		}
	}

	x0 = intervalo_escolhido(op);

	t = newton_raphson(x0, epsilon);

	printf("------------------------------------------------\n");
	printf("raiz: %.4f\n", t.x);
	printf("iteracao: %d", t.k);

	return 0;
}