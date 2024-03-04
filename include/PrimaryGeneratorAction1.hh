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
/// \file eventgenerator/particleGun/include/PrimaryGeneratorAction1.hh
/// \brief Definition of the PrimaryGeneratorAction1 class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#ifndef PrimaryGeneratorAction1_h
#define PrimaryGeneratorAction1_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4PrimaryVertex.hh"
#include "globals.hh"
#include <vector>

class G4ParticleGun;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction1
{
  public:
    PrimaryGeneratorAction1(G4ParticleGun*);    
   ~PrimaryGeneratorAction1() = default;

  public:
    void GeneratePrimaries(G4Event*);
    void GeneratePosition();
    G4PrimaryVertex* GenerateVertex(G4double);
    G4int GetNextLevel(std::vector<G4double> vec_probs,std::vector<G4int> vec_levels);

  private:
    G4ParticleGun*  fParticleGun = nullptr;
    G4ThreeVector position;

    std::vector<G4double> vec_E_levels;
    std::vector<std::vector<G4double>> vec_probs;
    std::vector<std::vector<G4int>> vec_levels;
    G4PrimaryVertex* myVertex;

  private:
    void InitFunction();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

