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
	direction = G4ParticleMomentum(1., 0., 0.);
	Radius = 0.1*m;
	SourcePosType = "Volume";
        Shape = "Point";
        HalfZ = 0.;
	MinTheta = 0.;
	MaxTheta = pi;
	MinPhi = 0.;
	MaxPhi = twopi;
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
  size_source_x = BoxXYZ.x()/cm;
  size_source_y = BoxXYZ.y()/cm;
  size_source_z = BoxXYZ.z()/cm;
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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GenerateDirection(G4ThreeVector new_direction)
{
  G4double rndm, rndm2;
  G4double px, py, pz;

  G4double sintheta, sinphi, costheta, cosphi;
  rndm = G4UniformRand();
  costheta = std::cos(MinTheta) - rndm * (std::cos(MinTheta)
                                - std::cos(MaxTheta));
  sintheta = std::sqrt(1. - costheta*costheta);
  
  rndm2 = G4UniformRand();
  Phi = MinPhi + (MaxPhi - MinPhi) * rndm2; 
  sinphi = std::sin(Phi);
  cosphi = std::cos(Phi);

  px = -sintheta * cosphi;
  py = -sintheta * sinphi;
  pz = -costheta;

  G4double ResMag = std::sqrt((px*px) + (py*py) + (pz*pz));
  px = px/ResMag;
  py = py/ResMag;
  pz = pz/ResMag;

  direction.setX(px);
  direction.setY(py);
  direction.setZ(pz);
  if(fSourceDirectionType == 0){
	  px = new_direction.x();
	  py = new_direction.y();
	  pz = new_direction.z();
	  ResMag = std::sqrt((px*px) + (py*py) + (pz*pz));
	  px = px/ResMag;
	  py = py/ResMag;
	  pz = pz/ResMag; 
	  direction.setX(px);
	  direction.setY(py);
	  direction.setZ(pz);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4double ionCharge   = 0.*eplus;
	G4double excitEnergy = 0.*eV;
	G4int Z=0, A=0;
	G4ParticleDefinition* ion;
	GeneratePointsInVolume();
	GenerateDirection(direction);

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
			fParticleGun->SetParticleMomentumDirection(direction);
			//fParticleGun->SetParticleDefinition(ion);
			//fParticleGun->SetParticleCharge(ionCharge);
			break;
		case 6:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
 			break;
		case 7:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("opticalphoton"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
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

