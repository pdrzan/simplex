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

// Checks if a variable is ilimited
bool isVariableIlimited(int var_index, int m, double **r)
{
    for (int j = 0; j < m; j++)
        if (r[j][var_index - 1] > EPSILON)
            return false;

    return true;
}

// Checks if a variable is in the base
bool isVariableInBase(int var_index, int m, int *i)
{
    for (int j = 0; j < m; j++)
        if (i[j] == var_index)
            return true;

    return false;
}

// Checks if the Tableau is ilimited
bool isTableauIlimited(int n, int m, double *c, double **r)
{
    for (int t = 0; t < n; t++)
        if (isVariableIlimited(t + 1, m, r))
            return true;

    return false;
}

// Gets the index of the variable with the best reduced cost in Tableau
int getBestReducedCostVariableIndex(int n, double *c)
{
    double best_reduced_cost = DBL_MIN;
    int index_var = -1;

    for (int t = 0; t < n; t++)
        if (c[t] > best_reduced_cost + EPSILON)
        {
            best_reduced_cost = c[t];
            index_var = t + 1;
        }

    return index_var;
}

// Gets the index of the variable that is going to leave the base
int getVariableToLeaveBase(int n, int m, int i_to_include_in_base, int *i, double **r)
{
    int b_y = INT_MAX;
    int index_var = -1;

    for (int j = 0; j < m; j++)
    {
        if (r[j][i_to_include_in_base - 1] > 0 && r[j][n] / r[j][i_to_include_in_base - 1] < b_y)
        {
            b_y = r[j][n] / r[j][i_to_include_in_base - 1];
            index_var = i[j];
        }
    }

    return index_var;
}

// Get the index of a number inside a array. If the number doesn't exists, returns -1
int getIndexOf(int number, int n, int *array)
{
    for (int t = 0; t < n; t++)
        if (array[t] == number)
            return t;

    return -1;
}

