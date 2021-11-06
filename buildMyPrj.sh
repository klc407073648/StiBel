curPath=`pwd`
echo $curPath

cd $curPath
buildTime=`date +"%Y%m%d"`
tar zxf StiBel_${buildTime}.tar.gz

cd $curPath/build
rm -rf ./*

#cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_BUILD_PROJECT_OPTION=all ..
cmake ..
make -j8

rm -rf $curPath/logs/log4cpp/*


