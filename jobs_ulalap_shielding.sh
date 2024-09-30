#!/bin/sh
det_type=0 
thick=75
thick_waffle=100
material_shield_waffle=G4_AIR
material_shielding=G4_WATER
for job_number in {1..20..1} 
do
	echo ${job_number}
        cd /pbs/home/l/lmanzani/ulalap_g4/build/
	FILE_MAC="n_mac_${job_number}_waffle_${material_shield_waffle}_${thick_waffle}_shield_${thick}_${material_shielding}.mac"
	/bin/cat <<EOM >$FILE_MAC

/ULALAP/det/setOutputDirectory /sps/lbno/lmanzani/ulalap_output/
/ULALAP/det/setSetupName file_${job_number}
/ULALAP/det/setDetectorType ${det_type}
/ULALAP/det/setShieldingMaterialWaffle ${material_shield_waffle}
#/ULALAP/det/setWaffleThickness ${thick_waffle} cm
/ULALAP/det/setShieldingMaterial ${material_shielding}
/ULALAP/det/setshieldingThickness ${thick} cm
#select output format, options are: csv root hdf5
#ULALAP/det/setDataType csv
/process/had/particle_hp/use_photo_evaporation true
/process/had/particle_hp/do_not_adjust_final_state false
/process/had/particle_hp/skip_missing_isotopes true
#direction type, if 1 isotropic
/ULALAP/gun/sourceDirectionType 0
#direction of the source
/ULALAP/gun/direction -1 0 0
#position of the center of the source in cm for x y z
/ULALAP/gun/position 3400 0 0
#/ULALAP/gun/sourceEnergy 0.023 eV
#choise of the source: 0 for gamma, 1 for Fe55, 2 for Cs137, 3 for Bi207, 4 for Sr90, 5 for Am241, 6 for electron, 7 for optical_photon, 8 for neutron
/ULALAP/gun/sourceType 11
#geometry of the source: Sphere Cone Box point
/ULALAP/gun/shape Box
#size of the box in cm for x y z
/ULALAP/gun/BoxXYZ 1 1500 1400
#number of events
/run/beamOn 500000


EOM
	FILEJOB="job_muon_${job_number}_${material_shield_waffle}_${thick_waffle}_shield_${thick}_${material_shielding}.sh"
	
        /bin/cat <<EOM >$FILEJOB
#!/bin/bash
#! /bin/bash

#SBATCH --job-name=multicpu_jobs       # Job name
#SBATCH --output=multicpu_jobs_%j.log  # Standard output and error log

#SBATCH --partition=htc                # Partition choice (htc by default)

#SBATCH --ntasks=1                     # Run up to two tasks
#SBATCH --time=3:00:00              # Max time limit = 7 days

#SBATCH --mem-per-cpu=1000             # Allocate 1G of memeory per core
#SBATCH --cpus-per-task=2              # Allocate 4 cores per task, i.e 8 cores in total in this example (ntasks x cpus-per-task)

#SBATCH --mail-user=manzanilla@lapp.in2p3.fr    # Where to send mail
#SBATCH --mail-type=NONE          # Mail events (NONE, BEGIN, END, FAIL, ALL)

#SBATCH --licenses=sps                 # Declaration of storage and/or software resources

source /pbs/home/l/lmanzani/setup_ulalap.sh
cd /pbs/home/l/lmanzani/ulalap_g4/build/
./ULALAP -m ${FILE_MAC} -r ${job_number} 
exit 0
EOM
sbatch $FILEJOB

done

