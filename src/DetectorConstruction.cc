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
#include "G4EllipticalTube.hh"
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
  halfSizeCavernFD2Y = halfSizeCavernFD2Z = 30.*m;
  halfSizeCavernFD2X = 100.*m;
  fSetupName = "DUNE";
  fDataType = "csv";
  halfDetectorX = 62.0/2.0*m;
  halfDetectorXActiveAr = 60.0/2.0*m;
  halfDetectorZ = 15.1/2.0*m;
  halfDetectorZActiveAr = 13.6/2.0*m;
  halfDetectorY = 14.0/2.0*m;
  halfDetectorYActiveAr = 13.44/2.0*m;
  cryostatThicknessPrimMembraneSS = 1.2*mm;
  cryostatThicknessSecondaryBarrierAl = 0.8*mm;
  cryostatThicknessVaporBarrierSS = 12.0*mm;
  cryostatThicknessInnerPU = 0.4*m;
  cryostatThicknessOuterPU = 0.4*m;
  cryostatThicknessInnerPlywood = 10.0*mm;
  cryostatThicknessOuterPlywood = 10.0*mm;
  cryostatThicknessOuterSteelSupport = 1.20*m;
  shieldingThickness = 23.0*cm;
  BottomShieldingThickness = 30.0*cm;
  BottomLeadShieldingThickness = 2.5*cm;
  n_captureLayerThickness = 0.1*cm;
  n_captWaffleBottomThickness = 0.00236*cm;
  fBiSourcePosition = G4ThreeVector(0.*cm, 15.*cm, 0.*cm);
  fBiSourcePosition2 = G4ThreeVector(10.*cm, 45.*cm, 10.*cm);
  fDetectorType = 1;
  fDetectorName = "FD2";
  fVolName = "World";
  materialConstruction = new UlalapMaterials;
  DefineMaterials();
  data_output_directory = "./";  
  fd2Material = G4Material::GetMaterial("G4_lAr");
  fShieldingMaterial = G4Material::GetMaterial("G4_AIR");
  materialShieldingWaffle = G4Material::GetMaterial("G4_WATER");
  materialNeutronCapture = G4Material::GetMaterial("Gd2O3_Powder");
  //I beams
  // IBeams
  fIFlangeWidth = 0.402 * m; // all m here.
  fIFlangeThick = 0.040 * m;
  fIFlangeWaist = 0.022 * m;
  fIFlangeHeight = 1.108 * m - 2*fIFlangeThick;
  //fIFlangeHeightInside = fIFlangeHeight - 2*fIFlangeThick;
  fIFlangeHeightInside = fIFlangeHeight;
  fITopLength = 18.94 *m ;
  fISideLength = 17.8 * m  - 2*fIFlangeHeight - 2*fIFlangeThick; // need a little space with these side beams

  fIPortHoleRad = 0.800/2 * m;
  fISidePortLoc = 5.907 * m - fIFlangeHeight/2. ;
  fIPortSpacing = 4.0*m ;
  fIBotPortLoc = 5.0*m;

  fBeltFlangeBotWidth = 0.2*m;
  fht = 4*m; // m, for beam and belt placement
  fst = (17.832/2 + 0.030)*m;
  fzpl = 64.732*m;
  fSpacing = 64.732/41 *m; // m
  //materialNeutronCapture = G4Material::GetMaterial("G4_WATER");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction(){
  //delete physicWorldBox;
  delete fDetectorMessenger;
  delete materialConstruction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::IBeams()
{
  // Solids
  G4Box* IBeamTopFlange = new G4Box("IBeamTopFlange", (fIFlangeWidth/2.), (fIFlangeThick/2.), (fITopLength/2.));
  G4Box* IBeamTopMid    = new G4Box("IBeamTopMid",    (fIFlangeWaist/2.), (fIFlangeHeight/2.), (fITopLength/2.));

  G4Box* IBeamSideFlange = new G4Box("IBeamSideFlange", (fIFlangeWidth/2.), (fIFlangeThick/2.), (fISideLength/2.));
  G4Box* IBeamSideMid0   = new G4Box("IBeamSideMid0",   (fIFlangeWaist/2.), (fIFlangeHeight/2.), (fISideLength/2.));

  G4Tubs* IBeamPort = new G4Tubs("IBeamPortHole", 0., fIPortHoleRad, (fIFlangeThick/2.), 0., 2.*CLHEP::pi);

  // Rotations
  G4RotationMatrix* fc = new G4RotationMatrix();
  fc->rotate(CLHEP::pi/2., G4ThreeVector(0.0, 1.0, 0.0));

  // Subtractions for Bottom and Side Mid
  G4SubtractionSolid* IBeamBotMidtmp = new G4SubtractionSolid("IBeamBottomtmp", IBeamTopMid, IBeamPort, fc, G4ThreeVector(0.0, 0.0, fIPortSpacing/2. ));
  G4SubtractionSolid* IBeamBotMid = new G4SubtractionSolid("IBeamBottom", IBeamBotMidtmp, IBeamPort, fc, G4ThreeVector(0.0, 0.0, -fIPortSpacing/2. ));

  G4double baseZ = (fISideLength/2. + fIFlangeHeight/2. - fISidePortLoc) ;
  G4SubtractionSolid* IBeamSideMid1 = new G4SubtractionSolid("IBeamSide1", IBeamSideMid0, IBeamPort, fc, G4ThreeVector(0.0, 0.0, baseZ));
  G4SubtractionSolid* IBeamSideMid2 = new G4SubtractionSolid("IBeamSide2", IBeamSideMid1, IBeamPort, fc, G4ThreeVector(0.0, 0.0, baseZ - fIPortSpacing ));
  G4SubtractionSolid* IBeamSideMid  = new G4SubtractionSolid("IBeamSide",  IBeamSideMid2, IBeamPort, fc, G4ThreeVector(0.0, 0.0, baseZ - 2.0*fIPortSpacing ));

  // Transformations
  HepGeom::Transform3D tnull = HepGeom::TranslateY3D(0.0);
  HepGeom::Transform3D tr1 = HepGeom::TranslateY3D((fIFlangeHeight/2. + fIFlangeThick/2.) );
  HepGeom::Transform3D tr2 = HepGeom::TranslateY3D((-fIFlangeHeight/2. - fIFlangeThick/2.) );

  // MultiUnions
  G4MultiUnion* fBeamTopVol = new G4MultiUnion("IBeamTopVol");
  fBeamTopVol->AddNode(IBeamTopMid, tnull);
  fBeamTopVol->AddNode(IBeamTopFlange, tr1);
  fBeamTopVol->AddNode(IBeamTopFlange, tr2);
  fBeamTopVol->Voxelize();

  G4MultiUnion* fBeamBotVol = new G4MultiUnion("IBeamBotVol");
  fBeamBotVol->AddNode(IBeamBotMid, tnull);
  fBeamBotVol->AddNode(IBeamTopFlange, tr1);
  fBeamBotVol->AddNode(IBeamTopFlange, tr2);
  fBeamBotVol->Voxelize();

  G4MultiUnion* fBeamSideVol = new G4MultiUnion("IBeamSideVol");
  fBeamSideVol->AddNode(IBeamSideMid, tnull);
  fBeamSideVol->AddNode(IBeamSideFlange, tr1);
  fBeamSideVol->AddNode(IBeamSideFlange, tr2);
  fBeamSideVol->Voxelize();

  // Logical Volumes (class members)
  fIBeamTopLog  = new G4LogicalVolume(fBeamTopVol, materialSteel, "IBeamTopLog");
  fIBeamBotLog  = new G4LogicalVolume(fBeamBotVol, materialSteel, "IBeamBotLog");
  fIBeamSideLog = new G4LogicalVolume(fBeamSideVol, materialSteel, "IBeamSideLog");

  // Visualization
  G4VisAttributes* vis = new G4VisAttributes(G4Colour::Green());
  vis->SetForceSolid(true);
  vis->SetForceAuxEdgeVisible(true);
  vis->SetDaughtersInvisible(true);

  fIBeamTopLog->SetVisAttributes(vis);
  fIBeamBotLog->SetVisAttributes(vis);
  fIBeamSideLog->SetVisAttributes(vis);
}

void DetectorConstruction::Belts()
{
  const double ht = fht;  // meters
  const double st = fst;
  const double halfSpacingZ = (fSpacing / 2.0 - fIFlangeWaist / 2.0 - 0.001*m) ;

  // Solids
  G4Box* BeltFlange = new G4Box("BeltFlange", fBeltFlangeBotWidth/2, (fIFlangeWaist / 2.0) , halfSpacingZ - fIFlangeWidth/2);
  G4Box* BeltMid    = new G4Box("BeltMid", (fIFlangeWaist / 2.0), (fIFlangeHeightInside / 2.0) , halfSpacingZ);
  G4Tubs* BeltPort  = new G4Tubs("BeltPortHole", 0.0, 0.25*m , (fIFlangeThick / 2.0) , 0.0, 2.0 * CLHEP::pi);

  G4Box* BeltFlangeTop = new G4Box("BeltFlangeTop", fBeltFlangeBotWidth/2, (fIFlangeWaist / 2.0) , halfSpacingZ);
  G4Box* BeltMidTop    = new G4Box("BeltMidTop", (fIFlangeWaist / 2.0), (fIFlangeHeightInside / 4.0) , halfSpacingZ);

  // Rotations
  G4RotationMatrix* fc2 = new G4RotationMatrix();
  fc2->rotateY(CLHEP::pi / 2.0);

  // Belt with a hole
  G4SubtractionSolid* BeltHole = new G4SubtractionSolid("BeltHole", BeltMid, BeltPort, fc2, G4ThreeVector(0, 0, 0));

  // Transforms
  HepGeom::Transform3D tnull = HepGeom::TranslateY3D(0.0);
  HepGeom::Transform3D tr1   = HepGeom::TranslateY3D((fIFlangeHeightInside / 2.0 + fIFlangeThick/2.) );
  HepGeom::Transform3D tr2   = HepGeom::TranslateY3D(-(fIFlangeHeightInside / 2.0 + + fIFlangeThick/2.));

  // Transform tops
  HepGeom::Transform3D tnullTop = HepGeom::TranslateY3D(0.0);
  HepGeom::Transform3D tr1Top   = HepGeom::TranslateY3D((fIFlangeHeightInside / 4.0 + fIFlangeThick/2.) );
  HepGeom::Transform3D tr2Top   = HepGeom::TranslateY3D(-(fIFlangeHeightInside / 4.0 +  fIFlangeThick/2.));

  // Belt with hole
  G4MultiUnion* BeltWithHole = new G4MultiUnion("BeltWithHole");
  BeltWithHole->AddNode(BeltHole, tnull);
  BeltWithHole->AddNode(BeltFlange, tr1);
  BeltWithHole->AddNode(BeltFlange, tr2);
  BeltWithHole->Voxelize();

  // Belt without hole
  G4MultiUnion* BeltWithoutHole = new G4MultiUnion("BeltWithoutHole");
  BeltWithoutHole->AddNode(BeltMid, tnull);
  BeltWithoutHole->AddNode(BeltFlange, tr1);
  BeltWithoutHole->AddNode(BeltFlange, tr2);
  BeltWithoutHole->Voxelize();

  // Belt without hole top
  G4MultiUnion* BeltWithoutHoleTop = new G4MultiUnion("BeltWithoutHoleTop");
  BeltWithoutHoleTop->AddNode(BeltMidTop, tnullTop);
  BeltWithoutHoleTop->AddNode(BeltFlangeTop, tr1Top);
  BeltWithoutHoleTop->AddNode(BeltFlangeTop, tr2Top);
  BeltWithoutHoleTop->Voxelize();


  // Create logical volumes
  fBeltWithHoleLog     = new G4LogicalVolume(BeltWithHole, materialSteel, "BeltWithHoleLog");
  fBeltWithoutHoleLog  = new G4LogicalVolume(BeltWithoutHole, materialSteel, "BeltWithoutHoleLog");
  fBeltWithoutHoleLogTop  = new G4LogicalVolume(BeltWithoutHoleTop, materialSteel, "BeltWithoutHoleLogTop");

  // Visualization
  G4VisAttributes* beltVis        = new G4VisAttributes(G4Colour::Cyan());
  G4VisAttributes* beltHoleVis    = new G4VisAttributes(G4Colour::Brown());

  beltVis->SetDaughtersInvisible(true);
  beltVis->SetForceSolid(true);
  beltVis->SetForceAuxEdgeVisible(true);

  beltHoleVis->SetDaughtersInvisible(true);
  beltHoleVis->SetForceSolid(true);
  beltHoleVis->SetForceAuxEdgeVisible(true);

  fBeltWithoutHoleLogTop->SetVisAttributes(beltVis);
  fBeltWithoutHoleLog->SetVisAttributes(beltVis);
  fBeltWithHoleLog->SetVisAttributes(beltHoleVis);
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
  halfDetectorX = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetLArDetectorThickness(G4double value){
  halfDetectorZ = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetLArDetectorWidth(G4double value){
  halfDetectorY = (value/2.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetshieldingThickness(G4double value){
  shieldingThickness = (value/1.)*mm;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//Bi sources position
void DetectorConstruction::SetBiSourcePosition(G4ThreeVector value){
  fBiSourcePosition = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetBiSourcePosition2(G4ThreeVector value){
  fBiSourcePosition2 = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetWaffleThickness(G4double value){
  BottomShieldingThickness = (value/1.)*mm;
  //UpdateGeometry();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
void DetectorConstruction::SetBottomWafflenCaptThickness(G4double value){
  n_captWaffleBottomThickness = (value/1.)*mm;
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
void DetectorConstruction::SetShieldingMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);

  if (pttoMaterial) {
    fShieldingMaterial = pttoMaterial;
    if(logicShieldingBoxLongLatWall)logicShieldingBoxLongLatWall->SetMaterial(fShieldingMaterial);
    if(logicShieldingBoxShortLatWall)logicShieldingBoxShortLatWall->SetMaterial(fShieldingMaterial);
    G4cout<<" material "<<fShieldingMaterial->GetName()<<G4endl;  
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}

void DetectorConstruction::SetShieldingMaterialWaffle(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);

  if (pttoMaterial) {
    materialShieldingWaffle = pttoMaterial;
    if(logicWaffleBottomShielding)logicWaffleBottomShielding->SetMaterial(materialShieldingWaffle);
    G4cout<<" material waffle"<<materialShieldingWaffle->GetName()<<G4endl;  
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterialWaffle : "
           << materialChoice << " not found" << G4endl;
  }
  G4RunManager::GetRunManager()->ReinitializeGeometry();
  G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
}

void DetectorConstruction::SetMaterialWaffleNeutronAbsorber(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);

  if (pttoMaterial) {
    materialNeutronCapture = pttoMaterial;
    if(logicNeutronAbsorberWaffle)logicNeutronAbsorberWaffle->SetMaterial(materialNeutronCapture);
    G4cout<<" material n absorber: "<<materialNeutronCapture->GetName()<<G4endl;  
  } else {
    G4cout << "\n--> warning from DetectorConstruction::SetMaterialWaffleNeutronAbsorber : "
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
  materialFR4 = G4Material::GetMaterial("FR4");
  //materialB4C = G4Material::GetMaterial("FLEXIBORE");
  materialB4C = G4Material::GetMaterial("MinwaxGdComposite");
  materialSteel = G4Material::GetMaterial("Steel_EN8");
  materialSS304L = G4Material::GetMaterial("Steel_SS304L");
  materialPUfoam = G4Material::GetMaterial("PU_foam");
  materialPlywood = G4Material::GetMaterial("Plywood");
  materialAcrylic = G4Material::GetMaterial("Acrylic");
  materialAlCryostat = G4Material::GetMaterial("Aluminium_6061");
  materialRock = G4Material::GetMaterial("G4_CONCRETE");
  materialTitanium = G4Material::GetMaterial("titanium");
  materialLead = G4Material::GetMaterial("G4_Pb");

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

  //
  IBeams();
  Belts();
  /////// Detector + cryostat (7 layers) + shielding
  //First layer
  G4double box_size_ss_membrane_x = halfDetectorX + cryostatThicknessPrimMembraneSS;
  G4double box_size_ss_membrane_y = halfDetectorY + cryostatThicknessPrimMembraneSS;
  G4double box_size_ss_membrane_z = halfDetectorZ + cryostatThicknessPrimMembraneSS;
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
  G4double box_shielding_inner_x = box_steel_support_x + 1*cm;
  G4double box_shielding_inner_y = box_steel_support_y + 1*cm;
  G4double box_shielding_inner_z = box_steel_support_z + 1*cm;
  //shielding B4C layer
  G4double box_shielding_inner_B4C_x = box_shielding_inner_x + n_captureLayerThickness;
  G4double box_shielding_inner_B4C_y = box_shielding_inner_y + n_captureLayerThickness;
  G4double box_shielding_inner_B4C_z = box_shielding_inner_z + n_captureLayerThickness;
  //shielding outer layer 1
  //G4double box_shielding_outer_x = box_shielding_inner_x + shieldingThickness + n_captureLayerThickness;
  //G4double box_shielding_outer_y = box_shielding_inner_y + shieldingThickness + n_captureLayerThickness;
  //G4double box_shielding_outer_z = box_shielding_inner_z + shieldingThickness + n_captureLayerThickness;
  //Air box
  G4double box_air_cavern_x = box_shielding_inner_B4C_x + 0.5*m;//change here to remove box_shielding_outer
  G4double box_air_cavern_y = box_shielding_inner_B4C_y + 0.1*m;
  G4double box_air_cavern_z = box_shielding_inner_B4C_z + 0.5*m;
  //Rock box
  G4double box_rock_x = box_air_cavern_x + 10*m;
  G4double box_rock_y = box_air_cavern_y + 10*m;
  G4double box_rock_z = box_air_cavern_z + 10*m;

  //World box
  fWorldBox = new G4Box("World",halfSizeCavernFD2X,halfSizeCavernFD2Y,halfSizeCavernFD2Z);
  logicWorldBox = new G4LogicalVolume(fWorldBox,materialRock,"World",0,0,0);
  physicWorldBox = new G4PVPlacement(0,G4ThreeVector(),logicWorldBox,"World",0,false,0);
  //Rock Box
  G4Box* rockBox = new G4Box("RockBox", box_rock_x, box_rock_y, box_rock_z);
  logicRockBox = new G4LogicalVolume(rockBox,materialRock,"Rock",0,0,0);
  //The experimental cavern
  G4Box* cavernBox = new G4Box("CavernBox",box_air_cavern_x, box_air_cavern_y, box_air_cavern_z);
  logicCavern = new G4LogicalVolume(cavernBox,materialAir,"Cavern",0,0,0);
  //Printing key coordinates
  G4cout<<"start of shielding: "<<box_shielding_inner_x/cm<<" y "<<box_shielding_inner_y/cm<<" z "<<box_shielding_inner_z/cm<<G4endl;
  G4cout<<"Cavern box filled with air x: "<<box_air_cavern_x/cm<<" y "<<box_air_cavern_y/cm<<" z "<<box_air_cavern_z/cm<<G4endl;
  //G4cout<<"End of shielding x: "<<box_shielding_outer_x/cm<<" y "<<box_shielding_outer_y/cm<<" z "<<box_shielding_outer_z/cm<<G4endl;
  G4cout<<"LAr Volume x: "<<halfDetectorX/cm<<" y "<<halfDetectorY/cm<<" z "<<halfDetectorZ/cm<<G4endl;
  G4cout<<"Active LAr Volume x: "<<halfDetectorXActiveAr/cm<<" y "<<halfDetectorYActiveAr/cm<<" z "<<halfDetectorZActiveAr/cm<<G4endl;
  G4cout<<"SS vapor barrier: "<<box_vapor_barrier_SS_x/cm<<" y "<<box_vapor_barrier_SS_y/cm<<" z "<<box_vapor_barrier_SS_z/cm<<G4endl;
  //Shielding 
  //G4Box* shieldingBoxOuter = new G4Box("b_shielding_outer", box_shielding_outer_x, box_shielding_outer_y, box_shielding_outer_z);
  //logicShieldingBoxLongLatWall = new G4LogicalVolume(shieldingBoxOuter, fShieldingMaterial, "Shielding", 0, 0, 0);

  G4Box* shieldingBoxB4C = new G4Box("b_shielding_b4c", box_shielding_inner_B4C_x, box_shielding_inner_B4C_y, box_shielding_inner_B4C_z);
  logicshieldingBoxB4C = new G4LogicalVolume(shieldingBoxB4C, materialB4C, "Shielding", 0, 0, 0);

  G4Box* shieldingBoxInner = new G4Box("b_shielding_inner", box_shielding_inner_x, box_shielding_inner_y, box_shielding_inner_z);
  logicshieldingBoxInner = new G4LogicalVolume(shieldingBoxInner, materialAir, "Shielding", 0, 0, 0);

  //Lateral shielding Box, long side
  G4Box* shieldingBoxLongLatWall = new G4Box("b_shielding_walls", box_steel_support_x, box_steel_support_y, shieldingThickness/2);
  logicShieldingBoxLongLatWall = new G4LogicalVolume(shieldingBoxLongLatWall, fShieldingMaterial, "Shielding", 0, 0, 0);
  
  //Lateral shielding Box, long side
  G4Box* shieldingBoxShortLatWall = new G4Box("b_shielding_walls", shieldingThickness/2, box_steel_support_y, box_steel_support_z);
  logicShieldingBoxShortLatWall = new G4LogicalVolume(shieldingBoxShortLatWall, fShieldingMaterial, "Shielding", 0, 0, 0);

  //Waffle shielding
  G4Box* shieldingBoxWaffle = new G4Box("b_shielding_waffle", box_steel_support_x, box_steel_support_y, box_steel_support_z);
  logicshieldingBoxWaffle = new G4LogicalVolume(shieldingBoxWaffle, materialAir, "ShieldingWaffle", 0, 0, 0);
  //G4double thick_support = box_steel_support_x - halfDetectorX;
  //SteelSupportConstruction mySteelSupport;
  //G4VSolid* SteelSupportTest = mySteelSupport.ConstructSteelSupport(box_steel_support_x, box_steel_support_y, box_steel_support_z, thick_support);
  //cryostat
  //1) steel support sttructure
  // to be defined in another file since need to do quite some subtraction operations
  G4int n_h_bars = 5;
  G4int n_h_bars_top = 6;
  G4int n_v_bars_long_side = 39;
  G4cout<<"n v bars: "<<n_v_bars_long_side<<G4endl;
  G4int n_v_bars_short_side = 9;
  G4cout<<"n v short: "<<n_v_bars_short_side<<G4endl;
  G4double bar_x = 0.022/2 *m;
  G4double bar_y = box_steel_support_y;
  G4double bar_z = cryostatThicknessOuterSteelSupport /2.0;
  
  //box_vapor_barrier_SS_z

  G4Box* SteelSupportBarTop = new G4Box("b_steel_support_V", bar_x, bar_z, box_vapor_barrier_SS_z);
  logicSteelSupportTop = new G4LogicalVolume(SteelSupportBarTop, materialSteel, "SteelSupports", 0, 0, 0);

  G4Box* SteelSupportBarVertical = new G4Box("b_steel_support_V", bar_x, bar_y, bar_z);
  logicSteelSupport = new G4LogicalVolume(SteelSupportBarVertical, materialSteel, "SteelSupports", 0, 0, 0);

  G4double bar_h_x = (2*box_steel_support_x - 2*n_v_bars_long_side*bar_x)/n_v_bars_long_side/2.;
  G4Box* SteelSupportBarHorizontal = new G4Box("b_steel_support_H", bar_h_x, bar_x, bar_z);
  logicSteelSupportHorizontal = new G4LogicalVolume(SteelSupportBarHorizontal, materialSteel, "SteelSupports", 0, 0, 0);

  G4double xBarSteel = fIFlangeHeight/2 + box_vapor_barrier_SS_x + fIFlangeThick;
  G4double yBarSteel = fIFlangeHeight/2 + box_vapor_barrier_SS_y + fIFlangeThick;
  G4double zBarSteel = fIFlangeHeight/2 + box_vapor_barrier_SS_z + fIFlangeThick;
  G4cout<<" zBarSteel: "<<zBarSteel<<" fIFlangeHeight/2: "<<fIFlangeHeight/2<<" fIFlangeThick: "<<fIFlangeThick<<G4endl;
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
  fd2DetectorBox = new G4Box("target", halfDetectorX, halfDetectorY, halfDetectorZ);
  fd2LogicVolume = new G4LogicalVolume(fd2DetectorBox,fd2Material, "target",0,0,0);
  //Cathode
  G4Box* BoxCathode = new G4Box("BoxCathode",halfDetectorXActiveAr,0.5*mm,halfDetectorZActiveAr);
  G4LogicalVolume* fLogicCathode = new G4LogicalVolume(BoxCathode,materialSS304L,"cathode");
  //Arapuca
  G4double arapuca_x = 65.*cm;
  G4double arapuca_y = 1.*cm;
  G4double arapuca_z = 65.*cm;
  
  G4double pitch_x_arapuca = 2 * halfDetectorXActiveAr/20.;
  G4double pitch_z_arapuca = 2 * halfDetectorZActiveAr/2.;
  G4int arapuca_row_x[21] = { 0, 2, 4, 1, 3, 2, 4, 1, 3,2, 4, 1, 3,2, 4, 1, 3,2, 4, 1, 3}; 
  
  
  
  G4Box* BoxArapuca = new G4Box("BoxAnode",arapuca_x/2,arapuca_y/2,arapuca_z/2);
  G4LogicalVolume* fLogicArapuca = new G4LogicalVolume(BoxArapuca,materialAcrylic,"arapuca");
  //Anodes
  G4Box* BoxAnode = new G4Box("BoxAnode",halfDetectorXActiveAr,3.2*mm,halfDetectorZActiveAr);
  G4LogicalVolume* fLogicAnode = new G4LogicalVolume(BoxAnode,materialFR4,"anode");
  //Field Cage
  //Longer lateral Wide
  G4EllipticalTube* EllipTubeWideLong = new G4EllipticalTube("EllipTubeWideLong",5.*mm,23.*mm,halfDetectorXActiveAr);
  G4EllipticalTube* EllipTubeWideLongInner = new G4EllipticalTube("EllipTubeWideLongInner", 4.*mm,19.0*mm,halfDetectorXActiveAr + 1.0*cm);
  G4Box* FCBoxLongWide = new G4Box("FCBoxLongWide",5*mm,10.*mm,halfDetectorXActiveAr + 1.*cm);
  G4VSolid* fSolidFCLongWide ;
  fSolidFCLongWide = new G4SubtractionSolid("FieldCageWide", EllipTubeWideLong, EllipTubeWideLongInner, 0, G4ThreeVector(0.,0.,0.));
  fSolidFCLongWide = new G4SubtractionSolid("FieldCageWide", fSolidFCLongWide, FCBoxLongWide, 0, G4ThreeVector(7.*mm,0.,0.));
  G4LogicalVolume* fLogicFCLongWide = new G4LogicalVolume(fSolidFCLongWide,materialAlCryostat,"FieldCageWide");
  //Longer lateral Slim
  G4EllipticalTube* EllipTubeSlimLong = new G4EllipticalTube("EllipTubeSlimLong",5.*mm,7.5*mm,halfDetectorXActiveAr);
  G4EllipticalTube* EllipTubeSlimLongInner = new G4EllipticalTube("EllipTubeSlimLongInner", 4.*mm,4.0*mm,halfDetectorXActiveAr + 1.0*cm);
  G4Box* FCBoxLongSlim = new G4Box("FCBoxLongSlim",3*mm,3.*mm,halfDetectorXActiveAr + 1.*cm);
  G4VSolid* fSolidFCLongSlim ;
  fSolidFCLongSlim = new G4SubtractionSolid("FieldCageSlim", EllipTubeSlimLong, EllipTubeSlimLongInner, 0, G4ThreeVector(0.,0.,0.));
  fSolidFCLongSlim = new G4SubtractionSolid("FieldCageSlim", fSolidFCLongSlim, FCBoxLongSlim, 0, G4ThreeVector(3.*mm,0.,0.));
  G4LogicalVolume* fLogicFCLongSlim = new G4LogicalVolume(fSolidFCLongSlim,materialAlCryostat,"FieldCageSlim");
  //Short lateral Wide
  G4EllipticalTube* EllipTubeWideShort = new G4EllipticalTube("EllipTubeWideShort",5.*mm,23.*mm,halfDetectorZActiveAr);
  G4EllipticalTube* EllipTubeWideShortInner = new G4EllipticalTube("EllipTubeWideShortInner", 4.*mm,19.0*mm,halfDetectorZActiveAr + 1.0*cm);
  G4Box* FCBoxShortWide = new G4Box("FCBoxShortWide",5*mm,10.*mm,halfDetectorZActiveAr + 1.0*cm);
  G4VSolid* fSolidFCShortWide ;
  fSolidFCShortWide = new G4SubtractionSolid("FieldCageWide", EllipTubeWideShort, EllipTubeWideShortInner, 0, G4ThreeVector(0.,0.,0.));
  fSolidFCShortWide = new G4SubtractionSolid("FieldCageWide", fSolidFCShortWide, FCBoxShortWide, 0, G4ThreeVector(7.*mm,0.,0.));
  G4LogicalVolume* fLogicFCShortWide = new G4LogicalVolume(fSolidFCShortWide,materialAlCryostat,"FieldCageWide");
  //Short lateral Slim
  G4EllipticalTube* EllipTubeSlimShort = new G4EllipticalTube("EllipTubeSlimShort",5.*mm,7.5*mm,halfDetectorZActiveAr);
  G4EllipticalTube* EllipTubeSlimShortInner = new G4EllipticalTube("EllipTubeSlimShortInner", 4.*mm,4.0*mm,halfDetectorZActiveAr + 1.0*cm);
  G4Box* FCBoxShortSlim = new G4Box("FCBoxShortSlim",3*mm,3.*mm,halfDetectorZActiveAr + 1.*cm);
  G4VSolid* fSolidFCShortSlim ;
  fSolidFCShortSlim = new G4SubtractionSolid("FieldCageSlim", EllipTubeSlimShort, EllipTubeSlimShortInner, 0, G4ThreeVector(0.,0.,0.));
  fSolidFCShortSlim = new G4SubtractionSolid("FieldCageSlim", fSolidFCShortSlim, FCBoxShortSlim, 0, G4ThreeVector(3.*mm,0.,0.));
  G4LogicalVolume* fLogicFCShortSlim = new G4LogicalVolume(fSolidFCShortSlim,materialAlCryostat,"FieldCageSlim");


  //source holder for Bi source
  G4double innerRadiusSourceContainer = 0.*mm;
  G4double externalRadiusSourceContainer = 20.*mm;
  halfSourceContainerThickness = 5.3*um;
  G4Tubs* SourceContainerDisk = new G4Tubs("sourceContainer",innerRadiusSourceContainer,externalRadiusSourceContainer,halfSourceContainerThickness,0.,360.*deg);
  G4LogicalVolume* logicSourceContainer = new G4LogicalVolume(SourceContainerDisk,materialTitanium,"sourceContainer",0,0,0);

  G4RotationMatrix* rotationMatrixSteelSupportsShortSides = new G4RotationMatrix();
  rotationMatrixSteelSupportsShortSides->rotateY(90.*deg);

  G4RotationMatrix* rotationMatrixSteelSupportsTopShorts = new G4RotationMatrix();
  rotationMatrixSteelSupportsTopShorts->rotateX(90.*deg);

  G4RotationMatrix* rotationMatrixArapucaLongX = new G4RotationMatrix();
  rotationMatrixArapucaLongX->rotateX(90.*deg);
  G4RotationMatrix* rotationMatrixArapucaShortZ = new G4RotationMatrix();
  rotationMatrixArapucaShortZ->rotateZ(90.*deg);
  G4RotationMatrix* rotationMatrixFCzp = new G4RotationMatrix();
  rotationMatrixFCzp->rotateY(-90.*deg);
  G4RotationMatrix* rotationMatrixFCzm = new G4RotationMatrix();
  rotationMatrixFCzm->rotateY(90.*deg);
  G4RotationMatrix* rotationMatrixFCx = new G4RotationMatrix();
  rotationMatrixFCx->rotateY(180.*deg);
  // ============================================================= Detectors =============================================================
  
  //G4cout<<" mySteelSupport size x "<<mySteelSupport.GetCollimatorSizeX()<<" thickness "<<thick_support<<G4endl;

  
  // Set Draw G4VisAttributes
  G4VisAttributes* visAttr = new G4VisAttributes();
  visAttr->SetVisibility(false);
  logicWorldBox->SetVisAttributes(visAttr);
  //logicSteelSupport->SetVisAttributes(visAttr);
  fLogicFCLongWide->SetVisAttributes(visAttr);
  fLogicFCLongSlim->SetVisAttributes(visAttr);

  //Detector
  G4VisAttributes* visualAttributesGeDetector = new G4VisAttributes(G4Colour::Blue());
  visualAttributesGeDetector->SetForceSolid(true);
  visualAttributesGeDetector->SetVisibility(true);
  visualAttributesGeDetector->SetForceWireframe(true);
  //visualAttributesGeDetector->SetForceAuxEdgeVisible(true);
  fd2LogicVolume->SetVisAttributes(visualAttributesGeDetector);

  G4VisAttributes* visPUfoam = new G4VisAttributes(yellow);
  visPUfoam->SetVisibility(true);
  //logicCryoOuterPUfoam->SetVisAttributes(visPUfoam);
  //logicCryoInnerPUfoam->SetVisAttributes(visPUfoam);

  G4VisAttributes* visLead = new G4VisAttributes(yellow);
  visLead->SetVisibility(true);
  //Arapuca
  G4VisAttributes* vis_arapuca = new G4VisAttributes(lblue);
  vis_arapuca->SetForceSolid(true);
  vis_arapuca->SetVisibility(true);
  fLogicArapuca->SetVisAttributes(vis_arapuca);

  //Shielding on waffle
  G4VisAttributes* vis_waffle = new G4VisAttributes(white);
  vis_waffle->SetForceSolid(true);
  vis_waffle->SetVisibility(true);
  vis_waffle->SetForceAuxEdgeVisible(true);
  //logicshieldingBoxWaffle->SetVisAttributes(vis_waffle);
  logicShieldingBoxLongLatWall->SetVisAttributes(vis_waffle);
  //logicShieldingBoxShortLatWall->SetVisAttributes(vis_waffle);

  G4VisAttributes* vis_anode = new G4VisAttributes(orange);
  vis_anode->SetForceSolid(true);
  vis_anode->SetVisibility(true);
  fLogicAnode->SetVisAttributes(vis_anode);

  G4VisAttributes* vis_cryo_SS_support = new G4VisAttributes(red);
  vis_cryo_SS_support->SetForceSolid(true);
  vis_cryo_SS_support->SetVisibility(true);
  //vis_cryo_SS_support->SetForceWireframe(true);
  vis_cryo_SS_support->SetForceAuxEdgeVisible(true);
  //vis_cryo_SS_support->SetForceLineSegmentsPerCircle (80);
  logicSteelSupport->SetVisAttributes(vis_cryo_SS_support);
  logicSteelSupportTop->SetVisAttributes(vis_cryo_SS_support);
  logicSteelSupportHorizontal->SetVisAttributes(vis_cryo_SS_support);
   //fLogicFCLongWide->SetVisAttributes(vis_cryo_SS_support);
  // fLogicFCLongSlim->SetVisAttributes(vis_cryo_SS_support);
   //fLogicFCShortSlim->SetVisAttributes(vis_cryo_SS_support);
  // fLogicFCShortWide->SetVisAttributes(vis_cryo_SS_support);


  G4double pitch_x = 2*box_steel_support_x/n_v_bars_long_side;
  G4double pitch_y = 2*box_steel_support_y/5;
  G4double pitch_z = 2*box_steel_support_z/6;

  G4double origin_x = -19*fSpacing;
  G4double origin_y = - box_steel_support_y + pitch_y/2;
  G4double origin_z = -4*fSpacing;

  G4double FCpitch = 60*mm;
  G4int nFCbars = 110;

  //Bottom waffle
  G4double waffleBoxX = fSpacing - fIFlangeWaist;
  G4double waffleBoxZ = fSpacing - fIFlangeWaist;
  G4double waffleBoxXLead = fSpacing - fIFlangeWaist - fIFlangeWidth;//For lead since we have the flange
  G4double waffleBoxZLead = fSpacing - fBeltFlangeBotWidth;//For lead since we have the flange

  //Volume for bottom shielding 
  G4Box* BoxInsideWaffle = new G4Box("b_waffle",waffleBoxX/2,BottomShieldingThickness/2,waffleBoxZ/2);
  logicWaffleBottomShielding = new G4LogicalVolume(BoxInsideWaffle, materialShieldingWaffle, "DaughterLV");

  //Volume for neutron absorber
  G4Box* BoxNcapture = new G4Box("b_waffle_n",waffleBoxX/2,n_captWaffleBottomThickness/2,waffleBoxZ/2);
  logicNeutronAbsorberWaffle = new G4LogicalVolume(BoxNcapture, materialNeutronCapture, "DaughterLV");
  //Place neutron absorber inside shielding brick at middle
  new G4PVPlacement(nullptr,                 // no rotation
                  G4ThreeVector(0,0,0),      // local position in D1
                  logicNeutronAbsorberWaffle,      // LV we just created
                  "WaffleNeutronAbsPV",                   // PV name
                  logicWaffleBottomShielding,// <--- mother LV is D1
                  false,                     // no booleans
                  0,                         // copy-number of D2
                  1);

  //Volume for lead layer in bottom waffle
  G4Box* BoxLeadInsideWaffle = new G4Box("b_lead_waffle",waffleBoxXLead/2,BottomLeadShieldingThickness/2,waffleBoxZLead/2);
  logicLeadWaffleLayer = new G4LogicalVolume(BoxLeadInsideWaffle, materialLead, "DaughterLV");

  G4double pos_x, pos_y, pos_z;
  G4double pos_y_lead = -yBarSteel - fIFlangeHeight/2 - fIFlangeThick + BottomLeadShieldingThickness;

  //logicWaffleBoxes->SetVisAttributes(vis_waffle);
  logicWaffleBottomShielding->SetVisAttributes(vis_waffle);
  logicLeadWaffleLayer->SetVisAttributes(visLead);

  G4RotationMatrix* rotationMatrix0 = new G4RotationMatrix(0,0,0);
  rotationMatrix0->rotateX(0*deg);
  G4RotationMatrix* rotationMatrixX = new G4RotationMatrix(0,0,0);
  rotationMatrixX->rotateX(90*deg);
  G4RotationMatrix* rotationMatrixY = new G4RotationMatrix(0,0,0);
  rotationMatrixY->rotateY(90*deg);
  G4RotationMatrix* rotationMatrixZ = new G4RotationMatrix(0,0,0);
  rotationMatrixZ->rotateZ(90*deg);
  G4RotationMatrix* rotationMatrixWaffleBoxes = new G4RotationMatrix(0,0,0);
  rotationMatrixWaffleBoxes->rotateX(180*deg);
  G4RotationMatrix* rotationMatrixXminus = new G4RotationMatrix(0,0,0);
  rotationMatrixXminus->rotateX(-90*deg);
  G4RotationMatrix* rotationMatrixYX = new G4RotationMatrix(0,0,0);
  rotationMatrixYX->rotateY(90*deg);
  rotationMatrixYX->rotateX(-90*deg);
  G4RotationMatrix* rotationMatrixXZ = new G4RotationMatrix(0,0,0);
  rotationMatrixXZ->rotateX(90*deg);
  rotationMatrixXZ->rotateZ(90*deg);
  G4RotationMatrix* rotationMatrixYZ = new G4RotationMatrix(0,0,0);
  rotationMatrixYZ->rotateY(90*deg);
  rotationMatrixYZ->rotateZ(90*deg);
  //  ============================================================= Place volumes =============================================================
  
  // Place main detector always at center of world volume
  switch(fDetectorType){

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //                      Case 0 with shielding all around detector 
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
	 //place shielding inside cavern 
	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,box_air_cavern_z - shieldingThickness/2 - 5*cm),
		logicShieldingBoxLongLatWall,
		"shielding_walls_long"+std::to_string(1),
		logicCavern,false,1,1);
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,-box_air_cavern_z + shieldingThickness/2 + 5*cm),
		logicShieldingBoxLongLatWall,
		"shielding_walls_long"+std::to_string(1),
		logicCavern,false,1,1);
	 //Short walls
  	 new G4PVPlacement(0, 
		G4ThreeVector(-box_air_cavern_x + shieldingThickness/2 + 5*cm,0,0),
		logicShieldingBoxShortLatWall,
		"shielding_walls_short"+std::to_string(1),
		logicCavern,false,1,1);

  	 new G4PVPlacement(0, 
		G4ThreeVector(box_air_cavern_x - shieldingThickness/2 -5*cm,0,0),
		logicShieldingBoxShortLatWall,
		"shielding_walls_short"+std::to_string(1),
		logicCavern,false,1,1);

	 //place shielding inner inside shielding outer 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		logicshieldingBoxInner,
		"inner_shielding_"+std::to_string(1),
		logicCavern,false,1,false);
	 //place shielding waffle box inside inside shielding inner 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		logicshieldingBoxWaffle,
		"waffle_shielding_"+std::to_string(1),
		logicshieldingBoxInner,false,1,1);
	 //belts
	 for(int i = 1 ; i <= n_v_bars_long_side + 1; i++){
		pos_x = origin_x + (i-1)*fSpacing - fSpacing/2;
		for(int j = 1 ; j <= n_v_bars_short_side; j++){
			pos_z = origin_z + (j-1)*fSpacing;
			//top without holes
			new G4PVPlacement(
              		rotationMatrixY,
                	G4ThreeVector(pos_x, yBarSteel,pos_z),
                	fBeltWithoutHoleLogTop,
                	"BeltBot",
                	logicshieldingBoxWaffle,
                	false,
                	1,
                	true);
			//bottom with holes
			new G4PVPlacement(
              		rotationMatrixY,
                	G4ThreeVector(pos_x, -yBarSteel,pos_z),
                	fBeltWithHoleLog,
                	"BeltBot",
                	logicshieldingBoxWaffle,
                	false,
                	1,
                	true);
		}
		//border belts
		for(int k = 1 ; k <= 4; k++){
			//z side +
			pos_y = yBarSteel - (k-1)*fht;
			new G4PVPlacement(
             		rotationMatrixYZ,
                	G4ThreeVector(pos_x, pos_y, zBarSteel),
                	fBeltWithHoleLog,
                	"BeltBot",
                	logicshieldingBoxWaffle,
                	false,
                	1,
                	true);
			//z side -
			new G4PVPlacement(
             		rotationMatrixYZ,
                	G4ThreeVector(pos_x, pos_y, -zBarSteel),
                	fBeltWithHoleLog,
                	"BeltBot",
                	logicshieldingBoxWaffle,
                	false,
                	1,
                	true);
		}
	 }

         //Short sides 
	 for(int i = 1 ; i <= n_v_bars_short_side; i++){
		pos_z = origin_z + (i-1)*fSpacing;
	 	new G4PVPlacement(rotationMatrixYX,
                  G4ThreeVector(xBarSteel, 0, pos_z),  // example position
                  fIBeamSideLog,
                  "IbeamtWithtHoleXp",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);

	 	new G4PVPlacement(rotationMatrixYX,
                  G4ThreeVector(-xBarSteel, 0, pos_z),  // example position
                  fIBeamSideLog,
                  "IbeamtWithtHoleXm",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);
	 }
	 for(int i = 0 ; i <= n_v_bars_short_side + 2; i++){
		pos_z = origin_z + (i-1)*fSpacing - fSpacing/2;
		for(int j = 1 ; j <= 4; j++){
			pos_y = yBarSteel - (j-1)*fht;
			//x plus
			new G4PVPlacement(
             		rotationMatrixZ,
                	G4ThreeVector(xBarSteel, pos_y, pos_z),
                	fBeltWithHoleLog,
                	"BeltBotXp",
                	logicshieldingBoxWaffle,
                	false,
                	1,
                	true);
			//x minus
			new G4PVPlacement(
             		rotationMatrixZ,
                	G4ThreeVector(-xBarSteel, pos_y, pos_z),
                	fBeltWithHoleLog,
                	"BeltBotXp",
                	logicshieldingBoxWaffle,
                	false,
                	1,
                	true);
		}
	 }
	 //Long sides
	 for(int i = 1 ; i <= n_v_bars_long_side; i++){
		pos_x = origin_x + (i-1)*fSpacing;
	 	new G4PVPlacement(rotationMatrixXminus,
                  G4ThreeVector(pos_x, 0, -zBarSteel),  // example position
                  fIBeamSideLog,
                  "IbeamtWithtHoleZm",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);

		new G4PVPlacement(rotationMatrixXminus,
                  G4ThreeVector(pos_x, 0, zBarSteel),  // example position
                  fIBeamSideLog,
                  "IbeamtWithtHoleZp",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);

	 	new G4PVPlacement(0,
                  G4ThreeVector(pos_x, yBarSteel,0),  // example position
                  fIBeamTopLog,
                  "IBeamWithoutHoleTop",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);

		new G4PVPlacement(0,
                  G4ThreeVector(pos_x, -yBarSteel,0),  // example position
                  fIBeamBotLog,
                  "IBeamWithtHoleBottom",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);
	 }

	 //Bottom shielding
	 pos_y = -yBarSteel - fIFlangeHeight/2 + BottomShieldingThickness/2;
	 for(int i = 1 ; i <= n_v_bars_long_side + 1; i++){
		pos_x = origin_x + (i-1)*fSpacing - fSpacing/2;
		for(int j = 1 ; j <= n_v_bars_short_side + 1; j++){
			pos_z = origin_z + (j-1)*fSpacing - fSpacing/2;
			new G4PVPlacement(0,
                  	G4ThreeVector(pos_x, pos_y, pos_z),  // example position
                  	logicWaffleBottomShielding,
                  	"BottomWaffleShileding",
                  	logicshieldingBoxWaffle,
                  	false,
                  	1,
                  	true);

			new G4PVPlacement(0,
                  	G4ThreeVector(pos_x, pos_y_lead, pos_z),  // example position
                  	logicLeadWaffleLayer,
                  	"BottomWaffleShileding",
                  	logicshieldingBoxWaffle,
                  	false,
                  	1,
                  	true);
		}
	 }
	 //SS vapor barrier inside shielding waffle box
	 new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                logicCryoSSVaporBarrier,
                "VaporBarrier_"+std::to_string(1),
                logicshieldingBoxWaffle,false,1,1);
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
	 //Bi source container
	 new G4PVPlacement(rotationMatrixX,
			 fBiSourcePosition,
                        logicSourceContainer,
                        "sourceContainer",
                        fd2LogicVolume, false, 0, false);
	 new G4PVPlacement(rotationMatrixX,
			 fBiSourcePosition2,
                        logicSourceContainer,
                        "sourceContainer2",
                        fd2LogicVolume, false, 0, false);
	//Anode
	new G4PVPlacement(0,
                G4ThreeVector(0,halfDetectorYActiveAr + 10.*mm,0),
                fLogicAnode,
                "anode",
                fd2LogicVolume,false,1,true);
	new G4PVPlacement(0,
                G4ThreeVector(0,-halfDetectorYActiveAr - 10.*mm,0),
                fLogicAnode,
                "anode",
                fd2LogicVolume,false,1,true);
	//cathode
	new G4PVPlacement(0,
                G4ThreeVector(0,30.*mm,0),
                fLogicCathode,
                "cathode",
                fd2LogicVolume,false,1,true);
	new G4PVPlacement(0,
                G4ThreeVector(0,-30.*mm,0),
                fLogicCathode,
                "cathode",
                fd2LogicVolume,false,1,true);
         //Arapucas in cathode
        for(int i =1 ; i < 92; i++){
        	pos_x = -halfDetectorXActiveAr + (i - 1)*arapuca_x +  arapuca_x/2;
		for(int k = 1; k < 20; k++){
			pos_z = -halfDetectorZActiveAr + arapuca_z/2 + (k-1)*arapuca_z;
			if( (i + arapuca_row_x[k] - 1) % 4 == 0  ){
			   //G4cout<<i<<" "<<arapuca_row_x[k]<<" k "<<k<<G4endl;
			   new G4PVPlacement(0,
			   G4ThreeVector(pos_x,0,pos_z),
  			   fLogicArapuca,     //its logical volume
			   "Arapuca",
  			   fd2LogicVolume,	false, 0, 0);
			}
		}
	}
	//Arapucas long sides	 
	 for(int i =1 ; i <= 20; i++){
        	pos_x = -halfDetectorXActiveAr + (i - 1)*pitch_x_arapuca +  2*arapuca_x;
		for(int j = 1; j <= 4; j++){
			pos_y = 330.*cm + arapuca_x/2 + (j-1)*arapuca_x + 10.*cm;
			//Top side front
			new G4PVPlacement(rotationMatrixArapucaLongX,
			G4ThreeVector(pos_x,pos_y, halfDetectorZActiveAr + 5.*cm),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Top side back
			new G4PVPlacement(rotationMatrixArapucaLongX,
			G4ThreeVector(pos_x,pos_y, -halfDetectorZActiveAr - 5.*cm),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Bottom side front
			new G4PVPlacement(rotationMatrixArapucaLongX,
			G4ThreeVector(pos_x,-pos_y, halfDetectorZActiveAr + 5.*cm),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Bottom side back
			new G4PVPlacement(rotationMatrixArapucaLongX,
			G4ThreeVector(pos_x,-pos_y, -halfDetectorZActiveAr - 5.*cm),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
		}
	}
	//Arapucas short sides
	for(int k = 1; k<=2; k++){
        	pos_z = -halfDetectorZActiveAr + (k - 1)*pitch_z_arapuca +  4 * arapuca_x;
		for(int j = 1; j <= 4; j++){
			pos_y = 330.*cm + arapuca_x/2 + (j-1)*arapuca_x + 10.*cm;
			//Top side plus
			new G4PVPlacement(rotationMatrixArapucaShortZ,
			G4ThreeVector(halfDetectorXActiveAr + 5.*cm,pos_y, pos_z),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Top side minus
			new G4PVPlacement(rotationMatrixArapucaShortZ,
			G4ThreeVector(-halfDetectorXActiveAr - 5.*cm,pos_y, pos_z),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Bottom side plus
			new G4PVPlacement(rotationMatrixArapucaShortZ,
			G4ThreeVector(halfDetectorXActiveAr + 5.*cm,-pos_y, pos_z),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Bottom side minus
			new G4PVPlacement(rotationMatrixArapucaShortZ,
			G4ThreeVector(-halfDetectorXActiveAr - 5.*cm,-pos_y, pos_z),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
		}
	}
			
 	 //Field cage inside iquid argon
         //Longer lateral
        
        for(int i =1 ; i < 42; i++){
        	pos_y =  FCpitch * i ; 
		new G4PVPlacement(rotationMatrixFCzp,
			G4ThreeVector(0,pos_y,halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongWide,     //its logical volume
			"FC1_long",
  			fd2LogicVolume,	false, 0, true);

		new G4PVPlacement(rotationMatrixFCzp,
			G4ThreeVector(0,-pos_y,halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongWide,     //its logical volume
			"FC1_long",
  			fd2LogicVolume,	false, 0, true);

		new G4PVPlacement(rotationMatrixFCzm,
			G4ThreeVector(0,pos_y,-halfDetectorZActiveAr - 7.*mm),
  			fLogicFCLongWide,     //its logical volume
			"FC1_long",
  			fd2LogicVolume,	false, 0, true);
		
		new G4PVPlacement(rotationMatrixFCzm,
			G4ThreeVector(0,-pos_y,-halfDetectorZActiveAr - 7.*mm),
  			fLogicFCLongWide,     //its logical volume
			"FC1_long",
  			fd2LogicVolume,	false, 0, true);
	        //short sides up
        	new G4PVPlacement(0,
			G4ThreeVector(-halfDetectorXActiveAr - 7.*mm,pos_y,0),
  			fLogicFCShortWide,     //its logical volume
			"FC1_short",
  			fd2LogicVolume,	false, 0, true);

        	new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(halfDetectorXActiveAr + 7.*mm,pos_y,0),
  			fLogicFCShortWide,     //its logical volume
			"FC1_short",
  			fd2LogicVolume,	false, 0, true);
		//short sides down
        	new G4PVPlacement(0,
			G4ThreeVector(-halfDetectorXActiveAr - 7.*mm,-pos_y,0),
  			fLogicFCShortWide,     //its logical volume
			"FC1_short",
  			fd2LogicVolume,	false, 0, true);

        	new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(halfDetectorXActiveAr + 7.*mm,-pos_y,0),
  			fLogicFCShortWide,     //its logical volume
			"FC1_short",
  			fd2LogicVolume,	false, 0, true);


	}
        for(int i =42 ; i < nFCbars; i++){

        	pos_y =  FCpitch * i ; 
		new G4PVPlacement(rotationMatrixFCzp,
			G4ThreeVector(0,pos_y,halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		   new G4PVPlacement(rotationMatrixFCzp,
			G4ThreeVector(0,-pos_y,halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		  new G4PVPlacement(rotationMatrixFCzm,
			G4ThreeVector(0,pos_y,-halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		  new G4PVPlacement(rotationMatrixFCzm,
			G4ThreeVector(0,-pos_y,-halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		//Short sides up
		new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(halfDetectorXActiveAr + 7.*mm, pos_y,0),
  			fLogicFCShortSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(-halfDetectorXActiveAr - 7.*mm, pos_y,0),
  			fLogicFCShortSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);
		//Short sides down
		new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(halfDetectorXActiveAr + 7.*mm, -pos_y,0),
  			fLogicFCShortSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(-halfDetectorXActiveAr - 7.*mm, -pos_y,0),
  			fLogicFCShortSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);
	}
         

	             
     break;
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //                      Case 1 with shielding filling the waffle 
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   case 1:
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
	 //place shielding inside cavern 
	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,box_air_cavern_z - shieldingThickness/2 - 5*cm),
		logicShieldingBoxLongLatWall,
		"shielding_walls_long"+std::to_string(1),
		logicCavern,false,1,1);
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,-box_air_cavern_z + shieldingThickness/2 + 5*cm),
		logicShieldingBoxLongLatWall,
		"shielding_walls_long"+std::to_string(1),
		logicCavern,false,1,1);
	 //Short walls
  	 new G4PVPlacement(0, 
		G4ThreeVector(-box_air_cavern_x + shieldingThickness/2 + 5*cm,0,0),
		logicShieldingBoxShortLatWall,
		"shielding_walls_short"+std::to_string(1),
		logicCavern,false,1,1);

  	 new G4PVPlacement(0, 
		G4ThreeVector(box_air_cavern_x - shieldingThickness/2 -5*cm,0,0),
		logicShieldingBoxShortLatWall,
		"shielding_walls_short"+std::to_string(1),
		logicCavern,false,1,1);

	 //place shielding inner inside shielding outer 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		logicshieldingBoxInner,
		"inner_shielding_"+std::to_string(1),
		logicCavern,false,1,false);
	 //place shielding waffle box inside inside shielding inner 
  	 new G4PVPlacement(0, 
		G4ThreeVector(0,0,0),
		logicshieldingBoxWaffle,
		"waffle_shielding_"+std::to_string(1),
		logicshieldingBoxInner,false,1,1);
	 //belts
	 for(int i = 1 ; i <= n_v_bars_long_side + 1; i++){
		pos_x = origin_x + (i-1)*fSpacing - fSpacing/2;
		for(int j = 1 ; j <= n_v_bars_short_side; j++){
			pos_z = origin_z + (j-1)*fSpacing;
			//top without holes
			new G4PVPlacement(
              		rotationMatrixY,
                	G4ThreeVector(pos_x, yBarSteel,pos_z),
                	fBeltWithoutHoleLogTop,
                	"BeltBot",
                	logicshieldingBoxWaffle,
                	false,
                	1,
                	true);
			//bottom with holes
			new G4PVPlacement(
              		rotationMatrixY,
                	G4ThreeVector(pos_x, -yBarSteel,pos_z),
                	fBeltWithHoleLog,
                	"BeltBot",
                	logicshieldingBoxWaffle,
                	false,
                	1,
                	true);
		}
		//border belts
		for(int k = 1 ; k <= 4; k++){
			pos_y = yBarSteel - (k-1)*fht;

			//z side +
			if ( (i+k) % 3 == 0 || k == 4){
				new G4PVPlacement(
             			rotationMatrixYZ,
                		G4ThreeVector(pos_x, pos_y, zBarSteel),
	                	fBeltWithHoleLog,
        	        	"BeltBot",
                		logicshieldingBoxWaffle,
                		false,
	                	1,
        	        	true);
				//z side -
				new G4PVPlacement(
	             		rotationMatrixYZ,
        	        	G4ThreeVector(pos_x, pos_y, -zBarSteel),
                		fBeltWithHoleLog,
                		"BeltBot",
                		logicshieldingBoxWaffle,
                		false,
                		1,
                		true);
			}else{	
				new G4PVPlacement(
             			rotationMatrixYZ,
                		G4ThreeVector(pos_x, pos_y, zBarSteel),
	                	fBeltWithoutHoleLog,
        	        	"BeltBot",
                		logicshieldingBoxWaffle,
                		false,
	                	1,
        	        	true);
				//z side -
				new G4PVPlacement(
	             		rotationMatrixYZ,
        	        	G4ThreeVector(pos_x, pos_y, -zBarSteel),
                		fBeltWithoutHoleLog,
                		"BeltBot",
                		logicshieldingBoxWaffle,
                		false,
                		1,
                		true);

			}
		}
	 }

         //Short sides 
	 for(int i = 1 ; i <= n_v_bars_short_side; i++){
		pos_z = origin_z + (i-1)*fSpacing;
	 	new G4PVPlacement(rotationMatrixYX,
                  G4ThreeVector(xBarSteel, 0, pos_z),  // example position
                  fIBeamSideLog,
                  "IbeamtWithtHoleXp",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);

	 	new G4PVPlacement(rotationMatrixYX,
                  G4ThreeVector(-xBarSteel, 0, pos_z),  // example position
                  fIBeamSideLog,
                  "IbeamtWithtHoleXm",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);
	 }
	 for(int i = 0 ; i <= n_v_bars_short_side + 2; i++){
		pos_z = origin_z + (i-1)*fSpacing - fSpacing/2;
		for(int j = 1 ; j <= 4; j++){
			pos_y = yBarSteel - (j-1)*fht;
			//x plus
			if ( (i+j) % 3 == 0 || j == 1){
				new G4PVPlacement(
             			rotationMatrixZ,
	                	G4ThreeVector(xBarSteel, pos_y, pos_z),
        	        	fBeltWithHoleLog,
                		"BeltBotXp",
                		logicshieldingBoxWaffle,
	                	false,
        	        	1,
                		true);
				//x minus
				new G4PVPlacement(
        	     		rotationMatrixZ,
                		G4ThreeVector(-xBarSteel, pos_y, pos_z),
                		fBeltWithHoleLog,
	                	"BeltBotXp",
        	        	logicshieldingBoxWaffle,
                		false,
                		1,
	                	true);
			}else{
				new G4PVPlacement(
             			rotationMatrixZ,
	                	G4ThreeVector(xBarSteel, pos_y, pos_z),
        	        	fBeltWithoutHoleLog,
                		"BeltBotXp",
                		logicshieldingBoxWaffle,
	                	false,
        	        	1,
                		true);
				//x minus
				new G4PVPlacement(
        	     		rotationMatrixZ,
                		G4ThreeVector(-xBarSteel, pos_y, pos_z),
                		fBeltWithoutHoleLog,
	                	"BeltBotXp",
        	        	logicshieldingBoxWaffle,
                		false,
                		1,
	                	true);

			}
		}
	 }
	 //Long sides
	 for(int i = 1 ; i <= n_v_bars_long_side; i++){
		pos_x = origin_x + (i-1)*fSpacing;
	 	new G4PVPlacement(rotationMatrixXminus,
                  G4ThreeVector(pos_x, 0, -zBarSteel),  // example position
                  fIBeamSideLog,
                  "IbeamtWithtHoleZm",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);

		new G4PVPlacement(rotationMatrixXminus,
                  G4ThreeVector(pos_x, 0, zBarSteel),  // example position
                  fIBeamSideLog,
                  "IbeamtWithtHoleZp",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);

	 	new G4PVPlacement(0,
                  G4ThreeVector(pos_x, yBarSteel,0),  // example position
                  fIBeamTopLog,
                  "IBeamWithoutHoleTop",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);

		new G4PVPlacement(0,
                  G4ThreeVector(pos_x, -yBarSteel,0),  // example position
                  fIBeamBotLog,
                  "IBeamWithtHoleBottom",
                  logicshieldingBoxWaffle,
                  false,
                  1,
                  true);
	 }

	 //Bottom shielding
	 pos_y = -yBarSteel - fIFlangeHeight/2 + BottomShieldingThickness/2;
	 for(int i = 1 ; i <= n_v_bars_long_side + 1; i++){
		pos_x = origin_x + (i-1)*fSpacing - fSpacing/2;
		for(int j = 1 ; j <= n_v_bars_short_side + 1; j++){
			pos_z = origin_z + (j-1)*fSpacing - fSpacing/2;
			new G4PVPlacement(0,
                  	G4ThreeVector(pos_x, pos_y, pos_z),  // example position
                  	logicWaffleBottomShielding,
                  	"BottomWaffleShileding",
                  	logicshieldingBoxWaffle,
                  	false,
                  	1,
                  	true);

			new G4PVPlacement(0,
                  	G4ThreeVector(pos_x, pos_y_lead, pos_z),  // example position
                  	logicLeadWaffleLayer,
                  	"BottomWaffleShileding",
                  	logicshieldingBoxWaffle,
                  	false,
                  	1,
                  	true);
		}
	 }
	 //SS vapor barrier inside shielding waffle box
	 new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                logicCryoSSVaporBarrier,
                "VaporBarrier_"+std::to_string(1),
                logicshieldingBoxWaffle,false,1,1);
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
	 //Bi source container
	 new G4PVPlacement(rotationMatrixX,
			 fBiSourcePosition,
                        logicSourceContainer,
                        "sourceContainer",
                        fd2LogicVolume, false, 0, false);
	 new G4PVPlacement(rotationMatrixX,
			 fBiSourcePosition2,
                        logicSourceContainer,
                        "sourceContainer2",
                        fd2LogicVolume, false, 0, false);
	//Anode
	new G4PVPlacement(0,
                G4ThreeVector(0,halfDetectorYActiveAr + 10.*mm,0),
                fLogicAnode,
                "anode",
                fd2LogicVolume,false,1,true);
	new G4PVPlacement(0,
                G4ThreeVector(0,-halfDetectorYActiveAr - 10.*mm,0),
                fLogicAnode,
                "anode",
                fd2LogicVolume,false,1,true);
	//cathode
	new G4PVPlacement(0,
                G4ThreeVector(0,30.*mm,0),
                fLogicCathode,
                "cathode",
                fd2LogicVolume,false,1,true);
	new G4PVPlacement(0,
                G4ThreeVector(0,-30.*mm,0),
                fLogicCathode,
                "cathode",
                fd2LogicVolume,false,1,true);
         //Arapucas in cathode
        for(int i =1 ; i < 92; i++){
        	pos_x = -halfDetectorXActiveAr + (i - 1)*arapuca_x +  arapuca_x/2;
		for(int k = 1; k < 20; k++){
			pos_z = -halfDetectorZActiveAr + arapuca_z/2 + (k-1)*arapuca_z;
			if( (i + arapuca_row_x[k] - 1) % 4 == 0  ){
			   //G4cout<<i<<" "<<arapuca_row_x[k]<<" k "<<k<<G4endl;
			   new G4PVPlacement(0,
			   G4ThreeVector(pos_x,0,pos_z),
  			   fLogicArapuca,     //its logical volume
			   "Arapuca",
  			   fd2LogicVolume,	false, 0, 0);
			}
		}
	}
	//Arapucas long sides	 
	 for(int i =1 ; i <= 20; i++){
        	pos_x = -halfDetectorXActiveAr + (i - 1)*pitch_x_arapuca +  2*arapuca_x;
		for(int j = 1; j <= 4; j++){
			pos_y = 330.*cm + arapuca_x/2 + (j-1)*arapuca_x + 10.*cm;
			//Top side front
			new G4PVPlacement(rotationMatrixArapucaLongX,
			G4ThreeVector(pos_x,pos_y, halfDetectorZActiveAr + 5.*cm),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Top side back
			new G4PVPlacement(rotationMatrixArapucaLongX,
			G4ThreeVector(pos_x,pos_y, -halfDetectorZActiveAr - 5.*cm),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Bottom side front
			new G4PVPlacement(rotationMatrixArapucaLongX,
			G4ThreeVector(pos_x,-pos_y, halfDetectorZActiveAr + 5.*cm),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Bottom side back
			new G4PVPlacement(rotationMatrixArapucaLongX,
			G4ThreeVector(pos_x,-pos_y, -halfDetectorZActiveAr - 5.*cm),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
		}
	}
	//Arapucas short sides
	for(int k = 1; k<=2; k++){
        	pos_z = -halfDetectorZActiveAr + (k - 1)*pitch_z_arapuca +  4 * arapuca_x;
		for(int j = 1; j <= 4; j++){
			pos_y = 330.*cm + arapuca_x/2 + (j-1)*arapuca_x + 10.*cm;
			//Top side plus
			new G4PVPlacement(rotationMatrixArapucaShortZ,
			G4ThreeVector(halfDetectorXActiveAr + 5.*cm,pos_y, pos_z),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Top side minus
			new G4PVPlacement(rotationMatrixArapucaShortZ,
			G4ThreeVector(-halfDetectorXActiveAr - 5.*cm,pos_y, pos_z),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Bottom side plus
			new G4PVPlacement(rotationMatrixArapucaShortZ,
			G4ThreeVector(halfDetectorXActiveAr + 5.*cm,-pos_y, pos_z),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
			//Bottom side minus
			new G4PVPlacement(rotationMatrixArapucaShortZ,
			G4ThreeVector(-halfDetectorXActiveAr - 5.*cm,-pos_y, pos_z),
  			fLogicArapuca,     //its logical volume
			"Arapuca",
  			fd2LogicVolume,	false, 0, 0);
		}
	}
			
 	 //Field cage inside iquid argon
         //Longer lateral
        
        for(int i =1 ; i < 42; i++){
        	pos_y =  FCpitch * i ; 
		new G4PVPlacement(rotationMatrixFCzp,
			G4ThreeVector(0,pos_y,halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongWide,     //its logical volume
			"FC1_long",
  			fd2LogicVolume,	false, 0, true);

		new G4PVPlacement(rotationMatrixFCzp,
			G4ThreeVector(0,-pos_y,halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongWide,     //its logical volume
			"FC1_long",
  			fd2LogicVolume,	false, 0, true);

		new G4PVPlacement(rotationMatrixFCzm,
			G4ThreeVector(0,pos_y,-halfDetectorZActiveAr - 7.*mm),
  			fLogicFCLongWide,     //its logical volume
			"FC1_long",
  			fd2LogicVolume,	false, 0, true);
		
		new G4PVPlacement(rotationMatrixFCzm,
			G4ThreeVector(0,-pos_y,-halfDetectorZActiveAr - 7.*mm),
  			fLogicFCLongWide,     //its logical volume
			"FC1_long",
  			fd2LogicVolume,	false, 0, true);
	        //short sides up
        	new G4PVPlacement(0,
			G4ThreeVector(-halfDetectorXActiveAr - 7.*mm,pos_y,0),
  			fLogicFCShortWide,     //its logical volume
			"FC1_short",
  			fd2LogicVolume,	false, 0, true);

        	new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(halfDetectorXActiveAr + 7.*mm,pos_y,0),
  			fLogicFCShortWide,     //its logical volume
			"FC1_short",
  			fd2LogicVolume,	false, 0, true);
		//short sides down
        	new G4PVPlacement(0,
			G4ThreeVector(-halfDetectorXActiveAr - 7.*mm,-pos_y,0),
  			fLogicFCShortWide,     //its logical volume
			"FC1_short",
  			fd2LogicVolume,	false, 0, true);

        	new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(halfDetectorXActiveAr + 7.*mm,-pos_y,0),
  			fLogicFCShortWide,     //its logical volume
			"FC1_short",
  			fd2LogicVolume,	false, 0, true);


	}
        for(int i =42 ; i < nFCbars; i++){

        	pos_y =  FCpitch * i ; 
		new G4PVPlacement(rotationMatrixFCzp,
			G4ThreeVector(0,pos_y,halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		   new G4PVPlacement(rotationMatrixFCzp,
			G4ThreeVector(0,-pos_y,halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		  new G4PVPlacement(rotationMatrixFCzm,
			G4ThreeVector(0,pos_y,-halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		  new G4PVPlacement(rotationMatrixFCzm,
			G4ThreeVector(0,-pos_y,-halfDetectorZActiveAr + 7.*mm),
  			fLogicFCLongSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		//Short sides up
		new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(halfDetectorXActiveAr + 7.*mm, pos_y,0),
  			fLogicFCShortSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(-halfDetectorXActiveAr - 7.*mm, pos_y,0),
  			fLogicFCShortSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);
		//Short sides down
		new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(halfDetectorXActiveAr + 7.*mm, -pos_y,0),
  			fLogicFCShortSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);

		new G4PVPlacement(rotationMatrixFCx,
			G4ThreeVector(-halfDetectorXActiveAr - 7.*mm, -pos_y,0),
  			fLogicFCShortSlim,     //its logical volume
			"FC1_slim",
  			fd2LogicVolume,	false, 0, true);
	}
         
            
     break;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  }

  //// Definition of simulation steps.
  //logicWorldBox->SetUserLimits(new G4UserLimits(0.1*mm));
  //fd2LogicVolume->SetUserLimits(new G4UserLimits(1.0*um));
  //logicCryostat->SetUserLimits(new G4UserLimits(5.0*nm));

      
  return physicWorldBox;
}
