# GUI System - Terminal-based Browser Engine

![Graph (1)](https://github.com/IIIHSUAN/myWINDOWS/assets/53747303/807bad4f-5d32-4ecf-885f-f155b2f2db35)

[Demo Video - GUI System within a Terminal Display - YouTube](https://www.youtube.com/watch?v=RFosePU5LI8&ab_channel=shibacookie)

### Intro

By this project, explore and get a thorough understanding of GUI system's core concepts such as:
- graphics rendering techniques 
- hardware event handling
- collaboration of apps and systems within sandboxed containers
- standardized design and implementation of system services with APIs
- GUI concepts:
    - window management
    - pointer control
    - real-time animations

So the objective here is straightforward:

> Learn and develop a virtual GUI system within a terminal display, aiming to create a modern browser application with corresponding environment and services.

First, considering the fundamental design philosophy of the system, from a top-down perspective, supplying all functionalities necessary for the graphical interface of the apps; on the other hand, from a bottom-up perspective, integrating input/output and hardware services implemented through APIs from the underlying operating system using a simulation approach, where functionalities are fulfilled at the bottom by the different system cores.

> The goal is to provide an OS-level virtualization and environment for apps that need GUI functions, system services (e.g., Networking, Processes ...), and anything needed for running the browsers and web applications.

#### *But why, why attempt to implement a BROWSER?*

My personal thought is, that the advantage of browsers over native applications lies not only in their integration of system-level services packaged into user-friendly and comprehensive APIs for access but also in their execution within a controlled environment. Application developers need not worry about implementing underlying logic. As browser standards continue to expand, there is no longer a need to master various platform rules and SDKs, thereby increasing cross-platform compatibility and reducing development and maintenance costs. With future advancements in hardware technology and optimizations in systems and browsers, coupled with increasingly comprehensive feature support across platforms comparable to native apps, developing and establishing related ecosystems on browsers becomes highly competitive.

Using a heuristic appoarch, try drawing rough step-by-step ideas for GUI system implementation:

- **Research:** GUI frameworks, graphics rendering, OOP with Composition Patterns, Chromium architecture, JavaScript engine's principles, micro-kernel services, sandboxed environment, cross-platform architectures ...
- **Environment Setup:** Use *make* tools and *Visual Studio* for C++ development environment setup.
- **System Design & Implementation:** Design the layout and components of the virtual GUI system, like Window, App, Event, and prototypes like Div, Span, Image, Button, and Input that resemble DOM Elements. And implement GUI concepts such as: system core, window management, event handling, real-time graphics rendering.
    - **System Core:** Define the behaviour for the system architecture interacting with high-level apps, same-level virtual (simulated) system services, and low-level system infrasrctures.
    - **Graphics Rendering:** Develop low-level graphics rendering methods tailored for terminal displays, focusing on efficiency and compatibility.
    - **Event Handling:** Implement event handling mechanisms to respond to user input, like handling INTRPT from the low-level system and packing it as a meaningful event such as MouseMove, KeyDown, etc., and dispatching it to high-level applications or propagating to descendants, for example, Window::OnResize, Elements::OnClick, and so on.
    - **Sandboxed Container:** To provide browsers with comprehensive and secure execution environments along with the necessary system services, design and implementat of the Sandbox required for executing JavaScript and other independent service tasks that better run in isolated containers, along with the concept of micro-kernel system services (or Daemons).
    - **Custom APIs Definition & Services Integration**: Explicitly define system resource allocation and access rules, establish which functionalities are supported (and not supported) by the system, plan a modular architecture, and finally integrate with low-level system's standardized APIs. For example: data access and caching facilities, system network services, and web standard implementation (with reference to [webkit APIs](https://webkit.org/web-inspector/)).
        :::info
        The browser initially constructs the services it needs from the system's predefined infrastructure. For instance, rendering processes and JavaScript engines are placed into the system's sandbox container during development. Using APIs defined by the container, they integrate with other external service resources. The functionalities that can be effectively invoked are standardized through an API layer (such as -webkit- for self-implemented functionalities). Finally, this API is integrated with W3C standard APIs to enable standard calls, facilitating future development of feature deletion or addition.
        :::
- **Testing & Debugging:** Develop and utilize debugging tools for efficient troubleshooting, and bundle them into a devkit for app development on this system.
- **Optimization and Refinement:** Identify bottlenecks and refine codebase for efficiency and scalability.
- **Documentation:** Document the development process, APIs, and usage guidelines for future reference.
- **Deployment:** Use tools to compile and bundle all essential facilities and environment setup for the public software.
- **Feedback Collection & Iteration:** Gather feedback from users to identify areas for improvement and iterate on the virtual GUI framework accordingly.

And some detailed concepts of browser implementation:

- **Network Resource Management:** Including integrating system network services and issuing HTTP requests, utilizing caching proxy server services, and implementing a cookies indexing database system.
- **Tab Window Management**: Implement tab window UI functionalities, URL search services, and multi-threaded task assignment for worker processes.
- **Parse & Run Web Pages:** Implement a JavaScript engine and execution unit within a closed sandbox container, such as screen rendering processes.
- **Render Web Pages:** Implement GUI infrastructures and GPU-related processes, providing visual component functionalities, and rendering methods, and using APIs for abstraction layers to implement Web standards.
- **Hardware Resource Management:** Including integrating system hardware APIs, managing memory, file systems, and other related tasks.

> Further Discussion: [:arrow_right: : Exploring the JavaScript Engine - shibarashinu](https://hackmd.io/@shibarashinu/r1XdprVCj)

May refer to Google's [Chromium Browser Engine](https://www.chromium.org/Home/) and [v8 JavaScript Engine](https://v8.dev/) as practical development framework references.

For example,

- **Video Element & Media Architecture Design in Chromium**
  ![HJsuH0CfT](https://github.com/IIIHSUAN/myWINDOWS/assets/53747303/e4282637-f980-4497-82a3-789545b9e7df)
  
  > Image Source: Chromium

### Highlights:

Learn from other modern desktop application development frameworks and implement a graphical user interface system for terminals.
Develop a system rendering pipeline and visual component management similar to the workings of a web browser.
In the future, it can be integrated into cross-platform GUI frameworks, such as incorporating the terminal rendering method I implemented into .NET MPAU (Multi-platform App UI).

### Implementation

- **GUI Frameworks:** Designed a graphical user interface system using C++, encompassing the development of applications, windows, and component concepts, establishing rendering pipelines, and integrating Windows input event APIs.
- **System Core:** Developed the core system based on the workings of .NET Framework and JavaScript in web browsers.
- **Browser Infrastructures:** Implemented window screen rendering and animations using concepts from the Document Object Model (DOM) and Cascading Style Sheets (CSS) in web pages.

### Development

Extended from my OOP course's project of *Command-line Interface*:

> Users interact with this system through the terminal. The system comes with a built-in chess app, but it is limited to interacting with the system solely through input strings. Moving chess pieces relies solely on coordinate positioning.

- **Command-Line Interface - Chess App**
  ![HJ2eX8Ozp](https://github.com/IIIHSUAN/myWINDOWS/assets/53747303/0117463a-cac5-41a3-a36f-3d75697e65a5)

Frequently navigating chess games through cumbersome input commands was a non-intuitive and challenging user experience. Consequently, the idea of implementing a GUI system on the terminal – with cursor and mouse-controlled window framework – emerged. Thus, efforts were initiated to rewrite the command-line interface.


- **GUI System - Terminal-based Graphical User Interface**
  ![B12l7U_Mp](https://github.com/IIIHSUAN/myWINDOWS/assets/53747303/7b511212-eaac-4704-96ea-923ef328a69e)
  
  ![H13lm8ufT](https://github.com/IIIHSUAN/myWINDOWS/assets/53747303/c7b6573e-3f36-4420-a252-91fc7b66a3fc)

However, due to its static nature, merely transforming the interface from a command line to a 2D display (effectively retaining the original system's reliance on user input to trigger new output screens, mirroring the core functionality of the command line interface).

- **Comparison of New and Old Architectures**
    - Old Command-line Interface
      ![H1G0ki9Ma](https://github.com/IIIHSUAN/myWINDOWS/assets/53747303/17dba866-5297-4a9b-bfd3-2176c315896a)

    - New GUI System
      ![HJ_VWjczT](https://github.com/IIIHSUAN/myWINDOWS/assets/53747303/78e2b4cb-36b9-4c5b-8adc-1bd22fc85021)

Furthermore, to achieve real-time screen refreshing, apart from relying on events to trigger the refreshing process, or continuously performing rendering tasks using the Polling method, it was also possible to separate the portion responsible for system rendering output from the input processing pipeline, turning it into a new execution unit. This unit can then collaborate with shared data structures such as graphical components to accomplish the task.

And the core concepts for system implementation:

- **Process for Windows/Tabs:** Independent process management for each window/tab.
- **Event Handling:** Events and other trigger processes are asynchronously queued and executed by a single-threaded main program, unifying all queued tasks.
- **Rendering Pipeline:** Parsing and computing DOM x CSS Tree → Forming current layout information to be rendered → Actual screen rendering → Output to target screen buffer.

Nevertheless, the program encountered a race condition while both input and output independent processes simultaneously accessed system-shared data structures. Initially, I considered applying mutex locks to resolve this issue. However, mindful of the pitfalls of excessive mutex usage without a comprehensive plan, which could cause the system difficult to maintain and potentially degrade performance, I began exploring alternative methods to tackle this synchronization problem.

Delving deeper into the core, the *input process* was designed to handle user input events promptly, while the *output process* aimed to render the screen quickly for real-time interactions. Both processes were essential for real-time screen output. Consolidating the code responsible for both processes into the same single-threaded rendering pipeline seemed like a promising solution to address the race condition.

The interesting aspect here lay in the initial approach taken to achieve real-time rendering of visuals. I separated the input and output processes into two distinct threads to accomplish this effect. However, then, due to synchronization issues, I merged them again, which shared many similarities to running JavaScript scripts:

- **Input Process:** This process focused solely on inserting pending events based on various conditions, akin to JavaScript's event queue (Callback queue).
- **Output Process:** Was responsible for continuously executing the main program's tasks in a loop, resembling JavaScript's call stack and event loop. This included:
    1. Program parsing and execution, event dispatching.
    1. Layout parameter computation, screen rendering.
    1. Outputting visuals.

### Issues

- **Tradeoffs:** While this approach could resolve the issue of synchronous race conditions, it also gave rise to other concerns. For instance, when the main *rendering process* was busy, due to the single-threaded nature, rendering tasks may be postponed, resulting in delays. This characteristic mirrors the behavior observed when running JavaScript in webpages.
- **Virtualization:** Similar to the applications of other virtual machines, such as Java's own virtual environment, which enables Java applications to run on different machines, blurring the boundaries between underlying hardware differences. The ultimate goal is to create a unified user and developer experience, thereby fully dominating the Java kingdom of computing platforms.
