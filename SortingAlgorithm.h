#pragma once

#include "SortEvent.h"

#include <vector>

// 정렬 알고리즘을 공통 인터페이스로 다루기 위한 추상 클래스
class SortingAlgorithm {
public:
    // 정렬 알고리즘을 실행하고, 원본 배열을 수정한다.
    virtual void sort(std::vector<int>& data) = 0;

    // sortEvent를 참조로 반환한다
    const std::vector<SortEvent>& getEvents() const {
        return sortEvent;
    }

protected:
    // 정렬 과정에서 발생하는 이벤트를 저장한다.
    std::vector<SortEvent> sortEvent;
};
