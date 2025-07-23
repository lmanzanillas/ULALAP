#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include <vector>

class DetectorConstruction;
class RunAction;

/// Event action class
///

class EventAction : public G4UserEventAction
{
  public:
    EventAction(DetectorConstruction* det, RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    virtual void AddInfo(G4double xF, G4double yF, G4double zF, G4double depE, G4double t_depE, G4int pdgCode);
    virtual void AddInfoSecondaries(G4String creator, G4int atNumber, G4int atMass, G4int pdgCode, G4double energy, G4double xn, G4double yn, G4double zn, G4double time_nuclei);
    virtual void AddInfoCaptureGammas(std::vector<G4double> gamma_vector);
    virtual void SetTimeCapture(G4double t_value_c);
    virtual void SetTimeDecay(G4double t_value_d);
    virtual void SetVertex(G4ThreeVector v_value_v);
    virtual void AddNeutronKinAtLAr(G4double kinEneutron, G4double x, G4double y, G4double z);


  private:

    DetectorConstruction*   fDetector;
    RunAction* 	fRunAction;

    int64_t eventNumber;
    G4double timeCapture;
    G4double timeDecay;
    G4int primPDG;
    G4ThreeVector vertex;
    std::vector<G4double> vertex_3d = std::vector<G4double>(3);
     

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
