#ifndef MyTrackInfo_h
#define MyTrackInfo_h 1

#include "G4VUserTrackInformation.hh"
#include "G4VPhysicalVolume.hh" // For G4VPhysicalVolume

class MyTrackInfo : public G4VUserTrackInformation
{
public:
    MyTrackInfo();
    virtual ~MyTrackInfo();

    void SetEnteredTarget1(G4bool val) { fEnteredTarget1 = val; }
    G4bool HasEnteredTarget1() const { return fEnteredTarget1; }

private:
    G4bool fEnteredTarget1;
};

#endif
