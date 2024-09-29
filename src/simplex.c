#include <float.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// Prints E 1 and exits program
void notPossibleToBuildTableau()
{
    printf("E 1\n");
    exit(0);
}

// Prints V N and exits program
void notFeasibleBase()
{
    printf("V N\n");
    exit(0);
}
// Reads instance and stores information in n, m, c, i and r arrays
void readInstance(char *instance_file_path, int *n, int *m, double **c, int **i, double ***r)
{
    int t, j;
    FILE *instance_file;

    instance_file = fopen(instance_file_path, "r");

    if (instance_file == NULL)
        notPossibleToBuildTableau();

    fscanf(instance_file, "P %d %d", n, m);

    *c = (double *)malloc(sizeof(double) * (*n + 1));
    *i = (int *)malloc(sizeof(int) * (*m));
    *r = (double **)malloc(sizeof(double *) * (*m));
    for (j = 0; j < *m; j++)
        (*r)[j] = (double *)malloc(sizeof(double) * (*n + 1));

    fscanf(instance_file, "\nF");
    for (t = 0; t < *n; t++)
    {
        fscanf(instance_file, "%lf", &(*c)[t]);
        (*c)[t] *= -1;
    }
    c[*n + 1] = 0;

    for (j = 0; j < *m; j++)
    {
        fscanf(instance_file, "\nR");

        for (t = 0; t < *n + 1; t++)
            fscanf(instance_file, "%lf", &(*r)[j][t]);

        fscanf(instance_file, "\n");
    }

    fscanf(instance_file, "\nB");
    for (j = 0; j < *m; j++)
        fscanf(instance_file, "%d", &(*i)[j]);
}

// Checks if a line[index] is equal to a number considering EPSILON as numeric error
bool isLineIndexEqualToNumber(double *line, int index, int number)
{
    return (line[index] > number && line[index] - number <= EPSILON) || (line[index] < number && number - line[index] <= EPSILON) || line[index] == number;
}

// Checks if the Tableau is in a optimal solution
bool isOptimalSolution(int n, double *c)
{
    for (int t = 0; t < n; t++)
        if (c[t] > EPSILON)
            return false;

    return true;
}
