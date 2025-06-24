#include "MyTrackInfo.hh"
#include "G4SystemOfUnits.hh" // For units if you print anything related to them

MyTrackInfo::MyTrackInfo()
: G4VUserTrackInformation(),
  fEnteredTarget1(false)
{
}

MyTrackInfo::~MyTrackInfo()
{
}

