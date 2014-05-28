This plugin use the Gammu library (http://wammu.eu/) (v1.33 minimum).

Here are the instructions on how to get it.

CMake must be installed.

------------------------------------------------
- Windows (and MSVC10) : build Gammu from sources

1 - Download the Gammu library (sources version) here : http://wammu.eu/download/gammu/source/
2 - Unzip downloaded file.
3 - Patch the sources (see below).
4 - Open a console in the unzip folder, and run cmake :
   cmake CMakeLists.txt
5 - Open Gammu.sln with MSVC
6 - It seems that CMake doesn't generate well a call to message compiler command line. So to correct it :
7.1 - In the "gsmsd" project, open smsd-event.h.rule properties (in the CMake Rules folder)
7.2 - In the command line property of the custom generation tool, and "mc " before the second line, so this line become like :
7.3 - Do the same for "gammu-smsd" project
mc smsd-event.mc -r {Your_path_to_gammu}/gammu-1.33.0/gammu-1.33.0/smsd -h {Your_path_to_gammu}/gammu-1.33.0/gammu-1.33.0/smsd
8 - Build the solution (for the Debug and Release configurations)
9 - After Gammu is built, don't forget to set it's path in smsDialer (in your CMakeListsUserConfig.txt file).

------------------------------------------------
- Linux (Ubuntu) : don't install gammu package, build it from sources (because it needs patch)

1 - Download the Gammu library (sources version) here : http://wammu.eu/download/gammu/source/
2 - Unzip downloaded file.
3 - Patch the sources (see below).
4 - build Gammu :
   make
9 - After Gammu is built, don't forget to set it's path in smsDialer (in your CMakeListsUserConfig.txt file, and set USE_PKGCONFIG to OFF)).


1 - Install gammu packages :
   sudo apt-get install libgammu7 libgammu-dev

------------------------------------------------
- RaspberryPi (cross-compilation on Ubuntu)

1 - Download the Gammu library (sources version) here : http://wammu.eu/download/gammu/source/
2 - Uncompress downloaded file.
3 - Patch the sources (see below).
4 - copy provided Toolchain-raspberryPi.cmake into {Your_path_to_gammu}/cmake
5 - edit Toolchain-raspberryPi.cmake to update the PATH_TO_RASPBERRY_TOOLCHAIN variable
6 - Fix the tool chain, edit {Your_path_to_raspberry_toolchain}/arm-bcm2708/arm-bcm2708-linux-gnueabi/arm-bcm2708-linux-gnueabi/sysroot/usr/lib/lobc.so to remove hard-coded paths :
   GROUP ( /lib/libc.so.6 /usr/lib/libc_nonshared.a  AS_NEEDED ( /lib/ld-linux.so.3 ) )
   ==>
   GROUP ( libc.so.6 libc_nonshared.a  AS_NEEDED ( ld-linux.so.3 ) )
7 - Go to gammu folder, create target dir :
   mkdir raspberry
   cd raspberry
8 - Build makefile :
   cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-raspberryPi.cmake -DBUILD_SHARED_LIBS=ON -DWITH_MySQL=OFF -DWITH_Postgres=OFF -DWITH_Iconv=OFF -DWITH_CURL=OFF
9 - Build Gammu :
   make
   

================================================
- Patch Gammu (to support phone power ON/OFF function)

1 - you have to apply 2 patches (available at https://drive.google.com/folderview?id=0B3dXqBwTbfcAYTVjSmY1MXcxbWc&usp=sharing).
2 - Apply patches :
   patch < 0001-Add-phone-power-ON-OFF-function.patch
   patch < 0002-Fix-linux-build.patch
