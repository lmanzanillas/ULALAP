# Particle matter interaction simulatons with Geant4



## Getting started

Geant4 simulation project developed at LAPP to study backgrounds in underground liquid argon detectors 

## How to use it

- It's highly recommended to use it in a cluster like lxplus or cc
- If you want to use it in your local computer you first need to install the geant4 software
- In lxplus you just need to connect to the cluster and then charge the required modules: 

```
# Modules:
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/12/x86_64-centos9/setup.sh #set up compiler
source /cvmfs/geant4.cern.ch/geant4/11.1/x86_64-centos9-gcc12-optdeb-MT/CMake-setup.sh #set up environment for Geant4
export CXX=`which g++` #tell CMake about compiler used
export CC=`which gcc`

# ROOT library.
export LD_LIBRARY_PATH=$BOOST/lib:$LD_LIBRARY_PATH:$ROOT6/lib:$GEANT4/lib64:/usr/lib64/:/usr/lib

```

You can just add the previous commands in a setup_geant4.sh file and just use 
```
source setup_geant4.sh

```
when you start a new session

## Installing the sofwtware 

If is the first time that you are using it, you can just clone it by doing: 

```
git clone git@github.com:lmanzanillas/ULALAP.git

```
The go to the ulalap folder 

```
cd ULALAP
mkdir build 
cd build
cmake ../
make 

```
That should install the software. You can just run in the terminal: 
```
./ULALAP
```
That will show you the current geomtry. To run an event just do: 
```
/run/beamOn 1
```
Then you can use a macro in batch mode (for example test.mac) if you are running thousands of events. To change the settings as energy of photons, degree of polarization or angle of polarization, etc just modify the test.mac file  

```
#just in case, it shoud be true by default

#Uncomment this line to set the output directory, by default you will save the file in the same build directory

/run/beamOn 9000000


```

- Then you can run it from your build file 


```
./ULALAP -m test.mac

```

This will create a .csv file or other format depending on what was your choice where it's stored the hits information position of interaction, number of event, energy ...
You can use the code of your choice to analyze the data  

