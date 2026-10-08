#include "InsertionSort.h"

using std::vector;

/*
 * 삽입 정렬의 애니메이션을 swap처럼 구현할 수도 있지만,
 * 실제로는 shift -> insert의 구조이므로,
 * 애니메이션도 shift와 insert를 구현한다.
 */
 // 삽입 정렬 알고리즘을 실행하고, 원본 배열을 수정한다.
void InsertionSort::sort(vector<int>& data) {
    for (int i = 1; i < data.size(); i++) {
        int key = data[i];  // i는 key 값의 위치
        events.push_back(SortEvent(EventType::SELECT, i, -1));
        int index = i - 1;  // index가 정렬된 부분의 끝을 가리키도록 초기화
        
        events.push_back(SortEvent(EventType::COMPARE, -1, index));
        while (index >= 0 && data[index] > key) {
            data[index + 1] = data[index];
            events.push_back(SortEvent(EventType::MOVE, index, index + 1));
            index -= 1;

            if (index >= 0)
                events.push_back(SortEvent(EventType::COMPARE, -1, index));
        }
        data[index + 1] = key;
        events.push_back(SortEvent(EventType::INSERT, -1, index + 1));
    }
}
