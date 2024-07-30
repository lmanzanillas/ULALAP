#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"

#include "G4RunManager.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:G4UImessenger(),
 fDetector(Det),
 fULALAPDir(0),
 fDetDir(0),
 commandSetWorldMaterial(0),
 commandSetDetectorType(0),
 commandSetLArDetectorLength(0),
 commandSetLArDetectorThickness(0),
 commandSetLArDetectorWidth(0),
 commandSetshieldingThickness(0),
 commandSetWaffleThickness(0),
 commandSetTargetMaterial(0),
 commandSetShieldingMaterial(0),
 commandSetShieldingMaterialWaffle(0),
 commandSetDetectorName(0),
 commandSetSetupName(0),
 commandSetDataType(0),
 commandSetOutputDirectory(0)
 {
  fDetDir = new G4UIdirectory("/ULALAP/det/");
  fDetDir->SetGuidance("detector construction commands");


  commandSetTargetMaterial = new G4UIcmdWithAString("/ULALAP/det/setTargetDetectorMat",this);
  commandSetTargetMaterial->SetGuidance("Select material of the target.");
  commandSetTargetMaterial->SetParameterName("choice",false);
  commandSetTargetMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetTargetMaterial->SetToBeBroadcasted(false);

  commandSetShieldingMaterial = new G4UIcmdWithAString("/ULALAP/det/setShieldingMaterial",this);
  commandSetShieldingMaterial->SetGuidance("Select material of the sample.");
  commandSetShieldingMaterial->SetParameterName("choice",false);
  commandSetShieldingMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetShieldingMaterial->SetToBeBroadcasted(false);


  commandSetShieldingMaterialWaffle = new G4UIcmdWithAString("/ULALAP/det/setShieldingMaterialWaffle",this);
  commandSetShieldingMaterialWaffle->SetGuidance("Select material of the waffle shielding.");
  commandSetShieldingMaterialWaffle->SetParameterName("choice",false);
  commandSetShieldingMaterialWaffle->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetShieldingMaterialWaffle->SetToBeBroadcasted(false);

  commandSetDetectorName = new G4UIcmdWithAString("/ULALAP/det/setDetectorName",this);
  commandSetDetectorName->SetGuidance("Select name of detector.");
  commandSetDetectorName->SetParameterName("choice",false);
  commandSetDetectorName->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDetectorName->SetToBeBroadcasted(false);

  commandSetSetupName = new G4UIcmdWithAString("/ULALAP/det/setSetupName",this);
  commandSetSetupName->SetGuidance("Select name of setup.");
  commandSetSetupName->SetParameterName("choice",false);
  commandSetSetupName->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetSetupName->SetToBeBroadcasted(false);

  commandSetDataType = new G4UIcmdWithAString("/ULALAP/det/setDataType",this);
  commandSetDataType->SetGuidance("Select format of data: csv, hdf5, root.");
  commandSetDataType->SetParameterName("choice",false);
  commandSetDataType->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDataType->SetToBeBroadcasted(false);

  commandSetOutputDirectory = new G4UIcmdWithAString("/ULALAP/det/setOutputDirectory",this);
  commandSetOutputDirectory->SetGuidance("Set output directory");
  commandSetOutputDirectory->SetParameterName("choice",false);
  commandSetOutputDirectory->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetOutputDirectory->SetToBeBroadcasted(false);

  commandSetWorldMaterial = new G4UIcmdWithAString("/ULALAP/det/setWorldMat",this);
  commandSetWorldMaterial->SetGuidance("Select material of the world.");
  commandSetWorldMaterial->SetParameterName("choice",false);
  commandSetWorldMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetWorldMaterial->SetToBeBroadcasted(false);

  commandSetLArDetectorLength = new G4UIcmdWithADoubleAndUnit("/ULALAP/det/setLArVolumeLength",this);
  commandSetLArDetectorLength->SetGuidance("Set length of target samples");
  commandSetLArDetectorLength->SetParameterName("SampleLength",false);
  commandSetLArDetectorLength->SetRange("SampleLength>0.");
  commandSetLArDetectorLength->SetUnitCategory("Length");
  commandSetLArDetectorLength->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetLArDetectorLength->SetToBeBroadcasted(false);

  commandSetLArDetectorThickness = new G4UIcmdWithADoubleAndUnit("/ULALAP/det/setLArVolumeThickness",this);
  commandSetLArDetectorThickness->SetGuidance("Set thickness of target samples");
  commandSetLArDetectorThickness->SetParameterName("SampleThickness",false);
  commandSetLArDetectorThickness->SetRange("SampleThickness>0.");
  commandSetLArDetectorThickness->SetUnitCategory("Length");
  commandSetLArDetectorThickness->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetLArDetectorThickness->SetToBeBroadcasted(false);

  commandSetLArDetectorWidth = new G4UIcmdWithADoubleAndUnit("/ULALAP/det/setLArVolumeWidth",this);
  commandSetLArDetectorWidth->SetGuidance("Set width of target samples");
  commandSetLArDetectorWidth->SetParameterName("SampleWidth",false);
  commandSetLArDetectorWidth->SetRange("SampleWidth>0.");
  commandSetLArDetectorWidth->SetUnitCategory("Length");
  commandSetLArDetectorWidth->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetLArDetectorWidth->SetToBeBroadcasted(false);

  commandSetshieldingThickness = new G4UIcmdWithADoubleAndUnit("/ULALAP/det/setshieldingThickness",this);
  commandSetshieldingThickness->SetGuidance("Set thickness of Al contact");
  commandSetshieldingThickness->SetParameterName("shieldingThickness",false);
  commandSetshieldingThickness->SetRange("shieldingThickness>0.");
  commandSetshieldingThickness->SetUnitCategory("Length");
  commandSetshieldingThickness->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetshieldingThickness->SetToBeBroadcasted(false);

  commandSetWaffleThickness = new G4UIcmdWithADoubleAndUnit("/ULALAP/det/setWaffleThickness",this);
  commandSetWaffleThickness->SetGuidance("Set thickness of waffle");
  commandSetWaffleThickness->SetParameterName("waffleThickness",false);
  commandSetWaffleThickness->SetRange("waffleThickness>0.");
  commandSetWaffleThickness->SetUnitCategory("Length");
  commandSetWaffleThickness->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetWaffleThickness->SetToBeBroadcasted(false);



  commandSetDetectorType = new G4UIcmdWithAnInteger("/ULALAP/det/setDetectorType",this);
  commandSetDetectorType->SetGuidance("Set detector type");
  commandSetDetectorType->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDetectorType->SetToBeBroadcasted(false);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fDetDir;
  delete fULALAPDir;
  delete commandSetWorldMaterial;
  delete commandSetDetectorType;
  delete commandSetLArDetectorLength;
  delete commandSetLArDetectorThickness;
  delete commandSetLArDetectorWidth;
  delete commandSetshieldingThickness;
  delete commandSetWaffleThickness;
  delete commandSetTargetMaterial;
  delete commandSetShieldingMaterial;
  delete commandSetShieldingMaterialWaffle;
  delete commandSetDetectorName;
  delete commandSetSetupName;
  delete commandSetDataType;
  delete commandSetOutputDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
   if( command == commandSetTargetMaterial ){
	fDetector->SetTargetMaterial(newValue);
   }

   if( command == commandSetShieldingMaterial ){
	fDetector->SetShieldingMaterial(newValue);
   }

   if( command == commandSetShieldingMaterialWaffle ){
	fDetector->SetShieldingMaterialWaffle(newValue);
   }

   if( command == commandSetDetectorName ){
	fDetector->SetDetectorName(newValue);
   }

   if( command == commandSetSetupName ){
	fDetector->SetSetupName(newValue);
   }

   if( command == commandSetDataType ){
	fDetector->SetDataType(newValue);
   }

   if( command == commandSetOutputDirectory ){
	fDetector->SetOutputDirectory(newValue);
   }

   if( command == commandSetWorldMaterial ){
	fDetector->SetWorldMaterial(newValue);
   }

   if( command == commandSetLArDetectorLength ){
	fDetector->SetLArDetectorLength(commandSetLArDetectorLength->GetNewDoubleValue(newValue));
   }

   if( command == commandSetLArDetectorThickness ){
	fDetector->SetLArDetectorThickness(commandSetLArDetectorThickness->GetNewDoubleValue(newValue));
   }

   if( command == commandSetshieldingThickness ){
	fDetector->SetshieldingThickness(commandSetshieldingThickness->GetNewDoubleValue(newValue));
   }

   if( command == commandSetWaffleThickness ){
	fDetector->SetWaffleThickness(commandSetWaffleThickness->GetNewDoubleValue(newValue));
   }

   if( command == commandSetLArDetectorWidth ){
	fDetector->SetLArDetectorWidth(commandSetLArDetectorWidth->GetNewDoubleValue(newValue));
   }

   if( command == commandSetDetectorType ){
	fDetector->SetDetectorType(commandSetDetectorType->GetNewIntValue(newValue));
   }
   
}
