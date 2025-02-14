# Particle-Matter Interaction Simulations with Geant4

## Overview

This Geant4 simulation project, developed at LAPP, is designed to study low-energy backgrounds in underground liquid argon detectors. A 10 kton DUNE-like module is simulated for this purpose.

![Alt text](images/10kton_LAr_module.png)

---

## Getting Started

### Recommended Environment

- It is highly recommended to use this project on a cluster (e.g., lxplus, MUST, CC).
- If you wish to run the simulation locally, you must first install the Geant4 software on your system.

### Setting Up in MUST

To set up the required environment in the MUST cluster:

1. Create a `setup_g4.sh` file with the following content:

   ```bash
   export MAMBA_ROOT_PREFIX=/mustfs/CONTAINERS/conda/neutrino/manzanilla
   eval "$(/grid_sw/soft-dev-lapp/LAPPSL7/x86_64/micromamba/latest/micromamba shell hook -s posix)"
   micromamba activate geant4
   export CMAKE_PREFIX_PATH=/mustfs/CONTAINERS/conda/neutrino/manzanilla/envs/geant4/
   ```

2. At the start of each session, run:

   ```bash
   source setup_g4.sh
   ```
### Setting Up in CC IN2P3

To set up the required environment in the CC IN2P3 cluster:

1. Create a `setup_g4.sh` file with the following content:

   ```bash
   module add Modelisation/geant4/11.2.1
   module add Production/cmake/3.29.2
   module load Analysis/root/6.30.06
   module add Compilers/gcc/13.2.0
   ```

2. At the start of each session, run:

   ```bash
   source setup_g4.sh
   ```


### Setting Up in LXPLUS

To set up the environment in LXPLUS:

1. Create a `setup_geant4.sh` file with the following content:

   ```bash
   # Modules:
   source /cvmfs/sft.cern.ch/lcg/contrib/gcc/12/x86_64-centos9/setup.sh # Set up the compiler
   source /cvmfs/geant4.cern.ch/geant4/11.1/x86_64-centos9-gcc12-optdeb-MT/CMake-setup.sh # Set up Geant4
   export CXX=`which g++` # Tell CMake about the compiler used
   export CC=`which gcc`

   # ROOT library
   export LD_LIBRARY_PATH=$BOOST/lib:$LD_LIBRARY_PATH:$ROOT6/lib:$GEANT4/lib64:/usr/lib64/:/usr/lib
   ```

2. At the start of each session, run:

   ```bash
   source setup_geant4.sh
   ```

---

## Simulation Geometry

### Key Dimensions (in cm)

- **Cavern box (filled with air):** Starts at x = ±3499.4, y = ±1154.4, z = ±1099.4.
- **Shielding start:** x = ±3284.4, y = ±939.4, z = ±884.4.
- **Shielding end (varies with thickness):** x = ±3294.4, y = ±949.4, z = ±894.4.
- **LAr volume:** Ends at x = ±3100.0, y = ±755.0, z = ±700.0.

### Placement of External Sources

To simulate neutrons from outside:

- Ensure your source coordinates are between the shielding and the cavern walls.
- Example for x-coordinates: Choose values between 3295 and 3499 (adjust based on shielding thickness).

---

## Installation

### Cloning the Repository

If this is your first time using the project, clone the repository:

```bash
git clone git@github.com:lmanzanillas/ULALAP.git
```

### Building the Software

1. Create and navigate to a `build` directory:

   ```bash
   mkdir build
   cd build
   ```

2. Run the following commands to configure and build the project:

   ```bash
   cmake ../ULALAP/ # Adjust the path if necessary
   make
   ```

3. To verify the installation, run:

   ```bash
   ./ULALAP
   ```

---

## Running the Simulation

### Interactive Mode

After running `./ULALAP`, you can view the geometry and run events interactively. Example:

```bash
/run/beamOn 1
```

### Batch Mode

For large-scale simulations, use a macro file (e.g., `test.mac`). Example:

```bash
./ULALAP -m test.mac
```

### Modifying `test.mac`

Several .mac files are provided and can be modified according to your case study

You can modify settings such as energy, particle type, and source geometry in the macro file. Example content:

```bash
#/ULALAP/det/setOutputDirectory /path/to/output/
#/ULALAP/det/setDetectorName MyDetector
#/ULALAP/det/setDetectorType 5
#/ULALAP/det/setShieldingMaterial water_borated
#/ULALAP/det/setShieldingThickness 100 cm
/process/had/particle_hp/use_photo_evaporation true
/process/had/particle_hp/do_not_adjust_final_state true
/process/had/particle_hp/skip_missing_isotopes true

# Source configuration
/ULALAP/gun/sourceType 9
/ULALAP/gun/position 10 20 30
/run/beamOn 100000
```

Uncomment and adjust the lines as needed to suit your simulation requirements.

---

## Additional Notes

- A Julia package has been created to read and analyze the geant4 output of this package: [DUNEatLapp.jl](https://github.com/MaelMartin17/DUNEatLapp.jl) 
- Ensure you are familiar with Geant4 macros and environment setup before running complex simulations.
- Check the output data format and directory settings to match your analysis pipeline.
- For support or contributions, refer to the repository documentation or contact the developers.

