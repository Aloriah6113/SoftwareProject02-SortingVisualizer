#include "PerformanceTester.h"
#include "DataGenerator.h"
#include "InsertionSort.h"
#include "QuickSort.h"

#include <vector>
#include <chrono>

using std::vector;

// 삽입정렬과 퀵정렬(pivot 선택 전략: MIDDLE)의 성능 비교
vector<ExperimentResult> PerformanceTester::benchmarkSorts(
    int startSize,
    int stepCount,
    int sizeGap) {
    DataGenerator dataGenerator;
    InsertionSort insertionSort;
    QuickSort quickSort(QuickSort::PivotStrategy::MIDDLE);

    // 원본 데이터셋 생성
    vector<vector<int>> datasets =
        dataGenerator.generateExperiment(startSize, stepCount, sizeGap);
    
    // 정렬용 데이터셋 복사
    vector<vector<int>> datasetsI = datasets;
    vector<vector<int>> datasetsQ = datasets;

    // 결과 반환을 위한 vector<ExperimentResult> 생성
    vector<ExperimentResult> results;

    // 삽입정렬 시간 측정
    for (int i = 0; i < stepCount; i++) {
        // 여기서부터 시간 측정
        auto start = std::chrono::steady_clock::now();
        
        insertionSort.sort(datasetsI[i], false);

        auto end = std::chrono::steady_clock::now();
        // 여기까지 시간 측정

        double sortTimeMs = std::chrono::duration<double, std::milli>(end - start).count();

        results.push_back(ExperimentResult(Algorithm::INSERTION_SORT, datasetsI[i].size(), sortTimeMs));
    }

    // 퀵정렬 시간 측정
    for (int i = 0; i < stepCount; i++) {
        // 여기서부터 시간 측정
        auto start = std::chrono::steady_clock::now();

        quickSort.sort(datasetsQ[i], false);

        auto end = std::chrono::steady_clock::now();
        // 여기까지 시간 측정

        double sortTimeMs = std::chrono::duration<double, std::milli>(end - start).count();

        results.push_back(ExperimentResult(Algorithm::QUICK_SORT_MIDDLE, datasetsQ[i].size(), sortTimeMs));
    }

    return results;
}

// 피봇 선택 전략별 성능 비교
vector<ExperimentResult> PerformanceTester::benchmarkPivots(
    int startSize,
    int stepCount,
    int sizeGap) {
    DataGenerator dataGenerator;
    QuickSort quickSortF(QuickSort::PivotStrategy::FIRST);
    QuickSort quickSortM(QuickSort::PivotStrategy::MIDDLE);
    QuickSort quickSortL(QuickSort::PivotStrategy::LAST);

    // 원본 데이터셋 생성
    vector<vector<int>> datasets =
        dataGenerator.generateExperiment(startSize, stepCount, sizeGap);

    // 정렬용 데이터셋 복사
    vector<vector<int>> datasetsF = datasets;
    vector<vector<int>> datasetsM = datasets;
    vector<vector<int>> datasetsL = datasets;

    // 결과 반환을 위한 vector<ExperimentResult> 생성
    vector<ExperimentResult> results;

    // FIRST 피벗 전략 시간 측정
    for (int i = 0; i < stepCount; i++) {
        auto start = std::chrono::steady_clock::now();

        quickSortF.sort(datasetsF[i], false);

        auto end = std::chrono::steady_clock::now();

        double sortTimeMs =
            std::chrono::duration<double, std::milli>(end - start).count();

        results.push_back(ExperimentResult(
            Algorithm::QUICK_SORT_FIRST,
            datasetsF[i].size(),
            sortTimeMs
        ));
    }

    // MIDDLE 피벗 전략 시간 측정
    for (int i = 0; i < stepCount; i++) {
        auto start = std::chrono::steady_clock::now();

        quickSortM.sort(datasetsM[i], false);

        auto end = std::chrono::steady_clock::now();

        double sortTimeMs =
            std::chrono::duration<double, std::milli>(end - start).count();

        results.push_back(ExperimentResult(
            Algorithm::QUICK_SORT_MIDDLE,
            datasetsM[i].size(),
            sortTimeMs
        ));
    }

    // LAST 피벗 전략 시간 측정
    for (int i = 0; i < stepCount; i++) {
        auto start = std::chrono::steady_clock::now();

        quickSortL.sort(datasetsL[i], false);

        auto end = std::chrono::steady_clock::now();

        double sortTimeMs =
            std::chrono::duration<double, std::milli>(end - start).count();

        results.push_back(ExperimentResult(
            Algorithm::QUICK_SORT_LAST,
            datasetsL[i].size(),
            sortTimeMs
        ));
    }

    return results;
}
