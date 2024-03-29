#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"

#include "G4SteppingManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fOneStepPrimaries(false), fEventAction(eventAction)
{

	fExpectedNextStatus = Undefined;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step * theStep)
{
	//auto analysisManager = G4AnalysisManager::Instance();
	G4Track* theTrack = theStep->GetTrack();
  	G4ParticleDefinition* particleType = theTrack->GetDefinition();
	//G4cout << "Particle ID: " << particleType->GetParticleName() << " parent ID: "<< theTrack->GetParentID() << G4endl;
	fExpectedNextStatus = Undefined;

	G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
	G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();
	//G4TouchableHistory* theTouchable = (G4TouchableHistory*)(thePrePoint->GetTouchable());
	//step->GetTrack()->GetParentID();
	//G4StepStatus PreStepStatus = thePrePoint->GetStepStatus();
       

        G4String process_name = theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	G4int StepNo = theTrack->GetCurrentStepNumber();
	G4int trackIDCapture = -1;

        G4double edepStep = theStep->GetTotalEnergyDeposit()/keV;


  	if(!process_name.compare("nCapture")){
     		G4double time_finish = theStep->GetPostStepPoint()->GetGlobalTime();
		fEventAction->SetTimeCapture(time_finish/us);
		fEventAction->SetTimeDecay(0.);
     		G4ThreeVector position_finish = theStep->GetPostStepPoint()->GetPosition();
		fEventAction->SetVertex(position_finish);
 		//G4cout <<"trackID capture "<<trackIDCapture <<" time_finish: " << time_finish/us << " position x y z: "<< position_finish <<" capture on step: "<<StepNo<< G4endl;
 		//G4cout << "Particle ID: " << particleType->GetParticleName()<< " Energy "<< partEnergy << " PDG code: "<< theTrack->GetParticleDefinition()->GetPDGEncoding() << G4endl;
    	}
	

       if(process_name == "RadioactiveDecay" )
       {
	        G4double partEnergy = theStep->GetPreStepPoint()->GetKineticEnergy()/keV;
     		G4double time_finish = theStep->GetPostStepPoint()->GetGlobalTime();
     		G4ThreeVector position_finish = theStep->GetPostStepPoint()->GetPosition();
		fEventAction->SetVertex(position_finish);
	        fEventAction->SetTimeDecay(time_finish/us);
	        fEventAction->SetTimeCapture(0.);
 		//G4cout << " step: "<<StepNo<<" process_name: " << process_name << G4endl;
       }	       

	//
	// Retrieve the secondary particles
	G4SteppingManager*  steppingManager = fpSteppingManager;
	G4TrackVector* fSecondary = steppingManager -> GetfSecondary();
   	std::vector<G4double> gamma_vector;  
   	for(size_t lp1=0;lp1<(*fSecondary).size(); lp1++)
  	{ 
     		// Retrieve particle
     		const G4ParticleDefinition* particleName = (*fSecondary)[lp1] -> GetDefinition();     

		if (particleName == G4Gamma::Definition())
    		{
         		G4String process = (*fSecondary)[lp1]-> GetCreatorProcess()-> GetProcessName();  
     	 		G4double time_finish = theStep->GetPostStepPoint()->GetGlobalTime();
      
         		//Retrieve the process originating it
		        //G4cout << "creator process " << process << G4endl;
         		if (process == "RadioactiveDecay" || process == "nCapture")
        		{
           			G4double energy = (*fSecondary)[lp1]  -> GetKineticEnergy();   
		   		gamma_vector.push_back(energy/keV);
	   			//G4cout<<"E gamma: "<<energy/keV<<" time: "<<time_finish/us<<G4endl;
         		}
      		}
  	 } 
   	if(size(gamma_vector) > 0)fEventAction->AddInfoCaptureGammas(gamma_vector);
	//
	if ( thePrePV->GetName()=="target_1"){
		G4double time_dep = theStep->GetPostStepPoint()->GetGlobalTime()/us;
                fEventAction->AddInfo(thePrePoint->GetPosition().x()/cm,thePrePoint->GetPosition().y()/cm,thePrePoint->GetPosition().z()/cm,edepStep,time_dep);
  	}

	G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
	G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();


  	if(!thePostPV)
  	{//out of world
  		fExpectedNextStatus=Undefined;
  		return;
  	}

}
