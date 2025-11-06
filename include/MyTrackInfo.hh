#ifndef MyTrackInfo_h
#define MyTrackInfo_h 1

#include "G4VUserTrackInformation.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"

class MyTrackInfo : public G4VUserTrackInformation
{
public:
    MyTrackInfo();
    ~MyTrackInfo() override;

    // --- Neutron flags ---
    void SetEnteredTarget1Neutron(G4bool val) { fEnteredTarget1Neutron = val; }
    G4bool HasEnteredTarget1Neutron() const { return fEnteredTarget1Neutron; }

    // --- Gamma flags ---
    void SetEnteredTarget1Gamma(G4bool val) { fEnteredTarget1Gamma = val; }
    G4bool HasEnteredTarget1Gamma() const { return fEnteredTarget1Gamma; }

    // Optional: flags for other behaviors
    void SetFromCapture(G4bool val) { fFromCapture = val; }
    G4bool IsFromCapture() const { return fFromCapture; }

private:
    // --- Per-track status flags ---
    G4bool fEnteredTarget1Neutron;  // True if neutron entered target_1
    G4bool fEnteredTarget1Gamma;    // True if gamma entered target_1
    G4bool fFromCapture;            // True if track (or parent) was created in nCapture
};

#endif
