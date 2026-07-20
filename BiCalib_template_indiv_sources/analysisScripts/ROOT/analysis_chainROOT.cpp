//user must change this path to where their MiModule is located.
#include "/sps/nemo/scratch/apitolaj/Modules/MiModule/include/MiEvent.h"
//#include "UTILS_PLACEHOLDER/functionUtils.h"

#include "../../../include/detectorGeometry.h"
#include "../../../include/eventSelection.h"
#include "../../../include/geometricCalculations.h"
#include "../../../include/treeData.h"
#include "../../../include/utils.h"

#include "TLatex.h"
#include "TVector3.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TList.h"

//user must change this path to where their MiModule is located.
R__LOAD_LIBRARY(/sps/nemo/scratch/apitolaj/Modules/MiModule/lib/libMiModule.so);

void analysis_chainROOT_Source_SOURCE_PLACEHOLDER(int minEnergy, int maxEnergy)
{

    int envelopeCount = 0;
    int noEnvelopeCount = 0;
    int totalCount = 0;

    MiVector3D* calibSourceVertexPos_Source_SOURCE_PLACEHOLDER = new MiVector3D();
	
	if(sourcePositions.count("Source_SOURCE_PLACEHOLDER"))
	{
		auto [Y, Z] = sourcePositions.at("Source_SOURCE_PLACEHOLDER");
		calibSourceVertexPos_Source_SOURCE_PLACEHOLDER->setY(Y);
		calibSourceVertexPos_Source_SOURCE_PLACEHOLDER->setZ(Z);
	}

	TChain chain("Event");
	populateChain(chain, "BASE_PLACEHOLDER/../SOURCES/Source_SOURCE_PLACEHOLDER/DATA/ROOTFiles");

	MiEvent *Eve = new MiEvent();
	chain.SetBranchAddress("Eventdata", &Eve);
	
	TFile *outFileROOT = new TFile("BASE_PLACEHOLDER/DST_PLACEHOLDER/analysisROOT.root", "RECREATE");

	treeData dataTree;
	makeTrees(outFileROOT, dataTree);

	for (int i = 0; i < chain.GetEntries(); i++)
	{
    
		dataTree.eventNumber = i;
		chain.GetEntry(i);
		
		MiCDParticle* particle = Eve->getPTD()->getpart(0);
		vector<MiVertex>* vertices = particle->getvertexv();
		vector<MiCDCaloHit>* caloHits = particle->getcalohitv();
		
		TVector3 dir = particle->getdirectionfromfoil();
		
		MiVector3D* omPos = nullptr;
		MiVector3D* tppPos = nullptr;

		for (MiVertex& vertex : *vertices)
		{
		    const auto& pos = vertex.getpos();

		    if (pos == "calo" || pos == "gveto" || pos == "xcalo")
			omPos = vertex.getr();

		    else if (pos == "reference source plane")
			tppPos = vertex.getr();
		}
		

		if(isEnergyinRange(calculateElectronEnergy(caloHits), minEnergy, maxEnergy)) 
		{
		
		    dataTree.zenith  		= calculateZenith(&dir);
		    dataTree.azimuth 		= calculateAzimuth(&dir);
		    dataTree.DistOM  		= calculateDistOM(omPos, calibSourceVertexPos_Source_SOURCE_PLACEHOLDER);
		    dataTree.DistTPP 		= calculateDistTPP(tppPos, calibSourceVertexPos_Source_SOURCE_PLACEHOLDER);
		    dataTree.TPPy   		= tppPos->getY();
		    dataTree.TPPz   		= tppPos->getZ();
		    dataTree.isEdgeTPP		= isEdgeTPP(tppPos);	
		    dataTree.electronEnergy 	= calculateElectronEnergy(caloHits);
		
		    if (envelopeInteraction(Eve))
		    {
		        dataTree.envelope->Fill();
		        envelopeCount++;
		    }

		    else
		    {
		        dataTree.noEnvelope->Fill();
		        noEnvelopeCount++;
		    }

		    	totalCount++;
		}
    }

        delete calibSourceVertexPos_Source_SOURCE_PLACEHOLDER;

        dataTree.envelope->Write("", TObject::kOverwrite);
        dataTree.noEnvelope->Write("", TObject::kOverwrite);
        dataTree.envelope->Delete();
        dataTree.noEnvelope->Delete();
        delete outFileROOT;
           
        std::ofstream outFileText("BASE_PLACEHOLDER/DST_PLACEHOLDER/angles_entryCounts_allEnergies_Source_SOURCE_PLACEHOLDER.txt");
        outFileText << "Entry count"       << std::endl;
        outFileText << "Envelope: "    << envelopeCount   << std::endl;
        outFileText << "No Envelope: " << noEnvelopeCount << std::endl;
        outFileText << "Total: "       << totalCount      << std::endl;
        outFileText.close();
}
