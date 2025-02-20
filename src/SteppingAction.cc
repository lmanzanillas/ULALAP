#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"

#include "G4SteppingManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fOneStepPrimaries(false), fEventAction(eventAction)
{

	fExpectedNextStatus = Undefined;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SteppingAction::UserSteppingAction(const G4Step* theStep) {
    // Retrieve the track and particle definition
    G4Track* theTrack = theStep->GetTrack();
    G4ParticleDefinition* particleDef = theTrack->GetDefinition();

    // Initialize expected next status
    fExpectedNextStatus = Undefined;

    // Get pre-step point and associated physical volume
    G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
    G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

    // Default process creator name
    G4String processCreator = "NoProcess";

    // Get particle properties
    G4int pdgCode = particleDef->GetPDGEncoding();
    G4double energy = theTrack->GetKineticEnergy();

    // Check conditions for specific processing
    if (theTrack->GetCurrentStepNumber() == 1 &&
        theTrack->GetCreatorProcess() &&
        thePrePV && thePrePV->GetName() == "target_1" &&
        particleDef->GetParticleType() == "nucleus" &&
        particleDef->GetParticleSubType() == "generic") {

        // Get additional particle details
        G4int atNumber = particleDef->GetAtomicNumber();
        G4int atMass = particleDef->GetAtomicMass();
        G4double time_nuclei = theStep->GetPostStepPoint()->GetGlobalTime() / us;
        G4ThreeVector position_nuclei = theStep->GetPostStepPoint()->GetPosition();

        // Get process creator name if available
        if (theTrack->GetCreatorProcess()) {
            processCreator = theTrack->GetCreatorProcess()->GetProcessName();
        }

        // Pass information to event action for storage
        fEventAction->AddInfoSecondaries(
            processCreator,
            atNumber,
            atMass,
            pdgCode,
            energy,
            position_nuclei.x() / cm,
            position_nuclei.y() / cm,
            position_nuclei.z() / cm,
            time_nuclei
        );
    }

    // Get post-step process and other step-related properties
    G4String process_name = theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    G4int stepNo = theTrack->GetCurrentStepNumber();
    G4double edepStep = theStep->GetTotalEnergyDeposit() / keV;

    // Handle neutron capture
    if (process_name == "nCapture") {
        G4double time_finish = theStep->GetPostStepPoint()->GetGlobalTime();
        fEventAction->SetTimeCapture(time_finish / us);
        fEventAction->SetTimeDecay(0.);
        G4ThreeVector position_finish = theStep->GetPostStepPoint()->GetPosition();
        fEventAction->SetVertex(position_finish);
    }

    // Handle radioactive decay
    if (process_name == "RadioactiveDecay") {
        G4double partEnergy = theStep->GetPreStepPoint()->GetKineticEnergy() / keV;
        G4double time_finish = theStep->GetPostStepPoint()->GetGlobalTime();
        G4ThreeVector position_finish = theStep->GetPostStepPoint()->GetPosition();
        fEventAction->SetVertex(position_finish);
        fEventAction->SetTimeDecay(time_finish / us);
        fEventAction->SetTimeCapture(0.);
    }

    // Retrieve secondary particles
    G4SteppingManager* steppingManager = fpSteppingManager;
    G4TrackVector* fSecondary = steppingManager->GetfSecondary();
    std::vector<G4double> gamma_vector;

    for (size_t lp1 = 0; lp1 < (*fSecondary).size(); lp1++) {
        const G4ParticleDefinition* particleDefi = (*fSecondary)[lp1]->GetDefinition();
        G4String process = (*fSecondary)[lp1]->GetCreatorProcess()->GetProcessName();
        energy = (*fSecondary)[lp1]->GetKineticEnergy();

        if (particleDefi == G4Gamma::Definition()) {
            if (process == "RadioactiveDecay" || process == "nCapture") {
                gamma_vector.push_back(energy / keV);
            }
        }
    }

    if (!gamma_vector.empty()) {
        fEventAction->AddInfoCaptureGammas(gamma_vector);
    }

    // Store energy deposition for target_1
    if (thePrePV && thePrePV->GetName() == "target_1") {
        G4double time_dep = theStep->GetPostStepPoint()->GetGlobalTime() / us;
        fEventAction->AddInfo(
            thePrePoint->GetPosition().x() / cm,
            thePrePoint->GetPosition().y() / cm,
            thePrePoint->GetPosition().z() / cm,
            edepStep,
            time_dep,
	    pdgCode
        );
    }

    // Check if the post-step volume is valid
    G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
    G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

    if (!thePostPV) {
        // Out of world
        fExpectedNextStatus = Undefined;
        return;
    }
}
