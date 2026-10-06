#pragma once

enum class EventType {
    SELECT,  // 값 선택
    COMPARE, // 두 값을 비교
    MOVE,    // 값을 다른 위치로 이동
    INSERT,  // 값을 특정한 위치에 삽입
    SWAP,    // 두 값을 교환
    PIVOT,   // Pivot 선택
    SORTED,  // 정렬 완료된 원소 표시
};

class SortEvent {
public:
    SortEvent(
        EventType eventType,
        int index1, 
        int index2
    )
        : eventType(eventType),
          index1(index1),
          index2(index2)
    {}

private:
    EventType eventType;
    int index1;
    int index2;
};