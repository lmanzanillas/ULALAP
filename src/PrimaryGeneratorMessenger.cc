#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tokenizer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* Gun)
  : G4UImessenger(),
    fAction(Gun),fGunDir(0)
{

  fGunDir = new G4UIdirectory("/ULALAP/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fSourceIon_ZA = new G4UIcommand("/ULALAP/gun/sourceIon_ZA", this);
  fSourceIon_ZA->SetGuidance("Choose the type of ion (Z A)");
  parZ = new G4UIparameter("parZ", 'i', false);
  fSourceIon_ZA->SetParameter(parZ);
  parA = new G4UIparameter("parA", 'i', false);
  fSourceIon_ZA->SetParameter(parA);


  fSourceType = new G4UIcmdWithAnInteger("/ULALAP/gun/sourceType",this);
  fSourceType->SetGuidance("Choose the type of source");
  fSourceType->SetParameterName("sourceType",true);
  fSourceType->SetDefaultValue(0);
  fSourceType->AvailableForStates(G4State_Idle);

  fSourceDirectionType = new G4UIcmdWithAnInteger("/ULALAP/gun/sourceDirectionType",this);
  fSourceDirectionType->SetGuidance("Choose the direction type");
  fSourceDirectionType->SetParameterName("sourceDirectionType",true);
  fSourceDirectionType->SetDefaultValue(0);
  fSourceDirectionType->AvailableForStates(G4State_Idle);

  fSourceGeometry = new G4UIcmdWithAnInteger("/ULALAP/gun/sourceGeometry",this);
  fSourceGeometry->SetGuidance("Choose the type of geometry for the source");
  fSourceGeometry->SetParameterName("sourceGeometry",true);
  fSourceGeometry->SetDefaultValue(0);
  fSourceGeometry->AvailableForStates(G4State_Idle);

  fSourceEnergy = new G4UIcmdWithADoubleAndUnit("/ULALAP/gun/sourceEnergy", this);
  fSourceEnergy->SetGuidance("Choose source energy");
  fSourceEnergy->SetParameterName("sourceEnergy",true);
  fSourceEnergy->SetDefaultValue(20.*keV);
  fSourceEnergy->AvailableForStates(G4State_Idle);


  fSourceDiameter = new G4UIcmdWithADoubleAndUnit("/ULALAP/gun/sourceDiameter",this);
  fSourceDiameter->SetGuidance("Set Source diameter or side");
  fSourceDiameter->SetParameterName("fDiameter",true);
  fSourceDiameter->SetDefaultValue(5.*mm);
  fSourceDiameter->AvailableForStates(G4State_Idle);

  //Position of the source, taking the center as reference
  fSourcePosition = new G4UIcmdWith3VectorAndUnit("/ULALAP/gun/position",this);
  fSourcePosition->SetGuidance("Set starting position of the particle.");
  fSourcePosition->SetParameterName("X","Y","Z",true,true);
  fSourcePosition->SetDefaultUnit("cm");

  //Position of the source Bi1, taking the center as reference
  fSourcePositionBi1 = new G4UIcmdWith3VectorAndUnit("/ULALAP/gun/positionBi1",this);
  fSourcePositionBi1->SetGuidance("Set starting position of the Bi1.");
  fSourcePositionBi1->SetParameterName("X","Y","Z",true,true);
  fSourcePositionBi1->SetDefaultUnit("cm");

  //Position of the source Bi2, taking the center as reference
  fSourcePositionBi2 = new G4UIcmdWith3VectorAndUnit("/ULALAP/gun/positionBi2",this);
  fSourcePositionBi2->SetGuidance("Set starting position of the Bi2.");
  fSourcePositionBi2->SetParameterName("X","Y","Z",true,true);
  fSourcePositionBi2->SetDefaultUnit("cm");

  //Box shape of the source, taking the center as reference
  fSourceBoxXYZ = new G4UIcmdWith3VectorAndUnit("/ULALAP/gun/BoxXYZ",this);
  fSourceBoxXYZ->SetGuidance("Set size of Box volume.");
  fSourceBoxXYZ->SetParameterName("X","Y","Z",true,true);
  fSourceBoxXYZ->SetDefaultUnit("cm");

  // half height of source
  fSourceHalfZ = new G4UIcmdWithADoubleAndUnit("/ULALAP/gun/halfz",this);
  fSourceHalfZ->SetGuidance("Set z half length of source.");
  fSourceHalfZ->SetParameterName("Halfz",true,true);
  fSourceHalfZ->SetDefaultUnit("cm");
  fSourceHalfZ->SetUnitCandidates("nm um mm cm m km");

  // centre coordinates
  fSourceCentreCoords = new G4UIcmdWith3VectorAndUnit("/ULALAP/gun/centre",this);
  fSourceCentreCoords->SetGuidance("Set centre coordinates of source.");
  fSourceCentreCoords->SetParameterName("X","Y","Z",true,true);
  fSourceCentreCoords->SetDefaultUnit("cm");
  fSourceCentreCoords->SetUnitCandidates("nm um mm cm m km");

  // source shape
  fSourceShape = new G4UIcmdWithAString("/ULALAP/gun/shape",this);
  fSourceShape->SetGuidance("Sets source shape type.");
  fSourceShape->SetParameterName("Shape",true,true);
  fSourceShape->SetDefaultValue("Point");
  fSourceShape->SetCandidates("Sphere Cylinder Point Box");

  // particle direction
  fSourceDirection = new G4UIcmdWith3Vector("/ULALAP/gun/direction",this);
  fSourceDirection->SetGuidance("Set momentum direction.");
  fSourceDirection->SetGuidance("Direction needs not to be a unit vector.");
  fSourceDirection->SetParameterName("Px","Py","Pz",true,true);
  fSourceDirection->SetRange("Px != 0 || Py != 0 || Pz != 0");


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fSourceIon_ZA;
  delete fSourceType;
  delete fSourceDirectionType;
  delete fSourceGeometry;
  delete fSourceEnergy;
  delete fGunDir;
  delete fSourceHalfZ;
  delete fSourceBoxXYZ;
  delete fSourcePosition;
  delete fSourcePositionBi1;
  delete fSourcePositionBi2;
  delete fSourceShape;
  delete fSourceDiameter;
  delete fSourceDirection;
  //delete fAction;//debug
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == fSourceType) {
  	fAction->SetSourceType(fSourceType->GetNewIntValue(newValue));
  }
  if(command == fSourceHalfZ) {
  	fAction->SetSourceHalfZ(fSourceHalfZ->GetNewDoubleValue(newValue));
  }
  if(command == fSourceBoxXYZ) {
  	fAction->SetSourceBoxXYZ(fSourceBoxXYZ->GetNew3VectorValue(newValue));
  }
  if(command == fSourceDirectionType) {
  	fAction->SetSourceDirectionType(fSourceDirectionType->GetNewIntValue(newValue));
  }
  if(command == fSourceGeometry) {
  	fAction->SetSourceGeometry(fSourceGeometry->GetNewIntValue(newValue));
  }
  if(command == fSourceEnergy){
  	fAction->SetSourceEnergy(fSourceEnergy->GetNewDoubleValue(newValue));
  }
  if(command == fSourceDiameter){
  	fAction->SetSourceDiameter(fSourceDiameter->GetNewDoubleValue(newValue));
  }
  if(command == fSourcePosition){
  	fAction->SetSourcePosition(fSourcePosition->GetNew3VectorValue(newValue));
  }
  if(command == fSourcePositionBi1){
  	fAction->SetSourcePositionBi1(fSourcePositionBi1->GetNew3VectorValue(newValue));
  }
  if(command == fSourcePositionBi2){
  	fAction->SetSourcePositionBi2(fSourcePositionBi2->GetNew3VectorValue(newValue));
  }
  if(command == fSourceShape){
  	fAction->SetSourceShape(newValue);
  }
  if(command == fSourceDirection) {
  	fAction->SetParticleDirection(fSourceDirection->GetNew3VectorValue(newValue));
  }
  if(command == fSourceIon_ZA) {
	// Tokenize the input string to extract Z and A values
    	G4int Z, A;
    	std::istringstream iss(newValue);
    	iss >> Z >> A;

    	// Call the corresponding function in fAction
    	fAction->SetSourceIon_ZA(Z, A);

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
