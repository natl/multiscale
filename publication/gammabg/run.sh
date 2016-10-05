MULTISCALE_DIR="/home/lampe/gits/geant4sims/multiscale"
$MULTISCALE_DIR/macro/build/macro -m "gammabg.mac" -t 16
hadd -f gammabgmacro.root macro*root
rm macro*root

$MULTISCALE_DIR/micro/build/micro -m "ecoli.mac" -t 16
hadd -f gammabgmicro.root micro*root
rm micro*root


