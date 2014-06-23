/* AP6, 04.06.2014 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include <windows.h>

#define N 3

static int P[N];

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

void Go( int Pos )
{
  FILE *F;
  int i;
  if (Pos == N)
  {
    if ((F = fopen("a.log", "a")) != NULL)
    {
      for (i = 0; i < N; i++)
        fprintf(F, "%i, ", P[i]);
      fprintf(F, "  p = %i\n", Count());
      fclose(F);
    }
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
  int i;
  for (i = 0; i < N; i++)
    P[i] = i + 1;
  Go(0);
}