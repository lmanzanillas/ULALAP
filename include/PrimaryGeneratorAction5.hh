#ifndef PRIMARYGENERATORACTION5_HH
#define PRIMARYGENERATORACTION5_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

#include <vector>
#include <random>

class G4Event;

class PrimaryGeneratorAction5
{
public:
    PrimaryGeneratorAction5(G4ParticleGun*);
   ~PrimaryGeneratorAction5() = default;


public:
    void LoadEnergyDistribution(const std::string& filename);
    void BuildCDF();
    G4double SampleEnergy();

private:
    std::vector<G4double> energies;
    std::vector<G4double> counts;
    std::vector<G4double> cdf;

    std::mt19937 generator;
    std::uniform_real_distribution<> distribution;
};

#endif
