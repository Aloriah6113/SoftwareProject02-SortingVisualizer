#pragma once
#include <windows.h>
#include <vector>
#include "../Data/SortEvent.h"

class VisualizerWidget {
public:
    void load(std::vector<int> values, std::vector<SortEvent> recorded);
    bool step();
    void reset();
    void paint(HDC dc, RECT area) const;
    bool finished() const { return position_ >= events_.size(); }
    size_t position() const { return position_; }
    size_t total() const { return events_.size(); }
    int size() const { return static_cast<int>(initial_.size()); }
private:
    std::vector<int> initial_, values_;
    std::vector<SortEvent> events_;
    std::vector<bool> sorted_;
    size_t position_ = 0;
    int key_ = 0, keyIndex_ = -1, markedA_ = -1, markedB_ = -1, pivot_ = -1;
    EventType last_ = EventType::COMPARE;
};
