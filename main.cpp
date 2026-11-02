#include "DataGenerator.h"
#include "InsertionSort.h"
#include "QuickSort.h"
#include "ExperimentResult.h"
#include "PerformanceTester.h"


#include<iostream>
#include<vector>

using std::vector;
using std::cout, std::endl;

int main() {

    // ===================================
    // Test DataGenerator
    // ===================================
    DataGenerator dataGenerator;


    vector<int> testData = dataGenerator.generate(50, 1, 1000);
    vector<vector<int>> testDatasets = dataGenerator.generateExperiment(50, 5, 50);


    cout << "===== Test DataGenerator =====" << endl;
    cout << endl;

    cout << "----- Test DataGenerator::generate() -----" << endl;
    cout << "dataSize: " << testData.size() << endl;
    cout << "testData: " << endl;
    cout << "{ ";
    for (int i : testData) {
        cout << i << " ";
    }
    cout << "}" << endl;
    cout << endl;

    cout << "----- Test DataGenerator::generateExperiment() -----" << endl;
    for (int i = 0; i < testDatasets.size(); i++) {
        cout << "dataset[" << i << "]  dataSize: " << testDatasets[i].size() << endl;
        cout << "{ ";
        for (int j = 0; j < testDatasets[i].size(); j++) {
            cout << testDatasets[i][j] << " ";
        }
        cout << "}" << endl;
        cout << endl;
    }
    cout << endl;

    // ===================================
    // Test SortingAlgorithm
    // ===================================
    InsertionSort insertionSort;

    QuickSort quickSortF(QuickSort::PivotStrategy::FIRST);
    QuickSort quickSortM(QuickSort::PivotStrategy::MIDDLE);
    QuickSort quickSortL(QuickSort::PivotStrategy::LAST);


    vector<int> copyDataI = testData;
    
    vector<int> copyDataQ1 = testData;
    vector<int> copyDataQ2 = testData;
    vector<int> copyDataQ3 = testData;

    insertionSort.sort(copyDataI);

    quickSortF.sort(copyDataQ1);
    quickSortM.sort(copyDataQ2);
    quickSortL.sort(copyDataQ3);

    cout << "===== Test SortingAlgorithm =====" << endl;
    cout << "dataSize: " << testData.size() << endl;
    cout << endl;

    cout << "----- Test InsertionSort -----" << endl;
    cout << "InsertionSort result: " << endl;
    cout << "{ ";
    for (int i : copyDataI) {
        cout << i << " ";
    }
    cout << "}" << endl;
    cout << endl;

    cout << "----- Test QuickSort -----" << endl;
    cout << "pivotStrategy: FIRST" << endl;
    cout << "{ ";
    for (int i : copyDataQ1) {
        cout << i << " ";
    }
    cout << "}" << endl;
    cout << endl;

    cout << "pivotStrategy: MIDDLE" << endl;
    cout << "{ ";
    for (int i : copyDataQ2) {
        cout << i << " ";
    }
    cout << "}" << endl;
    cout << endl;

    cout << "pivotStrategy: LAST" << endl;
    cout << "{ ";
    for (int i : copyDataQ3) {
        cout << i << " ";
    }
    cout << "}" << endl;
    cout << endl;

    // ===================================
    // Test PerformaceTester
    // ===================================
    PerformanceTester performanceTester;

    vector<ExperimentResult> result =
        performanceTester.benchmarkSorts(5000, 10, 5000);

    cout << "===== Test PerformaceTester =====" << endl;
    // 값 변경시 수정 필요
    cout << "startSize: 5000" << endl;
    cout << "stepCount: 10" << endl;
    cout << "sizeGap: 5000" << endl;
    cout << endl;

    for (int i = 0; i < result.size(); i++) {
        switch (result[i].getAlgorithm()) {
        case Algorithm::INSERTION_SORT:
            cout << "algorithm: INSERTION_SORT" << endl;
            break;
        case Algorithm::QUICK_SORT_FIRST:
            cout << "algorithm: QUICK_SORT_FIRST" << endl;
            break;
        case Algorithm::QUICK_SORT_MIDDLE:
            cout << "algorithm: QUICK_SORT_MIDDLE" << endl;
            break;
        case Algorithm::QUICK_SORT_LAST:
            cout << "algorithm: QUICK_SORT_LAST" << endl;
            break;
        }
        cout << "dataSize: " << result[i].getDataSize() << endl;
        cout << "sortTimeMs: " << result[i].getSortTimeMs() << endl;
        cout << endl;
    }



    return 0;
}