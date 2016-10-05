MULTISCALE_DIR="/home/lampe/gits/geant4sims/multiscale"
$MULTISCALE_DIR/macro/build/macro -m "neutron.mac" -t 16
hadd -f neutronmacro.root macro*root
rm macro*root
#cp $MULTISCALE_DIR/macro/build/macroanalysis.C ./
#root -b -q -l 'macroanalysis.C("neutronmacro.root")'
#rm macroanalysis.C
#mv neutronmacro.root.secondaryhisto secondaries.hist


$MULTISCALE_DIR/micro/build/micro -m "ecoli.mac" -t 16 --with-hadronic
hadd -f neutronmicro.root micro*root
#cp $MULTISCALE_DIR/micro/build/microanalysis.C ./
#root -b -q -l 'microanalysis.C("neutronmicro.root")'
rm micro*root
#rm microanalysis.C
#mv neutronmicro.root.cellelectronhisto cellelectrons.hist

