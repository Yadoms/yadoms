This plugin use the Gammu library (http://wammu.eu/).
At least version 1.34 is required. Download it here as sources : http://fr.wammu.eu/download/gammu/source/

Here are the instructions on how to build it.

CMake must be installed.

------------------------------------------------
- Windows (and MSVC10) : build Gammu from sources

1 - Download the Gammu library (see above)
2 - Open a console in the Gammu folder, and run cmake :
   cmake CMakeLists.txt
3 - Open Gammu.sln with MSVC
4 - It seems that CMake doesn't generate well a call to message compiler command line. So to correct it :
5.1 - In the "gsmsd" project, open smsd-event.h.rule properties (in the CMake Rules folder)
5.2 - In the command line property of the custom generation tool, add "mc " before the second line, so this line become like :
mc smsd-event.mc -r {Your_path_to_gammu}/smsd -h {Your_path_to_gammu}/smsd
5.3 - Do the same for "gammu-smsd" project
6 - Build the solution (for the Debug and Release configurations)
7 - After Gammu is built, don't forget to set it's path in CMakeListsUserConfig.txt file.

------------------------------------------------
- Linux (Ubuntu) :

1 - Download the Gammu library (see above), and extract it
2 - Go to gammu folder, create target dir :
   mkdir build
   cd build
3 - Build makefile :
   cmake .. -DBUILD_SHARED_LIBS=ON
4 - build Gammu :
   make
5 - After Gammu is built, don't forget to set it's path in CMakeListsUserConfig.txt file and set USE_PKGCONFIG to OFF.

------------------------------------------------
- RaspberryPi (cross-compilation on Ubuntu)

1 - Download the Gammu library (see above), and extract it
2 - copy provided Toolchain-raspberryPi.cmake into {Your_path_to_gammu}/cmake
3 - edit Toolchain-raspberryPi.cmake to update the PATH_TO_RASPBERRY_TOOLCHAIN variable
4 - Go to gammu folder, create target dir :
   mkdir raspberry
   cd raspberry
5 - Build makefile :
   cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-raspberryPi.cmake -DBUILD_SHARED_LIBS=ON -DWITH_MySQL=OFF -DWITH_Postgres=OFF -DWITH_Iconv=OFF -DWITH_CURL=OFF
6 - Build Gammu :
   make
