if [ $# -ne 2 ]
then
	echo "Usage: $0 [kiss-targets] [kiss-lexers]"
	exit 0
fi

rm -Rf deploy/*

qmake CONFIG+=kiss
make
make install

KISS=$PWD
cd $1
sh build.sh $KISS
cd -

cd $2
qmake -r
make
make install
cd -

cp $1/root/*.kiss deploy
cp $2/root/*.kiss deploy 
