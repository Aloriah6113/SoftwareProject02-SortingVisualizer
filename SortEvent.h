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
    LOW,     // Low 포인터 위치 갱신
    HIGH,    // High 포인터 위치 갱신
    SWAP,    // 두 값을 교환
    SORTED,  // 정렬 완료된 원소 표시 (피봇 확정 등)
};

class SortEvent {
public: 
    SortEvent(EventType eventType, int index1, int index2)  // COMPARE, INSERT에서 -1일 경우 key값을 의미(삽입 정렬)
        : eventType(eventType), index1(index1), index2(index2) {}

    // GUI의 이벤트 재생에 필요한 읽기 전용 접근자 (기존 필드·생성자 유지)
    EventType getEventType() const { return eventType; }
    int getIndex1() const { return index1; }
    int getIndex2() const { return index2; }

private:
    EventType eventType;
    int index1;
    int index2;
};