#include "DataGenerator.h"

using std::vector;

// 난수 생성 및 저장
DataGenerator::DataGenerator() 
    : rng(std::random_device{}())
{}

// 랜덤 데이터 생성
vector<int> DataGenerator::generate(int dataSize, int minValue, int maxValue) {
    vector<int> data;
    data.reserve(dataSize);

    // 분포 객체 생성
    std::uniform_int_distribution<int> dist(minValue, maxValue);
    
    for (int i = 0; i < dataSize; i++) {
        data.push_back(dist(rng));
    }

    return data;
}

// 실험을 위한 데이터셋 생성
vector<vector<int>> DataGenerator::generateExperiment(
    int startSize, 
    int stepCount, 
    int sizeGap) {
    vector<vector<int>> datasets;
    datasets.resize(stepCount);
    
    std::uniform_int_distribution<int> dist(1, 10000);

    int dataSize = startSize;

    for (int i = 0; i < stepCount; i++) {
        datasets[i].reserve(dataSize);

        for (int j = 0; j < dataSize; j++) {
            datasets[i].push_back(dist(rng));
        }
        dataSize += sizeGap;
    }

    return datasets;
}