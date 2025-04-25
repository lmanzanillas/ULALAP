#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4ParticleMomentum.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class PrimaryGeneratorMessenger;
class DetectorConstruction;
class PrimaryGeneratorAction0;
class PrimaryGeneratorAction1;
class PrimaryGeneratorAction2;
class PrimaryGeneratorAction3;
class PrimaryGeneratorAction4;
class PrimaryGeneratorAction5;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{

public:
	PrimaryGeneratorAction(DetectorConstruction*);
	~PrimaryGeneratorAction();

// public:
// 	PrimaryGeneratorAction();
// 	virtual ~PrimaryGeneratorAction();

public:
	virtual void GeneratePrimaries(G4Event*);

	PrimaryGeneratorAction0*  GetAction0() { return fAction0; };
	PrimaryGeneratorAction1*  GetAction1() { return fAction1; };
	PrimaryGeneratorAction2*  GetAction2() { return fAction2; };
	PrimaryGeneratorAction3*  GetAction3() { return fAction3; };
	PrimaryGeneratorAction4*  GetAction4() { return fAction4; };
	PrimaryGeneratorAction5*  GetAction5() { return fAction5; };

	void  SetSourceType(G4int newType);
	void  SetSourceIon_ZA(G4int newZ, G4int newA);
	void  SetSourceDirectionType(G4int newType);
	void  SetSourceGeometry(G4int newType);
	void  SetSourceEnergy(G4double newEnergy);
	void  SetParticleName(G4int Z, G4int A, G4double excitEnergy);
	void  SetSourcePosition(G4ThreeVector newValue);
	void  SetSourcePositionBi1(G4ThreeVector newValue);
	void  SetSourcePositionBi2(G4ThreeVector newValue);
	void  SetSourceDiameter(G4double newValue);
	void  SetSourceBoxXYZ(G4ThreeVector newValue);
	void  SetSourceHalfZ(G4double newValue);
	void  SetCentreCoords(G4ThreeVector coordsOfCentre);
        void  SetPosDisShape(G4String shapeType);
        void  SetSourceShape(G4String shapeType);
        void  GeneratePointsInVolume();
	void  GenerateDirection(G4ThreeVector d_vector);
	void  SetParticleDirection(G4ThreeVector d_vector);

	G4int GetParticleType(void){return particleType;};
	G4int GetIonZ(void){return ionZ;};
	G4int GetIonA(void){return ionA;};
	G4int GetSourceGeometry(void){return fSourceGeometry;};
	G4double GetSourceEnergy(void){return fSourceEnergy;};
	G4String GetParticleName(void){return fParticleName;};
	G4double GetSourcePositionX(void){return CentreCoords.x();};
	G4double GetSourcePositionY(void){return CentreCoords.y();};
	G4double GetSourcePositionZ(void){return CentreCoords.z();};
	G4double GetSourceDiameter(void){return 2*Radius;};

private:

	PrimaryGeneratorAction0* fAction0 = nullptr;
	PrimaryGeneratorAction1* fAction1 = nullptr;
	PrimaryGeneratorAction2* fAction2 = nullptr;
	PrimaryGeneratorAction3* fAction3 = nullptr;
	PrimaryGeneratorAction4* fAction4 = nullptr;
	PrimaryGeneratorAction5* fAction5 = nullptr;

	G4double size_source_x;
	G4double size_source_y;
	G4double size_source_z;
	G4double Radius;
	G4ParticleDefinition* fParticleDefinition;
	G4String fParticleName;
	PrimaryGeneratorMessenger* fPrimaryMessenger;
	G4ParticleGun*  fParticleGun;
	G4int           particleType;
	G4int           fSourceDirectionType;
	G4int           fSourceGeometry;
	G4int           ionZ;
	G4int           ionA;
	G4double	fSourceEnergy;
	G4ParticleMomentum     direction;
	G4ParticleMomentum     direction_input;
	DetectorConstruction*      fDetector;
	G4double		fPhotonWavelength;
	PrimaryGeneratorMessenger* fGunMessenger;
	G4ThreeVector fPoint;
	G4ThreeVector position;
	G4ThreeVector BoxXYZ;
	G4String SourcePosType;
	G4String Shape;
	G4double HalfZ;
	G4double MinTheta, MaxTheta, MinPhi, MaxPhi;
        G4double Phi;
	G4ThreeVector CentreCoords;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*PrimaryGeneratorAction_h*/
