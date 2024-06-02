gitVersioningCommand = "git describe --tags --dirty --always"
gitCurrentBranchCommand = "git symbolic-ref -q --short HEAD"

-- Quote the given string input as a C string
function cstrquote(value)
	if value == nil then
		return "\"\""
	end
	result = value:gsub("\\", "\\\\")
	result = result:gsub("\"", "\\\"")
	result = result:gsub("\n", "\\n")
	result = result:gsub("\t", "\\t")
	result = result:gsub("\r", "\\r")
	result = result:gsub("\a", "\\a")
	result = result:gsub("\b", "\\b")
	result = "\"" .. result .. "\""
	return result
end

-- Converts tags in "vX.X.X" format and given revision number Y to an array of numbers {X,X,X,Y}.
-- In the case where the format does not work fall back to padding with zeroes and just ending with the revision number.
-- partscount can be either 3 or 4.
function vertonumarr(value, vernumber, partscount)
	vernum = {}
	for num in string.gmatch(value or "", "%d+") do
		if #vernum < 3 then
			table.insert(vernum, tonumber(num))
		end
	end
	while #vernum < 3 do
		table.insert(vernum, 0)
	end
	if #vernum < partscount then
		table.insert(vernum, tonumber(vernumber))
	end
	return vernum
end

dependencies = {
	basePath = "./deps"
}

function dependencies.load()
	dir = path.join(dependencies.basePath, "premake/*.lua")
	deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		dep = dep:gsub(".lua", "")
		require(dep)
	end
end

function dependencies.imports()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.import()
		end
	end
end

function dependencies.projects()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.project()
		end
	end
end

newoption {
	trigger = "copy-to",
	description = "Optional, copy the EXE to a custom folder after build, define the path here if wanted.",
	value = "PATH"
}

newoption {
	trigger = "dev-build",
	description = "Enable development builds of the client."
}

newoption {
	trigger = "no-check",
	description = "Disable ownership checks."
}

dependencies.load()

workspace "boiii"
	startproject "client"
	location "./build"
	objdir "%{wks.location}/obj"
	targetdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

	configurations {"Debug", "Release"}

	language "C++"
	cppdialect "C++20"

	architecture "x86_64"
	platforms "x64"

	systemversion "latest"
	symbols "On"
	staticruntime "On"
	editandcontinue "Off"
	warnings "Extra"
	characterset "ASCII"

	if _OPTIONS["dev-build"] then
		defines {"DEV_BUILD"}
	end
	
	if _OPTIONS["no-check"] then
		defines {"NO_CHECK"}
	end

	if os.getenv("CI") then
		defines {"CI"}
	end

	flags {"NoIncrementalLink", "NoMinimalRebuild", "MultiProcessorCompile", "No64BitChecks"}

	filter "platforms:x64"
		defines {"_WINDOWS", "WIN32"}
	filter {}

	filter "configurations:Release"
		optimize "Size"
		buildoptions {"/GL"}
		linkoptions {"/IGNORE:4702", "/LTCG"}
		defines {"NDEBUG"}
		flags {"FatalCompileWarnings"}
	filter {}

	filter "configurations:Debug"
		optimize "Debug"
		defines {"DEBUG", "_DEBUG"}
	filter {}

project "common"
	kind "StaticLib"
	language "C++"

	files {"./src/common/**.hpp", "./src/common/**.cpp"}

	includedirs {"./src/common", "%{prj.location}/src"}

	resincludedirs {"$(ProjectDir)src"}

	dependencies.imports()

project "client"
	kind "WindowedApp"
	language "C++"

	targetname "boiii"

	pchheader "std_include.hpp"
	pchsource "src/client/std_include.cpp"

	files {"./src/client/**.rc", "./src/client/**.hpp", "./src/client/**.cpp", "./src/client/resources/**.*"}

	includedirs {"./src/client", "./src/common", "%{prj.location}/src"}

	resincludedirs {"$(ProjectDir)src"}

	links {"common"}

	prebuildcommands {"pushd %{_MAIN_SCRIPT_DIR}", "popd"}

	if _OPTIONS["copy-to"] then
		postbuildcommands {"copy /y \"$(TargetPath)\" \"" .. _OPTIONS["copy-to"] .. "\""}
	end

	dependencies.imports()

group "Dependencies"
	dependencies.projects()
