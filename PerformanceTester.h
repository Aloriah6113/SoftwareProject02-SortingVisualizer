#pragma once

#include "ExperimentResult.h"

#include <vector>

class PerformanceTester {
public:
    // 삽입정렬과 퀵정렬의 성능 비교
    std::vector<ExperimentResult> benchmarkSorts(
        int startSize,
        int stepCount,
        int sizeGap
    );

    // 피봇 선택 전략별 성능 비교
    std::vector<ExperimentResult> benchmarkPivots(
        int startSize,
        int stepCount,
        int sizeGap
    );

private:

};