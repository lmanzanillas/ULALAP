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
 fG4SOSDir(0),
 fDetDir(0),
 commandSetWorldMaterial(0),
 commandSetDetectorType(0),
 commandSetGeDetectorLength(0),
 commandSetGeDetectorThickness(0),
 commandSetGeDetectorWidth(0),
 commandSetshieldingThickness(0),
 commandSetBeWindowRadius(0),
 commandSetDistanceCollimatorDetector(0),
 commandSetDistanceSampleWindow(0),
 commandSetCollimatorMaterial(0),
 commandSetTargetMaterial(0),
 commandSetShiledingMaterial(0),
 commandSetGeContainerMaterial(0),
 commandSetGeContainerMaterialCoating(0),
 commandSetDetectorName(0),
 commandSetSetupName(0),
 commandSetDataType(0),
 commandSetOutputDirectory(0)
 {
  fDetDir = new G4UIdirectory("/G4SOS/det/");
  fDetDir->SetGuidance("detector construction commands");

  commandSetCollimatorMaterial = new G4UIcmdWithAString("/G4SOS/det/setCollimatorMat",this);
  commandSetCollimatorMaterial->SetGuidance("Select material of the collimator.");
  commandSetCollimatorMaterial->SetParameterName("choice",false);
  commandSetCollimatorMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetCollimatorMaterial->SetToBeBroadcasted(false);

  commandSetTargetMaterial = new G4UIcmdWithAString("/G4SOS/det/setGeDetectorMat",this);
  commandSetTargetMaterial->SetGuidance("Select material of the target.");
  commandSetTargetMaterial->SetParameterName("choice",false);
  commandSetTargetMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetTargetMaterial->SetToBeBroadcasted(false);

  commandSetShiledingMaterial = new G4UIcmdWithAString("/G4SOS/det/setSampleMaterial",this);
  commandSetShiledingMaterial->SetGuidance("Select material of the sample.");
  commandSetShiledingMaterial->SetParameterName("choice",false);
  commandSetShiledingMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetShiledingMaterial->SetToBeBroadcasted(false);


  commandSetGeContainerMaterial = new G4UIcmdWithAString("/G4SOS/det/setGeContainerMat",this);
  commandSetGeContainerMaterial->SetGuidance("Select material of the ge container.");
  commandSetGeContainerMaterial->SetParameterName("choice",false);
  commandSetGeContainerMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeContainerMaterial->SetToBeBroadcasted(false);

  commandSetGeContainerMaterialCoating = new G4UIcmdWithAString("/G4SOS/det/setGeContainerMatCoating",this);
  commandSetGeContainerMaterialCoating->SetGuidance("Select material of the ge container coating.");
  commandSetGeContainerMaterialCoating->SetParameterName("choice",false);
  commandSetGeContainerMaterialCoating->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeContainerMaterialCoating->SetToBeBroadcasted(false);

  commandSetDetectorName = new G4UIcmdWithAString("/G4SOS/det/setGeDetectorName",this);
  commandSetDetectorName->SetGuidance("Select name of detector.");
  commandSetDetectorName->SetParameterName("choice",false);
  commandSetDetectorName->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDetectorName->SetToBeBroadcasted(false);

  commandSetSetupName = new G4UIcmdWithAString("/G4SOS/det/setSetupName",this);
  commandSetSetupName->SetGuidance("Select name of setup.");
  commandSetSetupName->SetParameterName("choice",false);
  commandSetSetupName->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetSetupName->SetToBeBroadcasted(false);

  commandSetDataType = new G4UIcmdWithAString("/G4SOS/det/setDataType",this);
  commandSetDataType->SetGuidance("Select format of data: csv, hdf5, root.");
  commandSetDataType->SetParameterName("choice",false);
  commandSetDataType->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDataType->SetToBeBroadcasted(false);

  commandSetOutputDirectory = new G4UIcmdWithAString("/G4SOS/det/setOutputDirectory",this);
  commandSetOutputDirectory->SetGuidance("Set output directory");
  commandSetOutputDirectory->SetParameterName("choice",false);
  commandSetOutputDirectory->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetOutputDirectory->SetToBeBroadcasted(false);

  commandSetWorldMaterial = new G4UIcmdWithAString("/G4SOS/det/setWorldMat",this);
  commandSetWorldMaterial->SetGuidance("Select material of the world.");
  commandSetWorldMaterial->SetParameterName("choice",false);
  commandSetWorldMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetWorldMaterial->SetToBeBroadcasted(false);

  commandSetGeDetectorLength = new G4UIcmdWithADoubleAndUnit("/G4SOS/det/setGeDetectorLength",this);
  commandSetGeDetectorLength->SetGuidance("Set length of target samples");
  commandSetGeDetectorLength->SetParameterName("SampleLength",false);
  commandSetGeDetectorLength->SetRange("SampleLength>0.");
  commandSetGeDetectorLength->SetUnitCategory("Length");
  commandSetGeDetectorLength->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeDetectorLength->SetToBeBroadcasted(false);

  commandSetGeDetectorThickness = new G4UIcmdWithADoubleAndUnit("/G4SOS/det/setGeDetectorThickness",this);
  commandSetGeDetectorThickness->SetGuidance("Set thickness of target samples");
  commandSetGeDetectorThickness->SetParameterName("SampleThickness",false);
  commandSetGeDetectorThickness->SetRange("SampleThickness>0.");
  commandSetGeDetectorThickness->SetUnitCategory("Length");
  commandSetGeDetectorThickness->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeDetectorThickness->SetToBeBroadcasted(false);

  commandSetGeDetectorWidth = new G4UIcmdWithADoubleAndUnit("/G4SOS/det/setGeDetectorWidth",this);
  commandSetGeDetectorWidth->SetGuidance("Set width of target samples");
  commandSetGeDetectorWidth->SetParameterName("SampleWidth",false);
  commandSetGeDetectorWidth->SetRange("SampleWidth>0.");
  commandSetGeDetectorWidth->SetUnitCategory("Length");
  commandSetGeDetectorWidth->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetGeDetectorWidth->SetToBeBroadcasted(false);

  commandSetshieldingThickness = new G4UIcmdWithADoubleAndUnit("/G4SOS/det/setshieldingThickness",this);
  commandSetshieldingThickness->SetGuidance("Set thickness of Al contact");
  commandSetshieldingThickness->SetParameterName("shieldingThickness",false);
  commandSetshieldingThickness->SetRange("shieldingThickness>0.");
  commandSetshieldingThickness->SetUnitCategory("Length");
  commandSetshieldingThickness->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetshieldingThickness->SetToBeBroadcasted(false);

  commandSetBeWindowRadius = new G4UIcmdWithADoubleAndUnit("/G4SOS/det/setBeWindowRadius",this);
  commandSetBeWindowRadius->SetGuidance("Set radius of Be Window");
  commandSetBeWindowRadius->SetParameterName("WindowRadius",false);
  commandSetBeWindowRadius->SetRange("WindowRadius>0.");
  commandSetBeWindowRadius->SetUnitCategory("Length");
  commandSetBeWindowRadius->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetBeWindowRadius->SetToBeBroadcasted(false);

  commandSetDistanceCollimatorDetector = new G4UIcmdWithADoubleAndUnit("/G4SOS/det/setDistanceCollimatorDetector",this);
  commandSetDistanceCollimatorDetector->SetGuidance("Set distance collimator detector");
  commandSetDistanceCollimatorDetector->SetParameterName("CollimatorDistance",false);
  commandSetDistanceCollimatorDetector->SetRange("CollimatorDistance>0.");
  commandSetDistanceCollimatorDetector->SetUnitCategory("Length");
  commandSetDistanceCollimatorDetector->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDistanceCollimatorDetector->SetToBeBroadcasted(false);

  commandSetDistanceSampleWindow = new G4UIcmdWithADoubleAndUnit("/G4SOS/det/setDistanceSampleWindow",this);
  commandSetDistanceSampleWindow->SetGuidance("Set distance sample to window");
  commandSetDistanceSampleWindow->SetParameterName("SampleDistance",false);
  commandSetDistanceSampleWindow->SetRange("SampleDistance>0.");
  commandSetDistanceSampleWindow->SetUnitCategory("Length");
  commandSetDistanceSampleWindow->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDistanceSampleWindow->SetToBeBroadcasted(false);


  commandSetDetectorType = new G4UIcmdWithAnInteger("/G4SOS/det/setDetectorType",this);
  commandSetDetectorType->SetGuidance("Set detector type");
  commandSetDetectorType->AvailableForStates(G4State_PreInit,G4State_Idle);
  commandSetDetectorType->SetToBeBroadcasted(false);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fDetDir;
  delete fG4SOSDir;
  delete commandSetWorldMaterial;
  delete commandSetDetectorType;
  delete commandSetGeDetectorLength;
  delete commandSetGeDetectorThickness;
  delete commandSetGeDetectorWidth;
  delete commandSetshieldingThickness;
  delete commandSetBeWindowRadius;
  delete commandSetDistanceCollimatorDetector;
  delete commandSetDistanceSampleWindow;
  delete commandSetCollimatorMaterial;
  delete commandSetTargetMaterial;
  delete commandSetShiledingMaterial;
  delete commandSetGeContainerMaterial;
  delete commandSetGeContainerMaterialCoating;
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

   if( command == commandSetShiledingMaterial ){
	fDetector->SetShiledingMaterial(newValue);
   }

   if( command == commandSetGeContainerMaterial ){
	fDetector->SetGeContainerMaterial(newValue);
   }

   if( command == commandSetGeContainerMaterialCoating ){
	fDetector->SetGeContainerMaterialCoating(newValue);
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

   if( command == commandSetCollimatorMaterial ){
	fDetector->SetCollimatorMaterial(newValue);
   }

   if( command == commandSetOutputDirectory ){
	fDetector->SetOutputDirectory(newValue);
   }

   if( command == commandSetWorldMaterial ){
	fDetector->SetWorldMaterial(newValue);
   }

   if( command == commandSetGeDetectorLength ){
	fDetector->SetGeDetectorLength(commandSetGeDetectorLength->GetNewDoubleValue(newValue));
   }

   if( command == commandSetGeDetectorThickness ){
	fDetector->SetGeDetectorThickness(commandSetGeDetectorThickness->GetNewDoubleValue(newValue));
   }

   if( command == commandSetshieldingThickness ){
	fDetector->SetshieldingThickness(commandSetshieldingThickness->GetNewDoubleValue(newValue));
   }

   if( command == commandSetGeDetectorWidth ){
	fDetector->SetGeDetectorWidth(commandSetGeDetectorWidth->GetNewDoubleValue(newValue));
   }

   if( command == commandSetBeWindowRadius ){
	fDetector->SetBeWindowRadius(commandSetBeWindowRadius->GetNewDoubleValue(newValue));
   }

   if( command == commandSetDistanceCollimatorDetector ){
	fDetector->SetDistanceCollimatorDetector(commandSetDistanceCollimatorDetector->GetNewDoubleValue(newValue));
   }

   if( command == commandSetDistanceSampleWindow ){
	fDetector->SetDistanceSampleWindow(commandSetDistanceSampleWindow->GetNewDoubleValue(newValue));
   }

   if( command == commandSetDetectorType ){
	fDetector->SetDetectorType(commandSetDetectorType->GetNewIntValue(newValue));
   }
   
}
