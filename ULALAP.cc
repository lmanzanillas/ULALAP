#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "Randomize.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomTools.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include <ctime>
#include <cstdlib>

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " OpNovice [-m macro ] [-u UIsession] [-t nThreads] [-r seed] "
           << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

int main(int argc, char** argv) {
    G4cout << G4endl
           << "=================================================================" << G4endl
           << "                                                                 " << G4endl
           << "   #       #   #            ###   #            ###    #######    " << G4endl
           << "   #       #   #            # #   #            # #    #      #   " << G4endl
           << "   #       #   #           #  #   #           #  #    #      #   " << G4endl
           << "   #       #   #          #####   #          #####    #######    " << G4endl
           << "   #       #   #         #    #   #         #    #    #          " << G4endl
           << "   #       #   #        #     #   #        #     #    #          " << G4endl
           << "    #######    ####### #      #   ####### #      #    #          " << G4endl
           << "                                                                 " << G4endl
           << "=================================================================" << G4endl
           << "       Underground LAr detectors at LAPP                         " << G4endl
           << "                      Geant4 simplified                          " << G4endl
           << "=================================================================" << G4endl
           << G4endl;

    // Evaluate arguments
    G4UIExecutive* ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc, argv);

    G4String macro;
    G4String session;
    G4long myseed = time(nullptr);

    for (G4int i = 1; i < argc; i += 2) {
        if (G4String(argv[i]) == "-m" && i + 1 < argc) {
            macro = argv[i + 1];
        } else if (G4String(argv[i]) == "-u" && i + 1 < argc) {
            session = argv[i + 1];
        } else if (G4String(argv[i]) == "-r" && i + 1 < argc) {
            myseed = std::atoi(argv[i + 1]);
        } else {
            PrintUsage();
            return 1;
        }
    }

    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Construct the default run manager
    auto* runManager = new G4RunManager;

    // Seed the random number generator manually
    G4Random::setTheSeed(myseed);

    // Detector construction
    auto* det = new DetectorConstruction;
    runManager->SetUserInitialization(det);

    // Physics list
    G4PhysListFactory factory;
    G4VModularPhysicsList* physList = nullptr;
    G4String physListName = "FTFP_BERT_HP";
    physList = factory.GetReferencePhysList(physListName);
    runManager->SetUserInitialization(physList);

    // User action initialization
    runManager->SetUserInitialization(new ActionInitialization(det));
    runManager->Initialize();

    // Visualization manager
    G4VisManager* visManager = nullptr;
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        visManager = new G4VisExecutive;
        visManager->Initialize();
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    } else {
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macro);
    }

    // Job termination
    delete visManager;
    delete runManager;

    return 0;
}
