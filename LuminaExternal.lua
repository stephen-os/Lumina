-- LuminaExternal.lua

IncludeDir = {}
IncludeDir["glm"] = "../Dependencies/glm"

Library = {}
Library["OpenGL"] = "opengl32.lib"

group "Dependencies"
   include "Dependencies/imgui"
   include "Dependencies/glfw"
   include "Dependencies/glad"
group ""

group "Core"
include "Lumina"
group ""