call tools\core_launch.bat
cd out\appimage_debug
if exist .ninja_deps  (
	del /f /s /q .ninja_deps
)
if "%1"=="" (cmake ..\.. -G Ninja -U BUILD_CUST_PROJ) else (cmake ..\.. -G Ninja -D BUILD_CUST_PROJ=%1)
ninja
ninja >..\..\log.txt
cd ..\..\

