# Script base directory.
basedir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Update library path.
libdir=${basedir}/lib
[[ "${LD_LIBRARY_PATH}" =~ "${libdir}" ]] || export LD_LIBRARY_PATH=${libdir}:${LD_LIBRARY_PATH} 

# Update python path.
pydir=${basedir}/python
[[ "${PYTHONPATH}" =~ "${pydir}" ]] || export PYTHONPATH=${pydir}:${PYTHONPATH}

