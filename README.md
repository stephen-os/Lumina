# Lumina

**Lumina** is a custom application **starting point** for C++ projects based on **Walnut** by **TheCherno**. The goal of this project is to make it easy to build C++ applications that have a focus on **Computer Graphics**. **Lumina** features a custom OpenGL wrapper that takes insparation from the **Hazel Game Engine**. 

## 💡 Insparation

- **[Walnut](https://github.com/StudioCherno/Walnut)** - Simple application framework.
- **[Hazel](https://github.com/TheCherno/Hazel)** - Youtube series based game engine.  

## 🔧 Technical Dependencies
Tiles is built using several key libraries:

- **[ImGui](https://github.com/ocornut/imgui)** – A GUI library for in-application UI controls.  
- **[ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)** – A file dialog extension for ImGui, allowing easy texture selection.  
- **[GLFW](https://github.com/glfw/glfw)** – Handles window management and input handling.  
- **[Glad](https://github.com/Dav1dde/glad)** – An OpenGL loader that manages API function pointers.  
- **[GLM](https://github.com/g-truc/glm)** – A C++ mathematics library optimized for graphics applications.  
- **[spdlog](https://github.com/gabime/spdlog)** - A fast consol logging library.

### 🛠️ Setup Instructions
1. Clone the repository with all dependencies:
   ```sh
   git clone --recursive https://github.com/stephen-os/Lumina.git
   ```
2. Navigate to the `scripts` folder and run the `Setup` script.
3. Open the `.sln` file in Visual Studio 2017 or later.
4. Build and run the project from Visual Studio.