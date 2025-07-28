//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file OpNovice/include/OpNoviceDetectorConstruction.hh
/// \brief Definition of the OpNoviceDetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "UlalapMaterials.hh"
#include "G4GDMLParser.hh"
class DetectorMessenger;
class G4LogicalVolume;
class G4Material;
class UlalapMaterials;
class G4Box;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    static G4VPhysicalVolume *
    GetPhysicalVolumeByName(const G4String &name);

  private:
    G4LogicalVolume* fIBeamTopLog = nullptr;
    G4LogicalVolume* fIBeamBotLog = nullptr;
    G4LogicalVolume* fIBeamSideLog = nullptr;
    // --- Logical volumes for Belts ---
    G4LogicalVolume* fBeltWithHoleLog = nullptr;
    G4LogicalVolume* fBeltWithoutHoleLog = nullptr;
    G4LogicalVolume* fBeltWithoutHoleLogTop = nullptr;

  public:
    virtual G4VPhysicalVolume* Construct();
    void UpdateGeometry();
    //static G4VPhysicalVolume* GetPhysicalVolumeByName(const G4String &name);
    void SetSize  (G4double);
    void SetTargetMaterial(G4String);
    void SetShieldingMaterial(G4String);
    void SetShieldingMaterialWaffle(G4String);
    void SetMaterialWaffleNeutronAbsorber(G4String);
    void SetOutputDirectory(G4String);
    void SetWorldMaterial(G4String);
    void SetDetectorType(G4int);
    void SetLArDetectorLength(G4double);
    void SetLArDetectorThickness(G4double);
    void SetLArDetectorWidth(G4double);
    void SetshieldingThickness(G4double);
    void SetWaffleThickness(G4double);
    void SetBottomWafflenCaptThickness(G4double);
    void SetcryostatThicknessPrimMembraneSS(G4double);
    void SetBiSourcePosition(G4ThreeVector);
    void SetBiSourcePosition2(G4ThreeVector);
    void SetVolName(G4ThreeVector);
    void SetDetectorName(G4String);
    void SetSetupName(G4String);
    void SetDataType(G4String);

    void IBeams();
    void Belts();
    void ShieldingFloor();

  public:
    //const G4VPhysicalVolume* GetWorld() {return physicPenCryostatBox;};
    const G4VPhysicalVolume* GetWorld() {return physicWorldBox;};
    G4int GetDetectorType(){return fDetectorType;};
    G4ThreeVector GetBiSourceCapsulePosition1(){return fBiSourcePosition;}
    G4ThreeVector GetBiSourceCapsulePosition2(){return fBiSourcePosition2;}
    G4double GetBiSourceCapsuleX(){return fBiSourcePosition.x();};
    G4double GetBiSourceCapsuleY(){return fBiSourcePosition.y();};
    G4double GetBiSourceCapsuleZ(){return fBiSourcePosition.z();};
    G4String GetDetectorName(){return fDetectorName;};
    G4String GetDetectorOutputDataDirectory(){return data_output_directory;};
    G4String GetVolName(){return fVolName;};

    G4double GetcryostatThicknessPrimMembraneSS()  {return cryostatThicknessPrimMembraneSS;};
    G4double GetShieldingThickness()  {return shieldingThickness;};
    G4double GetNeutronCaptShieldingThickness()  {return n_captureLayerThickness;};
    G4double GetWaffleThickness()  {return BottomShieldingThickness;};
    G4double GetWafflenCaptLayerThickness()  {return n_captWaffleBottomThickness;};
    G4double GetGeDetectorLength()  {return halfDetectorX*2.0;};
    G4double GetGeDetectorThickness()  {return halfDetectorZ*2.0;};
    G4double GetGeDetectorWidth()  {return halfDetectorY*2.0;};
    G4double GetSamplePositionZ()  {return zPositionSample;};
    G4String GetSetupName(){return fSetupName;};
    G4String GetDataType(){return fDataType;};
    G4Material*        GetWorldMaterial()   {return fWorldMaterial;};
    G4Material*        GetTargetMaterial()   {return fd2Material;};
    G4Material*        GetShieldingMaterial()   {return fShieldingMaterial;};
    G4Material*        GetShieldingMaterialWaffle()   {return materialShieldingWaffle;};
    G4Material*        GetMaterialNeutronCapture()   {return materialNeutronCapture;};
    G4ThreeVector* fSourceVector;

    void               DefineMaterials();

  private:

    G4double halfSizeCavernFD2X;
    G4double halfSizeCavernFD2Y;
    G4double halfSizeCavernFD2Z;

    G4double halfDetectorX;
    G4double halfDetectorXActiveAr;
    G4double halfDetectorZ;
    G4double halfDetectorZActiveAr;
    G4double halfDetectorY;
    G4double halfDetectorYActiveAr;
    G4double shieldingThickness;
    G4double BottomShieldingThickness;
    G4double cryostatThicknessPrimMembraneSS;
    G4double cryostatThicknessSecondaryBarrierAl;
    G4double cryostatThicknessVaporBarrierSS;
    G4double cryostatThicknessInnerPU;
    G4double cryostatThicknessOuterPU;
    G4double cryostatThicknessInnerPlywood;
    G4double cryostatThicknessOuterPlywood;
    G4double cryostatThicknessOuterSteelSupport;
    G4double zPositionSample;
    G4double n_captureLayerThickness;
    G4double n_captWaffleBottomThickness;
    G4double halfSourceContainerThickness;
    G4double BottomLeadShieldingThickness;



    G4Material* fWorldMaterial;
    G4Material* fd2Material;
    G4Material* fShieldingMaterial;
    G4Material* materialShieldingWaffle;
    G4Material* materialSS304L;
    G4Material* materialSteel;
    G4Material* materialPUfoam;
    G4Material* materialPlywood;
    G4Material* materialAlCryostat;
    G4Material* materialRock;
    G4Material* materialFR4;
    G4Material* materialB4C;
    G4Material* materialAcrylic;
    G4Material* materialTitanium;
    G4Material* materialNeutronCapture;
    G4Material* materialLead;
    G4String fSetupName;
    G4Box* fWorldBox;
    G4Box* fd2DetectorBox;
    G4Box* shieldingBox;
    G4Box* CryostatBox;

    G4LogicalVolume* logicWaffleBoxes;
    G4LogicalVolume* logicWaffleBottomShielding;
    G4LogicalVolume* logicNeutronAbsorberWaffle;
    G4LogicalVolume* logicSteelSupport;
    G4LogicalVolume* logicSteelSupportHorizontal;
    G4LogicalVolume* logicSteelSupportTop;
    G4LogicalVolume* logicRockBox;
    G4LogicalVolume* logicCavern;
    G4LogicalVolume* logicshieldingBoxB4C;
    G4LogicalVolume* logicShieldingBoxLongLatWall;
    G4LogicalVolume* logicShieldingBoxShortLatWall;
    G4LogicalVolume* logicshieldingBoxInner;
    G4LogicalVolume* logicshieldingBoxWaffle;
    G4LogicalVolume* logicCryoPrimMembrane;
    G4LogicalVolume* logicCryoInnerPlywood;
    G4LogicalVolume* logicCryoInnerPUfoam;
    G4LogicalVolume* logicCryoSecondaryBarrierAl;
    G4LogicalVolume* logicCryoOuterPUfoam;
    G4LogicalVolume* logicCryoOuterPlywood;
    G4LogicalVolume* logicCryoSSVaporBarrier;
    G4LogicalVolume* fd2LogicVolume;
    G4LogicalVolume* logicCryostat;
    G4LogicalVolume* logicKaptonWindow;
    G4LogicalVolume* logicMetalWindow;
    G4LogicalVolume* logicMetalWindowBack;
    G4LogicalVolume* logicMetalTube;
    G4LogicalVolume* logicWorldBox;
    G4LogicalVolume* logicLeadWaffleLayer;


    G4VPhysicalVolume* physicWorldBox;



    G4Material* materialAir;
    G4Material* materialPolyethylene;

    G4int fDetectorType;
    G4ThreeVector fBiSourcePosition;
    G4ThreeVector fBiSourcePosition2;
    G4String fDetectorName;
    G4String fVolName;
    G4String data_output_directory;
    G4String fDataType;

    UlalapMaterials* materialConstruction;
    DetectorMessenger* fDetectorMessenger;

    //I beams
    G4double fIFlangeWidth;
    G4double fIFlangeThick;
    G4double fITopLength;
    G4double fISideLength;
    G4double fIFlangeWaist;
    G4double fIFlangeHeight;
    G4double fIPortHoleRad;
    G4double fISidePortLoc;
    G4double fIPortSpacing;
    G4double fIBotPortLoc;
    G4double fIFlangeHeightInside;

    G4double fSpacing;
    G4double fht;
    G4double fst;
    G4double fzpl;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*DetectorConstruction_h*/
