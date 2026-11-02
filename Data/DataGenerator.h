#pragma once

#include <vector>
#include <random>

class DataGenerator {
public:
    // 난수 생성 및 저장
    DataGenerator();

    // 랜덤 데이터 생성
    std::vector<int> generate(int dataSize, int minValue, int maxValue);

    // 실험을 위한 데이터셋 생성
    std::vector<std::vector<int>> generateExperiment(
        int startSize,
        int stepCount,
        int sizeGap
    );

private:
    std::mt19937 rng;
};