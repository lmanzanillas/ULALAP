#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4SteppingVerbose.hh"
#include "Randomize.hh"

#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"

#include "G4MTRunManager.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "G4ParticleHPManager.hh"
#include "unistd.h"
#include "time.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " OpNovice [-m macro ] [-u UIsession] [-t nThreads] [-r seed] "
           << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    G4cout << " " << G4endl
    << "================================================================="<< G4endl
    << "                                                                 "<< G4endl
    << "   #       #   #            ###   #            ###    #######    "<< G4endl
    << "   #       #   #            # #   #            # #    #      #   "<< G4endl
    << "   #       #   #           #  #   #           #  #    #      #   "<< G4endl
    << "   #       #   #          #####   #          #####    #######    "<< G4endl
    << "   #       #   #         #    #   #         #    #    #          "<< G4endl
    << "   #       #   #        #     #   #        #     #    #          "<< G4endl
    << "    #######    ####### #      #   ####### #      #    #          "<< G4endl
    << "                                                                 "<< G4endl
    << "================================================================="<< G4endl
    << "       Underground LAr detectors at LAPP                         "<< G4endl
    << "                      Geant4 simplified                          "<< G4endl
    << "================================================================="<< G4endl
    << G4endl << G4endl;
  // Evaluate arguments
  //
  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc,argv);


  G4String macro;
  G4String session;

  G4cout<<"seed initialized "<<G4endl;
  //G4long myseed = 345354;
  G4long myseed = time((time_t *)NULL);
  for ( G4int i=1; i<argc; i=i+2 ) {
     if      ( G4String(argv[i]) == "-m" ) macro   = argv[i+1];
     else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
     else if ( G4String(argv[i]) == "-r" ) myseed  = atoi(argv[i+1]);
     else {
     	PrintUsage();
     	return 1;
    }
  }

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Seed the random number generator manually
  G4Random::setTheSeed(myseed);

  // Initialize G4 kernel
  //

  // Set mandatory initialization classes
  //
  G4cout<<"constructing detector "<<G4endl;
  // Detector construction
  DetectorConstruction* det = new DetectorConstruction;
  runManager->SetUserInitialization(det);

  //
  // Physics list
  // Physics list factory
  G4PhysListFactory factory;
  G4VModularPhysicsList* physList = nullptr;
  // Reference PhysicsList via its name
  G4String physListName = "FTFP_BERT_HP";
  physList = factory.GetReferencePhysList(physListName);
  runManager->SetUserInitialization(physList);
  //runManager-> SetUserInitialization(new PhysicsList());

  //test, next line
  //G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization(det));
  runManager->Initialize();


  //initialize visualization
  G4VisManager* visManager = nullptr;

  //get the pointer to the User Interface manager 
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (ui)  {
   //interactive mode
   visManager = new G4VisExecutive;
   visManager->Initialize();
   UImanager->ApplyCommand("/control/execute vis.mac");
   ui->SessionStart();
   delete ui;
  } else  {
   //batch mode  
   G4String command = "/control/execute ";
   UImanager->ApplyCommand(command+macro);
  }

  //job termination
  delete visManager;
  delete runManager;


  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
