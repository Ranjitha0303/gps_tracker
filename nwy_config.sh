#setlocal enabledelayedexpansion
h_file="components\include\nwy_config.h"
lite_str="FEATURE_NWY_OPEN_LITE"

cmake_path=cmake/
cmake_file=core_config.cmake
mqtt_conf=CONFIG_NWY_OPEN_LITE_PAHOMQTT
ali_conf=CONFIG_NWY_OPEN_LITE_ALIMQTT
ivtts_conf=CONFIG_NWY_OPEN_LITE_IVTTS
yttts_conf=CONFIG_NWY_OPEN_LITE_YTTTS
ejtts_conf=CONFIG_NWY_OPEN_LITE_EJTTS
ydtts_conf=CONFIG_NWY_OPEN_LITE_YT_HINDITTS
rws_conf=CONFIG_NWY_OPEN_LITE_RWS
aws_conf=CONFIG_NWY_OPEN_LITE_AWSMQTT
function get_config
{
	echo ******************************Current Configurations******************************
	if [[ $(grep -c $mqtt_conf $cmake_path$cmake_file ) -ne '0' ]];
	then
		echo "MQTT : ON"
	else
		echo "MQTT : OFF"
	fi
	if [[ $(grep -c $ali_conf $cmake_path$cmake_file ) -ne '0' ]];
	then
		echo "ALIMQTT : ON"
	else
		echo "ALIMQTT : OFF"
	fi
	if [[ $(grep -c $ivtts_conf $cmake_path$cmake_file ) -ne '0' ]];
	then
		echo "IVTTS : ON"
	else
		echo "IVTTS : OFF"
	fi
	if [[ $(grep -c $yttts_conf $cmake_path$cmake_file ) -ne '0' ]];
	then
		echo "YTTTS : ON"
	else
		echo "YTTTS : OFF"
	fi
	if [[ $(grep -c $ejtts_conf $cmake_path$cmake_file ) -ne '0' ]];
	then
		echo "EJTTS : ON"
	else
		echo "EJTTS : OFF"
	fi
	if [[ $(grep -c $ydtts_conf $cmake_path$cmake_file ) -ne '0' ]];
	then
		echo "YDTTS : ON"
	else
		echo "YDTTS : OFF"
	fi
	if [[ $(grep -c $rws_conf $cmake_path$cmake_file ) -ne '0' ]];
	then
		echo "RWS : ON"
	else
		echo "RWS : OFF"
	fi
	if [[ $(grep -c $aws_conf $cmake_path$cmake_file ) -ne '0' ]];
	then
		echo "AWS : ON"
	else
		echo "AWS : OFF"
	fi

	echo ******************************Current Configurations******************************

}

function cmake_file_config
{
	sed -i "/${mqtt_conf}/d"  $cmake_path$cmake_file
	sed -i "/${ali_conf}/d"  $cmake_path$cmake_file
	sed -i "/${ivtts_conf}/d"  $cmake_path$cmake_file
	sed -i "/${yttts_conf}/d"  $cmake_path$cmake_file
	sed -i "/${ejtts_conf}/d"  $cmake_path$cmake_file
	sed -i "/${ydtts_conf}/d"  $cmake_path$cmake_file
	sed -i "/${rws_conf}/d"  $cmake_path$cmake_file
	sed -i "/${aws_conf}/d"  $cmake_path$cmake_file
	if [ $mqtt_on -ne '0' ];
		then sed -i "\$a\\set(${mqtt_conf} on)" $cmake_path$cmake_file
	fi
	if [ $ali_on -ne '0' ];
		then sed -i "\$a\\set(${ali_conf} on)" $cmake_path$cmake_file
	fi
	if [ $ivtts_on -ne '0' ];
		then sed -i "\$a\\set(${ivtts_conf} on)" $cmake_path$cmake_file
	fi
	if [ $yttts_on -ne '0' ];
		then sed -i "\$a\\set(${yttts_conf} on)" $cmake_path$cmake_file
	fi
	if [ $ejtts_on -ne '0' ];
		then sed -i "\$a\\set(${ejtts_conf} on)" $cmake_path$cmake_file
	fi
	if [ $ydtts_on -ne '0' ];
		then sed -i "\$a\\set(${ydtts_conf} on)" $cmake_path$cmake_file
	fi
	if [ $rws_on -ne '0' ];
		then sed -i "\$a\\set(${rws_conf} on)" $cmake_path$cmake_file
	fi
	if [ $aws_on -ne '0' ];
		then sed -i "\$a\\set(${aws_conf} on)" $cmake_path$cmake_file
	fi

}
#1.get current config
get_config
#2.set config
echo -n "Set the configurations 0-exit 1-set: "
read go_config
if [ $go_config -eq '0' ];
	then exit 1 
fi
echo -n "Please config MQTT 0-OFF 1-ON:"
read mqtt_on
echo -n "Please config Ali 0-OFF 1-ON:"
read ali_on
echo -n "Please config IVTTS 0-OFF 1-ON:"
read ivtts_on
echo -n "Please config YTTTS 0-OFF 1-ON:"
read yttts_on
echo -n "Please config EJTTS 0-OFF 1-ON:"
read ejtts_on
echo -n "Please config YDTTS 0-OFF 1-ON:"
read ydtts_on
echo -n "Please config RWS 0-OFF 1-ON:"
read rws_on
echo -n "Please config AWS 0-OFF 1-ON:"
read aws_on
cmake_file_config

