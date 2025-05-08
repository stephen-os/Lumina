-- LuminaExternal.lua

IncludeDir = {}
IncludeDir["glm"] = "../Dependencies/glm"
IncludeDir["spdlog"] = "../Dependencies/spdlog/include"

Library = {}
Library["OpenGL"] = "opengl32.lib"

group "Dependencies"
   include "Dependencies/imgui"
   include "Dependencies/glfw"
   include "Dependencies/glad"
   include "Dependencies/tinygltf"
   include "Dependencies/imguifd"
group ""

group "Core"
   include "Lumina"
group ""