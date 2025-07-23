#include "G4Timer.hh"
#include "RunAction.hh"
#include "RunActionMessenger.hh"
#include "G4Run.hh"
#include "G4Types.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "EventAction.hh"

#include <string>
#include <ctime>
#include <sys/stat.h>
#include <map>
#include <sstream>
#include <iomanip>

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* kin)
    : G4UserRunAction(), fDetector(det), fPrimary(kin), fTimer(0) {
    fTimer = new G4Timer;
    fMan = G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::~RunAction() {
	fTimer = new G4Timer;
    // Resources managed automatically by smart pointers.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::string datetime() {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%d_%m_%Y_%H_%M", timeinfo);
    return std::string(buffer);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String RunAction::GetFolderName() {
    G4String ionName = G4String("Ion_Z") + std::to_string(fPrimary->GetIonZ()) + "_A" + std::to_string(fPrimary->GetIonA());
    static const std::map<int, G4String> sourceMap = {
        {0, "gamma"}, {1, ionName}, {2, "2x_Bi207"}, {3, "Bi207"},
        {4, "Cavern_gammas"}, {5, "Am241"}, {6, "electron"}, {7, "Muon_mono_E"},
        {8, "neutron"}, {9, "GammasArNeutronCapture"}, {10, "Muon"}, {11, "neutron_spectrum"}, {12, "alpha"}
    };

    G4int sourceName = fPrimary->GetParticleType();
    G4String sourceString = (sourceMap.count(sourceName) > 0) ? sourceMap.at(sourceName) : "unknown";

    G4String positionSource = G4String("_x_y_z_") +
        std::to_string(static_cast<int>(fPrimary->GetSourcePositionX() / cm)) + "_" +
        std::to_string(static_cast<int>(fPrimary->GetSourcePositionY() / cm)) + "_" +
        std::to_string(static_cast<int>(fPrimary->GetSourcePositionZ() / cm)) + "_cm";

    G4String sourceEnergy = std::to_string(static_cast<int>(1000. * fPrimary->GetSourceEnergy())) + "_keV";
    G4String shieldingMaterial = fDetector->GetShieldingMaterial()->GetName();
    G4String shieldingThickness = "_shielding_" + std::to_string(static_cast<int>(round(fDetector->GetShieldingThickness() / cm))) + "cm";
    G4String s_waffle_thick = std::to_string(static_cast<int>(round(fDetector->GetWaffleThickness()/cm))) + "cm_";
    G4String s_shielding_waffle_material = "_WaffleMat_" + s_waffle_thick + fDetector->GetShieldingMaterialWaffle()->GetName();
    G4String detector_type = std::to_string(static_cast<int>(fDetector->GetDetectorType()));

    std::ostringstream folderStream;
    folderStream << fDetector->GetDetectorOutputDataDirectory() << sourceString << "_" << sourceEnergy
                 << positionSource << shieldingThickness << "_" << shieldingMaterial
		 << s_shielding_waffle_material
		 << "_setup_" <<detector_type;
    fFolderName = folderStream.str();

    return fFolderName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String RunAction::GetFileName() {
    G4String setupName = fDetector->GetSetupName();
    fFileName = fDetector->GetDetectorName() + "_" + setupName + "." + fDetector->GetDataType();
    return fFileName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::BeginOfRunAction(const G4Run* aRun) {
    if (mkdir(GetFolderName().c_str(), 0777) != 0) {
        G4cerr << "Error creating directory: " << strerror(errno) << G4endl;
    }

    fFileName = GetFolderName() + "/" + GetFileName();
    fMan = G4AnalysisManager::Instance();
    fMan->SetFirstNtupleId(1);

    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    fMan->SetVerboseLevel(0);
    fMan->OpenFile(fFileName);

    // Create ntuples
    fMan->CreateNtuple("ULALAP", "Detailed MC hits Info");
    fMan->CreateNtupleIColumn("EventID");
    fMan->CreateNtupleDColumn("x");
    fMan->CreateNtupleDColumn("y");
    fMan->CreateNtupleDColumn("z");
    fMan->CreateNtupleDColumn("E");
    fMan->CreateNtupleDColumn("t");
    fMan->CreateNtupleIColumn("pdgCode");
    fMan->FinishNtuple();

    fMan->CreateNtuple("Gammas", "Gammas");
    fMan->CreateNtupleIColumn("EventID");
    fMan->CreateNtupleDColumn("timeCapture");
    fMan->CreateNtupleDColumn("timeDecay");
    fMan->CreateNtupleDColumn("vertex", myVertex);
    fMan->CreateNtupleDColumn("gammas", mygammas);
    fMan->FinishNtuple();

    fMan->CreateNtuple("Primaries", "Primaries");
    fMan->CreateNtupleIColumn("EventID");
    fMan->CreateNtupleIColumn("pdgID");
    fMan->CreateNtupleDColumn("KinE");
    fMan->CreateNtupleDColumn("x");
    fMan->CreateNtupleDColumn("y");
    fMan->CreateNtupleDColumn("z");
    fMan->FinishNtuple();

    fMan->CreateNtuple("Secondaries", "Secondaries");
    fMan->CreateNtupleIColumn("EventID");
    fMan->CreateNtupleSColumn("creator");
    fMan->CreateNtupleIColumn("atNumber");
    fMan->CreateNtupleIColumn("atMass");
    fMan->CreateNtupleIColumn("pdgCode");
    fMan->CreateNtupleDColumn("energy");
    fMan->CreateNtupleDColumn("xn");
    fMan->CreateNtupleDColumn("yn");
    fMan->CreateNtupleDColumn("zn");
    fMan->CreateNtupleDColumn("tn");
    fMan->FinishNtuple();
    
    fMan->CreateNtuple("NeutronkinE", "NeutronKinE");
    fMan->CreateNtupleIColumn("EventID");
    fMan->CreateNtupleDColumn("kinEneutron");
    fMan->CreateNtupleDColumn("xLAr");
    fMan->CreateNtupleDColumn("yLAr");
    fMan->CreateNtupleDColumn("zLAr");
    fMan->FinishNtuple();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::SetFileName(G4String fileName) {
    fFileName = fileName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<G4double> RunAction::FillVectorGammas(std::vector<G4double> gamma_vector) {
    mygammas = gamma_vector;
    return mygammas;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::SetVertexOrigin(std::vector<G4double> v_origin) {
    myVertex = v_origin;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::EndOfRunAction(const G4Run* aRun) {
    fTimer->Stop();
    G4cout << "Number of events = " << aRun->GetNumberOfEvent()
           << " " << *fTimer << G4endl;
    G4cout << "End Run" << G4endl;

    fMan->Write();
    fMan->CloseFile();
}
