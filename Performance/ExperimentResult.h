#pragma once

enum class Algorithm {
    INSERTION_SORT,     // 삽입정렬
    QUICK_SORT_FIRST,   // 퀵정렬 - pivot 선택 전략 FIRST
    QUICK_SORT_MIDDLE,  // 퀵정렬 - pivot 선택 전략 MIDDLE
    QUICK_SORT_LAST     // 퀵정렬 - pivot 선택 전략 LAST
};

class ExperimentResult {
public:
    ExperimentResult(Algorithm algorithm, int dataSize, double sortTimeMs)
        : algorithm(algorithm),
          dataSize(dataSize),
          sortTimeMs(sortTimeMs)
    {}

    Algorithm getAlgorithm() const { return algorithm; }

    int getDataSize() const { return dataSize; }
    
    double getSortTimeMs() const { return sortTimeMs; }

private:
    Algorithm algorithm;
    int dataSize;
    double sortTimeMs;
};