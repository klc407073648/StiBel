curPath=`pwd`
echo $curPath

rm -rf $curPath/logs/log4cpp/*
cd $curPath/build
rm -rf ./*

echo "begin to build StiBel project"
echo $curPath
cmake .. && make -j8

echo "end to build StiBel project"



