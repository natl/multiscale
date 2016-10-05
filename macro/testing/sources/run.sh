cd spherecos
sh clean.sh
python generate.py
python run.py > out

cd ../sphereiso
sh clean.sh
python generate.py
python run.py > out

cd ../planeiso
sh clean.sh
python generate.py
python run.py > out

cd ../planecos
sh clean.sh
python generate.py
python run.py > out
