#!/bin/sh
for thick in {1..1..1}
do
        echo $thick
	for job in {1..5..1}
	do
		for energy in {1..10..1} 
		do
			 FILE="run_E_${energy}_collimator_thickness_${thick}mm_job_${job}.mac"

			 /bin/cat <<EOM >$FILE

/ULALAP/det/setOutputDirectory /nfs/tegile/work/experiences/detecteurs/manzanillas/LEAPS_INNOV/Geant4_output/
#select detector type: options are 0: detector without collimator and direct beam, 1 detector with collimator and direct beam, 2 and 3 same as 0 and 1 but with sample and beam on sample
/ULALAP/det/setDetectorType 0
/ULALAP/det/setSetupName run_${job}
#select output format, options are: csv root hdf5
/ULALAP/det/setDataType csv
#select material of the collimator, recomended options: titanium_grade1, titanium_grade2, tungsten_alloy, G4_BRASS, G4_Pb
#Set the position of the collimator, a values between -37 and 37 mm (size of samples)
#Choise of source type: 0 gamma,1 Fe-55,2 Cs-137; 1 Bi-207; 2 Sr-90; 3 e-
/run/initialize
/ULALAP/gun/sourceType 0
/ULALAP/gun/sourceDiameter 16.0 mm
/ULALAP/gun/sourceEnergy ${energy}.0  keV

/run/beamOn 500000
EOM
		FILEJOB="job_E_${energy}_${thick}mm_job_${job}.sh"

                /bin/cat <<EOM >$FILEJOB
#!/bin/bash
#SBATCH -n 1
#SBATCH --qos=parallel
#SBATCH --partition=sumo
#SBATCH --time=2:00:00
#SBATCH --cpus-per-task=2
#
source ~/environment_GeSOS.sh
./ULALAP -m run_E_${energy}_collimator_thickness_${thick}mm_job_${job}.mac 
exit 0
EOM
sbatch $FILEJOB
		done

	done
done

































