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

// Transforms line[index] to 1
void lineIndexToOne(double *line, int index, int line_size)
{
    for (int i = 0; i < line_size; i++)
        if (i != index)
            line[i] /= line[index];

    line[index] = 1;
}

// Transforms line[index] to 0
bool lineIndexToZero(double *line_to_zero, double *line_base, int index, int line_size)
{
    double factor_to_multiply = -1 * line_to_zero[index] / line_base[index];

    for (int i = 0; i < line_size; i++)
        if (i != index)
            line_to_zero[i] += factor_to_multiply * line_base[i];

    line_to_zero[index] = 0;
}

// Copies a base array to a to_copy array
void copyDoubleArray(int n, double *base, double *to_copy)
{
    for (int t = 0; t < n; t++)
        to_copy[t] = base[t];
}

// Copies a base array to a to_copy array
void copyIntArray(int n, int *base, int *to_copy)
{
    for (int t = 0; t < n; t++)
        to_copy[t] = base[t];
}

// Changes the base removing var_to_leave_base variable and adding var_to_enter_base variable
void changeBase(int var_to_enter_base, int var_to_leave_base, int n, int m, int *i, double **r)
{
    int index = 0, index_var_enter_base, index_var_leave_base;
    int *ii;
    double **rr;
    bool added = false;

    if (m == 1)
    {
        i[0] = var_to_enter_base;
        return;
    }

    ii = (int *)malloc(sizeof(int) * m);
    rr = (double **)malloc(sizeof(double *) * m);
    for (int j = 0; j < m; j++)
        rr[j] = (double *)malloc(sizeof(double) * (n + 1));

    for (int j = 0; j < m; j++)
    {
        if (i[j] != var_to_leave_base)
        {
            if (added || i[j] < var_to_enter_base)
            {
                ii[index] = i[j];
                copyDoubleArray(n + 1, r[j], rr[index]);

                index++;
            }
            else
            {
                ii[index] = var_to_enter_base;
                index_var_enter_base = index;
                index++;

                ii[index] = i[j];
                copyDoubleArray(n + 1, r[j], rr[index]);
                index++;

                added = true;
            }
        }
        else
        {
            index_var_leave_base = j;
        }
    }
    copyDoubleArray(n + 1, r[index_var_leave_base], rr[index_var_enter_base]);

    for (int j = 0; j < m; j++)
        copyDoubleArray(n + 1, rr[j], r[j]);

    copyIntArray(m, ii, i);

    free(ii);
    free(rr);
}

// Prints a line
void printDoubleLine(double *line, int line_size, bool printNegative)
{
    if (printNegative)
        for (int i = 0; i < line_size; i++)
        {
            if (i + 1 < line_size)
                printf("%.3lf ", -line[i]);
            else
                printf("%.3lf", -line[i]);
        }
    else
        for (int i = 0; i < line_size; i++)
        {
            if (i + 1 < line_size)
                printf("%.3lf ", line[i]);
            else
                printf("%.3lf", line[i]);
        }
    printf("\n");
}

// Prints a line
void printIntLine(int *line, int line_size, bool printNegative)
{
    if (printNegative)
        for (int i = 0; i < line_size; i++)
        {
            if (i + 1 < line_size)
                printf("%d ", -line[i]);
            else
                printf("%d", -line[i]);
        }
    else
        for (int i = 0; i < line_size; i++)
        {
            if (i + 1 < line_size)
                printf("%d ", line[i]);
            else
                printf("%d", line[i]);
        }
    printf("\n");
}

// Prints the Tableau in the format:
// X i1 i2 ...
// C c1 c2 ...
// Z z
void printTableau(int n, int m, double *c, int *i, double **r)
{
    double z, *b, *x;

    z = 0;

    x = (double *)calloc(sizeof(double), n);
    b = (double *)malloc(sizeof(double) * m);

    for (int j = 0; j < m; j++)
    {
        b[j] = r[j][n];
        x[i[j] - 1] = r[j][n];
    }

    printf("X ");
    for (int i = 0; i < n; i++)
    {
        if (i + 1 < n)
            printf("%.3lf ", x[i]);
        else
            printf("%.3lf", x[i]);
    }
    printf("\n");

    printf("C ");
    printDoubleLine(c, n, true);

    printf("Z %.3lf\n", -c[n]);

    free(b);
}

// Prints the vectors that represents the Tableau in the format:
// C c1 c2 ...
// R r1 r2 ...
// [...]
// I i1 i2 ...
void printVectors(int n, int m, double *c, int *i, double **r)
{
    printf("C ");
    printDoubleLine(c, n + 1, true);

    for (int j = 0; j < m; j++)
    {
        printf("R ");
        printDoubleLine(r[j], n + 1, false);
    }

    printf("I ");
    printIntLine(i, m, false);
}

