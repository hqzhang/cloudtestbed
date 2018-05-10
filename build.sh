#!/bin/bash
set -x
set -e
GOPATH=`pwd`:`pwd`/src/vendor
OUTPUT=`pwd`/output
echo $OUTPUT
mkdir -p $OUTPUT

export GOPATH



# auto generate version info
if command -v git &> /dev/null && git rev-parse &> /dev/null; then
	GITCOMMIT=$(git rev-parse --short HEAD)
	BUILDTIME=$(date -u)
	echo "Build on commit $GITCOMMIT"
	echo "Build time $BUILDTIME"
else
	echo >&2 'error: .git directory missing'
	echo >&2 'Please build with the .git directory accessible!'
	exit 1
fi

source ./script/gen_version.sh

echo "Building agent..."
go install agent
if [ $? -ne 0 ];then
    exit 1
fi

echo "Building controller..."
go install controller
if [ $? -ne 0 ];then
    exit 1
fi

echo "Building canal_cni..."
go install canal_cni
if [ $? -ne 0 ];then
    exit 1
fi

echo "cp host-local,netcon.txt to output"
cp -af src/controller/host-local $OUTPUT
cp -af src/controller/netcon.txt $OUTPUT
cp -af bin/agent  $OUTPUT/canal-agent
cp -af bin/controller $OUTPUT/canal-controller
cp -af bin/canal_cni script/cni/bin/
cp -arf script/cni/bin  $OUTPUT/cni/
echo "cp appChain,template,cni to output"
cp appChain $OUTPUT -rf
cp template $OUTPUT -rf
cp -rf script/cni $OUTPUT -rf

