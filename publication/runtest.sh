echo "Tests -- Penelope"
cd test-pen
sh run.sh


echo "Tests -- Livermore"
cd ../test-liv
sh run.sh


echo "Tests -- DNAPhysics"
cd ../test-dna
sh run.sh
echo "Done"

echo "Tests -- Option4"
cd ../test-option4
sh run.sh
