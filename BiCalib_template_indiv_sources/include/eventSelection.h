#pragma once

bool envelopeInteraction(MiEvent *Event)
{
	bool didInteractEnvelope= false;
	
	MiSD *sd = Event->getSD();
    
	if (sd)
        {
        	std::vector<MiSDCaloHit> *ch = sd->getcalohitv();

        	if (ch)
        	{
            		for (auto &hit : *ch)
            		{            				
            			if(hit.getGID()->gettype() == "1066")
            			{
            				didInteractEnvelope=true;
            			}
            			
            		}
        	}
        }
        
        return didInteractEnvelope;
}

bool isEnergyinRange(double inputtedEnergy, int minEnergy, int maxEnergy)
{

	if(minEnergy < inputtedEnergy && inputtedEnergy < maxEnergy)
	{
		return true;
	}
	
	return false;
}

double calculateElectronEnergy(vector<MiCDCaloHit>* caloHits)
{
	double energy;
	
	if(caloHits)
	{
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
