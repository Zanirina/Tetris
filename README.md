# 🧱 Tetris PBR Prototype

C++ OpenGL project using Physically Based Rendering (PBR)
(no normal or displacement maps)

Developed for the Computer Graphics Fundamentals course.
Implements a prototype of a 3D-rendered Tetris game using OpenGL, GLFW, GLM, and GLAD.

📁 Project Structure

tetris/ 

├─ CMakeLists.txt

├─ src/

│   ├─ main.cpp

│   ├─ Renderer.cpp / Renderer.h

│   ├─ Shader.cpp / Shader.h

│   ├─ Game.cpp / Game.h

├─ shaders/

│   ├─ pbr.vs

│   └─ pbr.fs

└─ README.md

⚙️ Setup & Build Instructions
🧰 Requirements

C++17 or higher

CMake ≥ 3.20

vcpkg package manager

GLFW, GLAD, GLM

Either:

✅ CLion

✅ Visual Studio 2022

1️⃣ Clone Repository
git clone https://github.com/Zanirina/Tetris
cd Tetris

2️⃣ Install vcpkg (if not installed)
git clone https://github.com/microsoft/vcpkg.git

cd vcpkg

.\bootstrap-vcpkg.bat

cd ..

3️⃣ Install Dependencies

vcpkg install glfw3:x64-windows glad:x64-windows glm:x64-windows

vcpkg integrate install


💡 “integrate install” makes Visual Studio and CMake automatically use vcpkg packages.

### 🧠 Option A — Building with Visual Studio 2022

Open Visual Studio

Choose "Open Folder" and select your project folder (tetris/)

VS should automatically detect CMakeLists.txt

Open CMake Settings (gear icon)

Add this line in CMake command arguments:

"-DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"


Build the project (Ctrl + Shift + B)

Run (Ctrl + F5)

⚠️ If you see a black window, check that:

Folder shaders/ is in the project root

The working directory in “Debugging” settings points to the project folder (so the shader files are found)

### 🧩 Option B — Building with CLion

Open folder in CLion

Go to
File → Settings → Build, Execution, Deployment → CMake
Add:

-DCMAKE_TOOLCHAIN_FILE=C:/Users/<you>/vcpkg/scripts/buildsystems/vcpkg.cmake


Set Build Type to Release

Check Run → Edit Configurations → Working Directory
→ must be C:\Users\<you>\CLionProjects\tetris

Click ▶ Run

### ▶️ Controls

Key	Action
←	Move piece left

→	Move piece right

↓	Move piece down faster

Space	Hard drop

### 🧠 Current Prototype Features

✅ Basic rendering loop

✅ Perspective camera and 3D cubes

✅ Simple Physically Based Rendering (PBR) lighting

✅ Basic keyboard control for piece movement

✅ Static grid system


### 💡 Next Steps (for final version)

Collision detection & line clearing

Dynamic materials for different shapes

Score & UI system

Game over detection

Optional texture-based PBR (roughness/metallic maps)

### 👩‍💻 Authors

Zarina Beketova — Lead Developer
Mainly responsible for the implementation of the project in C++ and OpenGL, including setting up the rendering loop, PBR lighting, and Tetris game logic.
Also contributed to the project report, documentation, and presentation materials.

Danial Yerzhigit — Developer & Co-author
Assisted in debugging, code optimization, and improving shader configurations.
Worked together on the project documentation, report, and presentation.
### 🏫 Course Info

Subject: Computer Graphics Fundamentals
Project: “Tetris Game using Physically Based Rendering (without normal & displacement maps)”
Instructor: Dilnaz Omarova