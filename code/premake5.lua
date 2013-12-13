-- solution
solution "pnp-test-framework"
  configurations { "Debug", "Release" }
  defines { "_CRT_SECURE_NO_WARNINGS" }
  flags
  {
    "Symbols",
    "NoMinimalRebuild", 
    "NoEditAndContinue", 
    "No64BitChecks",
    "FatalWarnings",
    "NoIncrementalLink",
    "NoManifest"
  }
  floatingpoint "Fast"
  language "C++"
  location "../project"
  vectorextensions "SSE2"
  warnings "Extra"
  
  configuration "Debug"
    defines { "_DEBUG" }
    targetsuffix "_Debug"
  configuration "Release"
    defines { "NDEBUG", "_RELEASE" }
    flags { "StaticRuntime" }
    optimize "Speed"
  configuration ""

  configuration "gmake"
    buildoptions { "-std=c++11" }
  configuration ""
  
-- project
project "test-framework"
  includedirs { "." }
  files { "**.hpp", "**.cpp" }
  kind "ConsoleApp"
  location "../project"
  targetdir "../bin/"