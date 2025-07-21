
#ifndef UI_HPP
#define UI_HPP

#include <ncurses.h>
#include <string>
#include <vector>
#include "Core/Calculator.hpp"

class UI {
public:
    UI();
    ~UI();

    void Run();

private:
    int ShowSplashScreen();

private:
    void InitializeColors();
    void CreateWindows();
    void DrawBorders();
    void ShowScientificCalculator();
    void ShowGraphingTool();
    void PlotAsciiGraph(const std::string& expr, int width, int height, double xMin, double xMax, double yMin, double yMax);

    // Scientific Calculator Helpers
    std::string GetScientificInput(WINDOW* inputWin);
    void ProcessScientificCommand(const std::string& command, WINDOW* logWin, WINDOW* resultWin);
    void DisplayScientificHelp(WINDOW* logWin);

    // Graphing Tool Helpers
    std::string GetGraphingExpression(WINDOW* inputWin);
    double GetGraphingRange(WINDOW* inputWin, const std::string& prompt, double defaultValue);
    void DisplayGraphingHelp(WINDOW* outputWin);
    void PrintInWindow(WINDOW* win, int y, int x, const std::string& text, short colorPair);

    WINDOW* m_InputWin;
    WINDOW* m_OutputWin;

    Calculator m_Calculator;
    std::vector<std::string> m_InputHistory;
    std::vector<std::string> m_SciLog;
    int m_HistoryIndex = 0;
};

#endif //UI_HPP
