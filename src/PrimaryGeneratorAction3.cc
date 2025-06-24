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
//Implementation of the expected cavern neutron flux at the DUNE caverns



#include "PrimaryGeneratorAction3.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include <fstream>
#include <numeric>
#include <stdexcept>
#include <iostream>
#include <random>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction3::PrimaryGeneratorAction3(G4ParticleGun* gun)
:
generator(std::random_device{}()),
distribution(0.0, 1.0)
{    
  // energy distribution

  G4ThreeVector zero(0., 0., 0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction3::LoadEnergyDistribution(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open energy data file");

    G4double e, c;
    while (file >> e >> c) {
        energies.push_back(e);
        counts.push_back(c);
    }

    if (energies.empty() || counts.empty())
        throw std::runtime_error("No energy data loaded");
}

void PrimaryGeneratorAction3::BuildCDF() {
    G4double total = std::accumulate(counts.begin(), counts.end(), 0.0);
    if (total == 0) throw std::runtime_error("Counts sum to zero");

    cdf.resize(counts.size());
    G4double cumulative = 0;
    for (size_t i = 0; i < counts.size(); ++i) {
        cumulative += counts[i] / total;
        cdf[i] = cumulative;
    }
    cdf.back() = 1.0; // Ensure it reaches 1
}

G4double PrimaryGeneratorAction3::SampleEnergy() {
    G4double r = distribution(generator);
    auto it = std::lower_bound(cdf.begin(), cdf.end(), r);
    size_t idx = std::distance(cdf.begin(), it);

    if (idx == 0) return energies[0];
    if (idx >= energies.size()) return energies.back();

    // Linear interpolation
    G4double x0 = cdf[idx - 1], x1 = cdf[idx];
    G4double e0 = energies[idx - 1], e1 = energies[idx];
    return e0 + (r - x0) / (x1 - x0) * (e1 - e0);
}
