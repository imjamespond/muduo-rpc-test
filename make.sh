echo shell begin
echo parameter num $#
echo proceed id $$
echo shell name $0

PROJECT=server

function_build()
{
mkdir -p build
cd build/
cmake . .. \
-DBOOST_ROOT=$1 \
-DROOT=$2 \
-DMUDUO=$3
make

}

function_zip()
{
find . -name "*~" -print0|xargs -0 rm -rf
tar czvf $PROJECT.tar.gz ./$PROJECT --exclude build --exclude ./$PROJECT/nbproject --exclude ./$PROJECT/dist --exclude ./$PROJECT/*.tar.gz
}

function_svn()
{
svn update
#svn add codechiev
svn ci -m "" $1
}

if [ "$1" = "build" ]; then
	protoc --java_out=./build ./rpc/node.proto
	protoc --java_out=./build ./rpc/flight.proto
	function_build /data0/source/boost_1_59_0 `pwd` /data0/lib

elif [ "$1" = "zip" ]; then
	function_zip
elif [ "$1" = "svn" ]; then
	function_svn
else
	echo "input build for build,\
zip for zip."
fi
