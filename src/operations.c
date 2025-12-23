#include "../include/calc.h"

void onClick(GtkWidget *widget, gpointer data)
{
    struct CalculatorData *state = (struct CalculatorData *)data;
    GtkEntry *display = GTK_ENTRY(state->display);

    // Get button label
    const char *label = gtk_button_get_label(GTK_BUTTON(widget));
    char buf[32];

    // Clear State
    if (g_strcmp0(label, "C") == 0)
    {
        state->lvalue = 0;
        state->rvalue = 0;
        state->operator = '\0';
        state->result = 0;
        gtk_editable_set_text(GTK_EDITABLE(display), "0");
        return;
    }

    // Backspace
    if (g_strcmp0(label, "↵") == 0)
    {
        if (state->operator == '\0')
        {
            state->lvalue /= 10;
            snprintf(buf, sizeof(buf), "%d", state->lvalue);
            gtk_editable_set_text(GTK_EDITABLE(display), buf);
        }
        else
        {
            state->rvalue /= 10;
            snprintf(buf, sizeof(buf), "%d", state->rvalue);
            gtk_editable_set_text(GTK_EDITABLE(display), buf);
        }
        return;
    }

    // If digit
    if (label[0] >= '0' && label[0] <= '9' && label[1] == '\0')
    {
        int digit = atoi(label);

        // Display number
        if (state->operator == '\0')
        {
            state->lvalue = state->lvalue * 10 + digit;
            snprintf(buf, sizeof(buf), "%d", state->lvalue);
            gtk_editable_set_text(GTK_EDITABLE(display), buf);  
        }
        else
        {
            state->rvalue = state->rvalue * 10 + digit;
            snprintf(buf, sizeof(buf), "%d", state->rvalue);
            gtk_editable_set_text(GTK_EDITABLE(display), buf);
        }
        return;
    }

    // If operator
    if (g_strcmp0(label, "=") != 0 && g_strcmp0(label, "↵") != 0)
    {
        state->operator = label[0];
        return;
    }
    else if (g_strcmp0(label, "=") == 0) // If equals
    {
        switch (state->operator)
        {
            case '+':
                state->result = state->lvalue + state->rvalue;
                break;
            case '-':
                state->result = state->lvalue - state->rvalue;
                break;
            case '*':
                state->result = state->lvalue * state->rvalue;
                break;
            case '/':
                if (state->rvalue != 0)
                    state->result = state->lvalue / state->rvalue;
                else
                {
                    gtk_editable_set_text(GTK_EDITABLE(display), "Error: Division by 0! ");
                    return;
                }
                break;
            case '%':
                state->result = state->lvalue % state->rvalue;
                break;
            default:
                return;
        }

        snprintf(buf, sizeof(buf), "%d", state->result);
        gtk_editable_set_text(GTK_EDITABLE(display), buf);

        // Set for next calculation
        state->lvalue = state->result;
        state->rvalue = 0;
        state->operator = '\0';
        return;
    }
    else
        return;
}
