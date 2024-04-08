#include "EventAction.hh"
#include "RunAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(DetectorConstruction* det, RunAction* runAction)
: G4UserEventAction(),fDetector(det),
fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* myEvent)
{
        eventNumber = myEvent->GetEventID();
	timeCapture = 0.;
	timeDecay = 0.;
	G4ThreeVector zero(0.,0.,0.);
	vertex = zero;
	vertex_3d = {0., 0., 0.};
	
	//mygammas.clear();
	if (myEvent->GetEventID() % 10000 == 0)
		G4cout << "starting event no.: " << myEvent->GetEventID() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SetTimeCapture(G4double t_capture){
	timeCapture = t_capture;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SetTimeDecay(G4double t_decay){
	timeDecay = t_decay;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SetVertex(G4ThreeVector v_vertex){
	vertex = v_vertex;
	vertex_3d[0] = vertex.x()/cm;
	vertex_3d[1] = vertex.y()/cm;
	vertex_3d[2] = vertex.z()/cm;
	fRunAction->SetVertexOrigin(vertex_3d);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::AddInfo(G4double xF, G4double yF, G4double zF, G4double depE, G4double t_depE){
	if (depE > 1.0){
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->FillNtupleIColumn(1, 0, eventNumber);
		analysisManager->FillNtupleDColumn(1, 1, xF);
		analysisManager->FillNtupleDColumn(1, 2, yF);
		analysisManager->FillNtupleDColumn(1, 3, zF);
		analysisManager->FillNtupleDColumn(1, 4, depE);
		analysisManager->FillNtupleDColumn(1, 5, t_depE);
		analysisManager->AddNtupleRow(1);
        }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::AddInfoSecondaries(G4String creator, G4int atNumber, G4int pdgCode,G4double energy){
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->FillNtupleIColumn(4, 0, eventNumber);
	analysisManager->FillNtupleSColumn(4, 1, creator);
	analysisManager->FillNtupleIColumn(4, 2, atNumber);
	analysisManager->FillNtupleIColumn(4, 3, pdgCode);
	analysisManager->FillNtupleDColumn(4, 4, energy);
	analysisManager->AddNtupleRow(4);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::AddInfoCaptureGammas(std::vector<G4double> gamma_vector){
	        fRunAction->FillVectorGammas(gamma_vector);
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager->FillNtupleIColumn(2, 0, eventNumber);
		analysisManager->FillNtupleDColumn(2, 1, timeCapture);
		analysisManager->FillNtupleDColumn(2, 2, timeDecay);
		//analysisManager->FillNtupleDColumn(2, 1, EGamma); not needed
		analysisManager->AddNtupleRow(2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* myEvent)
{
	G4int nV = myEvent->GetNumberOfPrimaryVertex();
	G4PrimaryVertex* Vertex;
	G4PrimaryParticle* PrimParticle;
	G4int pdgID =-999;
	G4double kinE = -99.9;
	for(G4int v = 0 ; v < nV; ++v ){
		//G4cout<<" starting for loop: "<<v<<G4endl;
		Vertex = myEvent->GetPrimaryVertex(v);
		G4int particlesInVertex = Vertex->GetNumberOfParticle();
		for (G4int p = 0; p < particlesInVertex; ++p ){
			PrimParticle = Vertex->GetPrimary(p);
			pdgID = PrimParticle->GetPDGcode();
			kinE = PrimParticle->GetKineticEnergy()/keV;
	        	//G4cout<<" pdg: "<<pdgID<<" kinE: "<<kinE<<G4endl;	
			auto analysisManager = G4AnalysisManager::Instance();
			analysisManager->FillNtupleIColumn(3, 0, eventNumber);
			analysisManager->FillNtupleIColumn(3, 1, pdgID);
			analysisManager->FillNtupleDColumn(3, 2, kinE);
			analysisManager->AddNtupleRow(3);
		}
	}
	if (myEvent->GetEventID() % 10000 == 0)
		G4cout << "finish event no.: " << myEvent->GetEventID() << G4endl;

}
