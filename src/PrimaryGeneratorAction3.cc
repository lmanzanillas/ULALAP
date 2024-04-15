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
/// \file eventgenerator/particleGun/src/PrimaryGeneratorAction3.cc
/// \brief Implementation of the PrimaryGeneratorAction3 class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Implementation of the expected muon flux at the DUNE deep
//Spectrum was taken from: 1811.07912 (Figure 2)
//The expected mu+/mu- ratio of 1.38 is included



#include "PrimaryGeneratorAction3.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction3::PrimaryGeneratorAction3(G4ParticleGun* gun)
: fParticleGun(gun)
{    
  // energy distribution
  //
  InitFunction();
  G4ThreeVector zero(0., 0., 0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction3::InitFunction()
{
  // tabulated function 
  // Y is assumed positive, linear per segment, continuous
  //
  fNPoints = 61;
  const G4double xx[] =   
	{ 0.0*keV, 52.045*keV, 74.349*keV, 171.004*keV, 304.833*keV, 408.922*keV, 602.230*keV, 810.409*keV, 988.848*keV, 1226.766*keV,
	1442.379*keV,1635.688*keV, 1843.866*keV, 2052.045*keV, 2282.528*keV, 2438.662*keV, 2587.361*keV, 2758.364*keV, 2877.323*keV, 
	3115.242*keV, 3263.940*keV, 3472.119*keV, 3687.732*keV, 3940.520*keV, 4118.959*keV, 4275.092*keV, 4520.446*keV, 4743.494*keV, 
	4944.238*keV, 5107.807*keV, 5315.985*keV, 5546.468*keV, 5769.517*keV, 5985.130*keV, 6215.613*keV, 6431.227*keV, 6691.450*keV, 
	6869.888*keV, 7100.372*keV, 7308.550*keV, 7420.074*keV,7501.859*keV, 7620.818*keV, 7769.517*keV, 7962.825*keV, 8096.654*keV, 
	8208.178*keV, 8371.747*keV, 8527.881*keV, 8728.624*keV, 8884.758*keV, 9003.717*keV, 9085.502*keV, 9226.766*keV, 9323.420*keV, 
	9479.554*keV, 9635.688*keV, 9806.691*keV, 10022.305*keV, 9754.647*keV, 9955.390*keV } ;
      
  const G4double yy[] =
	{ 32.433, 15.571, 7.863, 4.394, 3.412, 2.057, 2.424, 2.005, 1.443, 1.443, 1.256, 1.106, 0.915, 0.817, 0.806, 0.634, 0.468,
	 0.341, 0.258, 0.203, 0.139, 0.118, 0.101, 0.109, 0.122, 0.102, 0.113, 0.112, 0.0927, 0.0776, 0.0951, 0.0817, 0.0747, 0.0610,
	 0.0618, 0.0531, 0.0462, 0.0377, 0.0382, 0.0304, 0.0249, 0.0168, 0.0150, 0.00927, 0.0115, 0.0105, 0.00927, 0.00838, 0.008271,
	0.00693, 0.00566, 0.00439, 0.00312, 0.00233, 0.00188, 0.00152, 0.00144, 0.00304, 0.00286, 0.000480, 0.000474 };
  
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

G4double PrimaryGeneratorAction3::RejectAccept()
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

G4double PrimaryGeneratorAction3::InverseCumul()
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



