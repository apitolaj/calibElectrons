void th2dHisto_zenithDistOM_Source_SOURCE_PLACEHOLDER(const char* file)
{
	// ---- USER SETTINGS ----
	double energyMin = 600.0;
	double energyMax = 800.0;
	const char* energyBranch = "electronEnergy";   
	
	// ------------------------

	TFile *f = new TFile(file);
	if (!f || f->IsZombie()) 
	{
		std::cerr << "Error: cannot open " << file << std::endl;
		return;
	}
	    
	TTree *tEnv = (TTree*)f->Get("envelope");
	TTree *tNoEnv = (TTree*)f->Get("noEnvelope");

	if (!tEnv)
	{
		std::cerr << "Error: cannot find TTree" << std::endl;
		f->Close();
		return;
	}
	if (!tNoEnv)
	{
		std::cerr << "Error: cannot find TTree" << std::endl;
		f->Close();
		return;
	}

	TString energyLabel = Form("%g-%g keV", energyMin, energyMax);
	TCut energyCut = Form("%s >= %f && %s < %f", energyBranch, energyMin, energyBranch, energyMax);

	TCanvas *c = new TCanvas("c", "Comparison", 800, 1000);
	c->Divide(1, 2);

	// --- top ---
	c->cd(1);
	TH2D *h1 = new TH2D("h1", Form("Source_SOURCE_PLACEHOLDER: OM Vertex Dist. to Cali. Source vs. Zenith Angle (Env, %s);Zenith Angle [deg];Distance [mm]", energyLabel.Data()),
		                 180, 0, 180, 100, 0, 2000);
	    
	h1->SetStats(0);
	tEnv->Draw("DistOM:zenith >> h1", "", "COLZ");
	
	gPad->Update();
	TPaveText *pt1 = (TPaveText*)gPad->GetPrimitive("title");
	if (pt1) 
	{
		pt1->SetTextFont(63);
		pt1->SetTextSize(16);  
	}

	// --- bottom ---
	c->cd(2);
	TH2D *h2 = new TH2D("h2", Form("Source_SOURCE_PLACEHOLDER: OM Vertex Dist. to Cali. Source vs. Zenith Angle (No env, %s);Zenith Angle [deg];Distance [mm]", energyLabel.Data()),
		                 180, 0, 180, 100, 0, 2000);

	h2->SetStats(0);
	tNoEnv->Draw("DistOM:zenith >> h2", "", "COLZ");
	
	gPad->Update();
	TPaveText *pt2 = (TPaveText*)gPad->GetPrimitive("title");
	if (pt2) 
	{
		pt2->SetTextFont(63);
		pt2->SetTextSize(16);  
	}

	c->SaveAs("th2dHisto_zenithDistOM_Source_SOURCE_PLACEHOLDER.png");
	
	f->Close();
}


