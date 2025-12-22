#ifndef CALC_H
#define CALC_H

#include <gtk/gtk.h>

#define WINDOW_WIDTH   350
#define WINDOW_HEIGHT  650

struct CalculatorData
{
    GtkWidget *display;
    int lvalue;
    int rvalue;
    int result;
    char operator;
};

// Exported Functions
int runCalc(int argc, char **argv);

#endif // CALC_H
