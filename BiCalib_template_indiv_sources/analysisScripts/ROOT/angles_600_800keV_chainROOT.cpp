
//user must change this path to where their MiModule is located.
#include "/sps/nemo/scratch/apitolaj/Modules/MiModule/include/MiEvent.h"
#include "UTILS_PLACEHOLDER/functionUtils.h"
#include "TLatex.h"
#include "TVector3.h"
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TList.h"

//user must change this path to where their MiModule is located.
R__LOAD_LIBRARY(/sps/nemo/scratch/apitolaj/Modules/MiModule/lib/libMiModule.so);

void angles_600_800keV_chainROOT_Source_SOURCE_PLACEHOLDER()
{
	float azimuth;
	float zenith;
	int eventNumber;
	
	TChain chain("Event");
	populateChain(chain, "BASE_PLACEHOLDER/../SOURCES/Source_SOURCE_PLACEHOLDER/DATA/brios/SNCUTS/ROOT/1");

	MiEvent *Eve = new MiEvent();
	chain.SetBranchAddress("Eventdata", &Eve);
	
	TFile *outFile_envelope = new TFile("BASE_PLACEHOLDER/DST_PLACEHOLDER/angles_envelope_600_800keV_Source_SOURCE_PLACEHOLDER.root", "RECREATE");
	outFile_envelope->cd();
	TTree *tr_envelope = new TTree("azimuth_zenith", "contains the azimuth and zenith angles of fitted 3D direction of electrons that DID pass through the envelope");

	tr_envelope->Branch("azimuth", &azimuth, "azimuth/f");
	tr_envelope->Branch("zenith", &zenith, "zenith/f");
	tr_envelope->Branch("eventNum", &eventNumber, "eventNumber/I");
	
	TFile *outFile_noEnvelope = new TFile("BASE_PLACEHOLDER/DST_PLACEHOLDER/angles_noEnvelope_600_800keV_Source_SOURCE_PLACEHOLDER.root", "RECREATE");
	outFile_noEnvelope->cd();
	TTree *tr_noEnvelope = new TTree("azimuth_zenith", "contains the azimuth and zenith angles of fitted 3D direction of electrons that DID NOT pass through the envelope");

	tr_noEnvelope->Branch("azimuth", &azimuth, "azimuth/f");
	tr_noEnvelope->Branch("zenith", &zenith, "zenith/f");
	tr_noEnvelope->Branch("eventNum", &eventNumber, "eventNumber/I");
	
	int energyCount =0;

	for (int i = 0; i < chain.GetEntries(); i++)
	{
		chain.GetEntry(i);
		
		
		if(isEnergyBetween600_800keV(Eve))
		{	
		
			energyCount++;
			
			if (isCalibElectron_envelope(Eve))
			{
			    eventNumber = i;
			    zenith = calculateZenith(Eve);
			    azimuth = calculateAzimuth(Eve);
			    outFile_envelope->cd();
			    tr_envelope->Fill();
			}
			else if (isCalibElectron_noEnvelope(Eve))
			{
			    eventNumber = i;
			    zenith = calculateZenith(Eve);
			    azimuth = calculateAzimuth(Eve);
			    std::cout << "Event " << eventNumber << " : no envelope" << std::endl;
			    outFile_noEnvelope->cd();
			    tr_noEnvelope->Fill();
			}
		}
	}
	
	int envelopeCount   = tr_envelope->GetEntries();
	int noEnvelopeCount = tr_noEnvelope->GetEntries();
	int totalCount      = chain.GetEntries();

	outFile_envelope->cd();
	tr_envelope->Write("", TObject::kOverwrite);
	delete outFile_envelope;   
	outFile_envelope = nullptr;
	tr_envelope      = nullptr; 

	outFile_noEnvelope->cd();
	tr_noEnvelope->Write("", TObject::kOverwrite);
	delete outFile_noEnvelope; 
	outFile_noEnvelope = nullptr;
	tr_noEnvelope      = nullptr;

    	std::ofstream outFile("BASE_PLACEHOLDER/DST_PLACEHOLDER/angles_entryCounts_600_800keV_Source_SOURCE_PLACEHOLDER.txt");
	outFile << "Entry count (600-800 keV)" << std::endl;
	outFile << "Envelope: "    << envelopeCount   << std::endl;
	outFile << "No Envelope: " << noEnvelopeCount << std::endl;
	outFile << "Total: "       << totalCount      << std::endl;
	outFile.close();
}























