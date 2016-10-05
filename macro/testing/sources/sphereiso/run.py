#!/bin/python

import os,time

macros = [fname for fname in os.listdir('.') if '.mac' in fname]
print "running the following macros: "
print ' '.join(macros)

os.system("cp ../../../macroanalysis.C ./")
for macro in macros:
    print "Now running " + macro
    infilename = macro
    outfilename = macro.replace(".mac", ".out")
    cmd = "../../../macro -m " + infilename + " -t 4 > " + outfilename
    os.system(cmd)
    outrootname = macro.replace(".mac", ".root")
    os.system("hadd -f " + outrootname + " macro.root macro_t[0-3].root")
    os.system("rm macro*root")
    print "root -b -q -l 'macroanalysis.C(\"%s\")'" % outrootname
    time.sleep(1)
    os.system("root -b -q -l 'macroanalysis.C(\"%s\")'" % outrootname)



