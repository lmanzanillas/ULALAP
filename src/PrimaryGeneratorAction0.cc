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
/// \file eventgenerator/particleGun/src/PrimaryGeneratorAction0.cc
/// \brief Implementation of the PrimaryGeneratorAction0 class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PrimaryGeneratorAction0.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Initialize the vector containing the information about probabilities
PrimaryGeneratorAction0::PrimaryGeneratorAction0(G4ParticleGun* gun)
    : fParticleGun(gun) { 
    InitFunction();
    position = G4ThreeVector(0., 0., 0.);
    GeneratePosition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Determine the next level
G4int PrimaryGeneratorAction0::GetNextLevel(std::vector<G4double> vec_probs, std::vector<G4int> vec_levels) {
    if (vec_probs.empty() || vec_probs.size() != vec_levels.size()) {
        G4cerr << "Error: Probability and level vectors are mismatched or empty!" << G4endl;
        return -1;
    }

    G4double rand_toy = G4UniformRand();
    G4int next_level = -1;
    G4double min = 0.0;

    for (unsigned int i = 0; i < vec_probs.size(); ++i) {
        if (rand_toy >= min && rand_toy < vec_probs[i]) {
            next_level = i;
            break;
        }
        min = vec_probs[i];
    }

    if (next_level < 0 || next_level >= static_cast<G4int>(vec_levels.size())) {
        G4cerr << "Error: Invalid next level index!" << G4endl;
        return -1;
    }

    return vec_levels[next_level];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Store energy levels and probabilities
void PrimaryGeneratorAction0::InitFunction() { 
    vec_E_levels = {0.0, 1409.84, 1611.28, 2490.17, 3518.1, 3937.1, 3981.2, 4444.5, 4573.4, 
                    4634.8, 5089.53, 6579.9, 6821.4, 8791.2};
    vec_probs = {{0.0},
                 {1.0},
                 {1.0},
                 {0.0698, 1.0},
                 {0.2121, 0.3333, 0.9394, 1.0},
                 {0.1604, 1.0},
                 {1.0},
                 {0.9091, 1.0},
                 {1.0},
                 {0.1736, 1.0},
                 {0.2806, 1.0},
                 {1.0},
                 {1.0},
                 {0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0}};
    vec_levels = {{1}, {1}, {1}, {3, 1}, {4, 3, 2, 1}, {2, 1}, {1}, {4, 1}, 
                  {1}, {4, 2}, {4, 2}, {8}, {9}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}};
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Generate random position
void PrimaryGeneratorAction0::GeneratePosition() {  
    G4double xg = (G4UniformRand() - 0.5) * 60. * m;
    G4double yg = (G4UniformRand() - 0.5) * 10. * m;
    G4double zg = (G4UniformRand() - 0.5) * 10. * m;
    position.set(xg, yg, zg);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Generate primary vertex
G4PrimaryVertex* PrimaryGeneratorAction0::GenerateVertex(G4double Eg) {
    G4double theta = 2. * pi * G4UniformRand();
    G4double phi = acos(2. * G4UniformRand() - 1.);
    G4double ux = sin(phi) * cos(theta);
    G4double uy = sin(phi) * sin(theta);
    G4double uz = cos(phi);

    G4ThreeVector directionG(ux, uy, uz);
    G4PrimaryVertex* vertexG = new G4PrimaryVertex(position, 0. * s);

    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    G4PrimaryParticle* particleG = new G4PrimaryParticle(particleDefinition);
    particleG->SetMomentumDirection(directionG);
    particleG->SetKineticEnergy(Eg * keV);
    vertexG->SetPrimary(particleG);
    return vertexG;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Generate primaries
void PrimaryGeneratorAction0::GeneratePrimaries(G4Event* anEvent) {
    G4int n = vec_probs.size() - 1; 
    G4int new_level = GetNextLevel(vec_probs[n], vec_levels[n]);
    if (new_level < 0) return;

    while (new_level > 0) {
        G4int previous_level = new_level;
        new_level = GetNextLevel(vec_probs[previous_level], vec_levels[previous_level]);
        if (new_level < 0) break;

        G4double Eg = vec_E_levels[previous_level] - vec_E_levels[new_level];
        myVertex = GenerateVertex(Eg);
        anEvent->AddPrimaryVertex(myVertex);
    }
}

