
#!/bin/python
"""
Generate macro files for an cosine emitting source
These should all yield the same energy/volume
"""
import numpy as np

# 1: Testing cos emission distributions
srcs = [("sphereCos", "/macro/source/geometry sphere \n/macro/source/rad   1 m\n/macro/source/angDist cos\n/macro/source/centre 0 0 0 cm \n")]


dets = [("box",         "/macro/det/geometry box       \n/macro/det/size 0.2 0.2 0.2 m   \n"),
        #("rect",      "/macro/det/geometry box       \n/macro/det/size 0.4 0.4 0.01 m \n"),
        ("sphere",      "/macro/det/geometry sphere    \n/macro/det/rad 0.124 m \n")]#,
        #("ellipsoid",   "/macro/det/geometry ellipsoid \n/macro/det/size 0.124 0.062 0.248 m   \n")]
rotds = [("0",  "/macro/det/rotAngle 0 \n"),
        ("30", "/macro/det/rotAngle 30 \n"),
        ("60", "/macro/det/rotAngle 60 \n"),
        ("90", "/macro/det/rotAngle 90 \n")]

ens = [("1MeV",    "/macro/source/dist mono \n/macro/source/energy 1000 keV \n", "/run/beamOn 100000 \n")]

particles = [("electron", "/macro/source/particle e- \n")]

materials = [("H2O",   "/macro/det/sampleMat G4_WATER \n")]

enThreshs = [("100keV","/macro/det/energyThreshold 100 keV \n"),
             ("50keV" , "/macro/det/energyThreshold 50 keV \n")]

rotdaxs = [("x", "/macro/det/rotVector 1 0 0 \n"),
          ("y", "/macro/det/rotVector 0 1 0 \n"),
          ("z", "/macro/det/rotVector 0 0 1 \n")]

detCents = [("center", "/macro/det/position 0 0 0 m \n"),
            ("x", "/macro/det/position 0.3 0 0 m \n"),
            ("y", "/macro/det/position 0 0.3 0 m \n"),
            ("negxy", "/macro/det/position -0.2 -0.2 0 m \n")]


combs = []

for src in srcs:
    for det in dets:
        for material in materials:
            for enThresh in enThreshs:
                for detCent in detCents:
                    for rotd in rotds:
                            rotdax = rotdaxs[np.random.randint(0, len(rotdaxs))]
                            particle = particles[np.random.randint(0,len(particles))]
                            en = ens[np.random.randint(0, len(ens))]
                            d = {'src': src, 'det': det, 'rotd': rotd, 'detCent': detCent, 'en': en, 'particle': particle,
                            'material': material, 'enThresh': enThresh, 'rotdax': rotdax}
                            combs.append(d)


base = """# Physics
/macro/phys/addPhysics G4EmStandardPhysics
/run/setCut 10 um

# Geometry
/macro/det/useCustomGeometry false
/macro/det/worldSize 12 12 12 m
/macro/det/worldMat Galactic
"""
print "Generating %i macros" % len(combs)
for comb in combs:
    out = base + "\n\n"
    out+= comb['det'][1]
    out+= comb['detCent'][1]
    out+= comb['rotd'][1]
    out+= comb['rotdax'][1]
    out+= comb['material'][1]
    out+= "\n/run/initialize\n"
    out+= "\n\n# source\n\n"
    out+= comb['src'][1]

    out+= comb['particle'][1]
    out+= comb['en'][1]
    out+= "\n/macro/source/initialize\n"
    out+= "\n\n# SD\n\n"
    out+= comb['enThresh'][1]
    out+= "\n\n# run\n\n"
    out+= comb['en'][2]

    fname = "det-"      + comb["det"     ][0] + "_" +\
            "detCent-"  + comb["detCent" ][0] + "_" +\
            "rotd-"     + comb["rotd"    ][0] + "_" +\
            "rotdax-"   + comb["rotdax"  ][0] + "_" +\
            "material-" + comb["material"][0] + "_" +\
            "src-"      + comb["src"     ][0] + "_" +\
            "particle-" + comb["particle"][0] + "_" +\
            "en-"       + comb["en"      ][0] + "_" +\
            "enThresh-" + comb["enThresh"][0] + ".mac"
    f = open(fname, 'w')
    f.write(out)
    f.close()


