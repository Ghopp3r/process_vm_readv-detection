@ECHO off
CLS
setlocal

set CWD=%~dp0
set CMAKE=D:\Cheats\Projects\cmake-3.22.3\bin
set ANDROID_NDK=D:\Cheats\Projects\android-ndk-r25c
set ARCH_ABIS=arm64-v8a

for %%A in (%ARCH_ABIS%) do (
    echo ===== Building for %%A =====

    %CMAKE%\cmake.exe -H%CWD% ^
        -B%CWD%build\%%A% ^
        -GNinja ^
        -DCMAKE_MAKE_PROGRAM=%CMAKE%\ninja.exe ^
        -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake ^
        -DANDROID_NDK=%ANDROID_NDK% ^
        -DANDROID_PLATFORM=android-23 ^
        -DANDROID_ABI=%%A ^
        -DCMAKE_ANDROID_ARCH_ABI=%%A ^
        -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=%CWD%out\%%A% ^
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=%CWD%out\%%A% ^
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=%CWD%out\%%A% ^
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_FIND_ROOT_PATH=%CWD%build\%%A%\.cxx ^
        -DCMAKE_SYSTEM_NAME=Android ^
        -DCMAKE_SYSTEM_VERSION=23

    %CMAKE%\ninja.exe -C %CWD%build\%%A% -j7
)

echo.
echo === Build finished, binaries in: %CWD%out\%%ARCH_ABIS% ===
pause
