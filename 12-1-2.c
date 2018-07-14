#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Nx 4
#define Ny 4
#define h 1./3.
#define E 1e-10

void contorno(double **M)
{	
	double x=1, xmax=2, y=0, ymax=1;
	int i, j;
	
	for(i=0; i<Nx; i++)
	{	
		M[i][0] = 2*log(x);	
		M[i][Ny-1] = log(pow(x, 2) + 1);
		x += h;
	}

	for(i=0; i<Ny; i++)
	{	
		M[0][i] = log(pow(y, 2) + 1);	
		M[Nx-1][i] = log(pow(y, 2) + 4);
		y += h;
	}
}

double f()
{	
	return (0);
}

double u(double **M, double **Maux)
{	
	int i, j;
	double eps;
	double a=0;

	for(i=1; i<Nx-1; i++)
	{	
		for(j=1; j<Ny; j++)
		{	
			if(Maux[i][j] == 0)
			{	
				eps = M[i][j];
				M[i][j] = (M[i-1][j] + M[i+1][j] + M[i][j-1] + M[i][j+1] - pow(h, 2)*f())/ 4.;
				eps = fabs(eps - M[i][j]);
				
				if(eps > a)
				{
					a = eps;
				}
			}
		}
	}
	return (a);	
}

void imprime(double **M, int NL, int NC)
{
	int i, j;
	
	FILE *m;
	FILE *dados;

	m = fopen("matrizes2.dat", "a");
	dados = fopen("dados2.dat", "w");
	
	fprintf(m, "Matriz calculada\n");	
	for(i=0; i<NL; i++)
	{
		for(j=0; j<NC; j++)
		{
			fprintf(m, "%.2lf\t ", M[i][j]); 
			fprintf(dados, "%.2lf\t ", M[i][j]); 
		} 
		fprintf(m, "\n");
		fprintf(dados, "\n");
	}	
}

void solucao()
{	
	int i, j;
	double x=1, xmax=2, ymax=2;
	double y;
	
	FILE *m;
	m = fopen("matrizes2.dat", "a");

	fprintf(m, "\nMatriz exata\n");
	for(i=0; i<Nx; i++)
	{	
		y = 0;
		
		for(j=0; j<Ny; j++)
		{	
			fprintf(m, "%.2lf\t", log(pow(x, 2) + pow(y,2)));
			y += h;
		}
		x += h;  
		fprintf(m, "\n");
	}
}

int main(int argc, char **argv)
{	
	double	**M, **Maux;
	double erro;
	int  i, j;
	int cont=0;
 
	Maux = malloc(Nx*sizeof(double*));
	
	for(i=0; i<Ny; i++)
	{
		Maux[i] = malloc(Ny*sizeof(double));
	}
		
	M = malloc(Nx*sizeof(double*));
	
	for(i=0; i<Ny; i++)
	{
		M[i] = malloc(Ny*sizeof(double));
	}
		
	contorno(M);
	
	//mascara
	for(i=0; i<Nx; i++)
	{	
		Maux[i][0] = 1;	
		Maux[i][Ny-1] = 1;
	}
	
	for(i=0; i<Ny; i++)
	{	
		Maux[0][i] = 1;	
		Maux[Nx-1][i] = 1;
	}	
		
	do
	{	
		erro = u(M, Maux);
		cont++;
	}while(erro > E);
	imprime(M,Nx,Ny);
	solucao();
}
			

