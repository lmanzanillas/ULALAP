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
/// \file eventgenerator/particleGun/src/PrimaryGeneratorAction2.cc
/// \brief Implementation of the PrimaryGeneratorAction2 class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction2.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction2::PrimaryGeneratorAction2(G4ParticleGun* gun)
: fParticleGun(gun)
{ 
   //E levels and probs
   InitFunction();
   G4ThreeVector zero(0., 0., 0.);
   position = zero;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4int PrimaryGeneratorAction2::GetNextLevel(std::vector<G4double> vec_probs,std::vector<G4int> vec_levels)
{
  G4double rand_toy = G4UniformRand();
  G4int next_level = -99;
  unsigned int vecSize = vec_probs.size();
  G4double min = -999.0;
  //G4cout<<"rand "<<rand_toy<<" vecSize: "<<vecSize<<G4endl;
  for(unsigned int i = 0; i < vecSize; ++i)
  {
	if (i == 0){
            min = 0.0;
	}
        else{
            min = vec_probs[i-1];
	}
	if (min < rand_toy and rand_toy < vec_probs[i]){
            next_level = i;
	    //G4cout<<"i: "<<i<<" min: "<<min<<" vec_probs[i] "<<vec_probs[i]<<" next_level "<<next_level<<" vec[next]: "<<vec_levels[next_level]<<G4endl;
            break;
	}
  }
  return vec_levels[next_level];

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction2::InitFunction()
{ 

  vec_E_levels = {0.0, 167.11, 516.10, 1034.70, 1353.71, 2397.9, 2733.1, 2948.5, 3009.8, 3326.3, 3968.1, 4270.0, 6098.9};

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

  vec_levels = {{0},
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
void PrimaryGeneratorAction2::GeneratePosition()
{  
  //To do: read the ar volume dims and use that as info instead of hard coding.... 
  G4double xg = (G4UniformRand() - 0.5)*60.*m;
  G4double yg = (G4UniformRand() - 0.5)*10.*m;
  G4double zg = (G4UniformRand() - 0.5)*10.*m;
  position.setX(xg);
  position.setY(yg);
  position.setZ(zg);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4PrimaryVertex* PrimaryGeneratorAction2::GenerateVertex(G4double Eg)
{
  G4double theta = 2. * pi * G4UniformRand()*rad;
  G4double phi = acos(G4UniformRand() -1.);
  G4double ux = sin(phi)*cos(theta);
  G4double uy = sin(phi)*sin(theta);
  G4double uz = cos(phi);



  G4ThreeVector directionG(ux,uy,uz);
  G4double timeG = 0*s;
  G4PrimaryVertex* vertexG = new G4PrimaryVertex(position, timeG);
  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  G4PrimaryParticle* particleG = new G4PrimaryParticle(particleDefinition);
  particleG->SetMomentumDirection(directionG);
  particleG->SetKineticEnergy(Eg*keV);
  vertexG->SetPrimary(particleG);
  return vertexG;
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction2::GeneratePrimaries(G4Event* anEvent)
{
  G4int n = vec_probs.size(); 
  G4int new_level = GetNextLevel(vec_probs[n-1],vec_levels[n-1]);
  G4double Eg = vec_E_levels[n-1] - vec_E_levels[new_level];
  //G4cout<<"new level: "<<new_level<<" Eg: "<<Eg<<G4endl;
  GeneratePosition();
  myVertex = GenerateVertex(Eg);
  anEvent->AddPrimaryVertex(myVertex);
  //Loop until we reach the ground level
  while (new_level > 0){
        G4int previous_level = new_level;
        new_level = GetNextLevel(vec_probs[new_level],vec_levels[new_level]) - 1;
        Eg = vec_E_levels[previous_level] - vec_E_levels[new_level];
	myVertex = GenerateVertex(Eg);
	anEvent->AddPrimaryVertex(myVertex);
	//G4cout<<"Li: "<<vec_E_levels[previous_level]<<" Lf: "<<vec_E_levels[new_level]<<" new level: "<<new_level<<" Eg: "<<Eg<<G4endl;
  }
  
  
}


