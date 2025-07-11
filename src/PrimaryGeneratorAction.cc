#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction0.hh"
#include "PrimaryGeneratorAction1.hh"
#include "PrimaryGeneratorAction2.hh"
#include "PrimaryGeneratorAction3.hh"
#include "PrimaryGeneratorAction4.hh"
#include "PrimaryGeneratorAction5.hh"

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
	//Generator of gammas from n capture on Ar36
	fAction0 = new PrimaryGeneratorAction0(fParticleGun);
	//Generator of gammas from n capture on Ar40
	fAction1 = new PrimaryGeneratorAction1(fParticleGun);
	//Generator of muons with E spectrum expected at DUNE FD site caverns
	fAction2 = new PrimaryGeneratorAction2(fParticleGun);
	//Generator of neutrons with E spectrum expected at DUNE FD site caverns
	fAction3 = new PrimaryGeneratorAction3(fParticleGun);
	//Generator of two Bi sources as in ProtoDUNE-HD or 50L prototype
	fAction4 = new PrimaryGeneratorAction4(fParticleGun);
	//Generator of two cavern gammas at DUNE caverns
	fAction5 = new PrimaryGeneratorAction5(fParticleGun);
	//Set default Bi positions
	fAction3->LoadEnergyDistribution("DUNE_cavern_neutrons.txt");
        fAction3->BuildCDF();
	fAction5->LoadEnergyDistribution("DUNE_cavern_gammas.txt");
        fAction5->BuildCDF();
	fAction4 -> SetBi1Position(fDetector->GetBiSourceCapsulePosition1());
	fAction4 -> SetBi2Position(fDetector->GetBiSourceCapsulePosition2());
	G4ThreeVector zero(0., 0., 0.);
	position = zero;
	CentreCoords = zero;
	BoxXYZ = zero;
	direction = G4ParticleMomentum(1., 0., 0.);
	direction_input = G4ParticleMomentum(1., 0., 0.);
	Radius = 5.0*mm;
	SourcePosType = "Volume";
        Shape = "Point";
        HalfZ = 0.00001;
	MinTheta = 0.;
	MaxTheta = pi;
	MinPhi = 0.;
	MaxPhi = twopi;
        size_source_x = 50.;
        size_source_y = 10.;
        size_source_z = 10.;
	particleType = 8;
	fSourceDirectionType = 1;
	ionZ = 83;
	ionA = 207;
	fSourceEnergy = 1*eV;
	fPhotonWavelength = 0;
	fParticleName = "void";
	fParticleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("mu+");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fParticleGun;
	delete fAction0;
	delete fAction1;
	delete fAction2;
	delete fAction3;
	delete fAction4;
	delete fAction5;
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
  size_source_x = BoxXYZ.x();
  size_source_y = BoxXYZ.y();
  size_source_z = BoxXYZ.z();
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
    z = Radius*2.;
    while(((x*x)+(z*z)) > (Radius*Radius)) {
      x = G4UniformRand();
      y = G4UniformRand();
      z = G4UniformRand();
      x = (x*2.*Radius) - Radius;
      y = (y*2.*HalfZ) - HalfZ;
      z = (z*2.*Radius) - Radius;
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
  G4double ResMag = 0.;
  
  switch (fSourceDirectionType) {
	  //Fixed direction taking as input a vector ux,uy,uz
	  //normalization is applied in case the vector is not unitary
	 case 0:
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
		  break;

	  //Random direction, direction is randomly generated in all directions
	  case 1:
		  Phi = MinPhi + (MaxPhi - MinPhi) * rndm2; 
  		  sinphi = std::sin(Phi);
  		  cosphi = std::cos(Phi);

  		  px = -sintheta * cosphi;
  		  py = -sintheta * sinphi;
 		  pz = -costheta;

  		  ResMag = std::sqrt((px*px) + (py*py) + (pz*pz));
  		  px = px/ResMag;
  		  py = py/ResMag;
  		  pz = pz/ResMag;

  	  	  direction.setX(px);
  		  direction.setY(py);
  		  direction.setZ(pz);
		  break;

	  //Like option 0, but sampling a random direction around the given vector
	  //This should correspond to a 2pi half sphere direction
	  case 2:
		  G4ThreeVector zAxis(0, 0, 1); // Default direction
		  G4ThreeVector normAxis = new_direction.unit();

		  // Sample a point on hemisphere centered around +Z
   		  G4double cosTheta = G4UniformRand();              // cos(θ) in [0,1] → hemisphere
    		  G4double sinTheta = std::sqrt(1.0 - cosTheta*cosTheta);
    		  G4double phi = CLHEP::twopi * G4UniformRand();    // φ in [0, 2π]

    		  px = sinTheta * std::cos(phi);
    		  py = sinTheta * std::sin(phi);
    		  pz = cosTheta;

    		  G4ThreeVector localDir(px, py, pz);

 		  // Rotate localDir from +Z axis to the desired axis direction
		  direction = localDir.rotateUz(normAxis);
		  //G4cout<<"direction: "<<px<<" "<<py<<" "<<pz<<G4endl;
		  break;
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
	GenerateDirection(direction_input);
	G4double RandNb = G4UniformRand();
	if(particleType == 4){
	        fSourceEnergy = fAction5->SampleEnergy() * keV; 
	}
	if(particleType == 11){
	        fSourceEnergy = fAction3->SampleEnergy() * keV;
	}
	if(particleType == 10){
	        fSourceEnergy = fAction2->InverseCumul();
  		fParticleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("mu+"); 
  		if ( G4UniformRand() > 0.5798 ){//We know that the ratio mu+/mu- is 1.38, which gives 0.5798 prob of having mu+
	 		fParticleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("mu-"); 
 		 }
	}

	switch (particleType) {
		case 0:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
			fParticleGun->GeneratePrimaryVertex(anEvent);
 			break;
		case 1:
			//ion source with Z and A passed by command
			ion = G4IonTable::GetIonTable()->GetIon(ionZ,ionA,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			fParticleGun->GeneratePrimaryVertex(anEvent);
			break;
		case 2:
			//Two Bi source
			fAction4->GeneratePrimaries(anEvent);
			break;
		case 3:
			//Bi-207 source
			Z = 83;
			A = 207;
			ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
			fParticleGun->SetParticleEnergy(0.*eV);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
			fParticleGun->SetParticleDefinition(ion);
			fParticleGun->SetParticleCharge(ionCharge);
			fParticleGun->GeneratePrimaryVertex(anEvent);
			break;
		case 4:
			//Sr-90 source
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
			fParticleGun->GeneratePrimaryVertex(anEvent);
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
			fParticleGun->GeneratePrimaryVertex(anEvent);
			break;
		case 6:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
			fParticleGun->GeneratePrimaryVertex(anEvent);
 			break;
		case 7:
			//monoenergetic muons
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("mu-"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
			fParticleGun->GeneratePrimaryVertex(anEvent);
			break;
		case 8:
			//Mono-energetic neutrons
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("neutron"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
			fParticleGun->GeneratePrimaryVertex(anEvent);
 			break;
		case 9:
			//Gammas from neutron capture on Ar-36(3.5%) and Ar-40
 			if (RandNb < 0.035){
				fAction0->GeneratePrimaries(anEvent);
			}
			else{
				fAction1->GeneratePrimaries(anEvent);
			}
			break;
		case 10:
			//Muon with E spectrum at DUNE caverns
			fParticleGun->SetParticleDefinition(fParticleDefinition);
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
			fParticleGun->GeneratePrimaryVertex(anEvent);
    			break;
		case 11:
			//Neutrons with E spectrum (see before switch line) corresponding to DUNE caverns
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("neutron"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
			fParticleGun->GeneratePrimaryVertex(anEvent);
 			break;
		case 12:
			fParticleGun->SetParticleDefinition(particleTable->FindParticle("alpha"));
			fParticleGun->SetParticleEnergy(fSourceEnergy);
			fParticleGun->SetParticlePosition(position);
			fParticleGun->SetParticleMomentumDirection(direction);
			fParticleGun->GeneratePrimaryVertex(anEvent);
 			break;

	}
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
	 if (particleType == 3){
		 fDetector->SetBiSourcePosition(newPosition);
	 }
}
//Bi1 position
void PrimaryGeneratorAction::SetSourcePositionBi1(G4ThreeVector newPosition){
	fDetector->SetBiSourcePosition(newPosition);
	fAction4 -> SetBi1Position(newPosition);
}
//Bi2 position
void PrimaryGeneratorAction::SetSourcePositionBi2(G4ThreeVector newPosition){
	fDetector -> SetBiSourcePosition2(newPosition);
	fAction4 -> SetBi2Position(newPosition);
}
void PrimaryGeneratorAction::SetParticleDirection(G4ThreeVector new_direction){
	 direction_input = new_direction;
}
void PrimaryGeneratorAction::SetSourceDiameter(G4double newDiameter){
	Radius = newDiameter/2.;
}

void PrimaryGeneratorAction::SetSourceType(G4int newType)
{
	if (newType <= 12 && newType >= 0)
	{
		particleType = newType;
	}
	else
	{
		G4cerr << "The option is out of the possible values (0-9)!" << G4endl;
		G4cerr << "The default option (0) is set!" << G4endl;
		particleType = 0;
	}
}

void PrimaryGeneratorAction::SetSourceDirectionType(G4int newType)
{

	if (newType <= 2 && newType >= 0)
	{
		fSourceDirectionType = newType;
	}
	else
	{
		G4cerr << "The option is out of the possible values (0-2)!" << G4endl;
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
void PrimaryGeneratorAction::SetSourceIon_ZA(G4int newZ, G4int newA)
{
	if(newZ > 0  &&  newZ < 118){
		ionZ = newZ;
		ionA = newA;
	}
	else{
		G4cerr<<"Possible values are in the range 1 -118 for Z"<<G4endl;
		G4cerr<<"Setting the default value to 83 (Bi) "<<G4endl;
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

