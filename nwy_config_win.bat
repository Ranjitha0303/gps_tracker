@echo off

set cmake_path=cmake\
set cmake_file=core_config.cmake
set mqtt_conf=CONFIG_NWY_OPEN_LITE_PAHOMQTT
set ali_conf=CONFIG_NWY_OPEN_LITE_ALIMQTT
set ivtts_conf=CONFIG_NWY_OPEN_LITE_IVTTS
set yttts_conf=CONFIG_NWY_OPEN_LITE_YTTTS
set ejtts_conf=CONFIG_NWY_OPEN_LITE_EJTTS
set ydtts_conf=CONFIG_NWY_OPEN_LITE_YT_HINDITTS
set aws_conf=CONFIG_NWY_OPEN_LITE_AWSMQTT
call:get_config
set /p go_config=Set the configurations 0-exit 1-set:
if %go_config% equ 0 goto end
set /p mqtt_on=Please config MQTT 0-OFF 1-ON:
set /p ali_on=Please config Ali MQTT 0-OFF 1-ON:
set /p ivtts_on=Please config IVTTS 0-OFF 1-ON:
set /p yttts_on=Please config YTTTS 0-OFF 1-ON:
set /p ejtts_on=Please config EJTTS 0-OFF 1-ON:
set /p ydtts_on=Please config YDTTS 0-OFF 1-ON:
set /p aws_on=Please config AWS MQTT 0-OFF 1-ON:

call:cmake_file_config
goto end

:get_config
echo ******************************Current Configurations******************************
findstr %mqtt_conf% %cmake_path%%cmake_file% >nul
if %errorlevel% equ 0 (echo MQTT : ON) else (echo MQTT : OFF)
findstr %ali_conf% %cmake_path%%cmake_file% >nul
if %errorlevel% equ 0 (echo Ali MQTT : ON) else (echo Ali MQTT : OFF)
findstr %ivtts_conf% %cmake_path%%cmake_file% >nul
if %errorlevel% equ 0 (echo IVTTS : ON) else (echo IVTTS : OFF)
findstr %yttts_conf% %cmake_path%%cmake_file% >nul
if %errorlevel% equ 0 (echo YTTTS : ON) else (echo YTTTS : OFF)
findstr %ejtts_conf% %cmake_path%%cmake_file% >nul
if %errorlevel% equ 0 (echo EJTTS : ON) else (echo EJTTS : OFF)
findstr %ydtts_conf% %cmake_path%%cmake_file% >nul
if %errorlevel% equ 0 (echo YDTTS : ON) else (echo YDTTS : OFF)
findstr %aws_conf% %cmake_path%%cmake_file% >nul
if %errorlevel% equ 0 (echo AWS MQTT : ON) else (echo AWS MQTT : OFF)
echo ******************************Current Configurations******************************
goto:eof

:cmake_file_config
findstr /v "%mqtt_conf% %ali_conf% %ivtts_conf% %yttts_conf% %ejtts_conf% %ydtts_conf% %aws_conf% " %cmake_path%%cmake_file% > %cmake_path%%cmake_file%.tmp
if %mqtt_on% neq 0 echo set^(%mqtt_conf% on^) >> %cmake_path%%cmake_file%.tmp
if %ali_on% neq 0 echo set^(%ali_conf% on^) >> %cmake_path%%cmake_file%.tmp
if %ivtts_on% neq 0 echo set^(%ivtts_conf% on^) >> %cmake_path%%cmake_file%.tmp
if %yttts_on% neq 0 echo set^(%yttts_conf% on^) >> %cmake_path%%cmake_file%.tmp
if %ejtts_on% neq 0 echo set^(%ejtts_conf% on^) >> %cmake_path%%cmake_file%.tmp
if %ydtts_on% neq 0 echo set^(%ydtts_conf% on^) >> %cmake_path%%cmake_file%.tmp
if %aws_on% neq 0 echo set^(%aws_conf% on^) >> %cmake_path%%cmake_file%.tmp
cd %cmake_path%
del %cmake_file%
ren %cmake_file%.tmp %cmake_file%
cd ..
goto:eof

:end