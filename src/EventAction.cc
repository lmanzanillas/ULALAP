#include "EventAction.hh"
#include "RunAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include <vector>

// Constructor
EventAction::EventAction(DetectorConstruction* det, RunAction* runAction)
    : G4UserEventAction(),
      fDetector(det),
      fRunAction(runAction),
      eventNumber(-1),
      timeCapture(0.0),
      timeDecay(0.0),
      vertex(0., 0., 0.),
      primPDG(-999) {}

// Destructor
EventAction::~EventAction() {}

// Begin of Event Action
void EventAction::BeginOfEventAction(const G4Event* myEvent) {
    eventNumber = myEvent->GetEventID();
    timeCapture = 0.0;
    timeDecay = 0.0;
    vertex.set(0.0, 0.0, 0.0);
    vertex_3d = {0.0, 0.0, 0.0};

    G4int nVertices = myEvent->GetNumberOfPrimaryVertex();
    primPDG = -999;

    for (G4int v = 0; v < nVertices; ++v) {
        G4PrimaryVertex* vertex = myEvent->GetPrimaryVertex(v);
        if (!vertex) continue;

        G4int particlesInVertex = vertex->GetNumberOfParticle();
        for (G4int p = 0; p < particlesInVertex; ++p) {
            G4PrimaryParticle* primParticle = vertex->GetPrimary(p);
            if (primParticle) {
                primPDG = primParticle->GetPDGcode();
            }
        }
    }

    if (myEvent->GetEventID() % 10000 == 0) {
        G4cout << "Starting event no.: " << myEvent->GetEventID() << G4endl;
    }
}

// Set Time Capture
void EventAction::SetTimeCapture(G4double t_capture) {
    timeCapture = t_capture;
}

// Set Time Decay
void EventAction::SetTimeDecay(G4double t_decay) {
    timeDecay = t_decay;
}

// Set Vertex
void EventAction::SetVertex(G4ThreeVector v_vertex) {
    vertex = v_vertex;
    vertex_3d = {vertex.x() / cm, vertex.y() / cm, vertex.z() / cm};
    fRunAction->SetVertexOrigin(vertex_3d);
}

// Add Energy Deposition Info
void EventAction::AddInfo(G4double xF, G4double yF, G4double zF, G4double depE, G4double t_depE) {
    if (depE > 1.0 && std::abs(primPDG) != 13) { // Threshold and PDG check
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

// Add Secondary Info
void EventAction::AddInfoSecondaries(G4String creator, G4int atNumber, G4int atMass, G4int pdgCode,
                                     G4double energy, G4double xn, G4double yn, G4double zn, G4double tn) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(4, 0, eventNumber);
    analysisManager->FillNtupleSColumn(4, 1, creator);
    analysisManager->FillNtupleIColumn(4, 2, atNumber);
    analysisManager->FillNtupleIColumn(4, 3, atMass);
    analysisManager->FillNtupleIColumn(4, 4, pdgCode);
    analysisManager->FillNtupleDColumn(4, 5, energy);
    analysisManager->FillNtupleDColumn(4, 6, xn);
    analysisManager->FillNtupleDColumn(4, 7, yn);
    analysisManager->FillNtupleDColumn(4, 8, zn);
    analysisManager->FillNtupleDColumn(4, 9, tn);
    analysisManager->AddNtupleRow(4);
}

// Add Gamma Capture Info
void EventAction::AddInfoCaptureGammas(std::vector<G4double> gamma_vector) {
    fRunAction->FillVectorGammas(gamma_vector);
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(2, 0, eventNumber);
    analysisManager->FillNtupleDColumn(2, 1, timeCapture);
    analysisManager->FillNtupleDColumn(2, 2, timeDecay);
    analysisManager->AddNtupleRow(2);
}

// End of Event Action
void EventAction::EndOfEventAction(const G4Event* myEvent) {
    G4int nVertices = myEvent->GetNumberOfPrimaryVertex();

    for (G4int v = 0; v < nVertices; ++v) {
        G4PrimaryVertex* vertex = myEvent->GetPrimaryVertex(v);
        if (!vertex) continue;

        G4ThreeVector pos = vertex->GetPosition();
        G4int particlesInVertex = vertex->GetNumberOfParticle();

        for (G4int p = 0; p < particlesInVertex; ++p) {
            G4PrimaryParticle* primParticle = vertex->GetPrimary(p);
            if (primParticle) {
                G4int pdgID = primParticle->GetPDGcode();
                G4double kinE = primParticle->GetKineticEnergy() / keV;

                auto analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillNtupleIColumn(3, 0, eventNumber);
                analysisManager->FillNtupleIColumn(3, 1, pdgID);
                analysisManager->FillNtupleDColumn(3, 2, kinE);
                analysisManager->FillNtupleDColumn(3, 3, pos.x());
                analysisManager->FillNtupleDColumn(3, 4, pos.y());
                analysisManager->FillNtupleDColumn(3, 5, pos.z());
                analysisManager->AddNtupleRow(3);
            }
        }
    }

    if (myEvent->GetEventID() % 10000 == 0) {
        G4cout << "Finish event no.: " << myEvent->GetEventID() << G4endl;
    }
}

