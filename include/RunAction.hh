//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file OpNovice/include/OpNoviceRunAction.hh
/// \brief Definition of the OpNoviceRunAction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "RunActionMessenger.hh"
#include "G4AnalysisManager.hh"

class DetectorConstruction;
class PrimaryGeneratorAction;
class RunActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Timer;
class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim=0);
    ~RunAction();

  public:
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);
    void SetVertexOrigin(std::vector<G4double> v_origin);
    void SetFileName(G4String);
    G4String GetFileName();
    G4String GetFolderName();

    std::vector<G4double> GetVectorGammas(){return mygammas;}
    std::vector<G4double> FillVectorGammas(std::vector<G4double> v_gammas);

  private:
    G4AnalysisManager* fMan;
    DetectorConstruction*   fDetector;
    PrimaryGeneratorAction* fPrimary;
    G4Timer* fTimer;
    G4String fFileName;
    G4String fFolderName;
    std::vector<G4double> myVertex;
    std::vector<G4double>   mygammas;

    RunActionMessenger* fRunActionMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*RunAction_h*/
