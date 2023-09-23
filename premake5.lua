IG_ROOT = os.getenv("IG_ROOT")

workspace "image2igb"
	kind "ConsoleApp"
	language "C++"
	system "Windows"
	systemversion "latest"
	characterset "MBCS"
	location "build"
	configurations { "Debug", "Release" }
	targetdir "bin/%{cfg.buildcfg}"
	
	filter "configurations:Debug"
		libdirs { path.join(IG_ROOT, "DirectX9/libdbg") }
		defines { "WIN32", "_DEBUG", "_CONSOLE", "IG_ALCHEMY_DLL", "IG_TARGET_WIN32", "IG_GFX_DX9", "IG_COMPILER_MSVC", "IG_DEBUG", "IG_MEMORY_TRACKING" }
		symbols "On"
	filter {}
	
	filter "configurations:Release"
		libdirs { path.join(IG_ROOT, "DirectX9/lib") }
		defines { "WIN32", "NDEBUG", "_CONSOLE", "IG_ALCHEMY_DLL", "IG_TARGET_WIN32", "IG_GFX_DX9", "IG_COMPILER_MSVC" }
		optimize "On"
	filter {}
	
	includedirs { path.join(IG_ROOT, "include") }
	
	links { "libIGCore", "libIGGfx", "libIGSg", "libIGAttrs" }
   
	files { "src/*.*" }
   
project "image2igb"