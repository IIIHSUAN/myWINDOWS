workspace "myWINDOWS"
   configurations { "Debug", "Release" }
   architecture "x64"
   startproject "Core"

---------------------------------------------

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

---------------------------------------------

project "myWINDOWS"
   location "myWINDOWS"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-tmp/" .. outputdir .. "/%{prj.name}")
   staticruntime "on"
   files
   {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp"
   }
   includedirs
   {
      "%{prj.name}/src"
   }

   filter "system:windows"
      systemversion "latest"
      
   filter "configurations:Debug"
      runtime "Debug"
      symbols "On"
   filter "configurations:Release"
      runtime "Release"
      optimize "On"