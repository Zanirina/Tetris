# 🧱 Tetris PBR Prototype

C++ OpenGL project using Physically Based Rendering (PBR)  
*(no normal or displacement maps)*

Developed for the **Computer Graphics Fundamentals** course.  
Implements a prototype of a 3D-rendered Tetris game using **OpenGL**, **GLFW**, **GLM**, and **GLAD**.

---

## 📁 Project Structure

tetris/

├─ CMakeLists.txt

├─ src/

│ ├─ main.cpp

│ ├─ Renderer.cpp / Renderer.h

│ ├─ Shader.cpp / Shader.h

│ ├─ Game.cpp / Game.h

├─ shaders/


│ ├─ pbr.vs

│ └─ pbr.fs

└─ README.md


## ⚙️ Setup & Build Instructions

### 🧰 Requirements

- **C++17** or higher
- **CMake ≥ 3.20**
- **vcpkg package manager**
- Libraries: GLFW, GLAD, GLM

Supported IDEs:
- ✅ CLion
- ✅ Visual Studio 2022

---

## 🪄 Step 1 — Dependency Setup (via vcpkg)

💡 The most reliable and cross-platform method.  
 Works on **Windows, Linux, and macOS**.

Open **PowerShell** (or terminal) and run:


### Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg

### Bootstrap the tool
.\bootstrap-vcpkg.bat

### (Optional but recommended)
.\vcpkg integrate install

### Install necessary packages
.\vcpkg install glfw3 glm stb

### You can also add GLAD if needed
.\vcpkg install glad
This installs all required OpenGL dependencies.
integrate install automatically links vcpkg with Visual Studio and CMake.

---
## 🧠 Step 2 — Clone the Project
git clone https://github.com/Zanirina/Tetris
cd Tetris

---
## 🧩 Step 3A — Build with CLion (Recommended)
Open the tetris/ folder in CLion.

Go to
File → Settings → Build, Execution, Deployment → CMake

Add the following CMake option:


-DCMAKE_TOOLCHAIN_FILE=C:/Users/<you>/vcpkg/scripts/buildsystems/vcpkg.cmake

Set Build Type → Release


Check Run → Edit Configurations → Working Directory →
must be C:\Users\<you>\CLionProjects\tetris

Build the project and click ▶ Run

---
## 🧠 Step 3B — Build with Visual Studio 2022
Open Visual Studio

Choose “Open Folder” and select your tetris/ project

VS will detect CMakeLists.txt automatically

Open CMake Settings (gear icon ⚙️)

Add this line to CMake command arguments:


-DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
Build with Ctrl + Shift + B

Run with Ctrl + F5

### ⚠️ If you see a black or blue window:

Ensure the shaders/ folder exists in the project root

Make sure the working directory in debugging settings points to the project folder

### ▶️ Controls
Key	Action
←	Move piece left
→	Move piece right
↓	Move piece down faster
Space	Hard drop

### 🧠  Current Prototype Features
✅ Basic rendering loop

✅ Perspective camera and 3D cubes

✅ Simple Physically Based Rendering (PBR) lighting

✅ Basic keyboard control for piece movement

✅ Static grid system

### 💡 Next Steps (Planned Improvements)
Collision detection & line clearing

Dynamic materials for different shapes

Score & UI system

Game over detection

Optional texture-based PBR (roughness/metallic maps)

## 👩‍💻 Authors
>Zarina Beketova — Lead Developer
>Implemented the main C++ and OpenGL logic
>
>Developed the rendering loop, PBR lighting, and Tetris game mechanics
>
>Set up vcpkg, configured CMake, and handled shader management
>
>Contributed to the project report, documentation, and presentation materials

>Danial Yerzhigit — Developer & Co-author
Assisted with debugging, code optimization, and shader configuration fixes
>
>Helped integrate minor gameplay adjustments
>
>Collaborated on documentation, report, and presentation preparation

## 🏫 Course Info
Subject: Computer Graphics Fundamentals

Project: “Tetris Game using Physically Based Rendering (without normal & displacement maps)”

Instructor: Dilnaz Omarova


