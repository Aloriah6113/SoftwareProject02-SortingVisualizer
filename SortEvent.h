#pragma once

enum class EventType {
    // --- 공통 ---
    COMPARE, // 두 값을 비교

    // --- Insertion Sort ---
    KEY,     // Key 선택
    MOVE,    // 값을 다른 위치로 이동
    INSERT,  // 값을 특정한 위치에 삽입
    
    // --- Quick Sort ---
    PIVOT,   // Pivot 선택
    SWAP,    // 두 값을 교환
    SORTED,  // 정렬 완료된 원소 표시 (피봇 확정 등)
};

class SortEvent {
public: 
    SortEvent(
        EventType eventType,
        int index1,  // COMPARE, INSERT에서 -1일 경우 key값을 의미(삽입 정렬)
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