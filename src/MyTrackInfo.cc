#include "MyTrackInfo.hh"
#include "G4SystemOfUnits.hh" // For units if you print anything related to them

MyTrackInfo::MyTrackInfo()
    : fEnteredTarget1Neutron(false),
      fEnteredTarget1Gamma(false),
      fFromCapture(false)
{}

MyTrackInfo::~MyTrackInfo() = default;
