project "LuminaApp"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "../Lumina/src",

      "../Dependencies/imgui",
      "../Dependencies/glew/include",
      "../Dependencies/glfw/include",
      "../Dependencies/glm",
      "../Dependencies/glad/include",
      "../Dependencies/tinygltf"
   }

    links
    {
        "Lumina",

        "ImGui",
        "GLFW",
        "Glad",
        "TinyGLTF", 
 
        "opengl32.lib"
    }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"