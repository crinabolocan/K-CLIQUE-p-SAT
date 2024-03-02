#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int V;
    int k;

    FILE *f;
    FILE *out;
    char c;
    f = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if (f == NULL)
    {
        printf("Eroare la deschiderea fisierului");
        return -1;
    }
    if (out == NULL)
    {
        printf("Eroare la deschiderea fisierului");
        return -1;
    }
    int nr = 0;
    char *s;
    char *aux2;
    char aux[1000];
    // citirea din fisier
    fgets(aux, 1000, f);
    aux2 = strdup(aux);
    s = strsep(&aux2, " ");
    V = atoi(s);
    s = strsep(&aux2, " ");
    k = atoi(s);
    fprintf(out, "p cnf %d %d\n", V * k, V * k + V * (V - 1) / 2 + V * k * (V - 1) / 2);
    int i = 0;
    int j = 0;
    int legaturi[V + 1][V + 1];
    // initializare matrice de adiacenta cu 0
    for (i = 1; i <= V; i++)
    {
        for (j = 1; j <= V; j++)
        {
            legaturi[i][j] = 0;
        }
    }
    // citirea matricei de adiacenta
    for (i = 1; i <= V; i++)
    {
        fgets(aux, 1000, f);
        aux2 = strdup(aux);
        s = strsep(&aux2, " ");
        for (j = 1; s != NULL; j++)
        {
            legaturi[i][atoi(s)] = 1;
            legaturi[atoi(s)][i] = 1;
            s = strsep(&aux2, " ");
        }
    }
    fclose(f);
    // V x(i,v)
    int n = 0;
    j = 1;
    for (i = 1; i <= k; i++)
    {
        while (j <= V * i)
        {
            fprintf(out, "%d ", j);
            j++;
        }
        fprintf(out, "0\n");
        n++;
    }
    //! x(i,v) V !x(j,v)
    int l = 1;
    j = 1;
    for (l = 1; l <= V; l++)
    {
        for (i = l; i <= V * k; i += 30)
        {
            j = i + 30;
            while (j <= k * V)
            {

                fprintf(out, "-%d -%d 0\n", i, j);
                j += 30;
                n++;
            }
        }
    }
    // x(i,v) V x(j,u)
    i = 1;
    j = 1;
    l = 1;
    int o = 1;
    for (i = 1; i <= V; i++)
    {
        for (j = 1; j <= V; j++)
        {
            if (legaturi[i][j] == 0 && i != j)
            {
                for (l = i; l <= V * k; l += 30)
                {
                    o = j + 30 + l - i;
                    while (o <= k * V)
                    {
                        fprintf(out, "-%d -%d 0\n", l, o);
                        o += 30;
                        n++;
                    }
                }
            }
        }
    }
    // salvez intr-un fisier auxiliar
    fclose(out);
    out = fopen(argv[2], "r");
    FILE *temp = fopen("out/temp.txt", "w");
    char aux3[1000];
    while (fgets(aux3, 1000, out) != NULL)
    {
        if (aux3[0] == 'p')
        {
            fprintf(temp, "p cnf %d %d\n", k * V, n);
        }
        else
        {
            fprintf(temp, "%s", aux3);
        }
    }

    fclose(temp);
    fclose(out);
    // sterg fisierul initial
    // redenumesc fisierul auxiliar cu numele initial
    remove(argv[2]);
    rename("out/temp.txt", argv[2]);

    return 0;
}
