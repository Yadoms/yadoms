This plugin use the Gammu library (http://wammu.eu/).

Here are the instructions on how to build it. After Gammu is built, don't forget to set it's path in your CMakeListsUserConfig.txt.

CMake must be installed.

1 - Download the Gammu library (sources version) here : http://wammu.eu/download/gammu/source/
2 - Uncompress downloaded file.

------------------------------------------------
- Windows (and MSVC10)

1 - Open a console in the uncompressed folder, and run cmake :
   cmake CMakeLists.txt
   
2 - Open Gammu.sln with MSVC
3 - It seems that CMake doesn't generate well a call to message compiler command line. So to correct it :
3.1 - In the "gsmsd" project, open smsd-event.h.rule properties (in the CMake Rules folder)
3.2 - In the command line property of the custom generation tool, and "mc " before the second line, so this line become like :
mc smsd-event.mc -r {Your_path_to_gammu}/gammu-1.33.0/gammu-1.33.0/smsd -h {Your_path_to_gammu}/gammu-1.33.0/gammu-1.33.0/smsd
4 - Build the solution
