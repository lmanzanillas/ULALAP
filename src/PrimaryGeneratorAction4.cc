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
/// \file eventgenerator/particleGun/src/PrimaryGeneratorAction4.cc
/// \brief Implementation of the PrimaryGeneratorAction4 class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PrimaryGeneratorAction4.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Initialize the vector containing the information about probabilities
PrimaryGeneratorAction4::PrimaryGeneratorAction4(G4ParticleGun* gun)
    : fParticleGun(gun) { 
    G4ThreeVector zero(0., 0., 0.);
    CentreCoords = zero;
    position = zero;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Generate random position
void PrimaryGeneratorAction4::GeneratePosition(G4ThreeVector new_pos, G4double Radius) { 
    G4double x=0., y=0., z=0.;
    G4ThreeVector  RandPos = G4ThreeVector(0.,0.,0.);
    x = Radius*2.;
    z = Radius*2.;
    while(((x*x)+(z*z)) > (Radius*Radius)) {
      x = G4UniformRand();
      z = G4UniformRand();
      x = (x*2.*Radius) - Radius;
      z = (z*2.*Radius) - Radius;
    }
   RandPos.setX(x);
   RandPos.setY(y);
   RandPos.setZ(z);
   position = CentreCoords + new_pos + RandPos;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Generate primary vertex
G4PrimaryVertex* PrimaryGeneratorAction4::GenerateVertex(G4ThreeVector pos_bi) {
	G4double excitEnergy = 0.*eV;
	G4int Z=83, A=207;
    	G4ThreeVector directionIon(0., 0., 0.);
	G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
    	G4PrimaryParticle* particleBi207 = new G4PrimaryParticle(ion);
    	particleBi207->SetMomentumDirection(directionIon);

        GeneratePosition(pos_bi,0.25*cm);

        G4PrimaryVertex* vertexBi207 = new G4PrimaryVertex(position, 0. * s);
    	vertexBi207->SetPrimary(particleBi207);

    	return vertexBi207;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Generate primaries
void PrimaryGeneratorAction4::GeneratePrimaries(G4Event* anEvent) {
	//The units are taken as mm in the G4ThreeVectors below
	G4ThreeVector posBi1 = G4ThreeVector(0.*cm,15.*cm,0.*cm);
	G4ThreeVector posBi2 = G4ThreeVector(0.*cm,15.*cm,50.*cm);
	G4PrimaryVertex* myVertex1 = GenerateVertex(posBi1);
        anEvent->AddPrimaryVertex(myVertex1);
	//fDetector->SetBiSourcePosition(posBi1);
        G4PrimaryVertex* myVertex2 = GenerateVertex(posBi2);
        anEvent->AddPrimaryVertex(myVertex2);
}

