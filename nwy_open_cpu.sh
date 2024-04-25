
clear
if test -d prebuilts/linux; then
    echo has prebuilts/linux
else
    cp -rf ../prebuilts/linux ./prebuilts/
fi

set -e
source tools/core_launch.sh
cout
if [ "$1" ];then
	echo "have args"
	cmake ../.. -G Ninja -D BUILD_CUST_PROJ=$1
else
	echo "no args"
	cmake ../.. -G Ninja -U BUILD_CUST_PROJ
fi
ninja
croot

