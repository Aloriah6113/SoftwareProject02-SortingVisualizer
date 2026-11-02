#include "MainWindow.h"
#include "../Data/DataGenerator.h"
#include "../Sorting/InsertionSort.h"
#include "../Sorting/QuickSort.h"
#include "../Performance/PerformanceTester.h"
#include <algorithm>
#include <cmath>
#include <string>

namespace {
enum : int { ALGORITHM=101, COUNT, SPEED, GENERATE, PLAY, STEP, RESET, START, STEPS, GAP, BENCH_SORT, BENCH_PIVOT, STATUS };
COLORREF rgb(int r,int g,int b){return RGB(r,g,b);}
void block(HDC dc,RECT r,COLORREF c){HBRUSH b=CreateSolidBrush(c);FillRect(dc,&r,b);DeleteObject(b);}
void text(HDC dc,int x,int y,const wchar_t* s,COLORREF c){SetBkMode(dc,TRANSPARENT);SetTextColor(dc,c);TextOutW(dc,x,y,s,static_cast<int>(wcslen(s)));}
HWND button(HWND p,int id,const wchar_t* caption){return CreateWindowW(L"BUTTON",caption,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,0,0,0,0,p,(HMENU)(INT_PTR)id,nullptr,nullptr);}
HWND field(HWND p,int id,const wchar_t* value){return CreateWindowExW(WS_EX_CLIENTEDGE,L"EDIT",value,WS_CHILD|WS_VISIBLE|ES_NUMBER|ES_CENTER,0,0,0,0,p,(HMENU)(INT_PTR)id,nullptr,nullptr);}
void place(HWND h,int x,int y,int w,int height){MoveWindow(h,x,y,w,height,TRUE);}
const wchar_t* algorithmName(Algorithm a){switch(a){
case Algorithm::INSERTION_SORT:return L"삽입 정렬";
case Algorithm::QUICK_SORT_FIRST:return L"퀵 · FIRST";
case Algorithm::QUICK_SORT_MIDDLE:return L"퀵 · MEDIAN-OF-THREE";
case Algorithm::QUICK_SORT_LAST:return L"퀵 · LAST";
}return L"?";}
COLORREF seriesColor(Algorithm a){switch(a){case Algorithm::INSERTION_SORT:return rgb(239,119,145);
case Algorithm::QUICK_SORT_FIRST:return rgb(104,182,255);
case Algorithm::QUICK_SORT_MIDDLE:return rgb(89,219,169);
case Algorithm::QUICK_SORT_LAST:return rgb(250,195,99);}return RGB(255,255,255);}
}
bool MainWindow::create(HINSTANCE instance,int show){
    WNDCLASSW wc{}; wc.lpfnWndProc=windowProc;wc.hInstance=instance;wc.lpszClassName=L"SortingVisualizerNativeWindow";
    wc.hCursor=LoadCursor(nullptr,IDC_ARROW);wc.hbrBackground=CreateSolidBrush(rgb(13,20,33));
    if(!RegisterClassW(&wc) && GetLastError()!=ERROR_CLASS_ALREADY_EXISTS)return false;
    window_=CreateWindowW(wc.lpszClassName,L"SORTLAB  ·  정렬 알고리즘 시각화",WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,1120,830,nullptr,nullptr,instance,this);
    if(!window_)return false;ShowWindow(window_,show);UpdateWindow(window_);return true;
}
LRESULT CALLBACK MainWindow::windowProc(HWND window,UINT message,WPARAM wp,LPARAM lp){
    auto* self=reinterpret_cast<MainWindow*>(GetWindowLongPtrW(window,GWLP_USERDATA));
    if(message==WM_NCCREATE){self=static_cast<MainWindow*>(reinterpret_cast<CREATESTRUCTW*>(lp)->lpCreateParams);
        self->window_=window;SetWindowLongPtrW(window,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(self));}
    return self?self->handle(message,wp,lp):DefWindowProcW(window,message,wp,lp);
}
void MainWindow::controls(){
    algorithm_=CreateWindowW(L"COMBOBOX",L"",WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL,0,0,0,0,window_,(HMENU)ALGORITHM,nullptr,nullptr);
    for(const wchar_t* n:{L"삽입 정렬",L"퀵 정렬 · FIRST",L"퀵 정렬 · MEDIAN-OF-THREE",L"퀵 정렬 · LAST"})
        SendMessageW(algorithm_,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(n));
    SendMessageW(algorithm_,CB_SETCURSEL,2,0);
    count_=field(window_,COUNT,L"32"); speed_=field(window_,SPEED,L"120");
    generate_=button(window_,GENERATE,L"데이터 생성");play_=button(window_,PLAY,L"▶ 재생");
    step_=button(window_,STEP,L"한 단계");reset_=button(window_,RESET,L"처음으로");
    start_=field(window_,START,L"100");steps_=field(window_,STEPS,L"7");gap_=field(window_,GAP,L"200");
    benchSort_=button(window_,BENCH_SORT,L"정렬 비교");benchPivot_=button(window_,BENCH_PIVOT,L"피벗 비교");
    status_=CreateWindowW(L"STATIC",L"",WS_CHILD|WS_VISIBLE,0,0,0,0,window_,(HMENU)STATUS,nullptr,nullptr);
    HFONT font=CreateFontW(-16,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,DEFAULT_PITCH,L"맑은 고딕");
    // 프로세스 수명 동안 공유하는 GUI 글꼴
    for(HWND h:{algorithm_,count_,speed_,generate_,play_,step_,reset_,start_,steps_,gap_,benchSort_,benchPivot_,status_})
        SendMessageW(h,WM_SETFONT,reinterpret_cast<WPARAM>(font),TRUE);
    updateStatus();
}
void MainWindow::layout(){
    RECT r;GetClientRect(window_,&r);const int w=r.right, m=25;
    place(algorithm_,m,104,246,240);place(count_,m+258,104,75,30);place(speed_,m+344,104,75,30);
    place(generate_,m+432,103,126,32);place(play_,m+568,103,100,32);
    place(step_,m+678,103,90,32);place(reset_,m+778,103,90,32);
    place(start_,m,473,85,29);place(steps_,m+103,473,85,29);place(gap_,m+206,473,85,29);
    place(benchSort_,m+312,472,128,32);place(benchPivot_,m+450,472,128,32);
    place(status_,m,std::max(540,r.bottom-38),std::max(100,w-2*m),26);
    InvalidateRect(window_,nullptr,FALSE);
}
int MainWindow::readNumber(HWND h,int low,int high,int fallback) const{
    wchar_t s[64]{};GetWindowTextW(h,s,64);
    if(!s[0])return fallback;
    wchar_t* end=nullptr;long v=wcstol(s,&end,10);
    if(*end||v<low||v>high)return fallback;
    return static_cast<int>(v);
}
void MainWindow::setPlaying(bool value){playing_=value;
    if(value){delayMs_=readNumber(speed_,1,2000,120);SetTimer(window_,1,delayMs_,nullptr);}else KillTimer(window_,1);
    SetWindowTextW(play_,value?L"⏸ 일시정지":L"▶ 재생");
}
void MainWindow::generate(){
    setPlaying(false);
    const int n=readNumber(count_,2,200,32);
    DataGenerator generator;original_=generator.generate(n,1,100);
    std::vector<int> work=original_;int selected=static_cast<int>(SendMessageW(algorithm_,CB_GETCURSEL,0,0));
    std::vector<SortEvent> events;
    if(selected==0){InsertionSort sorter;sorter.sort(work,true);events=sorter.getEvents();}
    else{QuickSort::PivotStrategy strategy=QuickSort::PivotStrategy::MIDDLE;
        if(selected==1)strategy=QuickSort::PivotStrategy::FIRST;
        if(selected==3)strategy=QuickSort::PivotStrategy::LAST;
        QuickSort sorter(strategy);sorter.sort(work,true);events=sorter.getEvents();}
    view_.load(original_,std::move(events));updateStatus();InvalidateRect(window_,nullptr,FALSE);
}
void MainWindow::runBenchmark(bool pivots){
    setPlaying(false);
    const int start=readNumber(start_,2,5000,100),steps=readNumber(steps_,1,12,7),gap=readNumber(gap_,1,5000,200);
    // 삽입정렬 O(n²)과 동기 실행 중 UI 지연을 제한한다.
    if(start+static_cast<long long>(steps-1)*gap>10000){MessageBoxW(window_,L"마지막 데이터 크기는 10,000 이하로 설정하세요.",L"범위 확인",MB_OK|MB_ICONWARNING);return;}
    SetCursor(LoadCursor(nullptr,IDC_WAIT));
    PerformanceTester tester;results_=pivots?tester.benchmarkPivots(start,steps,gap):tester.benchmarkSorts(start,steps,gap);
    SetCursor(LoadCursor(nullptr,IDC_ARROW));pivotChart_=pivots;updateStatus();InvalidateRect(window_,nullptr,FALSE);
}
void MainWindow::updateStatus(){
    if(!status_)return;
    std::wstring s=L"VISUALIZER   "+std::to_wstring(view_.position())+L" / "+std::to_wstring(view_.total())+L" events";
    if(!results_.empty())s+=L"     ·     PERFORMANCE   "+std::to_wstring(results_.size())+L" measurements";
    SetWindowTextW(status_,s.c_str());
}
void MainWindow::draw(HDC dc){
    RECT r;GetClientRect(window_,&r);block(dc,r,rgb(13,20,33));
    HFONT title=CreateFontW(-30,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,0,0,CLEARTYPE_QUALITY,DEFAULT_PITCH,L"맑은 고딕");
    HGDIOBJ old=SelectObject(dc,title);text(dc,25,23,L"SORTLAB",rgb(235,243,255));SelectObject(dc,old);DeleteObject(title);
    text(dc,205,35,L"정렬 알고리즘  /  이벤트 리플레이 + 성능 분석",rgb(130,156,192));
    text(dc,25,80,L"알고리즘",rgb(160,181,208));text(dc,283,80,L"개수 2–200",rgb(160,181,208));
    text(dc,369,80,L"간격 ms",rgb(160,181,208));
    RECT panel{25,150,std::max(200,r.right-25),436};view_.paint(dc,panel);
    text(dc,25,449,L"PERFORMANCE  /  동일 데이터셋으로 비교",rgb(192,211,236));
    text(dc,25,507,L"시작 크기",rgb(155,176,205));text(dc,128,507,L"실험 횟수",rgb(155,176,205));
    text(dc,231,507,L"증가 폭",rgb(155,176,205));
    int plotTop=560,plotBottom=std::max(600,r.bottom-84),plotLeft=100,plotRight=std::max(250,r.right-145);
    RECT chart{25,535,std::max(200,r.right-25),std::max(620,r.bottom-53)};block(dc,chart,rgb(19,27,43));
    if(results_.empty()){text(dc,45,575,L"위의 비교 버튼을 눌러 실행 시간을 그래프로 확인하세요",rgb(145,166,194));return;}
    double maxTime=0;int minSize=results_.front().getDataSize(),maxSize=minSize;
    for(const auto& e:results_){maxTime=std::max(maxTime,e.getSortTimeMs());minSize=std::min(minSize,e.getDataSize());maxSize=std::max(maxSize,e.getDataSize());}
    maxTime=std::max(maxTime,0.001);
    HPEN grid=CreatePen(PS_SOLID,1,rgb(54,69,91));HGDIOBJ oldPen=SelectObject(dc,grid);
    for(int k=0;k<=4;++k){int y=plotBottom-(plotBottom-plotTop)*k/4;MoveToEx(dc,plotLeft,y,nullptr);LineTo(dc,plotRight,y);
        wchar_t num[60];swprintf_s(num,L"%.2f",maxTime*k/4);text(dc,36,y-9,num,rgb(155,175,200));}
    SelectObject(dc,oldPen);DeleteObject(grid);
    text(dc,plotLeft,plotBottom+10,std::to_wstring(minSize).c_str(),rgb(155,175,200));
    text(dc,plotRight-55,plotBottom+10,std::to_wstring(maxSize).c_str(),rgb(155,175,200));
    text(dc,37,plotTop-21,L"ms",rgb(155,175,200));
    std::vector<Algorithm> algorithms=pivotChart_?
        std::vector<Algorithm>{Algorithm::QUICK_SORT_FIRST,Algorithm::QUICK_SORT_MIDDLE,Algorithm::QUICK_SORT_LAST}:
        std::vector<Algorithm>{Algorithm::INSERTION_SORT,Algorithm::QUICK_SORT_MIDDLE};
    int legendY=plotTop;
    for(auto a:algorithms){
        HPEN pen=CreatePen(PS_SOLID,3,seriesColor(a));oldPen=SelectObject(dc,pen);bool first=true;
        for(const auto& e:results_)if(e.getAlgorithm()==a){
            int x=plotLeft+(plotRight-plotLeft)*(e.getDataSize()-minSize)/std::max(1,maxSize-minSize);
            int y=plotBottom-static_cast<int>((plotBottom-plotTop)*e.getSortTimeMs()/maxTime);
            if(first){MoveToEx(dc,x,y,nullptr);first=false;}else LineTo(dc,x,y);
            HBRUSH dot=CreateSolidBrush(seriesColor(a));HGDIOBJ oldBrush=SelectObject(dc,dot);
            Ellipse(dc,x-3,y-3,x+4,y+4);SelectObject(dc,oldBrush);DeleteObject(dot);MoveToEx(dc,x,y,nullptr);
        }
        SelectObject(dc,oldPen);DeleteObject(pen);
        text(dc,plotRight+13,legendY,algorithmName(a),seriesColor(a));legendY+=27;
    }
}
LRESULT MainWindow::handle(UINT message,WPARAM wp,LPARAM lp){
    switch(message){
    case WM_CREATE:controls();generate();return 0;
    case WM_SIZE:layout();return 0;
    case WM_COMMAND:
        if(HIWORD(wp)==CBN_SELCHANGE && LOWORD(wp)==ALGORITHM){generate();return 0;}
        if(HIWORD(wp)!=BN_CLICKED)return 0;
        switch(LOWORD(wp)){
        case GENERATE:generate();break;
        case PLAY:if(!view_.finished())setPlaying(!playing_);break;
        case STEP:setPlaying(false);view_.step();updateStatus();InvalidateRect(window_,nullptr,FALSE);break;
        case RESET:setPlaying(false);view_.reset();updateStatus();InvalidateRect(window_,nullptr,FALSE);break;
        case BENCH_SORT:runBenchmark(false);break;
        case BENCH_PIVOT:runBenchmark(true);break;
        }return 0;
    case WM_TIMER:
        if(wp==1){if(!view_.step())setPlaying(false);updateStatus();InvalidateRect(window_,nullptr,FALSE);return 0;}break;
    case WM_ERASEBKGND:return 1;
    case WM_PAINT:{PAINTSTRUCT ps;HDC screen=BeginPaint(window_,&ps);RECT r;GetClientRect(window_,&r);
        HDC buffer=CreateCompatibleDC(screen);HBITMAP bitmap=CreateCompatibleBitmap(screen,std::max(1,r.right),std::max(1,r.bottom));
        HGDIOBJ old=SelectObject(buffer,bitmap);draw(buffer);
        BitBlt(screen,0,0,r.right,r.bottom,buffer,0,0,SRCCOPY);
        SelectObject(buffer,old);DeleteObject(bitmap);DeleteDC(buffer);EndPaint(window_,&ps);return 0;}
    case WM_DESTROY:setPlaying(false);PostQuitMessage(0);return 0;
    }return DefWindowProcW(window_,message,wp,lp);
}
