-- LuminaExternal.lua

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["glm"] = "../Dependencies/glm"
IncludeDir["spdlog"] = "../Dependencies/spdlog/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["OpenGL"] = "opengl32.lib"
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"

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