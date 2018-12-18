workspace "schema"
  architecture "x64"

  configurations {
    "debug",
    "release",
    "dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "schema"
  location "schema"
  kind "StaticLib"
  language "C++"
  cppdialect "C++17"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

  files {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs {
    -- "%{prj.name}/vendor/spdlog/include"
  }

  filter "system:windows"
    staticruntime "On"
    systemversion "latest"

  filter "configurations:debug"
    defines "SC_DEBUG"
    symbols "On"

  filter "configurations:release"
    defines "SC_RELEASE"
    optimize "On"

  filter "configurations:dist"
    defines "SC_DIST"
    optimize "On"

project "sandbox"
  location "sandbox"
  kind "ConsoleApp"
  cppdialect "C++17"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

  files {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs {
    "schema/src"
  }

  links {
    "schema"
  }

  filter "system:windows"
    staticruntime "On"
    systemversion "latest"

  filter "configurations:debug"
    defines "SC_DEBUG"
    symbols "On"

  filter "configurations:release"
    defines "SC_RELEASE"
    optimize "On"

  filter "configurations:dist"
    defines "SC_DIST"
    optimize "On"
