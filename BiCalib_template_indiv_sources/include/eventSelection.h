#pragma once

//function to check if the envelope interacted with the copper envelope. 
bool envelopeInteraction(MiEvent *Event)
{
	//default boolean variable that will only be changed to true if there is a copper interaction.
	bool didInteractEnvelope= false;
	
	MiSD *sd = Event->getSD();
    
	if (sd)
        {
        	std::vector<MiSDCaloHit> *ch = sd->getcalohitv();

        	if (ch)
        	{
					//if there is a single copper hit in the SD bank, change the boolean to true.
            		for (auto &hit : *ch)
            		{            		
						//GIDID=1066 indicated copper envelope interaction.		
            			if(hit.getGID()->gettype() == "1066")
            			{
            				didInteractEnvelope=true;
            			}
            			
            		}
        	}
        }
        
        return didInteractEnvelope;
}

//function to do a simple check if the inputted electron energy is in a given range. 
bool isEnergyinRange(double inputtedEnergy, int minEnergy, int maxEnergy)
{
	if(minEnergy < inputtedEnergy && inputtedEnergy < maxEnergy)
	{
		return true;
	}
	
	return false;
}

//function to calculate an electron's energy.
double calculateElectronEnergy(vector<MiCDCaloHit>* caloHits)
{
	double energy;
	
	if(caloHits)
	{
		//sum all the electron calorimeter hits in the CD bank. 
		for(MiCDCaloHit& hit : *caloHits)
		{
		
			energy=+hit.getE();
		
		}
	
	}
	else
	{
		std::cerr<<"ERROR: no calo hits" <<std::endl;
		return -1;
	}

	return energy;
}
