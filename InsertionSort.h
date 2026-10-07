#pragma once

#include"SortingAlgorithm.h"

class InsertionSort : public SortingAlgorithm {
public:
    // 삽입 정렬 알고리즘을 실행하고, 원본 배열을 수정한다.
    void sort(std::vector<int>& data) override;
};