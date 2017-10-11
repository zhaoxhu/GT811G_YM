#!/bin/bash 
# Program
#	This program config onu products type
# Version V1.0
# Author zhuxh
#History
#2015/07/27 First release
        
export SWHOME=$(cd `dirname $0`;pwd)
echo "export SWHOME=$SWHOME"

# make sure you have SW_VERSION in your env,or it will use default value V1R01B000
# if you want to copy app.bin to your ftp dir,add FTP_DIR to your env
GWD_IMAGE_DIR=${SWHOME}/gwdimage
build_gwd_app(){
	IMAGE_DIR=${SWHOME}/iros_onu
	ORIGINALNAME1=iros_onu_asic_ram_lynxd_multi_ports.zblob
	ORIGINALNAME2=iros_onu_asic_ram_lynxd_multi_ports.wlb
	SW_VERSION_FINAL=V1R01B000
	
	if [ ! -f "${IMAGE_DIR}/${ORIGINALNAME1}" ]; then
			echo "${IMAGE_DIR}/${ORIGINALNAME1} not exist!!!"
			exit 0
	fi
	if [ ! -f "${IMAGE_DIR}/${ORIGINALNAME2}" ]; then
			echo "${IMAGE_DIR}/${ORIGINALNAME2} not exist!!!"
			exit 0
	fi
	
	rm -rf ${GWD_IMAGE_DIR}
	mkdir -p ${GWD_IMAGE_DIR}

	if test -z "$SW_VERSION" 
	then
		echo "not find SW_VERSION in env,use default value ${SW_VERSION_FINAL}"
	else
		SW_VERSION_FINAL=${SW_VERSION}
	fi

	echo "PRODUCT_TYPE ${PRODUCT_TYPE},SW_VERSION ${SW_VERSION_FINAL} "

	echo "make gwdimage..."
	cp ${IMAGE_DIR}/${ORIGINALNAME1} ${GWD_IMAGE_DIR}/${PRODUCT_TYPE}_${SW_VERSION_FINAL}.zblob
	cp ${IMAGE_DIR}/${ORIGINALNAME2} ${GWD_IMAGE_DIR}/${PRODUCT_TYPE}_${SW_VERSION_FINAL}.zblob.wlb
	wine ${SWHOME}/FileAdd ${GWD_IMAGE_DIR}/${PRODUCT_TYPE}_${SW_VERSION_FINAL}.zblob ${PRODUCT_TYPE} ${SW_VERSION_FINAL} 0 ${GWD_IMAGE_DIR}/${PRODUCT_TYPE}_app_${SW_VERSION_FINAL}.bin

	if test -n "$FTP_DIR" 
	then
		if [ ! -d "$FTP_DIR" ]; then
			echo "ftp dir $FTP_DIR not exist!!!"
			exit 0
		fi
		echo "copy gwdimage to ftp dir ${FTP_DIR} ..."
		cp ${GWD_IMAGE_DIR}/${PRODUCT_TYPE}_app_${SW_VERSION_FINAL}.bin ${FTP_DIR}/
	else
		echo "not find FTP_DIR in env,no copy to ftp dir"
	fi
}

clean_gwd_app(){
	rm -rf ${GWD_IMAGE_DIR}
}

echo ""
echo "[1] make GT811D"
echo "[2] make GT811G"
echo "[3] make GT873_M_4F4S"
echo "[0] make clean"
read -p "Select Product Type NO.:" ProductType

if [ $ProductType -eq 1 ];then
	PRODUCT=GT811D
	PRODUCT_TYPE=GT811_D
elif [ $ProductType -eq 2 ];then
	PRODUCT=GT811G
	PRODUCT_TYPE=GT811_G
elif [ $ProductType -eq 3 ];then
	PRODUCT=GT873_M_4F4S
	PRODUCT_TYPE=GT873M_4F4S
elif [ $ProductType -eq 0 ];then
	make clean
	echo "============================="
	echo "Make Clean Successfully!!"
	echo "============================="
	clean_gwd_app
	exit 0
else
echo "Select Error!"
exit 0
fi

echo "You select make ${PRODUCT}"


read LINE_READ < ${SWHOME}/products_config.h
LINE_WRITE="#define PRODUCT_CLASS  PRODUCTS_${PRODUCT}"

if [ "$LINE_READ" != "$LINE_WRITE" ]
then
echo -e "$LINE_WRITE" > ${SWHOME}/products_config.h
fi

sleep 1

if make
then
	echo "============================="
	echo "make ${PRODUCT} successfully!!"
	echo "============================="
	build_gwd_app
else
	echo "====================="
	echo "make ${PRODUCT} error!"
	echo "====================="
fi


