-- premake5.lua
workspace "LuminaApp"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "LuminaApp"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "LuminaExternal.lua"
include "LuminaApp"