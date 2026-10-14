#include "QuickSort.h"

using std::vector;

// 피봇 선택 전략을 결정한다.
QuickSort::QuickSort(PivotStrategy pivotStrategy) 
    : pivotStrategy(pivotStrategy) 
{}

// 정렬 알고리즘을 공통 인터페이스로 다루기 위한 진입점 역할을 한다.
void QuickSort::sort(vector<int>& data) {
    int left = 0;
    int right = data.size() - 1;

    quickSort(data, left, right);
}

// 실제 재귀 정렬을 수행하고, 원본 배열을 수정한다.
void QuickSort::quickSort(std::vector<int>& data, int left, int right) {
    if (left < right) {
        int p = partition(data, left, right);

        quickSort(data, left, p - 1);
        quickSort(data, p + 1, right);
    }
}

int QuickSort::partition(vector<int>& data, int left, int right) {
    /*
     * pivot 선택 전략(첫 번째 값, 중앙값, 마지막 값)에 따라,
     * pivot의 위치가 다르므로 pivot의 위치를 left로 통일하기 위해
     * pivot이 중앙값, 마지막 값일 경우 left와 pivot을 swap한다.
     */

    // pivot 선택 전략이 첫 번째 값일 경우
    if (pivotStrategy == PivotStrategy::FIRST) {
        events.push_back(SortEvent(EventType::PIVOT, left, -1));
    }
    
    // pivot 선택 전략이 중앙값일 경우
    else if (pivotStrategy == PivotStrategy::MIDDLE) {
        // 오버플로우 예방
        int mid = left + (right - left) / 2;
        /* 
         * 현재 정렬 대상 구간의 
         * 첫 번째 인덱스, 중앙 인덱스, 마지막 인덱스 3개의 값 중
         * 크기가 중앙인 값을 pivot으로 선택하는 전략.
         */

        // mid가 중앙값인 경우
        if ((data[left] <= data[mid] && data[mid] <= data[right]) ||
            (data[right] <= data[mid] && data[mid] <= data[left])) {
            // 피봇 결정 이벤트 생성
            events.push_back(SortEvent(EventType::PIVOT, mid, -1));

            swap(data, mid, left);
            events.push_back(SortEvent(EventType::SWAP, mid, left));
        }
        
        // right가 중앙값인 경우
        else if ((data[left] <= data[right] && data[right] <= data[mid]) ||
                 (data[mid] <= data[right] && data[right] <= data[left])) {
            events.push_back(SortEvent(EventType::PIVOT, right, -1));

            swap(data, right, left);
            events.push_back(SortEvent(EventType::SWAP, right, left));
        }

        // left가 중앙값인 경우
        else {
            events.push_back(SortEvent(EventType::PIVOT, left, -1));
        }
    }

    // pivot 선택 전략이 마지막 값일 경우
    else {
        events.push_back(SortEvent(EventType::PIVOT, right, -1));
        
        swap(data, right, left);
        events.push_back(SortEvent(EventType::SWAP, right, left));
    }

    /*
     * 피벗을 최종 위치에 두었을 때, 
     * 왼쪽은 모두 pivot보다 작고
     * 오른쪽은 모두 pivot 이상이어야 한다.
     */
    int low = left + 1;
    int high = right;
    int pivot = data[left];

    // 원소의 값이 pivot의 값과 같다면, pivot 기준 오른쪽으로 이동.
    while (low <= high) {
        // low는 pivot과 비교하여 크거나 같운 값을 찾음
        while (low <= high) {
            events.push_back(SortEvent(EventType::LOW, low, -1));
            events.push_back(SortEvent(EventType::COMPARE, low, left));
            
            if (data[low] >= pivot) break;
            low += 1;
        }

        // high pivot과 비교하여 작은 값을 찾음
        while (low <= high) {
            events.push_back(SortEvent(EventType::HIGH, high, -1));
            events.push_back(SortEvent(EventType::COMPARE, high, left));

            if (data[high] < pivot) break;
            high -= 1;
        }

        // 인덱스가 교차되지 않았다면 교환
        if (low < high) {
            swap(data, low, high);
            events.push_back(SortEvent(EventType::SWAP, low, high));
            
            low += 1;
            high -= 1;
        }
    }
    
    // 분할후, pivot 위치 확정
    if (left != high) {
        swap(data, left, high);
        events.push_back(SortEvent(EventType::SWAP, left, high));
    }
    events.push_back(SortEvent(EventType::SORTED, high, -1));

    return high;
}

void QuickSort::swap(vector<int>& data, int index1, int index2) {
    int temp = data[index1];
    data[index1] = data[index2];
    data[index2] = temp;
}
