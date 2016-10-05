MULTISCALE_DIR="/home/lampe/gits/geant4sims/multiscale"
$MULTISCALE_DIR/macro/build/macro -m "muon.mac" -t 16
hadd -f muonmacro.root macro*root
rm macro*root
#cp $MULTISCALE_DIR/macro/build/macroanalysis.C ./
#root -b -q -l 'macroanalysis.C("muonmacro.root")'
#rm macroanalysis.C
#mv muonmacro.root.secondaryhisto secondaries.hist


$MULTISCALE_DIR/micro/build/micro -m "ecoli.mac" -t 16 --with-hadronic
hadd -f muonmicro.root micro*root
#cp $MULTISCALE_DIR/micro/build/microanalysis.C ./
#root -b -q -l 'microanalysis.C("muonmicro.root")'
#rm micro*root
#rm microanalysis.C
#mv muonmicro.root.cellelectronhisto cellelectrons.hist

