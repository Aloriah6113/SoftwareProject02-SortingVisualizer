#pragma once

#include"SortingAlgorithm.h"

class QuickSort : public SortingAlgorithm {
public:
    enum class PivotStrategy {
        FIRST,
        MIDDLE,
        LAST
    };

    // pivot 선택 전략을 결정한다.
    QuickSort(PivotStrategy pivotStrategy = PivotStrategy::FIRST);

    // 정렬 알고리즘을 공통 인터페이스로 다루기 위한 진입점 역할을 한다.
    void sort(std::vector<int>& data, bool recordEvents = true) override;

private:
    PivotStrategy pivotStrategy;

    // 실제 재귀 정렬을 수행하고, 원본 배열을 수정한다.
    void quickSort(std::vector<int>& data, int left, int right, bool recordEvents = true);

    int partition(std::vector<int>& data, int left, int right, bool recordEvents = true);

    void swap(std::vector<int>& data, int index1, int index2);
};