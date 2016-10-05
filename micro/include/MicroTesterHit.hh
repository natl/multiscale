#ifndef TESTER_HIT_HH
#define TESTER_HIT_HH

//Parent includes
#include "G4VHit.hh"

//G4 includes
#include "globals.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//external includes
#include <iomanip>
#include <utility>


class MicroTesterHit : public G4VHit
{
public:
    MicroTesterHit();
    MicroTesterHit(const MicroTesterHit&);
    virtual ~MicroTesterHit();



    //operators
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    const MicroTesterHit& operator=(const MicroTesterHit&);
    G4int operator==(const MicroTesterHit&) const;

    //methods
    /**
     * \brief add edep to the energy depostion stored in this hit
     * \param const G4double& edep
     */
    void AddEdep (const G4double& edep) { fEdep += edep;
                                          fEdepVector.push_back(edep); };
    //void AddEdepOverSL (const G4double& edosl)  { fEdepOverSLs.pusj_back(edosl); };
    void AddXIn  (const G4double& y, const G4double& z) { inX.push_back(std::make_pair(y,z)); };
    void AddXOut (const G4double& y, const G4double& z) {outX.push_back(std::make_pair(y,z)); };
    void AddYIn  (const G4double& x, const G4double& z) { inY.push_back(std::make_pair(x,z)); };
    void AddYOut (const G4double& x, const G4double& z) {outY.push_back(std::make_pair(x,z)); };
    void AddZIn  (const G4double& x, const G4double& y) { inZ.push_back(std::make_pair(x,y)); };
    void AddZOut (const G4double& x, const G4double& y) {outZ.push_back(std::make_pair(x,y)); };

    void AddEdepAtRadius (const G4double& r, const G4double& e)
    {
        G4double idx = std::floor(r/um)*um;
        if (fEdepsAtRadii.find(idx) != fEdepsAtRadii.end()) fEdepsAtRadii.at(idx) += e;
    };


    //setters
    void SetEdep      (G4double edep)   { fEdep   = edep; };
    void SetEInit     (G4double einit)  { fEInit  = einit;};
    void SetTrackLength (G4double tl)   {fTrackLength = tl;};
    void SetEdepVector(std::vector<G4double> edepvector)     { fEdepVector = edepvector; };
    void SetXIn       (std::vector< std::pair< G4double, G4double> > xIn ) {inX  = xIn ; };
    void SetXOut      (std::vector< std::pair< G4double, G4double> > xOut) {outX = xOut; };
    void SetYIn       (std::vector< std::pair< G4double, G4double> > yIn ) {inY  = yIn ; };
    void SetYOut      (std::vector< std::pair< G4double, G4double> > yOut) {outY = yOut; };
    void SetZIn       (std::vector< std::pair< G4double, G4double> > zIn ) {inZ  = zIn ; };
    void SetZOut      (std::vector< std::pair< G4double, G4double> > zOut) {outZ = zOut; };

    void SetEdepsAtRadii (std::map<G4double, G4double> edeps) {fEdepsAtRadii = edeps; };


    //getters
    G4double GetEdep()        const { return fEdep;  };
    G4double GetEInit()       const { return fEInit; };
    G4double GetTrackLength() const { return fTrackLength; };

    std::vector<G4double> GetEdepVector()    const {return fEdepVector; };

    std::vector< std::pair<G4double, G4double> > GetXIn()  const{ return inX ; };
    std::vector< std::pair<G4double, G4double> > GetXOut() const{ return outX; };
    std::vector< std::pair<G4double, G4double> > GetYIn()  const{ return inY ; };
    std::vector< std::pair<G4double, G4double> > GetYOut() const{ return outY; };
    std::vector< std::pair<G4double, G4double> > GetZIn()  const{ return inZ ; };
    std::vector< std::pair<G4double, G4double> > GetZOut() const{ return outZ; };

    std::map<G4double, G4double> GetEdepsAtRadii() const{ return fEdepsAtRadii; };


protected:
private:
    G4double                                     fEInit;
    G4double                                     fEdep;
    G4double                                     fTrackLength;
    std::vector<G4double>                        fEdepVector;
    std::vector< std::pair<G4double, G4double> > inX, outX, inY, outY, inZ, outZ;
    std::map<G4double, G4double>                 fEdepsAtRadii; //keys are 0um, 1um, 2um ... 300um.
    //std::vector<G4double>                        fEdepOverSLs;
};

//typedefs
typedef G4THitsCollection<MicroTesterHit> MicroTesterHitsCollection;

//memory management
extern G4ThreadLocal G4Allocator<MicroTesterHit>* MicroTesterHitAllocator;

inline void* MicroTesterHit::operator new(size_t){
    if(!MicroTesterHitAllocator) {MicroTesterHitAllocator = new G4Allocator<MicroTesterHit>;}
    return (void *) MicroTesterHitAllocator->MallocSingle();
}

inline void MicroTesterHit::operator delete(void *hit){
    MicroTesterHitAllocator->FreeSingle( (MicroTesterHit*) hit);
}
#endif // TESTER_HIT
