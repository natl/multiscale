#include "MicroTesterHit.hh"

G4ThreadLocal G4Allocator<MicroTesterHit>* MicroTesterHitAllocator=0;

MicroTesterHit::MicroTesterHit():
G4VHit(),
fEInit(0.),
fEdep(0.),
fTrackLength(0.),
fEdepVector( std::vector<G4double>() ),
 inX(std::vector< std::pair<G4double,G4double> >()),
outX(std::vector< std::pair<G4double,G4double> >()),
 inY(std::vector< std::pair<G4double,G4double> >()),
outY(std::vector< std::pair<G4double,G4double> >()),
 inZ(std::vector< std::pair<G4double,G4double> >()),
outZ(std::vector< std::pair<G4double,G4double> >()),
fEdepsAtRadii(std::map<G4double, G4double>())
{
    fEdepVector.reserve(10000);
    for (int ii = 0; ii != 301; ii++)
    {
        fEdepsAtRadii.insert(std::make_pair(ii*um, 0*eV));
    }
}

MicroTesterHit::~MicroTesterHit()
{
    //dtor
}

/**
 * \brief optional constructor building from another event
 */
MicroTesterHit::MicroTesterHit(const MicroTesterHit& right)
{
    this->SetEdep        (right.GetEdep () );
    this->SetEInit       (right.GetEInit() );
    this->SetEdepVector  (right.GetEdepVector () );
    this->SetTrackLength (right.GetTrackLength());
    this->SetXIn         (right.GetXIn  () );
    this->SetXOut        (right.GetXOut () );
    this->SetYIn         (right.GetYIn  () );
    this->SetYOut        (right.GetYOut () );
    this->SetZIn         (right.GetZIn  () );
    this->SetZOut        (right.GetZOut () );
    this->SetEdepsAtRadii(right.GetEdepsAtRadii () );

}


const MicroTesterHit& MicroTesterHit::operator=(const MicroTesterHit& right){
    this->SetEdep        (right.GetEdep () );
    this->SetEInit       (right.GetEInit() );
    this->SetEdepVector  (right.GetEdepVector () );
    this->SetTrackLength (right.GetTrackLength());
    this->SetXIn         (right.GetXIn  () );
    this->SetXOut        (right.GetXOut () );
    this->SetYIn         (right.GetYIn  () );
    this->SetYOut        (right.GetYOut () );
    this->SetZIn         (right.GetZIn  () );
    this->SetZOut        (right.GetZOut () );
    this->SetEdepsAtRadii(right.GetEdepsAtRadii () );
    return *this;
}

G4int MicroTesterHit::operator==(const MicroTesterHit& right) const{
    return (this == &right) ? 1 : 0;
}
