This plugin use the Gammu library (http://wammu.eu/) v1.33.

Here are the instructions on how to get it.

CMake must be installed.

------------------------------------------------
- Windows (and MSVC10) : build Gammu from sources

1 - Download the Gammu library (sources version) here : http://wammu.eu/download/gammu/source/
2 - Uncompress downloaded file.
3 - Open a console in the uncompressed folder, and run cmake :
   cmake CMakeLists.txt
4 - Open Gammu.sln with MSVC
5 - It seems that CMake doesn't generate well a call to message compiler command line. So to correct it :
6.1 - In the "gsmsd" project, open smsd-event.h.rule properties (in the CMake Rules folder)
6.2 - In the command line property of the custom generation tool, and "mc " before the second line, so this line become like :
6.3 - Do the same for "gammu-smsd" project
mc smsd-event.mc -r {Your_path_to_gammu}/gammu-1.33.0/gammu-1.33.0/smsd -h {Your_path_to_gammu}/gammu-1.33.0/gammu-1.33.0/smsd
7 - Build the solution (for the Debug and Release configurations)
8 - After Gammu is built, don't forget to set it's path in smsDialer (in your CMakeListsUserConfig.txt file).

------------------------------------------------
- Linux (Ubuntu)

1 - Install gammu packages :
   sudo apt-get install libgammu7 libgammu-dev

------------------------------------------------
- RaspberryPi (cross-compilation on Ubuntu)

1 - Download the Gammu library (sources version) here : http://wammu.eu/download/gammu/source/
2 - Uncompress downloaded file.
3 - copy provided Toolchain-raspberryPi.cmake into {Your_path_to_gammu}/cmake
4 - edit Toolchain-raspberryPi.cmake to update the PATH_TO_RASPBERRY_TOOLCHAIN variable
5 - Fix the tool chain, edit {Your_path_to_raspberry_toolchain}/arm-bcm2708/arm-bcm2708-linux-gnueabi/arm-bcm2708-linux-gnueabi/sysroot/usr/lib/lobc.so to remove hard-coded paths :
   GROUP ( /lib/libc.so.6 /usr/lib/libc_nonshared.a  AS_NEEDED ( /lib/ld-linux.so.3 ) )
   ==>
   GROUP ( libc.so.6 libc_nonshared.a  AS_NEEDED ( ld-linux.so.3 ) )
6 - Go to gammu folder, create target dir :
   mkdir raspberry
   cd raspberry
7 - Build makefile :
   cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-raspberryPi.cmake -DBUILD_SHARED_LIBS=OFF -DWITH_MySQL=OFF -DWITH_Postgres=OFF -DWITH_Iconv=OFF -DWITH_CURL=OFF
8 - Build Gammu :
   make
   

