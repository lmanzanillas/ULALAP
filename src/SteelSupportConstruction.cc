#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"

#include "SteelSupportConstruction.hh"

// Class to define logical virtual volume for the light collimator

G4VSolid* SteelSupportConstruction::ConstructSteelSupport(G4double steel_x, G4double steel_y, G4double steel_z, G4double thickness){

  //Initial block dimensions
  halfSupportSteelSizeX = steel_x;
  halfSupportSteelSizeY = steel_y;
  halfSupportSteelSizeZ = steel_z;

  G4double halfInnerBlockX = steel_x - thickness;
  G4double halfInnerBlockY = steel_y - thickness;
  G4double halfInnerBlockZ = steel_z - thickness;

  G4double hole_half_x_size = 2.1*m;
  G4double hole_half_y_size = 2.1*m;
  G4double hole_half_z_size = thickness + 1.0*m;


  //Subtraction block 
  G4VSolid* initialBlock = new G4Box("s_block", halfSupportSteelSizeX, halfSupportSteelSizeY, halfSupportSteelSizeZ);

  G4VSolid* innerBlock = new G4Box("i_block", halfInnerBlockX, halfInnerBlockY, halfInnerBlockZ);

  G4double gap = 0.2*m;
  G4double pitch_x = 2*halfSupportSteelSizeX/15;
  G4double pitch_y = 2*halfSupportSteelSizeY/5;
  G4double pitch_z = 2*halfSupportSteelSizeZ/5;

  G4double security_cut = 5.0*cm;
  G4double size_hole_x = (pitch_x - 2*gap + security_cut)/2;
  G4double size_hole_y = (pitch_y - 2*gap + security_cut)/2;
  G4double size_hole_z = (pitch_z - 2*gap + security_cut)/2;

  G4double origin_x = - halfSupportSteelSizeX + pitch_x/2;
  G4double origin_y = - halfSupportSteelSizeY + pitch_y/2;
  G4double origin_z = - halfSupportSteelSizeZ + pitch_z/2;
  
  G4double pos_x, pos_y, pos_z;
  G4VSolid* SteelSupportHoles;
  G4MultiUnion* munion_solid = new G4MultiUnion("UnitedBoxes");

  G4cout<<" hole sizes: x "<<size_hole_x<<" y "<<size_hole_y<<" z "<<size_hole_z<<G4endl;
  //remove or make  holes in collimator
  int hole_number = 1;
  for(int i = 1; i <= 15; i++ ){
	  pos_x = origin_x + (i-1)*pitch_x;
	  for(int j = 1; j <= 5; j++){
		  pos_y = origin_y + (j-1)*pitch_y;
		  for(int k = 1; k <=5; k++){
			  pos_z = origin_z + (k-1)*pitch_z;
  		  	  G4VSolid* solidBoxHole = new G4Box("hole_inner", size_hole_x, size_hole_y, size_hole_z);
		  	  G4ThreeVector pos = G4ThreeVector(pos_x,pos_y,pos_z);
  		  	  G4RotationMatrix rot = G4RotationMatrix(0, 0, 0);
		  	  G4Transform3D tr = G4Transform3D(rot, pos);
		  	  munion_solid-> AddNode( *solidBoxHole, tr );
		  	  hole_number ++;
		  }
	  }
  }
  munion_solid-> Voxelize();
  SteelSupportHoles = new G4SubtractionSolid("SteelSupportHoles", initialBlock, innerBlock, 0, G4ThreeVector(0,0,0.));
  SteelSupportHoles = new G4SubtractionSolid("SteelSupportHoles", SteelSupportHoles, munion_solid, 0,  G4ThreeVector(0,0,0.));
  return SteelSupportHoles;

}



