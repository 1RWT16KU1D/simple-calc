#include "../include/calc.h"

static void onClick(GtkWidget *widget, gpointer data)
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
}

static void activate(GtkApplication *app, gpointer userData)
{
    // Set larger font size
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(
        provider,
        ".calculator entry { font-size: 40px; }"
        ".calculator button { font-size: 28px; }"
    );
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    // Initialisation
    struct CalculatorData *state = malloc(sizeof(struct CalculatorData));
    GtkWidget *win;
    GtkWidget *box;
    GtkWidget *display;
    GtkWidget *grid;
    GtkWidget *buttons[18];

    // Main Window
    win = gtk_application_window_new(app);
    gtk_widget_add_css_class(win, "calculator");
    gtk_window_set_title(GTK_WINDOW(win), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(win), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(win), FALSE);

    // Layout Box
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_window_set_child(GTK_WINDOW(win), box);

    // Display
    display = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(display), "0");
    gtk_entry_set_alignment(GTK_ENTRY(display), 1.0); // Right align
    gtk_editable_set_editable(GTK_EDITABLE(display), FALSE);
    gtk_widget_set_can_focus(display, FALSE);
    gtk_widget_set_size_request(display, -1, 140);
    gtk_box_append(GTK_BOX(box), display);

    // Button Grid
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 96);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 96);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), grid);

    // Buttons
    const char *labels[18] = {
        "7", "8", "9", "↵",
        "4", "5", "6", "/",
        "1", "2", "3", "*",
        "C", "0", "+", "-"
    };

    for (int i = 0; i < 16; i++)
    {
        buttons[i] = gtk_button_new_with_label(labels[i]);

        gtk_widget_set_margin_top(buttons[i], 4);
        gtk_widget_set_margin_bottom(buttons[i], 4);
        gtk_widget_set_margin_start(buttons[i], 4);
        gtk_widget_set_margin_end(buttons[i], 4);

        int row = i / 4;
        int col = i % 4;
        gtk_grid_attach(GTK_GRID(grid), buttons[i], col, row, 2, 2);
    }

    // Equals to
    buttons[16] = gtk_button_new_with_label("=");
    gtk_widget_set_margin_top(buttons[16], 4);
    gtk_widget_set_margin_bottom(buttons[16], 4);
    gtk_widget_set_margin_start(buttons[16], 4);
    gtk_widget_set_margin_end(buttons[16], 4);
    gtk_grid_attach(GTK_GRID(grid), buttons[16], 0, 4, 4, 2);
    g_signal_connect(buttons[16], "clicked", G_CALLBACK(onClick), state);

    // Modulus
    buttons[17] = gtk_button_new_with_label("%");
    gtk_widget_set_margin_top(buttons[17], 4);
    gtk_widget_set_margin_bottom(buttons[17], 4);
    gtk_widget_set_margin_start(buttons[17], 4);
    gtk_widget_set_margin_end(buttons[17], 4);
    gtk_grid_attach(GTK_GRID(grid), buttons[17], 3, 4, 2, 2);
    g_signal_connect(buttons[17], "clicked", G_CALLBACK(onClick), state);

    // Initialize State
    state->display = display;
    state->lvalue = 0;
    state->rvalue = 0;
    state->operator = '\0';
    state->result = 0;
    gtk_entry_set_placeholder_text(GTK_ENTRY(display), "0");

    // Connect Signals
    state->display = display;
    for (int i = 0; i < 16; i++) g_signal_connect(buttons[i], "clicked", G_CALLBACK(onClick), state);

    // Show Window
    gtk_window_present(GTK_WINDOW(win));
}

int runCalc(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.gtk.calc", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
