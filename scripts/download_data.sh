#!/bin/sh

pushd `dirname $0` > /dev/null
cd ../
SOURCE_BASE_DIR=`pwd -P`
popd > /dev/null
echo "BASEDIR:$SOURCE_BASE_DIR"

DBLP_PATH=$SOURCE_BASE_DIR/data
DBLP_REMOTE_URL="http://dblp.dagstuhl.de/xml/release/dblp-"`date +"%Y-%m-01"`".xml.gz"
DBLP_FILE=$DBLP_PATH/dblp.xml.gz
DBLP_DTD=$DBLP_PATH/dblp.dtd

echo "DBLP DIRECTORY:"$DBLP_PATH
echo "DBLP RESOURCE URL:"$DBLP_REMOTE_URL
echo "PATH OF XML:"$DBLP_FILE
echo "PATH OF DTD:"$DBLP_DTD
rm -rf $DBLP_PATH
mkdir -p $DBLP_PATH
#axel -n 3 -o $DBLP_FILE http://dblp.dagstuhl.de/xml/dblp.xml.gz
#axel -n 3 -o $DBLP_FILE http://dblp.dagstuhl.de/xml/release/dblp-2016-05-01.xml.gz 

if hash axel 2>/dev/null; then
    echo "downloading via axel.."$DBLP_REMOTE_URL
    axel -n 3 -o $DBLP_FILE $DBLP_REMOTE_URL
    axel -n 3 -o $DBLP_DTD http://dblp.dagstuhl.de/xml/dblp.dtd 
else
    echo "downloading via wget.."$DBLP_REMOTE_URL
    wget -O $DBLP_FILE $DBLP_REMOTE_URL
    wget -O $DBLP_DTD http://dblp.dagstuhl.de/xml/dblp.dtd
fi

gzip -d $DBLP_FILE
cp $DBLP_DTD $DBLP_PATH/dblp-2016-10-01.dtd 
cp $DBLP_DTD $DBLP_PATH/dblp-2017-03-29.dtd
echo "FINISHED DOWNLOADING"$DBLP_REMOTE_URL
