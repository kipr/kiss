if [ $# -ne 2 ]
then
	echo "Usage: $0 [kiss-targets] [kiss-lexers]"
	exit 0
fi

rm -Rf deploy/*

qmake
make
make install

cd $1
qmake
make
make install
cd -

cd $2
qmake
make
make install
cd -

cp $1/root/*.kiss deploy
cp $2/root/*.kiss deploy 
