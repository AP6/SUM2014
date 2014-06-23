/* AP6, 04.06.2014 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include <windows.h>

#define MAX 10
double A[MAX][MAX], GlobalDeterminant;
int P[MAX], Parity;
int N;


void Swap( int *a, int *b)
{
  int tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

int Count( void )
{
  int i, j, cnt = 0;
  for (i = 0; i < N - 1; i++)
    for (j = i + 1; j < N; j++)
      cnt += P[i] > P[j];
  return cnt;
}


void Load( char *FileName )
{
  FILE *F;
  int i, j;
  
  N = 0;

  if ((F = fopen(FileName, "r")) == NULL)
    return;
 
  fscanf(F, "%i", &N);
  if (N < 0)
    N = 0;
  if (N > MAX)
    N = MAX;
    
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
}

void Go( int Pos )
{
  int i;
  double prod = 1; 

  if (Pos == N)
  {
    for (i = 0; i < N; i++)
      prod *= A[i][P[i]];
    if (Count() % 2 == 0)
      GlobalDeterminant += prod;
    else
      GlobalDeterminant -= prod;
  }
  else
  {
    Go(Pos + 1);
    for (i = Pos + 1; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
    }
  }
}


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  FILE *F;
  int i;

  Load("b.log");

  for (i = 0; i < N; i++)
    P[i] = i;

  Go(0);
  if ((F = fopen("a.log", "a")) != NULL)
    {
      /*
      for (i = 0; i < N; i++)
        fprintf(F, "%i, ", P[i]);
      fprintf(F, "  p = %i\n", Count());
      */
      fprintf(F, "%lf\n ", GlobalDeterminant);
      fclose(F);
    }
} 