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
//Implementation of the expected muon flux at the DUNE deep
//Spectrum was taken from: 1811.07912 (Figure 2)
//The expected mu+/mu- ratio of 1.38 is included



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
  // energy distribution
  //
  InitFunction();
  G4ThreeVector zero(0., 0., 0.);
  position = zero;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction2::GeneratePosition()
{  
  //To do: read the LAr volume dims and use that as info instead of hard coding.... 
  //G4double xg = (G4UniformRand() - 0.5)*50.*m;
  //G4double zg = (G4UniformRand() - 0.5)*10.*m;
  G4double xg = (G4UniformRand() - 0.5)*50.*m;
  G4double zg = 8.*m + (G4UniformRand() - 0.5)*1.*m;//Test what bkg are coming from muon not being detected in detector
  G4double yg = 12.*m;
  position.setX(xg);
  position.setY(yg);
  position.setZ(zg);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction2::GeneratePrimaries(G4Event* anEvent)
{
  // uniform solid angle
  G4double cosAlpha = 1. - 2*G4UniformRand();   //cosAlpha uniform in [-1,+1]
  G4double sinAlpha = std::sqrt(1. - cosAlpha*cosAlpha);
  G4double psi      = twopi*G4UniformRand();   //psi uniform in [0, 2*pi]  
  G4ThreeVector dir(sinAlpha*std::cos(psi)/20.,-1,sinAlpha*std::sin(psi)/20.);//we will consider only muons going down (small ux, small uy,-1) 
  //Generate random position
  GeneratePosition();
  //set energy from a tabulated distribution
  //G4double energy = RejectAccept();
  G4double energy = InverseCumul(); 
  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("mu+"); 
  if ( G4UniformRand() > 0.5798 ){//We know that the ratio mu+/mu- is 1.38, which gives 0.5798 prob of having mu+
	 particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("mu-"); 
  }
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticlePosition(position);
  fParticleGun->SetParticleMomentumDirection(dir);
  fParticleGun->SetParticleEnergy(energy);    

  //create vertex
  //   
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction2::InitFunction()
{
  // tabulated function 
  // Y is assumed positive, linear per segment, continuous
  //
  fNPoints = 46;
  const G4double xx[] =   
	{ 1.070*GeV, 1.382*GeV, 1.702*GeV, 2.077*GeV, 2.708*GeV, 3.368*GeV, 4.186*GeV, 5.013*GeV, 
	  6.353*GeV, 8.362*GeV, 11.324*GeV, 15.627*GeV, 21.363*GeV, 28.929*GeV, 38.804*GeV, 54.062*GeV,
	  79.723*GeV, 114.271*GeV, 175.022*GeV, 253.256*GeV, 346.209*GeV, 442.911*GeV, 582.960*GeV, 704.578*GeV,
	  835.586*GeV, 972.352*GeV, 1131.504*GeV, 1341.893*GeV, 1576.395*GeV, 1851.877*GeV, 2154.988*GeV, 2460.641*GeV,
	  2809.648*GeV, 3147.939*GeV, 3560.535*GeV, 4065.546*GeV, 4598.412*GeV, 5250.630*GeV, 6052.426*GeV, 6781.159*GeV, 
	  7816.674*GeV, 8841.196*GeV, 9812.302*GeV, 11636.774*GeV, 12317.423*GeV, 14746.742*GeV } ;
      
  const G4double yy[] =
	{ 4.373e-11, 5.642e-11, 6.917e-11, 8.479e-11, 1.024e-10, 1.256e-10, 1.527e-10, 1.820e-10,
	  2.232e-10, 2.837e-10, 3.687e-10, 4.791e-10, 6.047e-10, 7.744e-10, 9.494e-10, 1.181e-9, 
	  1.417e-9, 1.627e-9, 1.724e-9, 1.638e-9, 1.469e-9, 1.251e-9, 9.918e-10, 8.149e-10, 6.597e-10, 
	  5.305e-10, 4.264e-10, 3.306e-10, 2.418e-10, 1.781e-10, 1.312e-10, 1.010e-10, 7.658e-11, 
	  5.937e-11, 4.373e-11, 3.367e-11, 2.461e-11, 1.813e-11, 1.297e-11, 9.217e-12, 6.741e-12, 
	  4.824e-12, 3.477e-12, 2.543e-12, 1.819e-12, 1.321e-12 };
  
  //copy arrays in std::vector and compute fMax
  //
  fX.resize(fNPoints); fY.resize(fNPoints);
  fYmax = 0.;
  for (G4int j=0; j<fNPoints; j++) {
    fX[j] = xx[j]; fY[j] = yy[j];
    if (fYmax < fY[j]) fYmax = fY[j];
  };
     
  //compute slopes
  //
  fSlp.resize(fNPoints);
  for (G4int j=0; j<fNPoints-1; j++) { 
    fSlp[j] = (fY[j+1] - fY[j])/(fX[j+1] - fX[j]);
  };
  
  //compute cumulative function
  //
  fYC.resize(fNPoints);  
  fYC[0] = 0.;
  for (G4int j=1; j<fNPoints; j++) {
    fYC[j] = fYC[j-1] + 0.5*(fY[j] + fY[j-1])*(fX[j] - fX[j-1]);
  };     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction2::RejectAccept()
{
  // tabulated function 
  // Y is assumed positive, linear per segment, continuous
  // (see Particle Data Group: pdg.lbl.gov --> Monte Carlo techniques)
  // 
  G4double Xrndm = 0., Yrndm = 0., Yinter = -1.;
  
  while (Yrndm > Yinter) {
    //choose a point randomly
    Xrndm = fX[0] + G4UniformRand()*(fX[fNPoints-1] - fX[0]);
    Yrndm = G4UniformRand()*fYmax;
    //find bin
    G4int j = fNPoints-2;
    while ((fX[j] > Xrndm) && (j > 0)) j--;
    //compute Y(x_rndm) by linear interpolation
    Yinter = fY[j] + fSlp[j]*(Xrndm - fX[j]);
  };
  return Xrndm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction2::InverseCumul()
{
  // tabulated function
  // Y is assumed positive, linear per segment, continuous 
  // --> cumulative function is second order polynomial
  // (see Particle Data Group: pdg.lbl.gov --> Monte Carlo techniques)
  
  //choose y randomly
  G4double Yrndm = G4UniformRand()*fYC[fNPoints-1];
  //find bin
  G4int j = fNPoints-2;
  while ((fYC[j] > Yrndm) && (j > 0)) j--;
  //y_rndm --> x_rndm :  fYC(x) is second order polynomial
  G4double Xrndm = fX[j];
  G4double a = fSlp[j];
  if (a != 0.) {
    G4double b = fY[j]/a, c = 2*(Yrndm - fYC[j])/a;
    G4double delta = b*b + c;
    G4int sign = 1; if (a < 0.) sign = -1;
    Xrndm += sign*std::sqrt(delta) - b;    
  } else if (fY[j] > 0.) {
    Xrndm += (Yrndm - fYC[j])/fY[j];
  };
  return Xrndm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



