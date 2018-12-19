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
    -- "%{prj.name}/vendor/spdlog"
  }

  filter "system:windows"
    staticruntime "On"
    systemversion "latest"
    sysincludedirs {
      "C:/Program Files/boost/boost_1_68_0"
    }

  filter "system:macosx"
    sysincludedirs {
      -- should use $BOOST_ROOT somehow, or configurable variable
      -- "/usr/local/boost_1_68_0"
      "/usr/local/Cellar/boost/1.68.0/include"
    }

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
    sysincludedirs {
      "C:/Program Files/boost/boost_1_68_0"
    }

  filter "system:macosx"
    sysincludedirs {
      "/usr/local/Cellar/boost/1.68.0/include"
    }

  filter "configurations:debug"
    defines "SC_DEBUG"
    symbols "On"

  filter "configurations:release"
    defines "SC_RELEASE"
    optimize "On"

  filter "configurations:dist"
    defines "SC_DIST"
    optimize "On"
