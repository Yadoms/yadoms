# === Cible : Raspberry Pi 2 (ARMv7 / armhf, cortex-A7) ===
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armv7)

# Toolchain croisée
set(TOOLCHAIN_PREFIX arm-linux-gnueabihf)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
# (optionnel mais utile)
set(CMAKE_C_COMPILER_TARGET   ${TOOLCHAIN_PREFIX})
set(CMAKE_CXX_COMPILER_TARGET ${TOOLCHAIN_PREFIX})

# Sysroot = image montée
set(RPI_SYSROOT "/mnt/raspi")
set(CMAKE_SYSROOT "${RPI_SYSROOT}")

# Où chercher includes/libs (ne pas sortir du sysroot)
set(CMAKE_FIND_ROOT_PATH "${RPI_SYSROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Arch triplet Debian pour armhf
set(CMAKE_LIBRARY_ARCHITECTURE arm-linux-gnueabihf)

# Flags CPU/ABI optimisés pour RPi2 (Cortex-A7, ARMv7, hard-float NEON)
set(RPI_CPU_FLAGS "-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -marm")
# Sécurise l’édition de liens contre les libs du sysroot
set(RPI_LINK_FLAGS "-Wl,-rpath-link,${RPI_SYSROOT}/usr/lib/arm-linux-gnueabihf \
                    -Wl,-rpath-link,${RPI_SYSROOT}/lib/arm-linux-gnueabihf \
                    -Wl,-rpath-link,${RPI_SYSROOT}/usr/lib \
                    -Wl,-rpath-link,${RPI_SYSROOT}/lib")

# Applique les flags
set(CMAKE_C_FLAGS_INIT   "--sysroot=${RPI_SYSROOT} ${RPI_CPU_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "--sysroot=${RPI_SYSROOT} ${RPI_CPU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT   "${RPI_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${RPI_LINK_FLAGS}")

# Si vous utilisez libatomic sur ARM (atomics 64b) :
# list(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " -latomic")
# list(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " -latomic")

# PKG-CONFIG côté cible
set(ENV{PKG_CONFIG_DIR} "")
set(ENV{PKG_CONFIG_SYSROOT_DIR} "${RPI_SYSROOT}")
set(ENV{PKG_CONFIG_LIBDIR} "${RPI_SYSROOT}/usr/lib/arm-linux-gnueabihf/pkgconfig:${RPI_SYSROOT}/usr/lib/pkgconfig:${RPI_SYSROOT}/usr/share/pkgconfig:${RPI_SYSROOT}/lib/arm-linux-gnueabihf/pkgconfig:${RPI_SYSROOT}/lib/pkgconfig")


# Pour que CMake choisisse les bons compilos dès le premier run
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

