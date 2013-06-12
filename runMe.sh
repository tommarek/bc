#!/bin/sh

#making project
echo "Do you wish to make project? (y/n)";
read program;
if [ $program = "y" ] ; then
	echo "Gooing to make project, this operation may take a while...";
	sleep 1;
	cd project;
	make D=0 && cp everythingIsPossible.packed ../result ;
	cd ..;
	echo "Output file is stored in ./result/";
fi

echo;

#making doxygen documentation
echo "Do you wish to make doxygen documentation? (y/n)";
read doc;
if [ $doc = "y" ] ; then
	echo "Going to make doxygen documentation...";
	sleep 1;
	cd doc/doxygen/;
	doxygen && cp -r doxygen_doc ../../result;
	cd ../..;
	echo "Output is stored in ./result/";
fi

echo ;

#making latex documentation
echo "Do you wish to make latex documentation? (y/n)";
read doc;
if [ $doc = "y" ] ; then
	echo "Going to make latex documentation...";
	sleep 1;
	cd doc/bachelor_thesis/;
	make && cp projekt.pdf ../../result;
	cd ../..;
	echo "Output file is stored in ./result/";
fi
