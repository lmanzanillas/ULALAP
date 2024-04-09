/**
 * @file PenMaterials.cc
 * @brief Define PenMaterials
 * @author: (modified by) Luis Manzanillas
 * @date 2020 Pen - Max Planck Institut fur Physik
 */


#include "UlalapMaterials.hh"

#include "G4Material.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4NistManager.hh"
#include "G4MTRunManager.hh"

#include <iterator>
#include <iostream>
#include <fstream>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
UlalapMaterials::UlalapMaterials()
{
    lightYieldAntracene=15500/MeV; //Anthracene
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
UlalapMaterials::~UlalapMaterials()
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void UlalapMaterials::Construct()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    // ------------------------------------------------------------------------
    // Elements
    // ------------------------------------------------------------------------
    //G4Element* Ge = new G4Element("Germanium", "Ge", 32., 72.64*g/mole);
    G4Element* Al = new G4Element("Aluminium", "Al", 13., 26.98*g/mole);
    G4Element* Pb = new G4Element("Lead", "Pb", 82., 207.2*g/mole);
    G4Element* O = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
    G4Element* N = new G4Element("Nitrogen", "N", 7., 14.00674*g/mole);
    G4Element* Si = new G4Element("Silicon",  "Si", 14., 28.09*g/mole);
    G4Element* C = new G4Element("Carbon", "C", 6., 12.011*g/mole);
    G4Element* C_graphite = new G4Element("TS_C_of_Graphite", "C_graphite", 6., 12.011*g/mole);
    G4Element* H = new G4Element("Hydrogen","H", 1., 1.00794*g/mole);
    G4Element* H_water = new G4Element("TS_H_of_Water","H_water", 1., 1.00794*g/mole);
    G4Element* H_polyethylene = new G4Element("TS_H_of_Polyethylene", "H_poly", 1., 1.00794*g/mole);
    //    G4Element* Cl = new G4Element("Chlorine", "Cl", 17., 35.45*g/mole);
    G4Element* F = new G4Element("Fluorine", "F", 9., 18.998*g/mole);
    G4Element* Zn = new G4Element("Zinc","Zn", 30., 65.38*g/mole);
    G4Element* S = new G4Element("Sulphur","S", 16., 32.06*g/mole);
    G4Element* Fe = new G4Element("Iron", "Fe", 26., 55.847*g/mole);

    G4Element* P = new G4Element("Phosphorus", "P", 15., 30.974*g/mole);
    G4Element* Ti = new G4Element("Titanium", "Ti", 22., 47.867*g/mole);
    G4Element* Cr = new G4Element("Chromium", "Cr", 24., 51.996*g/mole);
    G4Element* Cu = new G4Element("Copper", "Cu", 29., 63.546*g/mole);
    G4Element* Ni = new G4Element("Nickel", "Ni", 28., 58.693*g/mole);
    G4Element* W = new G4Element("Tungsten", "W", 74., 183.84*g/mole);
    //G4Element* Mo = new G4Element("Molybdenum", "Mo", 42., 95.94*g/mole);

    G4Element* Mg = new G4Element("Magnesium", "Mg", 12., 24.305*g/mole);
    G4Element* Mn = new G4Element("Manganese", "Mn", 25., 54.938*g/mole);

    G4Element* Sb = new G4Element("Antimony", "Sb", 51., 121.760*g/mole);
    G4Element* Cs = new G4Element("Cesium", "Cs", 55., 132.905*g/mole);
    G4Element* K = new G4Element("Potassium", "K", 19., 39.098*g/mole);
    // G4Element* B = new G4Element("Boron", "B", 5., 10.811*g/mole);
    G4Element* Ce = new G4Element("Cerium", "Ce", 58., 140.116 * g / mole);
    G4Element* Gd = new G4Element("Gadolinium", "Gd", 64., 157.25 * g / mole);
    G4Element* Ga = new G4Element("Gallium", "Ga", 31., 69.723 * g / mole);

    // ------------------------------------------------------------------------
    // Isotopes
    // ------------------------------------------------------------------------
    G4Isotope* Li6 = new G4Isotope("Li6", 3, 6, 6.*g/mole);
    G4Isotope* Li7 = new G4Isotope("Li7", 3, 7, 7.*g/mole);

    G4Isotope* B10 = new G4Isotope("B10", 5, 10, 10.*g/mole);
    G4Isotope* B11 = new G4Isotope("B11", 5, 11, 11.*g/mole);
    

    // ------------------------------------------------------------------------
    // Enriched Li6 - 95% by mass
    // ------------------------------------------------------------------------
    G4Element *Li6el = new G4Element("Li6el","Li", 2);
    Li6el->AddIsotope(Li6, 95.*perCent);
    Li6el->AddIsotope(Li7, 5.*perCent);
    
    // ------------------------------------------------------------------------
    // Pure Li6
    // ------------------------------------------------------------------------
    G4Element *Li6pure = new G4Element("Li6pure","Li", 1);
    Li6pure->AddIsotope(Li6, 100.0*perCent);

    // ------------------------------------------------------------------------
    // Enriched B10 - 95% by mass
    // ------------------------------------------------------------------------
    // G4Element *B10el = new G4Element("B10el","B", 2);
    // B10el->AddIsotope(B10, 95.*perCent);
    // B10el->AddIsotope(B11, 5.*perCent);

    // ------------------------------------------------------------------------
    // Enriched B10 - 95% by mass
    // ------------------------------------------------------------------------
    G4Element *B10el = new G4Element("B10el","B", 2);
    B10el->AddIsotope(B10, 95.*perCent);
    B10el->AddIsotope(B11, 5.*perCent);

    
    // ------------------------------------------------------------------------
    // PenMaterials
    // ------------------------------------------------------------------------
    G4double density;
    G4int nel;
    
    // temperature of experimental hall is controlled at 20 degree.
    G4double expTemp = STP_Temperature+20.*kelvin;

    
    // ------------------------------------------------------------------------
    // Vacuum *************************************************************
    // ------------------------------------------------------------------------
   
    nistManager->FindOrBuildMaterial("G4_Galactic"); 
    
    // ------------------------------------------------------------------------
    // Air
    // ------------------------------------------------------------------------
    nistManager->FindOrBuildMaterial("G4_AIR"); 
    
    density = 1.2929e-03 *g/cm3;  // at 20 degree
    
    G4Material* Air = new G4Material("Air", density, 2, kStateGas, expTemp);
    Air-> AddElement(N,  0.8);
    Air-> AddElement(O,  0.2);
    
    // ------------------------------------------------------------------------
    // Germanium
    // ------------------------------------------------------------------------

    G4double moleMass;
    G4double A;  // atomic mass
    G4double Z;  // atomic number
    G4String name, symbol;


    auto Ge70 = new G4Isotope("Ge70", Z = 32, A = 70, moleMass = 69.9 * g / mole);
    auto Ge72 = new G4Isotope("Ge72", Z = 32, A = 72, moleMass = 71.92 * g / mole);
    auto Ge73 = new G4Isotope("Ge73", Z = 32, A = 73, moleMass = 72.92 * g / mole);
    auto Ge74 = new G4Isotope("Ge74", Z = 32, A = 74, moleMass = 73.92 * g / mole);
    auto Ge76 = new G4Isotope("Ge76", Z = 32, A = 76, moleMass = 75.92 * g / mole);
    auto GeEn = new G4Element("enrichedGermanium", "GeEn", 5);
    GeEn->AddIsotope(Ge70, 0.001);
    GeEn->AddIsotope(Ge72, 0.001);
    GeEn->AddIsotope(Ge73, 0.001);
    GeEn->AddIsotope(Ge74, 0.130);
    GeEn->AddIsotope(Ge76, 0.867);
    auto matGeEn = new G4Material("EnGe", 5.539 * g / cm3, 1, kStateSolid);
    matGeEn->AddElement(GeEn, 1.0);
 
    //auto matGeNa = new G4Material("NaGe", 32, 72.61 * g / mole, 5.23 * g / cm3);

//
// DEFINE WATER IN MULTIPLE WAYS
//
    G4int ncomponents, natoms;

    // NIST Water
    nistManager->FindOrBuildMaterial("G4_WATER");
   
   	// Water mixture
    density = 1.000 *g/cm3;
    G4Material* water_mixture =
    		new G4Material("Water_mixture", density, ncomponents=2,
    						kStateLiquid, expTemp,1*atmosphere);
    water_mixture->AddElement(H, natoms=2);
    water_mixture->AddElement(O, natoms=1);
    water_mixture->GetIonisation()->SetMeanExcitationEnergy(78.0*eV); // ICRU 73, revised

   	// Water structure
    G4Material* water_structure =
    		new G4Material("Water_structure", density, ncomponents=2,
    						kStateLiquid, expTemp,1*atmosphere);
    water_structure->AddElement(H_water, natoms=2);
    water_structure->AddElement(O, natoms=1);
    water_structure->GetIonisation()->SetMeanExcitationEnergy(78.0*eV); // ICRU 73, revised


    // Boric Acid
    density = 1.435 *g/cm3;
    G4Material* boric_acid =
            new G4Material("Boric_Acid", density, ncomponents=3,
                            kStateLiquid, expTemp,1*atmosphere);
    boric_acid->AddElement(H, natoms=3);
    boric_acid->AddElement(O, natoms=3);
    // boric_acid->AddElement(B, natoms=1);
    boric_acid->AddElement(B10el, natoms=1);
    boric_acid->GetIonisation()->SetMeanExcitationEnergy(87.5*eV); // ESTAR calculated mean excitation energy

    // Borated Water
    G4Material* water_borated =
	    new G4Material("water_borated", 1.02 *g/cm3, ncomponents=2,
    						kStateLiquid, expTemp,1*atmosphere);
    water_borated->AddMaterial(water_mixture, 97.*perCent);
    water_borated->AddMaterial (boric_acid , 3.*perCent);
    //water_borated->AddElement (B10el , 3.*perCent);
    water_borated->GetIonisation()->SetMeanExcitationEnergy(75.6*eV); // ESTAR calculated mean excitation energy


    //
    // DEFINE GRAPHITE IN MULTIPLE WAYS
    //

    //NIST Graphite
    nistManager->FindOrBuildMaterial("G4_GRAPHITE");

    // Graphite mixture
    density = 2.27*g/cm3;
    G4Material* graphite_mixture =
    		new G4Material("Graphite_mixture", density, ncomponents=1,
    						kStateSolid, expTemp, 1*atmosphere);
    graphite_mixture->AddElement(C, natoms=1);
    graphite_mixture->GetIonisation()->SetMeanExcitationEnergy(78.0*eV); // NIST material database

    // Graphite structure
    G4Material* graphite_structure =
    		new G4Material("Graphite_structure", density, ncomponents=1,
    						kStateSolid, expTemp, 1*atmosphere);
    graphite_structure->AddElement(C_graphite, natoms=1);
    graphite_structure->GetIonisation()->SetMeanExcitationEnergy(78.0*eV); // NIST material database

    	// ------------------------------------------------------------------------
    // GAGG
    // ------------------------------------------------------------------------
    G4Material* matGAGG = new G4Material(name = "GAGG", density = 6.63 * g / cm3, ncomponents = 5);
    matGAGG->AddElement(Ce, natoms = 1);
    matGAGG->AddElement(Gd, natoms = 3);
    matGAGG->AddElement(Al, natoms = 2);
    matGAGG->AddElement(Ga, natoms = 3);
    matGAGG->AddElement(O, natoms = 12);

    // ------------------------------------------------------------------------
    // PTFE
    // ------------------------------------------------------------------------

    auto matPTFE = new G4Material("PTFE", density = 2.2 * g / cm3, 2);
    matPTFE->AddElement(C, natoms = 1);
    matPTFE->AddElement(F, natoms = 2);
    // ------------------------------------------------------------------------
    // LN2
    // ------------------------------------------------------------------------
    nistManager->FindOrBuildMaterial("G4_lN2");
    // ------------------------------------------------------------------------
    // LAr
    // ------------------------------------------------------------------------
    nistManager->FindOrBuildMaterial("G4_lAr");
    // ------------------------------------------------------------------------
    // Silicon Oil
    // ------------------------------------------------------------------------

    G4Material* silicone_oil = new G4Material(name = "silicone oil", density = 0.963 * g / cm3, ncomponents = 4);
    silicone_oil->AddElement(C, natoms = 2);
    silicone_oil->AddElement(H, natoms = 6);
    silicone_oil->AddElement(Si, natoms = 1);
    silicone_oil->AddElement(O, natoms = 1);

    // ------------------------------------------------------------------------
    // Epoxy (binders)
    // ------------------------------------------------------------------------
    G4Material * Epoxy = new G4Material("Epoxy",1*g/cm3,4,kStateSolid);
    Epoxy->AddElement(C,2);
    Epoxy->AddElement(H,6);
    Epoxy->AddElement(O,1);
    Epoxy->AddElement(Si,1);
    
       
    //lead for shielding
    G4Material* lead = new G4Material("Lead", 11.340*g/cm3, nel=1);
    lead->AddElement(Pb,1);
    
    //Aluminium for Structure
    G4Material* aluminium = new G4Material("Aluminium", 2.70*g/cm3, 1);
    aluminium->AddElement(Al,1);
    
    //HDPE for screen and reflector bars
    G4Material* HDPE = new G4Material("HDPE",0.97*g/cm3,nel=2);
    HDPE->AddElement(C,1);
    HDPE->AddElement(H,2);

    // Borated HDPE
    G4Material* HDPE_borated = new G4Material("HDPE_borated", 1.01*g/cm3, 2);
    HDPE_borated->AddMaterial(HDPE, 95.*perCent);
    HDPE_borated->AddElement(B10el, 5.*perCent);
     
    //polyurethane foam
    G4Material* PU_foam = new G4Material("PU_foam", 0.09*g/cm3, 4);
    PU_foam->AddElement(C,54);
    PU_foam->AddElement(H,60);
    PU_foam->AddElement(N,4);
    PU_foam->AddElement(O,15);
    //fiber glass 
    //G4Material* fiber_glass = new G4Material("fiber_glass", 0.09*g/cm3, 4);

    //Plywood
    G4Material* Plywood = new G4Material("Plywood", 0.5459*g/cm3, 3);
    Plywood->AddElement(C,6);
    Plywood->AddElement(H,10);
    Plywood->AddElement(O,15);
    //PPL for shielding
    G4Material* PPL = new G4Material("PPL",0.855*g/cm3,nel=2);
    PPL->AddElement(C,3);
    PPL->AddElement(H,6);
    
    //Iron for Table
    G4Material* iron = new G4Material("Iron", 8.05*g/cm3, 1);
    iron->AddElement(Fe,1);

    // ------------------------------------------------------------------------
    // Lithium Fluoride: ZnS - 50% Binder
    // ------------------------------------------------------------------------
    
    //Litium Fluoride
    G4Material* LiF = new G4Material("LiF", 2.64*g/cm3, 2);
    LiF->AddElement(Li6el,1);
    LiF->AddElement(F,  1);
    
    //Zinc Sulphide
    G4Material* ZnS = new G4Material("ZnS", 4.09*g/cm3, 2);
    ZnS->AddElement(Zn, 1);
    ZnS->AddElement(S,  1);

    G4Material* Li6ZnS = new G4Material("Li6ZnS", 2.229*g/cm3, 3);
    Li6ZnS->AddMaterial(LiF,26.14*perCent);
    Li6ZnS->AddMaterial(ZnS,52.28*perCent);
    Li6ZnS->AddMaterial(Epoxy,21.58*perCent);

    // G4cout << *(G4Material::GetMaterialTable()); // print the list of materials

    // Pure Li-6
    G4Material* Li6ZnS_Li6pure = new G4Material("Li6ZnS_Li6pure", 0.133*g/cm3, 1);
    Li6ZnS_Li6pure->AddElement(Li6pure,100.0*perCent);
    Li6ZnS->GetIonisation()->SetBirksConstant(0.008*mm/MeV);
    

    // ------------------------------------------------------------------------
    // LiF:ZnS backing => MELINEX 339, aka Polyethylene terephthalate
    // http://www.tekra.com/products/films/polyester-films/polyester-pet/melinex-339
    // https://en.wikipedia.org/wiki/Polyethylene_terephthalate
    // ------------------------------------------------------------------------
    G4Material* MELINEX = new G4Material("MELINEX", 1.38*g/cm3, nel=3);
    MELINEX->AddElement(C,10);
    MELINEX->AddElement(H,8);
    MELINEX->AddElement(O,4);

    
    // ------------------------------------------------------------------------
    // PVT Scintillator
    // ------------------------------------------------------------------------
    
    // NIST PVT
    G4Material* pvt_nist = nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    
    // PVT mixture
    density=1.023*g/cm3; // From EJ-200 data sheet
    G4Material* pvt_mixture = new G4Material("PVT_mixture",density,ncomponents=2,
    								kStateSolid, expTemp,1*atmosphere);
    pvt_mixture->AddElement(C,natoms=27);
    pvt_mixture->AddElement(H,natoms=30);
    pvt_mixture->GetIonisation()->SetMeanExcitationEnergy(64.7*eV); // NIST G4_PLASTIC_SC_VINYLTOLUENE
    
    // PVT structure
    G4Material* pvt_structure = new G4Material("PVT_structure",density,ncomponents=2,
    								kStateSolid, expTemp,1*atmosphere);
    pvt_structure->AddElement(C,natoms=27);
    pvt_structure->AddElement(H_polyethylene,natoms=30);
    pvt_structure->GetIonisation()->SetMeanExcitationEnergy(64.7*eV); // NIST G4_PLASTIC_SC_VINYLTOLUENE
    pvt_nist->GetIonisation()->SetBirksConstant(0.15*mm/MeV);
    pvt_mixture->GetIonisation()->SetBirksConstant(0.15*mm/MeV);
    pvt_structure->GetIonisation()->SetBirksConstant(0.15*mm/MeV);
    

    // ------------------------------------------------------------------------
    // Tyvek Wrapping
    // ------------------------------------------------------------------------
    
    G4Material* Tyvek = new G4Material("Tyvek", 0.38*g/cm3, 2);
    Tyvek->AddElement(C,2);
    Tyvek->AddElement(H,4);
    
        // ------------------------------------------------------------------------
    // WLS Fibre
    // ------------------------------------------------------------------------
    
    //Polystyrene WLS fibre core
    G4Material* Polystyrene = new G4Material("Polystyrene",1.05*g/cm3,2,kStateSolid,273.15*kelvin,1.0*atmosphere );
    Polystyrene->AddElement( H, 1 ); // Was 0.498
    Polystyrene->AddElement( C, 1 ); // Was 0.502
    
    //PMMA  WLS fibre cladding: PMMA (polymethylmethacrylate, C5H8O2) 
    G4Material* PMMA = new G4Material("PMMA",1.19*g/cm3,3,kStateSolid,273.15*kelvin,1.0*atmosphere );
    PMMA->AddElement( H, 8 ); // Was 0.532
    PMMA->AddElement( C, 5 ); // Was 0.336
    PMMA->AddElement( O, 2 ); // was 0.0132
    
    // ------------------------------------------------------------------------
    // MPPC
    // ------------------------------------------------------------------------
    
    G4Material* MPPCFilm = new G4Material("MPPCFilm",1.05*g/cm3,1,kStateSolid,273.15*kelvin,1.0*atmosphere);
    MPPCFilm->AddElement(Si,1);
    
   
    // ------------------------------------------------------------------------
    // Mirror
    // ------------------------------------------------------------------------
    
    G4Material* Mirror = new G4Material("Mirror", 1.00*g/cm3,1);
    Mirror->AddElement(Al,1);
    // top mirror optical properties
    // NOTE: mirror_surface is attached to the surface not the material!
    G4double mirror_PP[2] = { 2.0*eV, 4.0*eV };
    //G4double mirror_REFL[2]  = { 1.0,     1.0 };
    G4double mirror_RIND[2]  = { 100,     100 };
    G4MaterialPropertiesTable *mirror_surface = new G4MaterialPropertiesTable();
    //mirror_surface->AddProperty("REFLECTIVITY", mirror_PP, mirror_REFL, 2);
    mirror_surface->AddProperty("RINDEX", mirror_PP, mirror_RIND, 2);
    Mirror->SetMaterialPropertiesTable(mirror_surface);
    
    // ------------------------------------------------------------------------
    // Grease
    // ------------------------------------------------------------------------
    G4Material* Grease = new G4Material("Grease",
                            1.05*g/cm3,
                            2,
                            kStateSolid,
                            273.15*kelvin,
                            1.0*atmosphere );
    
    Grease->AddElement( H, 0.498 );
    Grease->AddElement( C, 0.502 );
    
   

    //Steel 080M40 (EN8)
    density = 7.87*g/cm3;
    G4Material* steel_EN8 = new G4Material("Steel_EN8", density, ncomponents=6, kStateSolid, expTemp, 1*atmosphere);
    steel_EN8->AddElement(C,   0.400*perCent);
    steel_EN8->AddElement(Si,  0.200*perCent);
    steel_EN8->AddElement(P,   0.035*perCent);
    steel_EN8->AddElement(S,   0.035*perCent);
    steel_EN8->AddElement(Mn,  0.800*perCent);
    steel_EN8->AddElement(Fe, 98.530*perCent);
    steel_EN8->GetIonisation()->SetMeanExcitationEnergy(320.7*eV); // ESTAR calculated mean excitation energy

    //Stainless steel primary membrane SS 304L corrugated
    density = 7.9*g/cm3;
    expTemp = 83.0*kelvin;
    G4Material* steel_SS304L = new G4Material("Steel_SS304L", density, ncomponents=9, kStateSolid, expTemp, 1*atmosphere);
    steel_SS304L->AddElement(C,   0.030*perCent);
    steel_SS304L->AddElement(P,   0.045*perCent);
    steel_SS304L->AddElement(S,   0.030*perCent);
    steel_SS304L->AddElement(N,   0.100*perCent);
    steel_SS304L->AddElement(Si,  0.750*perCent);
    steel_SS304L->AddElement(Mn,  2.000*perCent);
    steel_SS304L->AddElement(Cr,  19.000*perCent);
    steel_SS304L->AddElement(Cr,  10.000*perCent);
    steel_SS304L->AddElement(Fe, 68.045*perCent);
    steel_SS304L->GetIonisation()->SetMeanExcitationEnergy(320.7*eV); // ESTAR calculated mean excitation energy


    //Aluminium 6061 (EN 573-3)
    density = 2.72*g/cm3;
    G4Material* aluminium_6061 = new G4Material("Aluminium_6061", density, ncomponents=9,
                                              kStateSolid, expTemp, 1*atmosphere);
    aluminium_6061->AddElement(Mg,  1.000*perCent);
    aluminium_6061->AddElement(Al, 97.305*perCent);
    aluminium_6061->AddElement(Si,  0.600*perCent);
    aluminium_6061->AddElement(Ti,  0.075*perCent);
    aluminium_6061->AddElement(Cr,  0.195*perCent);
    aluminium_6061->AddElement(Mn,  0.075*perCent);
    aluminium_6061->AddElement(Fe,  0.350*perCent);
    aluminium_6061->AddElement(Cu,  0.275*perCent);
    aluminium_6061->AddElement(Zn,  0.125*perCent);
    aluminium_6061->GetIonisation()->SetMeanExcitationEnergy(188.6*eV); // ESTAR calculated mean excitation energy

    //CELOTEX foam (Polyisocyanurate) -->  C3H3N3O3
    density = 0.03*g/cm3;
    G4Material* celotex = new G4Material("CELOTEX", density, ncomponents=4,
                                              kStateSolid, expTemp, 1*atmosphere);
    celotex->AddElement(H, natoms=3);
    celotex->AddElement(C, natoms=3);
    celotex->AddElement(N, natoms=3);
    celotex->AddElement(O, natoms=3);
    celotex->GetIonisation()->SetMeanExcitationEnergy(46.1*eV); // ESTAR calculated mean excitation energy



    //
    // DEFINE BR2 MATERIALS
    //
    //Tantalium 
    nistManager->FindOrBuildMaterial("G4_Ta");

    // Berlium
    nistManager->FindOrBuildMaterial("G4_Be");

    //NIST Concrete, for BR2 floor and ceil
    nistManager->FindOrBuildMaterial("G4_CONCRETE");

    //NIST STAINLESS-STEEL
    nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    //NIST PARAFFIN
    nistManager->FindOrBuildMaterial("G4_PARAFFIN");


    //NIST Mylar
    nistManager->FindOrBuildMaterial("G4_MYLAR");

    //NIST Cadmium
    nistManager->FindOrBuildMaterial("G4_Cd");
    
    //NIST BRASS
    nistManager->FindOrBuildMaterial("G4_BRASS");
    
    //NIST Au
    nistManager->FindOrBuildMaterial("G4_Au");

    //NIST Au
    nistManager->FindOrBuildMaterial("G4_Cu");

    //NIST Lead
    nistManager->FindOrBuildMaterial("G4_Pb");

    //Kapton
    nistManager->FindOrBuildMaterial("G4_KAPTON");
    //
    // Define CROSS materials
    //

    //NIST Carbon
    nistManager->FindOrBuildMaterial("G4_C");

    //NIST Carbon
    nistManager->FindOrBuildMaterial("G4_NYLON-6/6");

    //NIST POM, for CROSS
    nistManager->FindOrBuildMaterial("G4_POLYOXYMETHYLENE");

    //NIST PE, for CROSS source capsule
    nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");

    G4int nelements;
    //  Peek chemical formula (C19H12O3)
    // C 114/150 = 76
    // H 12/150  = 8
    // O 24/150  = 16
    G4Material *PEEK = new G4Material("PEEK",   density = 1.31*g/cm3, nelements=3);
    PEEK->AddMaterial(nistManager->FindOrBuildMaterial("G4_C"), 76*perCent);
    PEEK->AddMaterial(nistManager->FindOrBuildMaterial("G4_H"),  8*perCent);
    PEEK->AddMaterial(nistManager->FindOrBuildMaterial("G4_O"), 16*perCent); 

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //                  PEN staff copied here
    ////////////////////////////////////////////////////////////////////////////////////////////////

    G4Material* fPOM = new G4Material("POM",density=1.41*g/cm3,nelements=3);
    fPOM->AddElement(O,1);
    fPOM->AddElement(C,1);
    fPOM->AddElement(H,2);
  
    G4Material* fABS = new G4Material("ABS",density=1.07*g/cm3,nelements=3);
    fABS->AddElement(C,15);
    fABS->AddElement(H,17);
    fABS->AddElement(N,1);
  
    // Scintillators
    G4int number_of_atoms;
    G4Material* PenMaterial = new G4Material("PEN", density= 1.3*g/cm3, nelements=3,kStateSolid, expTemp,1*atmosphere);
    //G4Material* PenMaterial = new G4Material("PEN", density= 1.023*g/cm3, nelements=3,kStateSolid, expTemp,1*atmosphere);
    PenMaterial->AddElement(O, number_of_atoms=4);
    PenMaterial->AddElement(H, number_of_atoms=10);
    PenMaterial->AddElement(C, number_of_atoms=14);
    PenMaterial->GetIonisation()->SetMeanExcitationEnergy(64.7*eV);
    PenMaterial->GetIonisation()->SetBirksConstant(0.15*mm/MeV);

    //Reflector 
    G4Material* materialVikuiti = new G4Material("Vikuiti", density = 1.29*g/cm3, nelements=3,kStateSolid, expTemp,1*atmosphere);
    materialVikuiti->AddElement(O, number_of_atoms=4);
    materialVikuiti->AddElement(H, number_of_atoms=8);
    materialVikuiti->AddElement(C, number_of_atoms=10);

    //Glass PMT
    nistManager->FindOrBuildMaterial("G4_Pyrex_Glass");
    G4Material* silica_SiO2 = nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    G4Material* boronOxide_B2O3 = nistManager->FindOrBuildMaterial("G4_BORON_OXIDE");
    G4Material* sodiumOxide_Na2O = nistManager->FindOrBuildMaterial("G4_SODIUM_MONOXIDE");
    G4Material* potasiumOxide_K2O = nistManager->FindOrBuildMaterial("G4_POTASSIUM_OXIDE");
    G4Material* aluminumOxide_Al2O3 = nistManager->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");

    
    density = 2.23*g/cm3;
    G4Material* matererialGlassPMT = new G4Material("BorosilicateGlass", density, ncomponents=5,
		    kStateSolid, expTemp, 1*atmosphere);
    matererialGlassPMT->AddMaterial(silica_SiO2, 81.*perCent);
    matererialGlassPMT->AddMaterial(boronOxide_B2O3, 13.*perCent);
    matererialGlassPMT->AddMaterial(sodiumOxide_Na2O, 2.*perCent);
    matererialGlassPMT->AddMaterial(potasiumOxide_K2O, 2.*perCent);
    matererialGlassPMT->AddMaterial(aluminumOxide_Al2O3, 2.*perCent);


    //Glass PMT
    
    nistManager->FindOrBuildMaterial("G4_Si");
    nistManager->FindOrBuildMaterial("G4_Ge");
  
    nistManager->FindOrBuildMaterial("G4_TEFLON");
  
  
    G4Material* materialTriggerFoilEJ212 = new G4Material("EJ212", density= 1.023*g/cm3, 2);
    materialTriggerFoilEJ212->AddElement(C, 0.475);
    materialTriggerFoilEJ212->AddElement(H, 0.525);

    //A complete guess of the photocathode material ... shouldn't impact the simulations
    G4Material* materialBialkali = new G4Material("Bialkali", density = 1.4 *g/cm3, 3);
    materialBialkali->AddElement(Sb, 0.32);
    materialBialkali->AddElement(Cs, 0.32);
    materialBialkali->AddElement(K, 0.36);
   
    //Tungsten alloy for collimator W95NiCu
    G4Material* materialTungsten = new G4Material("tungsten_alloy", 18.0*g/cm3, 3);
    materialTungsten->AddElement(W,0.95);
    materialTungsten->AddElement(Ni,0.035); 
    materialTungsten->AddElement(Cu,0.015); 

    //Titanium grade 1
    G4Material* materialTitaniumGrade1 = new G4Material("titanium_grade1", 4.54*g/cm3, 6);
    materialTitaniumGrade1->AddElement(Ti,0.9936);
    materialTitaniumGrade1->AddElement(Fe,0.002);  
    materialTitaniumGrade1->AddElement(O,0.0018);  
    materialTitaniumGrade1->AddElement(H,0.0015);  
    materialTitaniumGrade1->AddElement(C,0.0008);  
    materialTitaniumGrade1->AddElement(N,0.0003);  
    
    //Titanium grade 2
    G4Material* materialTitaniumGrade2 = new G4Material("titanium_grade2", 4.54*g/cm3, 6);
    materialTitaniumGrade2->AddElement(Ti,0.9919);
    materialTitaniumGrade2->AddElement(Fe,0.003);  
    materialTitaniumGrade2->AddElement(O,0.0025);  
    materialTitaniumGrade2->AddElement(H,0.0015);  
    materialTitaniumGrade2->AddElement(C,0.0008);  
    materialTitaniumGrade2->AddElement(N,0.0003);  


}
