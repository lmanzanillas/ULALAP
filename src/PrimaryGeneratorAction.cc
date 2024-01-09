#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4MTRunManager.hh"

#include "Randomize.hh"
#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4DecayTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalConstants.hh"

#include "G4Navigator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
: G4VUserPrimaryGeneratorAction()
{
        fDetector = det;
	fPrimaryMessenger = new PrimaryGeneratorMessenger(this);
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);
	G4ThreeVector zero(0., 0., 0.);
	position = zero;
	CentreCoords = zero;
	BoxXYZ = zero;
	Radius = 0.1*m;
	SourcePosType = "Volume";
        Shape = "Point";
        HalfZ = 0.;
        size_source_x = 50.;
        size_source_y = 10.;
        size_source_z = 10.;
	particleType = 8;
	fSourceDirectionType = 1;
	fSourceEnergy = 1*eV;
	fPhotonWavelength = 0;
	fParticleName = "void";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::SetPosDisShape(G4String shapeType)
{
  Shape = shapeType;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::SetCentreCoords(G4ThreeVector coordsOfCentre)
{
  CentreCoords = coordsOfCentre;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::SetSourceBoxXYZ(G4ThreeVector sizeBoxXYZ)
{
  BoxXYZ = sizeBoxXYZ;
  size_source_x = BoxXYZ[0];
  size_source_y = BoxXYZ[1];
  size_source_z = BoxXYZ[2];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePointsInVolume()
{
  G4ThreeVector RandPos;
  G4double x=0., y=0., z=0.;
  
  if(SourcePosType != "Volume")
    G4cout << "Error SourcePosType not Volume" << G4endl;
  
  if(Shape == "Sphere") {
    x = Radius*2.;
    y = Radius*2.;
    z = Radius*2.;
    while(((x*x)+(y*y)+(z*z)) > (Radius*Radius)) {
      x = G4UniformRand();
      y = G4UniformRand();
      z = G4UniformRand();
      
      x = (x*2.*Radius) - Radius;
      y = (y*2.*Radius) - Radius;
      z = (z*2.*Radius) - Radius;
    }
  }

  else if(Shape == "Cylinder") {
    x = Radius*2.;
    y = Radius*2.;
    while(((x*x)+(y*y)) > (Radius*Radius)) {
      x = G4UniformRand();
      y = G4UniformRand();
      z = G4UniformRand();
      x = (x*2.*Radius) - Radius;
      y = (y*2.*Radius) - Radius;
      z = (z*2.*HalfZ) - HalfZ;
    }
  }

  else if(Shape == "Box") {
      x = (G4UniformRand() - 0.5)*size_source_x;
      y = (G4UniformRand() - 0.5)*size_source_y;
      z = (G4UniformRand() - 0.5)*size_source_z;
  }
  
  else if(Shape == "Point") {
      RandPos = G4ThreeVector(0.,0.,0.);
  }
  else
    G4cout << "Error: Volume Shape Does Not Exist" << G4endl;

  RandPos.setX(x);
  RandPos.setY(y);
  RandPos.setZ(z);
  position = CentreCoords + RandPos;

}

// Randomises placement and momentum vectors for required sources.

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4double ionCharge   = 0.*eplus;
	G4double excitEnergy = 0.*eV;
	G4int Z=0, A=0;
	G4ParticleDefinition* ion;
	GeneratePointsInVolume();
        //Define a more realistic source geometry
        G4double theta = 2. * pi * G4UniformRand()*rad;
	//G4cout<<" r "<<r<<" theta: "<<theta<<" x "<<x_sourceframe<<" y "<<y_sourceframe<<G4endl;

	//For random direction in order to optimize we generate random point in a sphere, but we are interested only in the values close to phi = 180 degrees, which is why we use random()/20 -1
        theta = 2. * pi * G4UniformRand()*rad;
        G4double phi = acos(G4UniformRand()/20. -1.);
	G4double x_direction = sin(phi)*cos(theta);
	G4double y_direction = sin(phi)*sin(theta);
	G4double z_direction = cos(phi);
  	G4ThreeVector direction = G4ThreeVector(0,-1,0);


	if(fSourceDirectionType == 1){
		direction = G4ThreeVector(x_direction,y_direction,z_direction);
	}

	switch (particleType) {
		case 0:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
 			break;
		case 1:
			Z = 26;
			A = 55;
			ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 2:
			Z = 55;
			A = 137;
			ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 3:
			Z = 83;
			A = 207;
			ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 4:
			Z = 38;
			A = 90;
			ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 5:
			//Z = 95;
			//A = 241;
			//ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
			fParticleGun->SetParticleEnergy(59.5*keV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x_direction,y_direction,z_direction));
			//fParticleGun->SetParticleDefinition(ion);
			//fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 6:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));
 			break;
		case 7:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("opticalphoton"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(G4ThreeVector(fPositionX,fPositionY,fPositionZ));
			fParticleGun->SetParticleMomentumDirection(direction);
			break;
		case 8:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("neutron"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
 			break;

	}
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetParticleName(G4int Z, G4int A, G4double excitEnergy)
{
	fParticleName = G4IonTable::GetIonTable()->GetIonName(Z,A,excitEnergy);
}

void PrimaryGeneratorAction::SetSourceHalfZ(G4double newValue){
	HalfZ = newValue;
}

void PrimaryGeneratorAction::SetSourceShape(G4String newValue){
	Shape = newValue;
}
void PrimaryGeneratorAction::SetSourcePosition(G4ThreeVector newPosition){
	 CentreCoords = newPosition;
}

void PrimaryGeneratorAction::SetSourceDiameter(G4double newDiameter){
	Radius = newDiameter/2.;
}

void PrimaryGeneratorAction::SetSourceType(G4int newType)
{
	if (newType <= 8 && newType >= 0)
	{
		particleType = newType;
	}
	else
	{
		G4cerr << "The option is out of the possible values (0-8)!" << G4endl;
		G4cerr << "The default option (0) is set!" << G4endl;
		particleType = 0;
	}
}

void PrimaryGeneratorAction::SetSourceDirectionType(G4int newType)
{

	if (newType <= 1 && newType >= 0)
	{
		fSourceDirectionType = newType;
	}
	else
	{
		G4cerr << "The option is out of the possible values (0-1)!" << G4endl;
		G4cerr << "The default option (0) is set!" << G4endl;
		particleType = 0;
	}
}
void PrimaryGeneratorAction::SetSourceGeometry(G4int newType)
{
	if (newType <= 2 && newType >= 0){ 
		fSourceGeometry = newType;}
	else{ 
		G4cerr<<"Possible values are 0 for square and 1 for circunference"<<G4endl;
		G4cerr<<"Setting the default value to square "<<G4endl;
  		fSourceGeometry = 0;
	}
}
void PrimaryGeneratorAction::SetSourceEnergy(G4double newEnergy)
{
	if (newEnergy>0)
	{
		fSourceEnergy = newEnergy;
	}
	else{
		G4cerr << "New energy is < 0." << G4endl;
		G4cerr << "The default option 20 keV is set!" << G4endl;
		fSourceEnergy = 20.*keV;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

