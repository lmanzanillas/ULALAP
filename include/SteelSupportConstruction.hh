#ifndef COLLIMATORCONSTRUCTION_H
#define COLLIMATORCONSTRUCTION_H

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"

class SteelSupportConstruction
{
  public:
    G4VSolid* ConstructSteelSupport(G4double c_x, G4double c_y, G4double c_z, G4double c_thick);

    G4double GetCollimatorSizeX()  {return halfSupportSteelSizeX;};
    G4double GetCollimatorSizeY()  {return halfSupportSteelSizeY;};
    G4double GetCollimatorSizeZ()  {return halfSupportSteelSizeZ;};

  private:

    //Initial block dimensions
    G4double halfSupportSteelSizeX ;
    G4double halfSupportSteelSizeY ;
    G4double halfSupportSteelSizeZ ;

};
#endif
