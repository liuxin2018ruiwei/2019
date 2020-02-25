#!/bin/sh
showHelp()
{
	echo " "
	echo "---------------------------------------------------------------------"
	echo " -c       : cross compile. (default compile for x86"
	echo " -h       : the help information"
	echo " -i [arg] : copy the package.zip to service ,for making a firmware"
	echo " -o [arg] : get the firmware from service"
	echo " -p       : make a package"
	echo " -s       : just install idem to system/bin"
	echo " -t       : tar the source code as the formate of gzip"  
	echo " -r       : run the exe if compiled successfully"  
	echo "---------------------------------------------------------------------"
}

make_package()
{
	rm -rf Package.zip
	
	mkdir -p "Package/libs"
	mkdir -p "Package/include"

	cp -r src/Include/*  Package/include/
	cp -r build/*.so Package/libs/
	cp -r TestSuitDriver/sample.c Package/
	cp -r TestSuitDriver/CMakeList.txt Package/
	cp -r lrzsz/src/lsz Package/
	cp -r Sample Package/Sample
	zip -r Package.zip Package
	  

	verNum=`svn info |grep "Last Changed Rev:"|awk '{printf $4}'`
	dstfile=${PWD##*/}_$(date +%m%d%H%M)_r"$verNum"

	bakpath=~/bak
	mkdir -p $bakpath
	mv Package.zip $bakpath/$dstfile.zip

	md5sum $bakpath/$dstfile.zip | awk '{print $1}'>> $bakpath/ReleaseNote.txt
	echo "the destination ->-> $bakpath/$dstfile.zip"
}


makePkgFlg=0
installPkgFlg=0
loginFlg=0
outputFlg=0
setBinFlg=0
crossCompileFlg=0
tarSrcFlg=0
runFlg=0
showHelpFlg=0
while getopts "pi:csrtohl" arg
do

	case $arg in
		p)
			crossCompileFlg=1
			makdPkgFlg=1
			;;
		i)
			installPkgFlg=1
			;;
		l)
			loginFlg=1
			;;
		o)
			outputFlg=1
			;;
		s)
			setBinFlg=1
			;;
		c)
			crossCompileFlg=1
			;;
		t)
			tarSrcFlg=1
			;;
		r)
			runFlg=1
			;;
		h)
			showHelpFlg=1
			;;
		*)
			showHelp
			;;
	esac
done

if [[ $showHelpFlg -eq 1 ]]
then
	showHelp
	exit 0
fi

make clean

cat x86_CMakeLists.txt > CMakeLists.txt
if [ $crossCompileFlg -eq 1 ]
then
	#将lrzsz编译到build目录下
	#./lrzsz/runShell.sh
	echo "---NOTICE !! build  by cross compiler-------"
	cat hisi_CMakeLists.txt > CMakeLists.txt
	
	
	
fi

cmake . && make -j 2
if [ $? != 0 ]
then
	echo "faile to make, can't continue!!"
	exit -1;
fi

if [[ $runFlg -eq 1 ]]
then
	./build/TestSuitDriver
fi


if [[ $makdPkgFlg -eq 1 ]]
then
	make_package
fi

if [ $tarSrcFlg -eq 1 ]
then
	tar -czvf ~/bak/"${PWD##*/}_SRC_$(date +%Y%m%d-%H%M%S).tgz" src/ TestSuitDriver/ TestSuitStub/ CMakeLists.txt allmake.sh 
	echo "the destinatin: ~/bak/${PWD##*/}_SRC_$(date +%Y%m%d-%H%M%S).tgz"
fi

if [ $installPkgFlg -eq 1 ]
then
	echo "scp $OPTARG etc/installPkg.sh security@172.16.3.250:/home/security/workspace/r16-tina/
			ssh -l security 172.16.3.250"
fi


if [ $loginFlg -eq 1 ]
then
	echo "ssh -l security 172.16.3.250"
fi

if [ $outputFlg -eq 1 ]
then
	echo "scp security@172.16.3.250:/home/security/workspace/r16-tina/lichee/tools/pack/*.img ./"
fi


if [ $setBinFlg -eq 1 ]
then
	echo "install_app /system/bin"
fi


rm -rf Package
