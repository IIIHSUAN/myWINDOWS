# C++ Retro ASCII Browser Engine | C++ Retro 像素瀏覽器引擎

![Graph (1)](https://github.com/IIIHSUAN/myWINDOWS/assets/53747303/807bad4f-5d32-4ecf-885f-f155b2f2db35)

[C++ Retro ASCII Browser Engine | C++ Retro 像素瀏覽器引擎 - YouTube](https://www.youtube.com/watch?v=RFosePU5LI8&ab_channel=shibacookie)

## English Intro

What if the computer's terminal can handle outputting modern multi-tasking desktop applications in retro computer style, or even display modern responsive web pages in a terminal? Will it look great?

Starting from the core of a desktop application system, the concepts of App, Window, and Element are gradually added. With handling Input events, the software can respond to user any input in real time. The terminal that could only do simple std::cin and std::cout suddenly become much more lively.

At present, this software refers to and combines certain basic operating principles of Winform and browsers to design its own desktop application development environment. One task of the software is to provide practical API interfaces for application use, while another is to ensure all basic input and output are well served with operating system's API. The future goal is to let the software function like a browser, at least be able to display correct webpage layouts and run simple JavaScript normally, manage secure sandbox environments and implement some useful system-level functionalities (Ex. Support the browser's web API, access low-level process resources ...).

On one hand, the program actually completes the mission by calling and cooperating with the Windows API. On the other hand, it outputs a "fictitious" screen to the user to pretend there is such a terminal operating system being executed. This software development is actually more like developing an emulator.

Hope simple JavaScript interpreter, network context process, the support for Web Workers, GPU rendering and more exciting functions will be added in the future.

## 中文介紹

若終端機介面支援現代多工桌面應用程式的畫面輸出，或更甚支援顯示現代網頁和互動，那會是什麼畫面?

從桌面應用程式系統的核心開始建起，陸續加入應用程式 (app)、視窗 (window)、元件 (element) 概念，並結合輸入 (input) 事件使軟體可以對使用者的輸入做即時相對應的回應。讓原本只能作簡單 std::cin 和 std::cout 的終端機頓時變得活潑了許多。

而目前此軟體參考並結合 Winform 和瀏覽器的基礎運作原理，設計自己一套上對 App 提供實用 API 接口供開發者使用，下也對作業系統基本的輸入輸出 API 作良好的對應。未來目標希望軟體能如同瀏覽器運行分頁那樣，在應用程式和作業系統之中建立獨立安全的沙箱環境，並定義好所有應用程式、網頁可調用的系統功能 (Ex. 支援瀏覽器的 web API、支持透過呼叫系統存取各功能型進程資源 ... )。

程式一方面透過調用 Windows API 實際完成任務，一方面向使用者輸出「虛構」的畫面，來表現真的好像有這麼一套終端機系統在運作，此軟體開發老實說比較像是一個模擬器 (emulator) 的簡單原理實作。而未來會完成簡易 JavaScript 直譯器、網路功能進程調用和管理、支援 Web Workers、GPU 渲染等功能之實作。

---

more at [coffcat.com](https://coffcat.com)
