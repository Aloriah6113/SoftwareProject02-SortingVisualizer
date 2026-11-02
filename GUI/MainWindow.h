#pragma once
#include <windows.h>
#include <vector>
#include "VisualizerWidget.h"
#include "../Performance/ExperimentResult.h"

class MainWindow {
public:
    bool create(HINSTANCE instance, int show);
    static LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wp, LPARAM lp);
private:
    LRESULT handle(UINT message, WPARAM wp, LPARAM lp);
    void controls();
    void layout();
    void draw(HDC dc);
    void generate();
    void runBenchmark(bool pivots);
    int readNumber(HWND field,int low,int high,int fallback) const;
    void updateStatus();
    void setPlaying(bool value);
    HWND window_=nullptr, algorithm_=nullptr, count_=nullptr, speed_=nullptr;
    HWND generate_=nullptr, play_=nullptr, step_=nullptr, reset_=nullptr;
    HWND start_=nullptr, steps_=nullptr, gap_=nullptr, benchSort_=nullptr, benchPivot_=nullptr;
    HWND status_=nullptr;
    VisualizerWidget view_;
    std::vector<ExperimentResult> results_;
    std::vector<int> original_;
    bool playing_=false, pivotChart_=false;
    int delayMs_=120;
};
