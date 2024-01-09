#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4ParticleMomentum.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class PrimaryGeneratorMessenger;
class DetectorConstruction;

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
	void  SetSourceType(G4int newType);
	void  SetSourceDirectionType(G4int newType);
	void  SetSourceGeometry(G4int newType);
	void  SetSourceEnergy(G4double newEnergy);
	void  SetParticleName(G4int Z, G4int A, G4double excitEnergy);
	void  SetSourcePosition(G4ThreeVector newValue);
	void  SetSourceDiameter(G4double newValue);
	void  SetSourceBoxXYZ(G4ThreeVector newValue);
	void  SetSourceHalfZ(G4double newValue);
	void  SetCentreCoords(G4ThreeVector coordsOfCentre);
        void  SetPosDisShape(G4String shapeType);
        void  SetSourceShape(G4String shapeType);
        void  GeneratePointsInVolume();
	void  GenerateDirection(G4ThreeVector d_vector);

	G4int GetParticleType(void){return particleType;};
	G4int GetSourceGeometry(void){return fSourceGeometry;};
	G4double GetSourceEnergy(void){return fSourceEnergy;};
	G4String GetParticleName(void){return fParticleName;};
	G4double GetSourcePositionX(void){return fPositionX;};
	G4double GetSourcePositionY(void){return fPositionY;};
	G4double GetSourcePositionZ(void){return fPositionZ;};
	G4double GetSourceDiameter(void){return 2*Radius;};

private:
	G4double fPositionX;
	G4double fPositionY;
	G4double fPositionZ;
	G4double size_source_x;
	G4double size_source_y;
	G4double size_source_z;
	G4double Radius;
	G4String fParticleName;
	PrimaryGeneratorMessenger* fPrimaryMessenger;
	G4ParticleGun*  fParticleGun;
	G4int           particleType;
	G4int           fSourceDirectionType;
	G4int           fSourceGeometry;
	G4double	fSourceEnergy;
	G4ParticleMomentum     direction;
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
