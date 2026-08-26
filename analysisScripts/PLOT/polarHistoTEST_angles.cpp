#include "TFile.h"
#include "TTree.h"
#include "TH2Poly.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLatex.h"
#include "TLine.h"
#include "TEllipse.h"
#include "TVirtualPad.h"
#include "TPaletteAxis.h"
#include "../../include/polarHisto.h"
 
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
 
const char* ZENITH_BRANCH   = "zenith";
const char* AZIMUTH_BRANCH  = "azimuth";
const char* ENERGY_BRANCH   = "electronEnergy";   
 
// Energy window for the "B" numerator (envelope, restricted energy)
const double ENERGY_CUT_LO = 600.0;
const double ENERGY_CUT_HI = 800.0;
 
// Number of azimuth wedges and zenith rings.
const int N_AZ_BINS  = 200;
const int N_ZEN_BINS = 100;
 
// Points per arc segment when building wedge polygons.
// Higher = smoother arcs.
const int ARC_STEPS = 32;
 
void polarHistoTEST_angles_SOURCE_PLACEHOLDER(const char* filename)
{
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	gStyle->SetPalette(kBird);
	 
	TFile* f = TFile::Open(filename, "READ");
	 
	if (!f || f->IsZombie())
	{
		std::cerr << "[ERROR] Cannot open " << filename << std::endl;
		return;
	}
	 
	    // ----------------------------------------------------------------
	    // Part 1: ORIGINAL raw-count plots, unchanged output.
	    // ----------------------------------------------------------------
	 
	TCanvas* c = new TCanvas("c_both_trees", "Hemisphere Plots (2 trees)", 2200, 2200);
	 
	c->Divide(2, 2);
	 
	TH2Poly *envLeft = nullptr, *envRight = nullptr;
	TH2Poly *noenvLeft = nullptr, *noenvRight = nullptr;
	 
	bool okEnv = DrawTreeRow(f, "envelope", "Source SOURCE_PLACEHOLDER (Env., ENERGY_PLACEHOLDER)", "env", c->GetPad(1), c->GetPad(2), envLeft, envRight);
	 
	bool okNoEnv = DrawTreeRow(f, "noEnvelope", "Source SOURCE_PLACEHOLDER (No env., ENERGY_PLACEHOLDER)", "noenv", c->GetPad(3), c->GetPad(4), noenvLeft, noenvRight);
	 
	if (!okEnv && !okNoEnv)
	{
		std::cerr << "[ERROR] Neither tree could be processed. Aborting." << std::endl;
		f->Close();
		return;
	}
	 
	c->Update();
	c->SaveAs("polarHisto_Source_SOURCE_PLACEHOLDER.png");
 
    // ----------------------------------------------------------------
    // Part 2: RATIO plots, saved to a separate file.
    //
    //   Ratio1 = envelope(all E)              / [envelope + noEnvelope](all E)
    //   Ratio2 = envelope(E in [600,800])      / [envelope + noEnvelope](all E)
    //
    // Requires both trees to have been read successfully.
    // ----------------------------------------------------------------
 
	if (okEnv && okNoEnv)
	{
		TTree* envTree = nullptr;
		f->GetObject("envelope", envTree);
 
		if (!envTree)
		{
		    std::cerr << "[ERROR] Could not re-fetch 'envelope' tree for energy-cut fill." << std::endl;
		    f->Close();
		    return;
		}
 
		// Numerator B: envelope, restricted energy window.
		TH2Poly* bLeft  = FillHemispherePoly(envTree, false, "env_ecut_left",  true,  true, ENERGY_CUT_LO, ENERGY_CUT_HI);
		TH2Poly* bRight = FillHemispherePoly(envTree, true,  "env_ecut_right", false, true, ENERGY_CUT_LO, ENERGY_CUT_HI);
	 
		// Denominator D: envelope(all E) + noEnvelope(all E).
		TH2Poly* dLeft  = SumPolys(envLeft,  noenvLeft,  "denom_left");
		TH2Poly* dRight = SumPolys(envRight, noenvRight, "denom_right");
	 
		// Ratio 1: envelope(all E) / D
		TH2Poly* ratio1Left  = DivideHemispheres(envLeft,  dLeft,  "ratio1_left");
		TH2Poly* ratio1Right = DivideHemispheres(envRight, dRight, "ratio1_right");
	 
		// Ratio 2: envelope(600-800) / D
		TH2Poly* ratio2Left  = DivideHemispheres(bLeft,  dLeft,  "ratio2_left");
		TH2Poly* ratio2Right = DivideHemispheres(bRight, dRight, "ratio2_right");
 
		TCanvas* cRatio = new TCanvas("c_ratios", "Hemisphere Ratio Plots", 2200, 2200);
	 
		cRatio->Divide(2, 2);
	 
		std::string r1LeftTitle  = "Env (all E) / (Env+NoEnv): Italian Side.";
		std::string r1RightTitle = "Env (all E) / (Env+NoEnv): French Side.";
	 
		std::string r2LeftTitle  = Form("Env (%.0f-%.0f) / (Env+NoEnv): Italian Side.", ENERGY_CUT_LO, ENERGY_CUT_HI);
		std::string r2RightTitle = Form("Env (%.0f-%.0f) / (Env+NoEnv): French Side.", ENERGY_CUT_LO, ENERGY_CUT_HI);
	 
		StyleAndDrawPoly(ratio1Left,  r1LeftTitle.c_str(),  cRatio->GetPad(1), false, true);
		StyleAndDrawPoly(ratio1Right, r1RightTitle.c_str(), cRatio->GetPad(2), true,  false);
		StyleAndDrawPoly(ratio2Left,  r2LeftTitle.c_str(),  cRatio->GetPad(3), false, true);
		StyleAndDrawPoly(ratio2Right, r2RightTitle.c_str(), cRatio->GetPad(4), true,  false);
	 
		cRatio->Update();
		cRatio->SaveAs("polarHisto_Source_SOURCE_PLACEHOLDER_ratios.png");
	}
	
	else
	{
		std::cerr << "[WARN] Skipping ratio plots -- both 'envelope' and 'noEnvelope' " << "trees are required." << std::endl;
	}
 
	f->Close();
 
	std::cout << "[INFO] Done." << std::endl;
	
}

