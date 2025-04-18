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
/// \file eventgenerator/particleGun/include/PrimaryGeneratorAction4.hh
/// \brief Definition of the PrimaryGeneratorAction4 class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#ifndef PrimaryGeneratorAction4_h
#define PrimaryGeneratorAction4_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4PrimaryVertex.hh"
#include "globals.hh"
#include <vector>

class G4ParticleGun;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction4
{
  public:
    PrimaryGeneratorAction4(G4ParticleGun*);    
   ~PrimaryGeneratorAction4() = default;
   void SetBi1Position(G4ThreeVector);
   void SetBi2Position(G4ThreeVector);

  public:
    void GeneratePrimaries(G4Event*);
    void GeneratePosition(G4ThreeVector v_pos, G4double radius);
    G4PrimaryVertex* GenerateVertex(G4ThreeVector newpos);
    G4ThreeVector GetBi1Position(){return positionBi1;}
    G4ThreeVector GetBi2Position(){return positionBi2;}

  private:
    G4ParticleGun*  fParticleGun = nullptr;
    G4ThreeVector positionBi;
    G4ThreeVector positionBi1;
    G4ThreeVector positionBi2;
    G4ThreeVector CentreCoords;

    G4PrimaryVertex* myVertex;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

