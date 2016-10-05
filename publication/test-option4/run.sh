MULTISCALE_DIR="/home/lampe/gits/geant4sims/multiscale"

$MULTISCALE_DIR/micro/build/micro -m "ecoli.mac" -t 16
hadd -f gammabgmicro.root micro*root
rm micro*root


