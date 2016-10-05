MULTISCALE_DIR="/home/nlampe/gits/geant4sims/multiscale"
$MULTISCALE_DIR/macro/build/macro -m "gammabg.mac" -t 4
hadd -O -f gammabgmacro.root macro*root
rm macro*root
#cp $MULTISCALE_DIR/macro/build/macroanalysis.C ./
#root -b -q -l 'macroanalysis.C("gammabgmacro.root")'
#rm macroanalysis.C
#mv gammabgmacro.root.secondaryhisto secondaries.hist


$MULTISCALE_DIR/micro/build/micro -m "radiodurans.ln" -t 4
hadd -O -f gammabgmicro.root micro*root
#cp $MULTISCALE_DIR/micro/build/microanalysis.C ./
root -b -q -l 'microanalysis.C("gammabgmicro.root")'
rm micro*root
#rm microanalysis.C
#mv gammabgmicro.root.cellelectronhisto cellelectrons.hist

