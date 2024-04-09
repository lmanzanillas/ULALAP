#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "SteelSupportConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4MaterialTable.hh"
#include "UlalapMaterials.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4Torus.hh"
#include "G4Hype.hh"

#include "G4Transform3D.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4NistManager.hh"
#include <G4UserLimits.hh>

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4PSEnergyDeposit.hh"
#include <G4VPrimitiveScorer.hh>

#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4VoxelLimits.hh"

#include "G4MTRunManager.hh"
#include "G4PhysicalConstants.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

#include "G4GDMLParser.hh"

#include <G4VisAttributes.hh>
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*
Constructs DetectorConstruction, defines default values.
*/

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),  fd2DetectorBox(nullptr), 
logicSteelSupport(nullptr), 
logicCryoPrimMembrane(nullptr), 
fd2LogicVolume(nullptr) 
{
  fDetectorMessenger = new DetectorMessenger(this);
  halfSizeCavernFD2Y = halfSizeCavernFD2Z = 15.*m;
  halfSizeCavernFD2X = 100.*m;
  fSetupName = "DUNE";
  fDataType = "csv";
  halfDetectorLength = 62.0/2.0*m;
  halfDetectorThickness = 14.0/2.0*m;
  halfDetectorWidth = 15.1/2.0*m;
  cryostatThicknessPrimMembraneSS = 1.2*mm;
  cryostatThicknessSecondaryBarrierAl = 0.8*mm;
  cryostatThicknessVaporBarrierSS = 12.0*mm;
  cryostatThicknessInnerPU = 0.4*m;
  cryostatThicknessOuterPU = 0.4*m;
  cryostatThicknessInnerPlywood = 10.0*mm;
  cryostatThicknessOuterPlywood = 10.0*mm;
  cryostatThicknessOuterSteelSupport = 1.0*m;
  shieldingThickness = 10.0*cm;
  fDetectorType = 0;
  fDetectorName = "FD2";
  fVolName = "World";
  materialConstruction = new UlalapMaterials;
  DefineMaterials();
  data_output_directory = "./";  
  fd2Material = G4Material::GetMaterial("G4_lAr");
  fShieldingMaterial = G4Material::GetMaterial("G4_POLYETHYLENE");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction(){
  //delete physicWorldBox;
  delete fDetectorMessenger;
  delete materialConstruction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::GetPhysicalVolumeByName(const G4String &name)
{
  // access the store of physical volumes
  G4PhysicalVolumeStore * pvs= G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume* pv;
  G4int npv= pvs->size();
  G4int ipv;
  for (ipv=0; ipv<npv; ipv++) {
    pv= (*pvs)[ipv];
    if (!pv)
      break;
    //G4cout<<" pv->GetName() "<<pv->GetName()<<G4endl;
    if (pv->GetName() == name)
      return pv;
  }
  return NULL;
}




/*
Sets which detector geometry is used.
*/
void DetectorConstruction::SetDetectorType(G4int value){
  fDetectorType=value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  //G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}

//Sets dimmensions of target, thickness corresponds to the Z coordinate, Length to x.
void DetectorConstruction::SetLArDetectorLength(G4double value){
  halfDetectorLength = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetLArDetectorThickness(G4double value){
  halfDetectorThickness = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetLArDetectorWidth(G4double value){
  halfDetectorWidth = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetshieldingThickness(G4double value){
  shieldingThickness = (value/1.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//Sets material of target.
void DetectorConstruction::SetTargetMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);

  if (pttoMaterial) {
    fd2Material = pttoMaterial;
    if(fd2LogicVolume)fd2LogicVolume->SetMaterial(fd2Material);
    G4cout<<" material: "<<fd2Material->GetName()<<G4endl;  
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}

//Sets material of Ge Container coating

void DetectorConstruction::SetDetectorName(G4String detectorNAME)
{
	fDetectorName = detectorNAME;
}

void DetectorConstruction::SetSetupName(G4String setupNAME)
{
	fSetupName = setupNAME;
}
void DetectorConstruction::SetDataType(G4String dataType)
{
	fDataType = dataType;
}
/*
Sets material of sample.
*/
void DetectorConstruction::SetShiledingMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);

  if (pttoMaterial) {
    fShieldingMaterial = pttoMaterial;
    if(logicshieldingBoxOuter)logicshieldingBoxOuter->SetMaterial(fShieldingMaterial);
    G4cout<<" material "<<fShieldingMaterial->GetName()<<G4endl;  
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}



void DetectorConstruction::SetOutputDirectory(G4String output_directory)
{
    data_output_directory = output_directory;  
}
/*
Sets material of world volume.
*/
void DetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
     G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) {
    fWorldMaterial = pttoMaterial;
    if ( logicWorldBox ) { logicWorldBox->SetMaterial(fWorldMaterial); }
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  //G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}


/*
Defines materials used in simulation. Sets material properties for PEN and other optical components.
*/
void DetectorConstruction::DefineMaterials(){
  // ============================================================= Materials =============================================================
  //materialConstruction = new PenMaterials;
  materialConstruction-> Construct();
  materialAir = G4Material::GetMaterial("Air");
  materialSteel = G4Material::GetMaterial("Steel_EN8");
  materialSS304L = G4Material::GetMaterial("Steel_SS304L");
  materialPUfoam = G4Material::GetMaterial("PU_foam");
  materialPlywood = G4Material::GetMaterial("Plywood");
  materialAlCryostat = G4Material::GetMaterial("Aluminium_6061");
  materialRock = G4Material::GetMaterial("G4_CONCRETE");

  G4cout<<" materials imported succesfully "<<G4endl;

}

void DetectorConstruction::SetVolName(G4ThreeVector thePoint){
  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* myVolume= theNavigator->LocateGlobalPointAndSetup(thePoint);
  fVolName =  myVolume->GetName();
}


void DetectorConstruction::UpdateGeometry(){

  //define new one
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::Clean();
  G4LogicalVolumeStore::Clean();
  G4SolidStore::Clean();

  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

/*
Clears stored geometry, then constructs all volumes that can be used in the simulation.

Builds and places volumes in world.

Defines detector sensitivities and properties.
*/
G4VPhysicalVolume* DetectorConstruction::Construct()
{
// ============================================================= Define Volumes =============================================================
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();
  //Define colors
  G4Colour  white   (1.0, 1.0, 1.0) ;
  G4Colour  grey    (0.5, 0.5, 0.5) ;
  G4Colour  lgrey   (.85, .85, .85) ;
  G4Colour  red     (1.0, 0.0, 0.0) ;
  G4Colour  blue    (0.0, 0.0, 1.0) ;
  G4Colour  cyan    (0.0, 1.0, 1.0) ;
  G4Colour  magenta (1.0, 0.0, 1.0) ;
  G4Colour  yellow  (1.0, 1.0, 0.0) ;
  G4Colour  orange  (.75, .55, 0.0) ;
  G4Colour  lblue   (0.0, 0.0, .75) ;
  G4Colour  lgreen  (0.0, .75, 0.0) ;
  G4Colour  green   (0.0, 1.0, 0.0) ;
  G4Colour  brown   (0.7, 0.4, 0.1) ;

  /////// Detector + cryostat (7 layers) + shileding
  //First layer
  G4double box_size_ss_membrane_x = halfDetectorLength + cryostatThicknessPrimMembraneSS;
  G4double box_size_ss_membrane_y = halfDetectorWidth + cryostatThicknessPrimMembraneSS;
  G4double box_size_ss_membrane_z = halfDetectorThickness + cryostatThicknessPrimMembraneSS;
  //Second layer
  G4double box_plywood_inner_x = box_size_ss_membrane_x + cryostatThicknessInnerPlywood; 
  G4double box_plywood_inner_y = box_size_ss_membrane_y + cryostatThicknessInnerPlywood; 
  G4double box_plywood_inner_z = box_size_ss_membrane_z + cryostatThicknessInnerPlywood; 
  //Third layer
  G4double box_PUfoam_inner_x = box_plywood_inner_x + cryostatThicknessInnerPU; 
  G4double box_PUfoam_inner_y = box_plywood_inner_y + cryostatThicknessInnerPU; 
  G4double box_PUfoam_inner_z = box_plywood_inner_z + cryostatThicknessInnerPU; 
  //Fourth layer
  G4double box_Al_barrier_x = box_PUfoam_inner_x + cryostatThicknessSecondaryBarrierAl; 
  G4double box_Al_barrier_y = box_PUfoam_inner_y + cryostatThicknessSecondaryBarrierAl; 
  G4double box_Al_barrier_z = box_PUfoam_inner_z + cryostatThicknessSecondaryBarrierAl;
  //Five layer
  G4double box_PUfoam_outer_x = box_Al_barrier_x +cryostatThicknessOuterPU; 
  G4double box_PUfoam_outer_y = box_Al_barrier_y +cryostatThicknessOuterPU; 
  G4double box_PUfoam_outer_z = box_Al_barrier_z +cryostatThicknessOuterPU; 
  //Six layer
  G4double box_plywood_outer_x = box_PUfoam_outer_x + cryostatThicknessOuterPlywood; 
  G4double box_plywood_outer_y = box_PUfoam_outer_y + cryostatThicknessOuterPlywood; 
  G4double box_plywood_outer_z = box_PUfoam_outer_z + cryostatThicknessOuterPlywood; 
  //Seven layer
  G4double box_vapor_barrier_SS_x = box_plywood_outer_x + cryostatThicknessVaporBarrierSS; 
  G4double box_vapor_barrier_SS_y = box_plywood_outer_y + cryostatThicknessVaporBarrierSS; 
  G4double box_vapor_barrier_SS_z = box_plywood_outer_z + cryostatThicknessVaporBarrierSS; 
  //support steel structure
  G4double box_steel_support_x = box_vapor_barrier_SS_x + cryostatThicknessOuterSteelSupport;
  G4double box_steel_support_y = box_vapor_barrier_SS_y + cryostatThicknessOuterSteelSupport;
  G4double box_steel_support_z = box_vapor_barrier_SS_z + cryostatThicknessOuterSteelSupport;
  //shielding inner layer 1
  G4double box_shileding_inner_x = box_steel_support_x + 1*cm;;
  G4double box_shileding_inner_y = box_steel_support_y + 1*cm;
  G4double box_shileding_inner_z = box_steel_support_z + 1*cm;
  //shielding outer layer 1
  G4double box_shileding_outer_x = box_shileding_inner_x + shieldingThickness;
  G4double box_shileding_outer_y = box_shileding_inner_y + shieldingThickness;
  G4double box_shileding_outer_z = box_shileding_inner_z + shieldingThickness;
  //Air box
  G4double box_air_cavern_x = box_shileding_outer_x + 1*m;
  G4double box_air_cavern_y = box_shileding_outer_y + 1*m;
  G4double box_air_cavern_z = box_shileding_outer_z + 1*m;
  //Rock box
  G4double box_rock_x = box_air_cavern_x + 1*m;
  G4double box_rock_y = box_air_cavern_y + 1*m;
  G4double box_rock_z = box_air_cavern_z + 1*m;

  //World box
  fWorldBox = new G4Box("World",halfSizeCavernFD2X,halfSizeCavernFD2Y,halfSizeCavernFD2Z);
  logicWorldBox = new G4LogicalVolume(fWorldBox,materialAir,"World",0,0,0);
  physicWorldBox = new G4PVPlacement(0,G4ThreeVector(),logicWorldBox,"World",0,false,0);
  //Rock Box
  G4Box* rockBox = new G4Box("RockBox", box_rock_x, box_rock_y, box_rock_z);
  logicRockBox = new G4LogicalVolume(rockBox,materialRock,"Rock",0,0,0);
  //The experimental cavern
  G4Box* cavernBox = new G4Box("CavernBox",box_air_cavern_x, box_air_cavern_y, box_air_cavern_z);
  logicCavern = new G4LogicalVolume(cavernBox,materialAir,"Cavern",0,0,0);
  //Printing key coordinates
  G4cout<<"Cavern box filled with air x: "<<box_air_cavern_x<<" y "<<box_air_cavern_y<<" z "<<box_air_cavern_z<<G4endl;
  G4cout<<"Start of shielding x: "<<box_shileding_outer_x<<" y "<<box_shileding_outer_y<<" z "<<box_shileding_outer_z<<G4endl;
  G4cout<<"LAr Volume x: "<<halfDetectorLength<<" y "<<halfDetectorWidth<<" z "<<halfDetectorThickness<<G4endl;
  //Shileding 
  G4Box* shieldingBoxOuter = new G4Box("b_shileding_outer", box_shileding_outer_x, box_shileding_outer_y, box_shileding_outer_z);
  logicshieldingBoxOuter = new G4LogicalVolume(shieldingBoxOuter, fShieldingMaterial, "Shielding", 0, 0, 0);

  G4Box* shieldingBoxInner = new G4Box("b_shileding_inner", box_shileding_inner_x, box_shileding_inner_y, box_shileding_inner_z);
  logicshieldingBoxInner = new G4LogicalVolume(shieldingBoxInner, materialAir, "Shielding", 0, 0, 0);
  
  G4double thick_support = box_steel_support_x - halfDetectorLength;
  SteelSupportConstruction mySteelSupport;
  G4VSolid* SteelSupportTest = mySteelSupport.ConstructSteelSupport(box_steel_support_x, box_steel_support_y, box_steel_support_z, thick_support);
  //cryostat
  //1) steel support sttructure
  // to be defined in another file since need to do quite some subtraction operations
  G4int n_h_bars = 5;
  G4int n_v_bars = 15;
  G4double bar_x = 0.2*m;
  G4double bar_y = box_shileding_outer_y;
  G4double bar_z = 0.5*m;

  G4Box* SteelSupportBarV = new G4Box("b_steel_support_V", bar_x, bar_y, bar_z);
  logicSteelSupport = new G4LogicalVolume(SteelSupportBarV, materialSteel, "Shielding", 0, 0, 0);

  G4double bar_h_x = (2*box_steel_support_x - 2*n_v_bars*bar_x)/n_v_bars;
  G4Box* SteelSupportBarH = new G4Box("b_steel_support_H", bar_h_x, bar_x, bar_z);
  logicSteelSupportH = new G4LogicalVolume(SteelSupportBarH, fShieldingMaterial, "Shielding", 0, 0, 0);

  G4double xBarSteel = bar_x + box_plywood_outer_x;
  G4double yBarSteel = bar_y + box_plywood_outer_y;
  G4double zBarSteel = bar_z + box_plywood_outer_z;
  // 2) box vapor barrier 
  G4Box* CryoSSVaporBarrier = new G4Box("b_CryoSSVaporBarrier", box_vapor_barrier_SS_x, box_vapor_barrier_SS_y, box_vapor_barrier_SS_z);
  logicCryoSSVaporBarrier = new G4LogicalVolume(CryoSSVaporBarrier,materialSS304L,"VaporBarrier",0,0,0);

  // 3) outer plywood  
  G4Box* CryoOuterPlywood = new G4Box("b_CryoOuterPlywwod", box_plywood_outer_x, box_plywood_outer_y, box_plywood_outer_z);
  logicCryoOuterPlywood = new G4LogicalVolume(CryoOuterPlywood,materialPlywood,"OuterPlywood",0,0,0);

  // 4) outer PUfoam  
  G4Box* CryoOuterPUfoam = new G4Box("b_CryoOuterPUfoam", box_PUfoam_outer_x, box_PUfoam_outer_y, box_PUfoam_outer_z);
  logicCryoOuterPUfoam = new G4LogicalVolume(CryoOuterPUfoam,materialPUfoam,"OuterPUfoam",0,0,0);

  // 5) Secondary barrier Al
  G4Box* CryoSecondaryBarrierAl = new G4Box("b_CryoSecondaryBarrierAl", box_Al_barrier_x, box_Al_barrier_y, box_Al_barrier_z);
  logicCryoSecondaryBarrierAl = new G4LogicalVolume(CryoSecondaryBarrierAl,materialAlCryostat,"SecondaryBarrier",0,0,0);

  // 6) Inner PUfoam  
  G4Box* CryoInnerPUfoam = new G4Box("b_CryoInnerPUfoam", box_PUfoam_inner_x, box_PUfoam_inner_y, box_PUfoam_inner_z);
  logicCryoInnerPUfoam = new G4LogicalVolume(CryoInnerPUfoam,materialPUfoam,"InnerPUfoam",0,0,0);

  // 7) inner plywood  
  G4Box* CryoInnerPlywood = new G4Box("b_CryoInnerPlywwod", box_plywood_inner_x, box_plywood_inner_y, box_plywood_inner_z);
  logicCryoInnerPlywood = new G4LogicalVolume(CryoInnerPlywood,materialPlywood,"InnerPlywood",0,0,0);

  // 8) primary membrane  
  G4Box* CryoPrimMembrane = new G4Box("b_CryoPrimMembrane", box_size_ss_membrane_x, box_size_ss_membrane_y, box_size_ss_membrane_z);
  logicCryoPrimMembrane = new G4LogicalVolume(CryoPrimMembrane,materialSS304L,"InnerPlywood",0,0,0);

  //LAr volume
  fd2DetectorBox = new G4Box("target", halfDetectorLength, halfDetectorWidth, halfDetectorThickness);
  fd2LogicVolume = new G4LogicalVolume(fd2DetectorBox,fd2Material, "target",0,0,0);



  G4RotationMatrix* rotationMatrixSample = new G4RotationMatrix();
  rotationMatrixSample->rotateY(45.*deg);

  // ============================================================= Detectors =============================================================
  
  G4cout<<" mySteelSupport size x "<<mySteelSupport.GetCollimatorSizeX()<<" thickness "<<thick_support<<G4endl;

  
  // Set Draw G4VisAttributes
  G4VisAttributes* visAttr = new G4VisAttributes();
  visAttr->SetVisibility(false);
  logicWorldBox->SetVisAttributes(visAttr);

  //Detector
  G4VisAttributes* visualAttributesGeDetector = new G4VisAttributes(G4Colour::Blue());
  visualAttributesGeDetector->SetForceSolid(true);
  visualAttributesGeDetector->SetVisibility(true);
  visualAttributesGeDetector->SetForceWireframe(true);
  //visualAttributesGeDetector->SetForceAuxEdgeVisible(true);
  fd2LogicVolume->SetVisAttributes(visualAttributesGeDetector);

  G4VisAttributes* visPUfoam = new G4VisAttributes(yellow);
  visPUfoam->SetVisibility(true);
  logicCryoOuterPUfoam->SetVisAttributes(visPUfoam);
  logicCryoInnerPUfoam->SetVisAttributes(visPUfoam);

  G4VisAttributes* vis_cryo_SS_support = new G4VisAttributes(cyan);
  vis_cryo_SS_support->SetForceSolid(true);
  vis_cryo_SS_support->SetVisibility(true);
  vis_cryo_SS_support->SetForceWireframe(true);
  vis_cryo_SS_support->SetForceAuxEdgeVisible(true);
  logicSteelSupport->SetVisAttributes(vis_cryo_SS_support);
  logicSteelSupportH->SetVisAttributes(vis_cryo_SS_support);


  G4double pitch_x = 2*box_steel_support_x/15;
  G4double pitch_y = 2*box_steel_support_y/5;
  G4double pitch_z = 2*box_steel_support_z/5;

  G4double origin_x = - box_steel_support_x + pitch_x/2;
  G4double origin_y = - box_steel_support_y + pitch_y/2;
  G4double origin_z = - box_steel_support_z + pitch_z/2;

  G4double pos_x, pos_y, pos_z;
  //  ============================================================= Place volumes =============================================================
  
  // Place main detector always at center of world volume
  switch(fDetectorType){

  //Detector alone for "direct" beam radiation	  
  case 0:
       
	 //place rock cavern inside world   
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		logicRockBox,
		"rock_"+std::to_string(1),
		logicWorldBox,false,1,false);
	 //place cavern hall inside rock 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		logicCavern,
		"cavern_"+std::to_string(1),
		logicRockBox,false,1,false);
	 //place shileding inside cavern  
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		logicshieldingBoxOuter,
		"shileding_"+std::to_string(1),
		logicCavern,false,1,false);
	 //place shileding inner inside shileding outer 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		logicshieldingBoxInner,
		"inner_shielding_"+std::to_string(1),
		logicshieldingBoxOuter,false,1,false);
	 //place many copies of steel bar support inside shielding inner
	 for(int i =1 ; i <= n_v_bars; i++){
	        pos_x = origin_x + (i-1)*pitch_x;

		new G4PVPlacement(0, 
		  G4ThreeVector(pos_x,0,zBarSteel),
		  logicSteelSupport,
		  "SS_support_zp_"+std::to_string(i),
		  logicshieldingBoxInner,false,1,false);

		new G4PVPlacement(0, 
		  G4ThreeVector(pos_x,0,-zBarSteel),
		  logicSteelSupport,
		  "SS_support_zm_"+std::to_string(i),
		  logicshieldingBoxInner,false,1,false);
	 }
	 //horizontal bars
         for(int i =1 ; i < n_v_bars - 1; i++){
	        pos_x = origin_x + (i-1)*pitch_x + bar_h_x + bar_x;
		for(int j=1; j <= n_h_bars; j++){
	                pos_y = origin_y + (j-1)*pitch_y;
			new G4PVPlacement(0, 
		  		G4ThreeVector(pos_x, pos_y, zBarSteel),
			  	logicSteelSupportH,
			  	"SS_support_zp_i"+std::to_string(i)+"_j_"+std::to_string(j),
			  	logicshieldingBoxInner,false,1,false);
			new G4PVPlacement(0, 
		  		G4ThreeVector(pos_x, pos_y, -zBarSteel),
			  	logicSteelSupportH,
			  	"SS_support_zm_i"+std::to_string(i)+"_j_"+std::to_string(j),
			  	logicshieldingBoxInner,false,1,false);
		}

	 }
	 //SS vapor barrier inside inner shileding box
	 new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                logicCryoSSVaporBarrier,
                "VaporBarrier_"+std::to_string(1),
                logicshieldingBoxInner,false,1,false);
	 //Outer plywood inside SS vapor barrier box
	 new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                logicCryoOuterPlywood,
                "Outer_Plywood"+std::to_string(1),
                logicCryoSSVaporBarrier,false,1,false);
	 //Outer PU foam inside plywood box
	 new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                logicCryoOuterPUfoam,
                "Outer_PUfoam"+std::to_string(1),
                logicCryoOuterPlywood,false,1,false);
	 //Secondary Al barrier inside outer PU foam box
	 new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                logicCryoSecondaryBarrierAl,
                "Secondary_Barrier_Al"+std::to_string(1),
                logicCryoOuterPUfoam,false,1,false);
	 //Inner PU foam inside secondary barrier box
	 new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                logicCryoInnerPUfoam,
                "Inner_PUfoam"+std::to_string(1),
                logicCryoSecondaryBarrierAl,false,1,false);
	 //Inner plywood inside inner PU foam box
	 new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                logicCryoInnerPlywood,
                "Inner_Plywood"+std::to_string(1),
                logicCryoInnerPUfoam,false,1,false);
	 //Primary membrane inside inner plywood box
	 new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                logicCryoPrimMembrane,
                "Inner_Plywood"+std::to_string(1),
                logicCryoInnerPlywood,false,1,false);
         //liquid argon volume inside primary membrane box
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		fd2LogicVolume,
		"target_"+std::to_string(1),
		logicCryoPrimMembrane,false,1,false);
            
     break;
   //Detector + sample
   case 1:
 	new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		logicshieldingBoxOuter,
		"contact_"+std::to_string(1),
		logicWorldBox,false,1,false);
    //place many copies of steel bar support
	 for(int i =1 ; i < 15; i++){
	        pos_x = origin_x + (i-1)*pitch_x;
		new G4PVPlacement(0, 
		  G4ThreeVector(pos_x,0,zBarSteel),
		  logicSteelSupport,
		  "SS_support_"+std::to_string(i),
		  logicshieldingBoxOuter,false,1,false);
	 }
     break;

  }

  //// Definition of simulation steps.
  //logicWorldBox->SetUserLimits(new G4UserLimits(0.1*mm));
  //fd2LogicVolume->SetUserLimits(new G4UserLimits(1.0*um));
  //logicBeWindow->SetUserLimits(new G4UserLimits(50.0*um));
  //logicCryostat->SetUserLimits(new G4UserLimits(5.0*nm));

      
  return physicWorldBox;
}
