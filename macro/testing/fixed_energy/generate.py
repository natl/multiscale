#!/bin/python
"""
Generate macro files that should all yield the same energy
"""
import numpy as np

# 1: Testing iso and cos emission distributions
srcs = [("box",       "/macro/source/geometry box   \n/macro/source/halfx 1 cm\n/macro/source/halfy 1 cm\n/macro/source/halfz 1 cm\n/macro/source/angDist iso\n"),
       ("sphereIso", "/macro/source/geometry sphere \n/macro/source/rad   1 cm\n/macro/source/angDist iso\n"),
       ("sphereCos", "/macro/source/geometry sphere \n/macro/source/rad   1 cm\n/macro/source/angDist cos\n"),
       ("planeIso",  "/macro/source/geometry plane  \n/macro/source/halfx 1 cm\n/macro/source/halfy 1 cm\n/macro/source/angDist iso\n"),
       ("planeCos",  "/macro/source/geometry plane  \n/macro/source/halfx 1 cm\n/macro/source/halfy 1 cm\n/macro/source/angDist cos\n"),
       ("pointIso",  "/macro/source/geometry point  \n/macro/source/angDist iso \n"),
       ("pointx",    "/macro/source/geometry point  \n/macro/source/angDist uniform \n/macro/source/direction 1 0 0\n"),
       ("pointy",    "/macro/source/geometry point  \n/macro/source/angDist uniform \n/macro/source/direction 0 1 0\n"),
       ("pointz",    "/macro/source/geometry point  \n/macro/source/angDist uniform \n/macro/source/direction 0 0 1\n")]
dets = [("bigbox",         "/macro/det/geometry box       \n/macro/det/size 1 1 1 m   \n"),
        ("littlebox",      "/macro/det/geometry box       \n/macro/det/size 0.5 0.5 0.5 m \n"),
        ("bigsphere",      "/macro/det/geometry sphere    \n/macro/det/rad 1 m \n"),
        ("littlesphere",   "/macro/det/geometry sphere    \n/macro/det/rad 0.5 m \n"),
        ("bigellipsoid",   "/macro/det/geometry ellipsoid \n/macro/det/size 2 1 1 m   \n"),
        ("littleellipsoid","/macro/det/geometry ellipsoid \n/macro/det/size 1 0.5 0.5 m \n")    ]
rotds = [("0",  "/macro/det/rotAngle 0 \n"),
        ("30", "/macro/det/rotAngle 30 \n"),
        ("60", "/macro/det/rotAngle 60 \n"),
        ("90", "/macro/det/rotAngle 90 \n")]
rotss = [("0",  "/macro/source/rotAngle  0 \n"),
        ("30", "/macro/source/rotAngle -30 \n"),
        ("60", "/macro/source/rotAngle -60 \n"),
        ("90", "/macro/source/rotAngle -90 \n")]
ens = [("500keV", "/macro/source/dist mono \n/macro/source/energy 500 keV \n",                                                                     "/run/beamOn 10000 \n"),
      ("hist",    "/macro/source/dist hist \n/macro/source/histPoint 0.499 0\n/macro/source/histPoint 0.5 1 \n/macro/source/histPoint 0.799 1\n/macro/source/histPoint 0.8 2 \n/macro/source/histPoint 0.999 2 \n/macro/source/histPoint 1.0 0.0 \n",    "/run/beamOn 6306 \n"),
      ("1MeV",    "/macro/source/dist mono \n/macro/source/energy 1000 keV \n",                                                                    "/run/beamOn 5000 \n")]
particles = [("proton", "/macro/source/particle proton \n"),
            ("electron", "/macro/source/particle e- \n")]
materials = [("H2O",   "/macro/det/sampleMat G4_WATER \n")]
#             ("Cu",   "/macro/det/sampleMat G4_Cu \n"),
#             ("Pb",   "/macro/det/sampleMat G4_Pb \n"),
#             ("C",    "/macro/det/sampleMat G4_C \n")]
enThreshs = [("100keV","/macro/det/energyThreshold 100 keV \n"),
            ("50keV" , "/macro/det/energyThreshold 50 keV \n")]
#            ("10keV" ,"/macro/det/energyThreshold 10 keV \n")]
rotdaxs = [("x", "/macro/det/rotVector 1 0 0 \n"),
          ("y", "/macro/det/rotVector 0 1 0 \n"),
          ("z", "/macro/det/rotVector 0 0 1 \n")]
rotsaxs = [("x", "/macro/source/rotVector 1 0 0 \n"),
          ("y", "/macro/source/rotVector 0 1 0 \n"),
          ("z", "/macro/source/rotVector 0 0 1 \n")]

srcCents = [("pos",  "/macro/source/centre 10 10 10 cm \n"),
           ("zero", "/macro/source/centre 0 0 0 cm \n"),
           ("neg",  "/macro/source/centre -10 -10 -10 cm \n")]

combs = []

for src in srcs:
    for det in dets:
        for material in materials:
            for enThresh in enThreshs:
                particle = particles[np.random.randint(0,len(particles))]
                rotd = rotds[np.random.randint(0, len(rotds))]
                rots = rotss[np.random.randint(0, len(rotss))]
                rotdax = rotdaxs[np.random.randint(0, len(rotdaxs))]
                rotsax = rotsaxs[np.random.randint(0, len(rotsaxs))]
                en = ens[np.random.randint(0, len(ens))]
                srcCent = srcCents[np.random.randint(0, len(srcCents))]
                d = {'src': src, 'det': det, 'rotd': rotd, 'rots': rots, 'en': en, 'particle': particle,
                'material': material, 'enThresh': enThresh, 'rotdax': rotdax, 'rotsax': rotsax, 'srccent': srcCent}
                combs.append(d)


base = """# Physics
/macro/phys/addPhysics G4EmStandardPhysics
/run/setCut 10 um

# Geometry
/macro/det/useCustomGeometry false
/macro/det/worldSize 5 5 5 m
/macro/det/worldMat G4_AIR
"""
print "Generating %i macros" % len(combs)
for comb in combs:
    out = base + "\n\n"
    out+= comb['det'][1]
    out+= comb['rotd'][1]
    out+= comb['rotdax'][1]
    out+= comb['material'][1]
    out+= "\n/run/initialize\n"
    out+= "\n\n# source\n\n"
    out+= comb['src'][1]
    out+= comb['srccent'][1]
    out+= comb['rots'][1]
    out+= comb['rotsax'][1]
    out+= comb['particle'][1]
    out+= comb['en'][1]
    out+= "\n/macro/source/initialize\n"
    out+= "\n\n# SD\n\n"
    out+= comb['enThresh'][1]
    out+= "\n\n# run\n\n"
    out+= comb['en'][2]

    fname = "det-"      + comb["det"     ][0] + "_" +\
            "rotd-"     + comb["rotd"    ][0] + "_" +\
            "rotdax-"   + comb["rotdax"  ][0] + "_" +\
            "material-" + comb["material"][0] + "_" +\
            "src-"      + comb["src"     ][0] + "_" +\
            "srccent-"  + comb["srccent" ][0] + "_" +\
            "rots-"     + comb["rots"    ][0] + "_" +\
            "rotsax-"   + comb["rotsax"  ][0] + "_" +\
            "particle-" + comb["particle"][0] + "_" +\
            "en-"       + comb["en"      ][0] + "_" +\
            "enThresh-" + comb["enThresh"][0] + ".mac"
    f = open(fname, 'w')
    f.write(out)
    f.close()


