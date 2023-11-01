![image](https://github.com/IIIHSUAN/myWINDOWS/assets/53747303/3837b701-ee4b-42a9-9f5b-926fe517d9fe)# C++ Retro ASCII Browser Engine | C++ Retro 像素瀏覽器引擎

[C++ Retro ASCII Browser Engine | C++ Retro 像素瀏覽器引擎 - YouTube](https://www.youtube.com/watch?v=RFosePU5LI8&ab_channel=shibacookie)

## English Intro

What if the computer's terminal can handle outputting modern multi-tasking desktop applications in retro computer style, or even browse modern responsive web pages in a terminal? Will it look great?

Starting from developing the core of a desktop application system, then are App, Window, and Element introduced into. With Input events that come from Windows APIs, the software can respond to user inputs in realtime. This makes the monotonous terminal screen that can only do simple std::cin and std::cout become way more lively.

At present, this software refers to and combines the basic operating principles of C++ Winform and browsers core concept to design a system that can: on one hand it provide practical API for developers to use upon the App; on the other hand it provide decent event handlers for OS APIs underneath this system to properly handle user input and output.

In the future it is a plan for developing sandbox environment based on the system: designing a **sandboxed container** created and managed by the system so that each application can call this through system calls A secure isolated container environment, in which independent computing tasks are performed, such as: JavaScript engine that executes scripts independently for tabs, local caching proxy server service, etc.

With the secure sandbox environment, by defining the API interface, it can interact with the system in the style of API rules, and on the basis of this, we can develop a microkernel-like **services (daemon)**, such as: network Road services, file system access services ... etc.

After building the system infrastructure, we can then implement the browser application developed based on this system architecture. In order to implement the W3C's web standards and support various web compatibility issues, a layer of abstract standard API can be first defined to map the functional methods and services that web pages can use, and then correspond all abstract methods to the actual sandbox's API methods, so as to achieve good independent development architecture. Now here completes the basic framework of a modern browser's core, and then allows us to move on to the next stage of developing internal browser functions.

The browser engine implements all functions related to browsing the web, including:

- **Network Resource Management**: Cooperate with the network function services and local caching proxy server services provided by the system, and complete browser internal network related services with its own cookies, network request resource management ... etc.
- **Tab management**: Implement window's essential functionalities, URL search functions, thread-pool worker management for core-related services ... etc.
- **Read web pages**: Develop a JavaScript engine or apply a ready-made open source project (Ex. Google's v8 engine), place the program in a closed sandbox environment for execution, cooperate with the screen rendering process to output web pages, execute JavaScript and implement web pages Functions such as: Web APIs, DOM management, CSS parsing and pre-, post-processing ... etc.
- **Display screen**: Use various visual components provided by the system for UI displays.
- **Hardware Resource Management**: Provide practical methods to call the corresponding OS hardware API.

## 中文介紹

若能在終端機中操作復古的像素化桌面視窗系統，並在終端機中使用瀏覽器瀏覽網頁、使用 google 搜尋、線上即時聊天、甚至播放 YouTube 影片。如同其他主流瀏覽器那樣如 chrome，那將會是個什麼樣的畫面?

從桌面應用程式系統的核心開始建起，陸續加入應用程式 (App)、視窗 (Window)、元件 (Element) 概念，並結合輸入 (Input) 事件使軟體可以對使用者的輸入做即時相對應的回應。讓原本只能做簡單 std::cin 和 std::cout 的單調終端機畫面變得活潑了許多。

而目前此軟體參考並結合 Winform 和瀏覽器的基礎運作原理，設計一套系統能夠做到: 對其上的 App 提供實用 API 接口供開發者使用；對其下的作業系統 API 提供良好的應對程序以處理使用者的輸入輸出。

未來規劃進階參考瀏覽器執行 JavaScript 引擎的獨立沙箱環境: 設計一個由系統創建和管理的 **沙箱容器 (sandboxed container)**，使每個應用程式都可透過系統呼叫來調用這種安全的隔離容器環境，並在其中執行獨立運算任務如: 各分頁獨立執行腳本的 JavaScript 引擎、本地資源快取 proxy 伺服器服務 ... 等。

有了安全的沙箱環境後，透過制定容器給定的 API 接口與系統交互，可以以此容器為基礎發展 microkernel-like 的實用 **常駐系統功能服務 (daemon)**，如: 網路服務、檔案系統存取服務 ... 等。

建設好系統基礎設施，接下來便可實作基於此系統架構開發的瀏覽器應用程式。為實現由 W3C 所訂定的網路標準和支援各項兼容措施，可先定義一層抽象標準 API 串接網頁和服務的呼叫，再與實際各容器開放的 API 做對應，以此實現良好的分離模組開發架構。如此以來便具備基本的現代瀏覽器基礎框架，可邁入下一步瀏覽器內部功能開發階段。

瀏覽器引擎實現所有跟瀏覽網頁有關的功能，包括:

- **網路資源管理**: 配合系統提供的網路功能服務和本地資源 proxy 伺服器服務，與自身 Cookies 、請求資源管理等完成瀏覽器內部網路相關服務。
- **分頁視窗管理**: 實作視窗功能、URL 搜尋功能、多線程工人管理 ... 等。
- **讀取網頁**: 開發自己一套 JavaScript 引擎或套用現成開源專案 (Ex. Google's v8 引擎)，將程序置於封閉沙箱環境中執行，配合畫面渲染程序輸出網頁畫面，執行 JavaScript 和實現網頁功能如: Web APIs、DOM 管理、CSS 解析前後處理 ... 等。
- **顯示畫面**: 以系統提供的各視覺元件實現 UI 介面顯示。
- **硬體資源管理**: 串接系統硬體 API，並提供實用方法給內部其他服務如 JavaScript 引擎使用。

---

more at [coffcat.com](https://coffcat.com)
