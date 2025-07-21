#include "UI/UI.hpp"
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

UI::UI() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    InitializeColors();
    CreateWindows();
    DrawBorders();
    refresh();
}

UI::~UI() {
    delwin(m_InputWin);
    delwin(m_OutputWin);
    endwin();
}

void UI::InitializeColors() {
    use_default_colors();
    init_pair(1, COLOR_CYAN, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_RED, -1);
    init_pair(5, COLOR_WHITE, -1);
}

void UI::CreateWindows() {
    int height, width;
    getmaxyx(stdscr, height, width);
    m_InputWin = newwin(3, width, height - 3, 0);
    m_OutputWin = newwin(height - 3, width, 0, 0);
}

void UI::DrawBorders() {
    box(m_InputWin, 0, 0);
    mvwprintw(m_InputWin, 0, 2, " Input ");
    box(m_OutputWin, 0, 0);
    mvwprintw(m_OutputWin, 0, 2, " Output ");
    wrefresh(m_InputWin);
    wrefresh(m_OutputWin);
}

void UI::PrintInWindow(WINDOW* win, int y, int x, const std::string& text, short colorPair) {
    wattron(win, COLOR_PAIR(colorPair));
    mvwprintw(win, y, x, "%s", text.c_str());
    wattroff(win, COLOR_PAIR(colorPair));
    wrefresh(win);
}

void UI::Run() {
    while (true) {
        int choice = ShowSplashScreen();

        switch (choice) {
            case 0: // Scientific Calculator
                ShowScientificCalculator();
                break;
            case 1: // Graphing Tool
                ShowGraphingTool();
                break;
            case 2: // Exit
                return;
        }
    }
}

int UI::ShowSplashScreen() {
    werase(stdscr);
    refresh();

    int height, width;
    getmaxyx(stdscr, height, width);

    // ASCII Art (adjust as needed)
    std::vector<std::string> asciiArt = {
        "   __  __       _   _           _ ",
        "  |  \\/  | __ _| |_| |__   __ _| |",
        "  | |\\/| |/ _` | __| '_ \\ / _` | |",
        "  | |  | | (_| | |_| | | | (_| | |",
        "  |_|  |_|\\__,_|\\__|_| |_|\\__,_|_|",
        "          ASCII MATH v1.0         "
    };


    // Options
    std::vector<std::string> options = {
        "Scientific Calculator",
        "Graphing Tool",
        "Exit"
    };

    int artHeight = asciiArt.size();
    int optionHeight = options.size();
    int selected = 0;
    int ch;

    while (true) {
        werase(stdscr);

        // Draw ASCII Art
        int startArtY = (height - artHeight - optionHeight - 5) / 2;
        if (startArtY < 0) startArtY = 0; // Ensure it's not negative
        for (int i = 0; i < artHeight; ++i) {
            mvwprintw(stdscr, startArtY + i, (width - asciiArt[i].length()) / 2, "%s", asciiArt[i].c_str());
        }

        // Draw Copyright
        std::string copyright = "(C) 2025 assembler-0. All rights reserved.";
        mvwprintw(stdscr, startArtY + artHeight + 2, (width - copyright.length()) / 2, "%s", copyright.c_str());

        // Draw Options
        int startOptionY = startArtY + artHeight + 4;
        for (int i = 0; i < optionHeight; ++i) {
            if (i == selected) {
                wattron(stdscr, A_REVERSE);
            }
            mvwprintw(stdscr, startOptionY + i, (width - options[i].length()) / 2, "%s", options[i].c_str());
            wattroff(stdscr, A_REVERSE);
        }

        wrefresh(stdscr);

        ch = getch();
        switch (ch) {
            case KEY_UP:
                selected--;
                if (selected < 0) selected = optionHeight - 1;
                break;
            case KEY_DOWN:
                selected++;
                if (selected >= optionHeight) selected = 0;
                break;
            case '\n':
                return selected;
        }
    }
}

void UI::ShowScientificCalculator() {
    werase(stdscr);
    refresh();

    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW* logWin = newwin(height - 6, width, 0, 0);
    WINDOW* resultWin = newwin(3, width, height - 6, 0);
    WINDOW* inputWin = newwin(3, width, height - 3, 0);

    box(logWin, 0, 0);
    mvwprintw(logWin, 0, 2, " Log ");
    box(resultWin, 0, 0);
    mvwprintw(resultWin, 0, 2, " Result ");
    box(inputWin, 0, 0);
    mvwprintw(inputWin, 0, 2, " Input ");

    wrefresh(logWin);
    wrefresh(resultWin);
    wrefresh(inputWin);

    auto printToLog = [&](const std::string& s) {
        m_SciLog.push_back(s);
        werase(logWin);
        box(logWin, 0, 0);
        mvwprintw(logWin, 0, 2, " Log ");
        int y = 1;
        for (const auto& line : m_SciLog) {
            mvwprintw(logWin, y++, 2, "%s", line.c_str());
        }
        wrefresh(logWin);
    };

    printToLog("Scientific Calculator. Enter 'h' for help, 'q' to quit.");

    while (true) {
        std::string command = GetScientificInput(inputWin);
        if (command == "q") break;
        if (command == "h") {
            DisplayScientificHelp(logWin);
            continue;
        }
        ProcessScientificCommand(command, logWin, resultWin);
    }

    delwin(logWin);
    delwin(resultWin);
    delwin(inputWin);
    werase(stdscr);
    refresh();
}

std::string UI::GetScientificInput(WINDOW* inputWin) {
    char inputStr[100] = {0};
    mvwprintw(inputWin, 1, 2, "> ");
    echo();
    int ch;
    int i = 0;
    keypad(inputWin, TRUE);
    while ((ch = mvwgetch(inputWin, 1, 4 + i)) != '\n') {
        if (ch == KEY_UP) {
            if (m_HistoryIndex > 0) {
                m_HistoryIndex--;
                strcpy(inputStr, m_InputHistory[m_HistoryIndex].c_str());
                i = strlen(inputStr);
                mvwprintw(inputWin, 1, 4, "%s", inputStr);
                wclrtoeol(inputWin);
            }
        } else if (ch == KEY_DOWN) {
            if (m_HistoryIndex < static_cast<int>(m_InputHistory.size())) {
                m_HistoryIndex++;
                if (m_HistoryIndex == static_cast<int>(m_InputHistory.size())) {
                    inputStr[0] = '\0';
                    i = 0;
                    wmove(inputWin, 1, 4);
                    wclrtoeol(inputWin);
                } else {
                    strcpy(inputStr, m_InputHistory[m_HistoryIndex].c_str());
                    i = strlen(inputStr);
                    mvwprintw(inputWin, 1, 4, "%s", inputStr);
                    wclrtoeol(inputWin);
                }
            }
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (i > 0) {
                i--;
                inputStr[i] = '\0';
                mvwprintw(inputWin, 1, 4 + i, " ");
                wmove(inputWin, 1, 4 + i);
            }
        } else {
            if (i < 99) {
                inputStr[i++] = ch;
            }
        }
    }
    noecho();
    werase(inputWin);
    box(inputWin, 0, 0);
    mvwprintw(inputWin, 0, 2, " Input ");
    wrefresh(inputWin);

    std::string expr(inputStr);
    if (!expr.empty()) {
        m_InputHistory.push_back(expr);
        m_HistoryIndex = m_InputHistory.size();
    }
    return expr;
}

void UI::ProcessScientificCommand(const std::string& command, WINDOW* logWin, WINDOW* resultWin) {
    auto printToLog = [&](const std::string& s) {
        m_SciLog.push_back(s);
        werase(logWin);
        box(logWin, 0, 0);
        mvwprintw(logWin, 0, 2, " Log ");
        int y = 1;
        for (const auto& line : m_SciLog) {
            mvwprintw(logWin, y++, 2, "%s", line.c_str());
        }
        wrefresh(logWin);
    };

    printToLog("> " + command);
    double result = m_Calculator.EvaluateExpression(command);

    if (!std::isnan(result)) {
        werase(resultWin);
        box(resultWin, 0, 0);
        mvwprintw(resultWin, 0, 2, " Result ");
        wattron(resultWin, COLOR_PAIR(3));
        mvwprintw(resultWin, 1, 2, "%s", std::to_string(result).c_str());
        wattroff(resultWin, COLOR_PAIR(3));
        wrefresh(resultWin);
    } else {
        werase(resultWin);
        box(resultWin, 0, 0);
        mvwprintw(resultWin, 0, 2, " Result ");
        wattron(resultWin, COLOR_PAIR(4));
        mvwprintw(resultWin, 1, 2, "%s", "Invalid expression");
        wattroff(resultWin, COLOR_PAIR(4));
        wrefresh(resultWin);
    }
}

void UI::DisplayScientificHelp(WINDOW* logWin) {
    m_SciLog.clear();
    auto printToLog = [&](const std::string& s) {
        m_SciLog.push_back(s);
        werase(logWin);
        box(logWin, 0, 0);
        mvwprintw(logWin, 0, 2, " Log ");
        int y = 1;
        for (const auto& line : m_SciLog) {
            mvwprintw(logWin, y++, 2, "%s", line.c_str());
        }
        wrefresh(logWin);
    };

    printToLog("--- Scientific Calculator Help ---");
    printToLog("Functions: sin, cos, tan, asin, acos, atan, sinh, cosh, tanh, asinh, acosh, atanh");
    printToLog("Operators: +, -, *, /, ^");
    printToLog("Constants: pi, e");
    printToLog("Special: factorial(n), gcd(a,b), lcm(a,b), prod(expr,start,end), sum(expr,start,end)");
}

void UI::ShowGraphingTool() {
    werase(stdscr);
    refresh();

    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW* graphOutputWin = newwin(height - 3, width, 0, 0);
    WINDOW* graphInputWin = newwin(3, width, height - 3, 0);

    box(graphOutputWin, 0, 0);
    mvwprintw(graphOutputWin, 0, 2, " Graph Output ");
    box(graphInputWin, 0, 0);
    mvwprintw(graphInputWin, 0, 2, " Graph Input ");

    wrefresh(graphOutputWin);
    wrefresh(graphInputWin);

    PrintInWindow(graphOutputWin, 1, 2, "Graphing Tool", 1);
    PrintInWindow(graphOutputWin, 3, 2, "Enter a function in terms of x.", 5);
    PrintInWindow(graphOutputWin, 4, 2, "The graph will be plotted for x between xMin and xMax.", 5);
    PrintInWindow(graphOutputWin, 5, 2, "Press 'i' for info, 'q' to quit.", 5);

    while (true) {
        std::string exprStr = GetGraphingExpression(graphInputWin);
        if (exprStr.empty()) continue;

        if (exprStr == "q") break;
        if (exprStr == "i") {
            DisplayGraphingHelp(graphOutputWin);
            continue;
        }

        double xMin = GetGraphingRange(graphInputWin, "xMin: ", -10.0);
        double xMax = GetGraphingRange(graphInputWin, "xMax: ", 10.0);

        double yMin, yMax;

        int plotWidth, plotHeight;
        getmaxyx(graphOutputWin, plotHeight, plotWidth);
        plotWidth -= 4; // for border
        plotHeight -= 4;

        m_Calculator.CalculateMinMaxY(exprStr, xMin, xMax, plotWidth, yMin, yMax);

        if (std::isnan(yMin) || std::isnan(yMax)) {
            PrintInWindow(graphOutputWin, 1, 2, "Could not determine Y range.", 4);
        } else {
            PlotAsciiGraph(exprStr, plotWidth, plotHeight, xMin, xMax, yMin, yMax);
        }

        wgetch(graphOutputWin);
    }

    delwin(graphOutputWin);
    delwin(graphInputWin);
    werase(stdscr);
    refresh();
}

void UI::DisplayGraphingHelp(WINDOW* outputWin) {
    werase(outputWin);
    box(outputWin, 0, 0);
    mvwprintw(outputWin, 0, 2, " Graph Info ");

    int y = 1;
    PrintInWindow(outputWin, y++, 2, "--- Graphing Tool Information ---", 1);
    PrintInWindow(outputWin, y++, 2, "Enter mathematical expressions in terms of 'x'.", 5);
    PrintInWindow(outputWin, y++, 2, "Example: sin(x), x^2 + 2*x - 1, 1/x", 5);
    y++;
    PrintInWindow(outputWin, y++, 2, "Supported functions (from exprtk):", 5);
    PrintInWindow(outputWin, y++, 4, "  - Basic arithmetic: +, -, *, /, ^ (power)", 5);
    PrintInWindow(outputWin, y++, 4, "  - Trigonometric: sin, cos, tan, asin, acos, atan", 5);
    PrintInWindow(outputWin, y++, 4, "  - Hyperbolic: sinh, cosh, tanh, asinh, acosh, atanh", 5);
    PrintInWindow(outputWin, y++, 4, "  - Logarithmic: log (natural), log10", 5);
    PrintInWindow(outputWin, y++, 4, "  - Other: abs, sqrt, cbrt, exp, round, floor, ceil", 5);
    y++;
    PrintInWindow(outputWin, y++, 2, "Constants: pi, e", 5);
    y++;
    PrintInWindow(outputWin, y++, 2, "Input 'xMin' and 'xMax' to define the range for plotting.", 5);
    PrintInWindow(outputWin, y++, 2, "The graph will automatically scale the Y-axis.", 5);
    y++;
    PrintInWindow(outputWin, y++, 2, "Press any key to return to graphing.", 1);

    wrefresh(outputWin);
    wgetch(outputWin);

    // Clear the info and redraw the initial graphing tool prompts
    werase(outputWin);
    box(outputWin, 0, 0);
    mvwprintw(outputWin, 0, 2, " Graph Output ");
    PrintInWindow(outputWin, 1, 2, "Graphing Tool", 1);
    PrintInWindow(outputWin, 3, 2, "Enter a function in terms of x.", 5);
    PrintInWindow(outputWin, 4, 2, "The graph will be plotted for x between xMin and xMax.", 5);
    PrintInWindow(outputWin, 5, 2, "Press 'i' for info, 'q' to quit.", 5);
    wrefresh(outputWin);
}

std::string UI::GetGraphingExpression(WINDOW* inputWin) {
    char exprStr[100] = {0};
    mvwprintw(inputWin, 1, 2, "f(x): ");
    echo();
    mvwgetstr(inputWin, 1, 8, exprStr);
    noecho();
    werase(inputWin);
    box(inputWin, 0, 0);
    mvwprintw(inputWin, 0, 2, " Graph Input ");
    wrefresh(inputWin);
    return std::string(exprStr);
}

double UI::GetGraphingRange(WINDOW* inputWin, const std::string& prompt, double defaultValue) {
    char valStr[20] = {0};
    mvwprintw(inputWin, 1, 2, "%s", prompt.c_str());
    echo();
    mvwgetstr(inputWin, 1, 2 + prompt.length(), valStr);
    noecho();
    werase(inputWin);
    box(inputWin, 0, 0);
    mvwprintw(inputWin, 0, 2, " Graph Input ");
    wrefresh(inputWin);
    if (strlen(valStr) == 0) return defaultValue;
    return std::stod(valStr);
}

void UI::PlotAsciiGraph(const std::string& expr, int width, int height, double xMin, double xMax, double yMin, double yMax) {
    werase(m_OutputWin);
    DrawBorders();

    std::vector<std::string> canvas(height, std::string(width, ' '));

    int xAxisRow = -1;
    if (yMin <= 0 && yMax >= 0) {
        xAxisRow = static_cast<int>((yMax - 0) * (height - 1) / (yMax - yMin));
    }

    if (xAxisRow >= 0 && xAxisRow < height) {
        for (int w = 0; w < width; ++w) canvas[xAxisRow][w] = '-';
    }

    int yAxisCol = -1;
    if (xMin <= 0 && xMax >= 0) {
        yAxisCol = static_cast<int>((0 - xMin) * (width - 1) / (xMax - xMin));
    }

    if (yAxisCol >= 0 && yAxisCol < width) {
        for (int h = 0; h < height; ++h) canvas[h][yAxisCol] = '|';
    }

    if (xAxisRow >= 0 && xAxisRow < height && yAxisCol >= 0 && yAxisCol < width) {
        canvas[xAxisRow][yAxisCol] = '+';
    }

    // Enhanced sampling for plotting
    const int samplesPerColumn = 5; // Evaluate 5 points per character column
    double xStepPerSample = (xMax - xMin) / (width * samplesPerColumn);

    for (int col = 0; col < width; ++col) {
        for (int s = 0; s < samplesPerColumn; ++s) {
            double x = xMin + (col * samplesPerColumn + s) * xStepPerSample;
            m_Calculator.SetX(x);
            double y = m_Calculator.EvaluateExpression(expr);

            if (!std::isnan(y)) {
                int plotY = static_cast<int>((yMax - y) * (height - 1) / (yMax - yMin));
                if (plotY >= 0 && plotY < height) {
                    canvas[plotY][col] = '*'; // Mark the column if any sample falls in it
                }
            }
        }
    }

    for (int h = 0; h < height; ++h) {
        PrintInWindow(m_OutputWin, h + 2, 2, canvas[h], 5);
    }
}
