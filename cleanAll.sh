#!/bin/sh

echo "Cleaning project... ";
cd project;
make clean;
cd ..;
echo "Done";

echo "Cleaning latex documentation... ";
cd doc/bachelor_thesis;
make clean;
cd ../..;
echo "Done";

echo "Cleaning doxygen documentation...";
cd doc/doxygen;
rm -r doxygen_doc;
cd ../..;
echo "Done";

echo "Cleaning result directory...";
cd result;
rm -r doxygen_doc;
rm projekt.pdf;
rm everythingIsPossible.packed;
echo "Done";


