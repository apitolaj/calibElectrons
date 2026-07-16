//user must change this path to where their MiModule is located.
#include "/sps/nemo/scratch/apitolaj/Modules/MiModule/include/MiEvent.h"
#include "UTILS_PLACEHOLDER/functionUtils.h"
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

        if(isEnergyinRange(Eve, minEnergy, maxEnergy)) 
        {
            dataTree.zenith  = calculateZenith(Eve);
            dataTree.azimuth = calculateAzimuth(Eve);
            dataTree.DistOM  = calculateDistOM(Eve, calibSourceVertexPos_Source_SOURCE_PLACEHOLDER);
            dataTree.DistTPP = calculateDistTPP(Eve, calibSourceVertexPos_Source_SOURCE_PLACEHOLDER);
            dataTree.TPPy    = calculatePosTPPVector(Eve)->getY();
            dataTree.TPPz    = calculatePosTPPVector(Eve)->getZ();

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
