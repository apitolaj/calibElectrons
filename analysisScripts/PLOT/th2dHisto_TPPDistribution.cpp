#include "/sps/nemo/scratch/apitolaj/Modules/MiModule/include/MiEvent.h"
#include "../../../include/detectorGeometry.h"

R__LOAD_LIBRARY(/sps/nemo/scratch/apitolaj/Modules/MiModule/lib/libMiModule.so);

void th2dHisto_TPPDistribution_Source_SOURCE_PLACEHOLDER(const char* file)
{
	// ---- USER SETTINGS ----
	double energyMin = 600.0;
	double energyMax = 800.0;

	const char* energyBranch = "electronEnergy";   
	const char* zenithBranch = "zenith";   

	const char* envTreeName   = "envelope";    
	const char* noEnvTreeName = "noEnvelope";  
	// ------------------------

	MiVector3D* calibSourceVertexPos_Source_SOURCE_PLACEHOLDER = new MiVector3D();

	if(sourcePositions.count("Source_SOURCE_PLACEHOLDER"))
	{
		auto [Y, Z] = sourcePositions.at("Source_SOURCE_PLACEHOLDER");
		calibSourceVertexPos_Source_SOURCE_PLACEHOLDER->setY(Y);
		calibSourceVertexPos_Source_SOURCE_PLACEHOLDER->setZ(Z);
	}

	TFile *f = new TFile(file);
	if (!f || f->IsZombie())
	{
		std::cerr << "Error: cannot open " << file << std::endl;
		return;
	}

	TTree *tEnv   = (TTree*)f->Get(envTreeName);
	TTree *tNoEnv = (TTree*)f->Get(noEnvTreeName);

	if (!tEnv)
	{
		std::cerr << "Error: cannot find TTree " << envTreeName << std::endl;
		f->Close();
		return;
	}
	if (!tNoEnv)
	{
		std::cerr << "Error: cannot find TTree " << noEnvTreeName << std::endl;
		f->Close();
		return;
	}

	TString energyLabel = Form("%g-%g keV", energyMin, energyMax);
	
	// Build the energy-range cut once, reuse across trees
	TString energyCut = Form("%s >= %f && %s < %f", energyBranch, energyMin, energyBranch, energyMax);

	// Zenith-only cuts (env split is now handled by which tree we draw from)
	TCut cutHighZen = Form("%s && %s>=60", energyCut.Data(), zenithBranch);
	TCut cutLowZen  = Form("%s && %s<60",  energyCut.Data(), zenithBranch);

	// Create canvas with 2x2 grid
	TCanvas *c = new TCanvas("c", "Comparison", 1600, 2000);
	
	gStyle->SetPadLeftMargin(0.15);   // more room for Y-axis title/labels
	gStyle->SetPadRightMargin(0.12);  // room for COLZ palette + its label
	gStyle->SetPadBottomMargin(0.12); // room for X-axis title if that's tight too
	gStyle->SetPadTopMargin(0.12);    // room for your 2-line splitline title
	
	c->Divide(2, 2);

	double YPos = calibSourceVertexPos_Source_SOURCE_PLACEHOLDER->getY();
	double ZPos = calibSourceVertexPos_Source_SOURCE_PLACEHOLDER->getZ();

	int YScaleWindow = 20;
	int ZScaleWindow = 50;

	int Ybins = 100;
	int Zbins = 250;

	TH2D *h1 = new TH2D("h1",
	Form("#splitline{  Source_SOURCE_PLACEHOLDER: TPP Distribution}{(Env., zenith >= 60, %s)};Y [mm];Z [mm]",
		energyLabel.Data()),
	Ybins, YPos-YScaleWindow, YPos+YScaleWindow,
	Zbins, ZPos-ZScaleWindow, ZPos+ZScaleWindow);
	
	c->cd(1);
	h1->SetStats(0);
	tEnv->Draw("TPPz:TPPy >> h1", cutHighZen, "COLZ");
	
	gPad->Update();
	TPaveText *pt1 = (TPaveText*)gPad->GetPrimitive("title");
	if (pt1) 
	{
		pt1->SetTextFont(63);
		pt1->SetTextSize(28);  
	}

	TH2D *h2 = new TH2D("h2",
	Form("#splitline{Source_SOURCE_PLACEHOLDER: TPP Distribution}{(Env., zenith < 60, %s)};Y [mm];Z [mm]",
		energyLabel.Data()),
	Ybins, YPos-YScaleWindow, YPos+YScaleWindow,
	Zbins, ZPos-ZScaleWindow, ZPos+ZScaleWindow);
	
	c->cd(2);
	h2->SetStats(0);
	tEnv->Draw("TPPz:TPPy >> h2", cutLowZen, "COLZ");
	
	gPad->Update();
	TPaveText *pt2 = (TPaveText*)gPad->GetPrimitive("title");
	if (pt2) 
	{
		pt2->SetTextFont(63);
		pt2->SetTextSize(28);  
	}

	TH2D *h3 = new TH2D("h3",
	Form("#splitline{Source_SOURCE_PLACEHOLDER: TPP Distribution}{(No env., zenith >= 60, %s)};Y [mm];Z [mm]",
		energyLabel.Data()),
	Ybins, YPos-YScaleWindow, YPos+YScaleWindow,
	Zbins, ZPos-ZScaleWindow, ZPos+ZScaleWindow);
	
	c->cd(3);
	h3->SetStats(0);
	tNoEnv->Draw("TPPz:TPPy >> h3", cutHighZen, "COLZ");
	
	gPad->Update();
	TPaveText *pt3 = (TPaveText*)gPad->GetPrimitive("title");
	if (pt3) 
	{
		pt3->SetTextFont(63);
		pt3->SetTextSize(28);  
	}

	TH2D *h4 = new TH2D("h4",
	Form("#splitline{Source_SOURCE_PLACEHOLDER: TPP Distribution}{(No env., zenith < 60, %s)};Y [mm];Z [mm]",
		energyLabel.Data()),
	Ybins, YPos-YScaleWindow, YPos+YScaleWindow,
	Zbins, ZPos-ZScaleWindow, ZPos+ZScaleWindow);
	
	c->cd(4);
	h4->SetStats(0);
	tNoEnv->Draw("TPPz:TPPy >> h4", cutLowZen, "COLZ");
	
	gPad->Update();
	TPaveText *pt4 = (TPaveText*)gPad->GetPrimitive("title");
	if (pt4) 
	{
		pt4->SetTextFont(63);
		pt4->SetTextSize(28);  
	}

	c->SaveAs("th2dHisto_TPPDistribution_Source_SOURCE_PLACEHOLDER.png");

	delete calibSourceVertexPos_Source_SOURCE_PLACEHOLDER;

	f->Close();
}
