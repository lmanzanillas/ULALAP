// Make this appear first!
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

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* kin)
 : G4UserRunAction(),fDetector(det),fPrimary(kin),
   fTimer(0)
{
  fTimer = new G4Timer;
  fMan = G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::string datetime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d_%m_%Y_%H_%M",timeinfo);
    return std::string(buffer);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String RunAction::GetFolderName()
{
  G4int sourceName = fPrimary->GetParticleType();
  G4String sourceString;

  switch (sourceName){
    case 0:
      sourceString = "gamma";
      break;
    case 1:
      sourceString = "Fe55";
      break;
    case 2:
      sourceString = "Cs137";
      break;
    case 3:
      sourceString = "Bi207";
      break;
    case 4:
      sourceString = "Sr90";
      break;
    case 5:
      sourceString = "Am241";
      break;
    case 6:
      sourceString = "electron";
      break;
    case 7:
      sourceString = "optical_photon";
      break;
    case 8:
      sourceString = "neutron";
      break;
    case 9:
      sourceString = "GammasArNeutronCapture";
      break;
    case 10:
      sourceString = "Muon";
      break;
    case 11:
      sourceString = "neutron_spectrum";
      break;
  }

  G4cout << sourceString << G4endl;

  G4String positionSource ="_x_y_z_"+std::to_string((int)(fPrimary->GetSourcePositionX()/cm))+"_"+std::to_string((int)(fPrimary->GetSourcePositionY()/cm))+"_"+std::to_string((int)(fPrimary->GetSourcePositionZ()/cm))+"_cm";
  G4String sourceEnergy =std::to_string((int)(1000. * fPrimary->GetSourceEnergy()))+"_keV";

  fTimer->Start();
  G4String s_shileding_thickness ="_shileding_" + std::to_string((int)round(fDetector->GetShieldingThickness()/cm)) + "cm";
  G4String s_shileding_material =s_shileding_thickness + "_" + fDetector->GetShieldingMaterial()->GetName();
  G4String s_waffle_thick = std::to_string((int)round(fDetector->GetWaffleThickness()/cm)) + "cm_";
  G4String s_shielding_waffle_material = "_WaffleMat_" + s_waffle_thick + fDetector->GetShieldingMaterialWaffle()->GetName();
  G4String s_setup_type = s_shileding_material + s_shielding_waffle_material + "_setup_" + std::to_string((int)fDetector->GetDetectorType());

  fFolderName = fDetector->GetDetectorOutputDataDirectory()+sourceString+"_"+sourceEnergy
	  +positionSource 
	  +s_setup_type
	  +"";

  return fFolderName;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String RunAction::GetFileName()
{
  G4String s_setup_Name =fDetector->GetSetupName();
  fFileName = fDetector->GetDetectorName()
	  +"_"+s_setup_Name+".csv";
  return fFileName;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::BeginOfRunAction(const G4Run* aRun)
{


  //Create folder
  mkdir(GetFolderName(), 0777);
  //create GetFileName
  fFileName =GetFolderName() + "/" + GetFileName();
  //crate analysis manager	
  fMan = G4AnalysisManager::Instance();
  //fMan->SetDefaultFileType("Hdf5");

  //fMan->SetNtupleMerging(true);
  fMan->SetFirstNtupleId(1);
  
  

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  fMan->SetVerboseLevel(0);
  fMan->OpenFile(fFileName);
  fMan->CreateNtuple("ULALAP","Detailed MC Info");
  fMan->CreateNtupleIColumn("EventID");
  fMan->CreateNtupleDColumn("x");
  fMan->CreateNtupleDColumn("y");
  fMan->CreateNtupleDColumn("z");
  fMan->CreateNtupleDColumn("E");
  fMan->CreateNtupleDColumn("t");
  fMan->FinishNtuple();

  //declared in include .hh file
  //std::vector<G4double>   mygammas;

  fMan->CreateNtuple("Gammas","Gammas");
  fMan->CreateNtupleIColumn("EventID");
  fMan->CreateNtupleDColumn("timeCapture");
  fMan->CreateNtupleDColumn("timeDecay");
  fMan->CreateNtupleDColumn("vertex",myVertex);
  fMan->CreateNtupleDColumn("gammas",mygammas);
  fMan->FinishNtuple();

  fMan->CreateNtuple("Primaries","Primaries");
  fMan->CreateNtupleIColumn("EventID");
  fMan->CreateNtupleIColumn("pdgID");
  fMan->CreateNtupleDColumn("KinE");
  fMan->CreateNtupleDColumn("x");
  fMan->CreateNtupleDColumn("y");
  fMan->CreateNtupleDColumn("z");
  fMan->FinishNtuple();

  fMan->CreateNtuple("Secondaries","Secondaries");
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
}

void RunAction::SetFileName(G4String fileName)
{
  fFileName = fileName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector<G4double> RunAction::FillVectorGammas(std::vector<G4double> gamma_vector)
{
  mygammas = gamma_vector;
  return mygammas;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::SetVertexOrigin(std::vector<G4double> v_origin)
{
	myVertex = v_origin;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;
  G4cout << "End Run" << G4endl;

  fMan->Write();
  fMan->CloseFile();
  //delete fMan;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
