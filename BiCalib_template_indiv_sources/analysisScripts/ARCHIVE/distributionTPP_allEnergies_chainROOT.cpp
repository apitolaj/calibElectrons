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

R__LOAD_LIBRARY(/sps/nemo/scratch/apitolaj/Modules/MiModule/lib/libMiModule.so);

void distributionTPP_allEnergies_chainROOT_Source_SOURCE_PLACEHOLDER()
{
	int eventNumber;
			
	float zenith;
	float TPPy;
	float TPPz;
	
	TChain chain("Event");
	populateChain(chain, "BASE_PLACEHOLDER/../SOURCES/Source_SOURCE_PLACEHOLDER/DATA/ROOTFiles");
	
	MiEvent *Eve = new MiEvent();
	chain.SetBranchAddress("Eventdata", &Eve);

	TFile *outFile_envelope_g60 = new TFile("BASE_PLACEHOLDER/DST_PLACEHOLDER/distributionTPP_envelope_allEnergies_zenithg60_Source_SOURCE_PLACEHOLDER.root", "RECREATE");
	outFile_envelope_g60->cd();
	TTree *tr_envelope_g60 = new TTree("TPPy_TPPz", "WRITE");

	tr_envelope_g60->Branch("TPPy", &TPPy, "TPPy/f");
	tr_envelope_g60->Branch("TPPz", &TPPz, "TPPz/f");
	tr_envelope_g60->Branch("eventNumber", &eventNumber, "eventNumber/I");
	
	TFile *outFile_envelope_l60 = new TFile("BASE_PLACEHOLDER/DST_PLACEHOLDER/distributionTPP_envelope_allEnergies_zenithl60_Source_SOURCE_PLACEHOLDER.root", "RECREATE");
	outFile_envelope_l60->cd();
	TTree *tr_envelope_l60 = new TTree("TPPy_TPPz", "WRITE");

	tr_envelope_l60->Branch("TPPy", &TPPy, "TPPy/f");
	tr_envelope_l60->Branch("TPPz", &TPPz, "TPPz/f");
	tr_envelope_l60->Branch("eventNumber", &eventNumber, "eventNumber/I");
	
	TFile *outFile_noEnvelope_g60 = new TFile("BASE_PLACEHOLDER/DST_PLACEHOLDER/distributionTPP_noEnvelope_allEnergies_zenithg60_Source_SOURCE_PLACEHOLDER.root", "RECREATE");
	outFile_noEnvelope_g60->cd();
	TTree *tr_noEnvelope_g60 = new TTree("TPPy_TPPz", "WRITE");

	tr_noEnvelope_g60->Branch("TPPy", &TPPy, "TPPy/f");
	tr_noEnvelope_g60->Branch("TPPz", &TPPz, "TPPz/f");
	tr_noEnvelope_g60->Branch("eventNumber", &eventNumber, "eventNumber/I");
	
	TFile *outFile_noEnvelope_l60 = new TFile("BASE_PLACEHOLDER/DST_PLACEHOLDER/distributionTPP_noEnvelope_allEnergies_zenithl60_Source_SOURCE_PLACEHOLDER.root", "RECREATE");
	outFile_noEnvelope_l60->cd();
	TTree *tr_noEnvelope_l60 = new TTree("TPPy_TPPz", "WRITE");

	tr_noEnvelope_l60->Branch("TPPy", &TPPy, "TPPy/f");
	tr_noEnvelope_l60->Branch("TPPz", &TPPz, "TPPz/f");
	tr_noEnvelope_l60->Branch("eventNumber", &eventNumber, "eventNumber/I");
	
	for (int i = 0; i < chain.GetEntries(); i++)
	{
		chain.GetEntry(i);
		eventNumber = i;
		
		if (isCalibElectron_envelope(Eve))
		{
		
			zenith = calculateZenith(Eve);
			MiVector3D* posVector = calculatePosTPPVector(Eve);
			TPPy = posVector->getY();
			TPPz = posVector->getZ();
			
			if (zenith >= 60) 
			{
			        outFile_envelope_g60->cd();
				tr_envelope_g60->Fill();
			}
			
			else  
			{
			        outFile_envelope_l60->cd();  
				tr_envelope_l60->Fill();
			}
			
		}
		else if (isCalibElectron_noEnvelope(Eve))
		{	
		
			zenith = calculateZenith(Eve);
			MiVector3D* posVector = calculatePosTPPVector(Eve);
			TPPy = posVector->getY();
			TPPz = posVector->getZ();
			
			if (zenith >= 60) 
			{
			        outFile_noEnvelope_g60->cd();
				tr_noEnvelope_g60->Fill();
			}
			
			else 
			{
			        outFile_noEnvelope_l60->cd();
				tr_noEnvelope_l60->Fill();
			}
		}

	}
	
	int count_envelope_g60   = tr_envelope_g60->GetEntries();
	int count_envelope_l60   = tr_envelope_l60->GetEntries();
	int count_noEnvelope_g60 = tr_noEnvelope_g60->GetEntries();
	int count_noEnvelope_l60 = tr_noEnvelope_l60->GetEntries();
	int totalCount           = chain.GetEntries();

	outFile_envelope_g60->cd();
	tr_envelope_g60->Write("", TObject::kOverwrite);
	delete outFile_envelope_g60;
	outFile_envelope_g60 = nullptr;
	tr_envelope_g60      = nullptr;

	outFile_envelope_l60->cd();
	tr_envelope_l60->Write("", TObject::kOverwrite);
	delete outFile_envelope_l60;
	outFile_envelope_l60 = nullptr;
	tr_envelope_l60      = nullptr;

	outFile_noEnvelope_g60->cd();
	tr_noEnvelope_g60->Write("", TObject::kOverwrite);
	delete outFile_noEnvelope_g60;
	outFile_noEnvelope_g60 = nullptr;
	tr_noEnvelope_g60      = nullptr;

	outFile_noEnvelope_l60->cd();
	tr_noEnvelope_l60->Write("", TObject::kOverwrite);
	delete outFile_noEnvelope_l60;
	outFile_noEnvelope_l60 = nullptr;
	tr_noEnvelope_l60      = nullptr;


	std::ofstream outFile("BASE_PLACEHOLDER/DST_PLACEHOLDER/distributionTPP_entryCounts_allEnergies_Source_SOURCE_PLACEHOLDER.txt");

	outFile << "Entry count (all energies)" << std::endl;
	outFile << "Envelope (zenith >= 60): "    << count_envelope_g60   << std::endl;
	outFile << "Envelope  (zenith < 60): "    << count_envelope_l60   << std::endl;
	outFile << "No Envelope  (zenith >= 60): "<< count_noEnvelope_g60 << std::endl;
	outFile << "No Envelope  (zenith < 60): " << count_noEnvelope_l60 << std::endl;
	outFile << "Total: "                       << totalCount           << std::endl;

	outFile.close();
}






































