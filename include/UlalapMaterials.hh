/**
 * @file PenMaterials.hh
 * @author: (modified by) Luis Manzanillas
 * @date 2020, Max Planck Institute for Physics
 */


#ifndef UlalapMaterials_H
#define UlalapMaterials_H

#include "globals.hh"


class UlalapMaterials {
public:
  UlalapMaterials();
  ~UlalapMaterials();
  
  void Construct();
  
private:
  G4double lightYieldAntracene;
    
};

#endif
