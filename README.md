STM32 University Projects - Coursework Solutions
This repository contains solutions for the exercises proposed in my Embedded Systems courses.

⚠️ Important: Library Dependency
These projects rely on shared drivers stored in the Personal_libs folder. If a project does not compile, you must link this folder to your environment:

Right-click on the project -> Select "Properties".

Go to C/C++ General -> Paths and Symbols.

Select the "Source Location" tab.

Click on "Link Folder...".

Folder name: Enter Personal_libs.

Check the box "Link to folder in the file system".

Click "Browse..." and select your local Personal_libs directory.

Click OK -> Apply and Close.



📂 How to Add New Libraries to the Project
If you add a new module (like Delay or JSNSR04T) to the Personal_libs folder, follow these steps to ensure the IDE and the Compiler (GCC) recognize them:

1. Refresh the Project Explorer
After moving or creating files in your Linux file system (ThinkPad), the IDE needs to sync:

Right-click on the Personal_libs folder in the Project Explorer.

Select Refresh (or simply press F5). The new Inc and Src folders should now be visible.

2. Configure Include Paths (CRITICAL)
Even if the files are visible in the explorer, the compiler won't find the .h files unless you explicitly tell it where to look:

Right-click on the project name -> Properties.

Navigate to C/C++ Build -> Settings.

Under the Tool Settings tab, go to: MCU GCC Compiler -> Include paths.

Click the Add... icon (the page with the + symbol).

Click on Workspace..., navigate to the specific Inc folder (e.g., Personal_libs/Delay/Inc), and click OK.

Click Apply and Close.

Note: You must repeat step 2 for every new library module you create to avoid "No such file or directory" errors during compilation.





Project Directory

📂 TP1 (Unit 1)
LED Blink: Basic implementation using External Interrupts (EXTI) to handle push-button inputs, avoiding CPU blocking.



📂 TP2 (Unit 2)
Non-blocking Delays: Timing implementation using the HAL_GetTick() method to allow multitasking.

Bonus Exercise: Found in the folder ending in _3.

Custom Driver: I developed a modular library (del.c and del.h) to encapsulate delay logic for future projects.

Theory: Check Notes.txt within the project folder for the technical Q&A.
