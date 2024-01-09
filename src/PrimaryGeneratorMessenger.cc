#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* Gun)
  : G4UImessenger(),
    fAction(Gun),fGunDir(0)
{

  fGunDir = new G4UIdirectory("/ULALAP/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

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

  //Position of the source, taking the center as reference
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
  fSourceShape->SetCandidates("Sphere Cylinder Point");



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fSourceType;
  delete fSourceDirectionType;
  delete fSourceGeometry;
  delete fSourceEnergy;
  delete fGunDir;
  delete fSourceHalfZ;
  delete fSourceBoxXYZ;
  delete fSourcePosition;
  delete fSourceShape;
  delete fSourceDiameter;

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
  if(command == fSourceShape){
  	fAction->SetSourceShape(newValue);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
