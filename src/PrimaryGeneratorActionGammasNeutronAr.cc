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
/// \file eventgenerator/particleGun/src/PrimaryGeneratorAction1.cc
/// \brief Implementation of the PrimaryGeneratorAction1 class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorActionGammasNeutronAr.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction1::PrimaryGeneratorAction1(G4ParticleGun* gun)
: fParticleGun(gun)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4int PrimaryGeneratorAction1::GetNextLevel(std::vector<G4double> vec_probs,std::vector<G4int> vec_levels)
{
  G4double rand_toy = G4UniformRand();
  G4int next_level = -99;
  unsigned int vecSize = vec_probs.size();
  G4double min = -99.0;
  for(unsigned int i = 0; i < vecSize; ++i)
  {
	if (i == 0){
            min = 0.0;
	}
        else{
            min = vec_probs[i-1];
	}
	if (min < rand_toy < vec_probs[i]){
            next_level = i;
            break;
	}
  }
  return vec_levels[next_level];

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction1::InitFunction()
{ 

  vec_E_levels = {0.0, 167.11, 515.77, 1033.94, 1353.71, 2397.9, 2733.1, 2948.5, 3009.8, 3326.3, 3968.1, 4270.0, 6098.9};

  vec_probs = { {0.0},
		{1.0},
		{0.2188, 1.0},
		{0.4413, 1.0},
		{0.1613, 0.9677, 1.0},
		{0.6494, 0.9545, 1.0},
		{0.0826, 1.0},
		{0.0482, 0.3976, 1.0},
		{1.0},
		{0.1037, 0.1308, 0.9282, 1.0},
		{0.5917, 1.0},
		{1.0},
	  	{0.0, 0.0, 0.1161, 0.1187, 0.6691, 0.767, 0.8091, 0.8491, 0.86, 0.946, 0.99, 1.0} };

  vec_levels = {{1},
		{1},
		{2, 1},
		{3, 2},
		{3, 2, 1},
		{5, 3, 2},
		{5, 2},
		{5, 3, 2},
		{2},
		{5, 4, 3, 2},
		{5, 3},
		{2},
  		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} };
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction1::GeneratePrimaries(G4Event* anEvent)
{
  G4int n = vec_probs.size(); 
  //cout << v[n - 1] << endl; 
  G4int new_level = GetNextLevel(vec_probs[n-1],vec_levels[n-1]) - 1;
  G4double Eg = vec_E_levels[n-1] - vec_E_levels[new_level];
  G4cout<<"new level: "<<new_level<<" Eg: "<<Eg<<G4endl;
  //First gamma

  while (new_level != 0){
        G4int previous_level = new_level;
        new_level = GetNextLevel(vec_probs[new_level],vec_levels[new_level]) - 1;
        Eg = vec_E_levels[previous_level] - vec_E_levels[new_level];
	G4cout<<"new level: "<<new_level<<" Eg: "<<Eg<<G4endl;
  }
  const G4double r = 2*mm;
  const G4double zmax = 8*mm;   
  
  //vertex 1 uniform on cylinder
  //
  G4double alpha = twopi*G4UniformRand();  //alpha uniform in (0, 2*pi)
  G4double ux = std::cos(alpha);
  G4double uy = std::sin(alpha);
  G4double z = zmax*(2*G4UniformRand() - 1);  //z uniform in (-zmax, +zmax)
        
  fParticleGun->SetParticlePosition(G4ThreeVector(r*ux,r*uy,z));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,0));    
  fParticleGun->SetParticleEnergy(1*MeV);
  fParticleGun->GeneratePrimaryVertex(anEvent);
  
  //vertex 2 at opposite
  //
  alpha += pi;
  ux = std::cos(alpha);
  uy = std::sin(alpha);        
  fParticleGun->SetParticlePosition(G4ThreeVector(r*ux,r*uy,z));
  
  //particle 2 at vertex 2
  //
  const G4double dalpha = 10*deg;
  ux = std::cos(alpha + dalpha);
  uy = std::sin(alpha + dalpha);        
  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,0));    
  fParticleGun->SetParticleEnergy(1*keV);
  fParticleGun->GeneratePrimaryVertex(anEvent);
   
  //particle 3 at vertex 3 (same as vertex 2)
  //
  ux = std::cos(alpha - dalpha);
  uy = std::sin(alpha - dalpha);        
  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,0));    
  fParticleGun->SetParticleEnergy(1*GeV);
  fParticleGun->GeneratePrimaryVertex(anEvent);
  
  // randomize time zero of anEvent
  //
  G4double tmin = 0*s, tmax = 10*s;
  G4double t0 = tmin + (tmax - tmin)*G4UniformRand();
  G4PrimaryVertex* aVertex = anEvent->GetPrimaryVertex();
  while (aVertex) {
    aVertex->SetT0(t0);
    aVertex = aVertex->GetNext();
  }
}


