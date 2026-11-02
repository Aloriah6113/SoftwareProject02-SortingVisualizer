#include "VisualizerWidget.h"
#include <algorithm>
#include <string>

namespace {
COLORREF color(int r,int g,int b) { return RGB(r,g,b); }
void fill(HDC dc, RECT r, COLORREF c) { HBRUSH b=CreateSolidBrush(c); FillRect(dc,&r,b); DeleteObject(b); }
void label(HDC dc,int x,int y,const wchar_t* text,COLORREF c) { SetTextColor(dc,c); SetBkMode(dc,TRANSPARENT); TextOutW(dc,x,y,text,static_cast<int>(wcslen(text))); }
}
void VisualizerWidget::load(std::vector<int> values,std::vector<SortEvent> recorded) {
    initial_=std::move(values); events_=std::move(recorded); reset();
}
void VisualizerWidget::reset() {
    values_=initial_; sorted_.assign(values_.size(),false); position_=0;
    key_=0; keyIndex_=-1; markedA_=markedB_=pivot_=-1; last_=EventType::COMPARE;
}
bool VisualizerWidget::step() {
    if (finished()) return false;
    const auto& e=events_[position_++];
    const int a=e.getIndex1(), b=e.getIndex2(), n=static_cast<int>(values_.size());
    auto valid=[n](int i){return i>=0 && i<n;};
    last_=e.getEventType(); markedA_=a; markedB_=b;
    switch(last_) {
    case EventType::KEY: if(valid(a)){key_=values_[a]; keyIndex_=a;} break;
    case EventType::MOVE: if(valid(a)&&valid(b)) values_[b]=values_[a]; break;
    case EventType::INSERT: if(valid(b)) { values_[b]=key_; keyIndex_=-1; markedA_=b; } break;
    case EventType::PIVOT: pivot_=a; break;
    case EventType::SWAP:
        if(valid(a)&&valid(b)) { std::swap(values_[a],values_[b]);
            if(pivot_==a) pivot_=b; else if(pivot_==b) pivot_=a; }
        break;
    case EventType::SORTED: if(valid(a)) sorted_[a]=true; pivot_=-1; break;
    default: break;
    }
    return true;
}
void VisualizerWidget::paint(HDC dc, RECT area) const {
    fill(dc,area,color(19,27,43));
    const int n=static_cast<int>(values_.size());
    if(n==0) {label(dc,area.left+24,area.top+25,L"데이터 생성 버튼을 눌러 시작하세요",color(155,172,196));return;}
    const int left=area.left+20, right=area.right-20, top=area.top+40, bottom=area.bottom-36;
    if(bottom<=top||right<=left) return;
    label(dc,left,area.top+13,L"ARRAY  /  이벤트별 정렬 과정",color(175,196,222));
    const int maxv=*std::max_element(initial_.begin(),initial_.end());
    const double slot=static_cast<double>(right-left)/n;
    for(int i=0;i<n;++i){
        const int x=left+static_cast<int>(i*slot), next=left+static_cast<int>((i+1)*slot);
        const int h=std::max(2,static_cast<int>((bottom-top)*static_cast<double>(values_[i])/std::max(1,maxv)));
        COLORREF c=color(66,143,238);
        if(sorted_[i]) c=color(74,204,155);
        if(i==pivot_) c=color(231,175,80);
        if(i==markedA_||i==markedB_) c=color(234,111,130);
        if(i==keyIndex_) c=color(176,123,247);
        RECT bar{x,bottom-h,std::max(x+1,next-2),bottom}; fill(dc,bar,c);
        if(n<=24){std::wstring number=std::to_wstring(values_[i]);
            label(dc,x+2,bottom+6,number.c_str(),color(193,206,225));}
    }
    RECT baseline{left,bottom,right,bottom+1}; fill(dc,baseline,color(65,81,107));
}
