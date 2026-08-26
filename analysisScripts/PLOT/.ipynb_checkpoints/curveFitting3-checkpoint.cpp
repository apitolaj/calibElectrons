#include "../../include/curveFitting.h"
 
//normalization function
void Normalize(TH1 *h)
{
	if (!h) return;
	
	double integral = h->Integral();
	
	if (integral > 0) h->Scale(1.0 / integral);
}
 
//---------------------------------------------------------------------------
// Build a "basic" residual histogram: residual = data - fit, evaluated only
// over [xmin,xmax] (the fit range). Bins outside that range are left at 0.
// This is NOT a pull plot -- no division by the error is performed.
//---------------------------------------------------------------------------
TH1F* MakeResidualHist(const char *name, TH1F *data, TF1 *fitFunc, double xmin, double xmax)
{
	TH1F *hRes = (TH1F*)data->Clone(name);
	hRes->Reset();
	hRes->SetTitle(";Energy [keV];Data - Fit");
 
	for (int i = 1; i <= data->GetNbinsX(); ++i)
	{
		double x = data->GetBinCenter(i);
		if (x < xmin || x > xmax) continue;
 
		double res = data->GetBinContent(i) - fitFunc->Eval(x);
		hRes->SetBinContent(i, res);
		hRes->SetBinError(i, data->GetBinError(i));
	}
 
	return hRes;
}
 
//---------------------------------------------------------------------------
// Cosmetic setup for a main (top) pad and its residual (bottom) pad, mimicking
// the look of TRatioPlot but for a plain difference plot.
//---------------------------------------------------------------------------
void StyleMainAndResidualPads(TPad *padMain, TPad *padRes)
{
	padMain->SetBottomMargin(0.02);
	padMain->SetLeftMargin(0.12);
 
	padRes->SetTopMargin(0.03);
	padRes->SetBottomMargin(0.35);
	padRes->SetLeftMargin(0.12);
}
 
//---------------------------------------------------------------------------
// Center the residual histogram's y-axis on 0, with a symmetric range that
// comfortably contains all the data (content +/- error). A padding factor
// is applied so points near the edge aren't squished against the frame.
//---------------------------------------------------------------------------
void CenterResidualAxisOnZero(TH1F *hRes, double paddingFactor = 1.3)
{
	double maxDev = 0.0;
 
	for (int i = 1; i <= hRes->GetNbinsX(); ++i)
	{
		double content = hRes->GetBinContent(i);
		double error   = hRes->GetBinError(i);
		if (content == 0.0 && error == 0.0) continue; // untouched bin, skip
 
		double dev = TMath::Abs(content) + error;
		if (dev > maxDev) maxDev = dev;
	}
 
	if (maxDev <= 0.0) maxDev = 1.0; // fallback so range isn't degenerate
 
	double range = 0.005;
	hRes->SetMinimum(-range);
	hRes->SetMaximum(range);
}
 
void StyleResidualHist(TH1F *hRes, TH1F *refHist)
{
	// Match x-axis range/labels to the main plot but make the residual pad's
	// text large enough to read given it's a short pad.
	hRes->GetXaxis()->SetLabelSize(0.11);
	hRes->GetXaxis()->SetTitleSize(0.12);
	hRes->GetXaxis()->SetTitleOffset(1.1);
 
	hRes->GetYaxis()->SetLabelSize(0.10);
	hRes->GetYaxis()->SetTitleSize(0.11);
	hRes->GetYaxis()->SetTitleOffset(0.45);
	hRes->GetYaxis()->SetNdivisions(505);
 
	hRes->SetMarkerStyle(20);
	hRes->SetMarkerSize(0.7);
	hRes->SetLineColor(kBlack);
 
	// hide the main plot's x-axis labels/title since the residual pad shows them
	refHist->GetXaxis()->SetLabelSize(0);
	refHist->GetXaxis()->SetTitleSize(0);
}
 
void curveFitting3(const char *inputtedROOTFile1)
{
	//check if the inputted file is valid
	TFile *f1 = new TFile(inputtedROOTFile1);
	if (!f1 || f1->IsZombie())
	{
		std::cerr << "Error: cannot open file 1" << std::endl;return;
	}
 
	//create a canvas, taller than before to make room for residual pads
	TCanvas *c = new TCanvas("c", "Energy Spectra", 1200, 700);
 
	//---------------------------------------------------------------
	// Build 4 pads by hand: column 1 = noEnvelope (main + residual),
	// column 2 = combined data (main + residual). Top pads take 70%
	// of the vertical space, bottom (residual) pads take the other 30%.
	//---------------------------------------------------------------
	TPad *pad1     = new TPad("pad1",     "", 0.00, 0.30, 0.50, 1.00);
	TPad *pad1_res = new TPad("pad1_res", "", 0.00, 0.00, 0.50, 0.30);
	TPad *pad2     = new TPad("pad2",     "", 0.50, 0.30, 1.00, 1.00);
	TPad *pad2_res = new TPad("pad2_res", "", 0.50, 0.00, 1.00, 0.30);
 
	StyleMainAndResidualPads(pad1, pad1_res);
	StyleMainAndResidualPads(pad2, pad2_res);
 
	pad1->Draw();
	pad1_res->Draw();
	pad2->Draw();
	pad2_res->Draw();
 
	pad1->cd();
	//get the noEnvelope tree from the root file and create a normalized TH1F histogram of the energy spectrum.
	TTree *t1_noEnvelope = (TTree*)f1->Get("noEnvelope");
	TH1F *h1_noEnv = new TH1F("h1_noEnv", "Bi207 Calibration Source Energy Spectrum (non-envelope electrons);Energy [keV];Normalized counts", 100, 0, 1200);
	
	t1_noEnvelope->Draw("electronEnergy>>h1_noEnv", "", "goff");
	h1_noEnv->SetLineColor(kRed);
		
	Normalize(h1_noEnv);
 
	//domain of the fitting function
	double xmin = 100.0, xmax = 1150.0;
	
	//creation of the to-be-fitted six gaussian function for the noEnvelope histogram; 5 free parameters are given.
	TF1 *fitFunc = new TF1("fitFunc", sixGaussianSingleFit, xmin, xmax, 5);
	
	//names of the free parameters and their starting guesses.
	fitFunc->SetParNames("n1", "mu1", "sig1", "n4", "mu4");
	
	fitFunc->SetParameter(0, 0.5);   
	fitFunc->SetParameter(1, 450.0); 
	fitFunc->SetParameter(2, 20.0);  
	fitFunc->SetParameter(3, 10.0);  
	fitFunc->SetParameter(4, 960.0); 
 
	//boundary limits of the parameters.
	fitFunc->SetParLimits(0, 0.0, 1.0);
	fitFunc->SetParLimits(1, 400.0, 500.0);
	fitFunc->SetParLimits(2, 10.0, 35.0);
	fitFunc->SetParLimits(3, 5.0, 20.0);
	fitFunc->SetParLimits(4, 940.0, 980.0);
	
	//resulting fitted six gaussian function
	TFitResultPtr fitResult = h1_noEnv->Fit(fitFunc, "RS+");
	
	//save the values of the fitted parameters in a data structure.
	std::vector<double> fixedParams(5);
	fitFunc->GetParameters(fixedParams.data());
	
	//draw the histogram and the fitted function in a reasonable axis range.
	h1_noEnv->Draw("HIST");
	
	fitFunc->SetLineColor(kBlack);
	fitFunc->SetLineWidth(2);
	fitFunc->Draw("SAME");
	
	h1_noEnv->SetMaximum(1.2 * h1_noEnv->GetMaximum());
 
	//create and draw the legend in the top left corner of the plot.
	TLegend *leg = new TLegend(0.12, 0.72, 0.37, 0.90);
	
	leg->AddEntry(h1_noEnv, "Simulated Data (Source_0_3)", "l");
	leg->AddEntry(fitFunc, "Fit", "l");
	leg->SetTextSize(0.04);
	leg->SetBorderSize(0);   
	leg->SetFillStyle(0);
	
	leg->Draw();
 
	//---------------------------------------------------------------
	// Residual pad for the noEnvelope fit
	//---------------------------------------------------------------
	pad1_res->cd();
	TH1F *hRes1 = MakeResidualHist("hRes1", h1_noEnv, fitFunc, xmin, xmax);
	StyleResidualHist(hRes1, h1_noEnv);
	CenterResidualAxisOnZero(hRes1);
 
	hRes1->Draw("E1");
 
	TLine *zeroLine1 = new TLine(xmin, 0, xmax, 0);
	zeroLine1->SetLineColor(kBlack);
	zeroLine1->SetLineStyle(2);
	zeroLine1->Draw("SAME");
 
	//get the envelope tree from the root file and create a normalized histogram of the electron energy spectrum; this is not drawn on the canvas but used later for fitting the total electron energy spectrum.
	TTree *t1_envelope = (TTree*)f1->Get("envelope");
	TH1F *h1_env = new TH1F("h1_env", "Bi207 Calibration Source Energy Spectrum (envelope electrons);Energy [keV];Normalized counts", 100, 0, 1200);
	
	t1_envelope->Draw("electronEnergy>>h1_env", "", "goff");
	Normalize(h1_env);
 
	//change to the next main pad on the canvas.
	pad2->cd();
	//create a copy of the noEnvelope histogram and add to it the envelope histogram.
	TH1F *h2_data = (TH1F*)h1_noEnv->Clone("h2_data");
	h2_data->Add(h1_env); 
	
	h2_data->SetTitle("Bi207 Calibration Source Energy Spectrum (all electrons);Energy [keV];Normalized counts");
	h2_data->SetLineColor(kGreen);
	
	//creation of one single function of the sum of the previous fixed-six guassian shape and the to-be-fitted envelope signature.
	TF1 *fCombined = new TF1
	(
	
	//set the name of the function and allow the function to take copies of the envelope histogram and the fixed gaussian parameters
	"fCombined", [h1_env, fixedParams](double *x, double *p) 
	
	//mutable allows the function to make internal changes to its copied envelope and gaussian parameters if needed.
	mutable
	
	{
		//calculate the value of the gaussian part of the function from the fixed parameter data structure.
		double gausPart = sixGaussianSingleFit(x, fixedParams.data());
		
		//get the bin in the envelope hisotgram correspoding to the evaluated x-value.
		int bin = h1_env->GetXaxis()->FindBin(x[0]);
		
		//if the bin is outside the allowable range, reasign it to either the first or last bin.
		bin = std::max(1, std::min(bin, h1_env->GetNbinsX()));
				//calculate the value of the the envelope histogram shape at the evaluated bin; it is stored up to a free parameter coefficient to be fitted later in the code.
		double histPart = p[0] * h1_env->GetBinContent(bin);
		
		//return the total function value.
		return gausPart + histPart;
	},
	
	//set the domain of the function to be the range of the envelope histogram.
	xmin,
	xmax,
	
	//declare that there is one free parameter.
	1
	);
 
	//set the name of the free parameter and its bounds.
	fCombined->SetParName(0, "envelopeCoeff");
	fCombined->SetParLimits(0, 0.0, 10.0);
 
	//fit the total function over the total electron spectrum.
	TFitResultPtr fitResult2 = h2_data->Fit(fCombined, "RS+");
 
	//draw the total electron spectrum histogram and the fitted function on the canvas on a reasonable axis range.
	h2_data->Draw("HIST");
	
	fCombined->SetLineColor(kBlack);
	fCombined->SetLineWidth(2);
	fCombined->Draw("SAME");
	
	h2_data->SetMaximum(1.2 * h2_data->GetMaximum());
 
	//draw the legend of the total energy spectrum on the top left corner of the plot.
	TLegend *leg2 = new TLegend(0.12, 0.72, 0.45, 0.90);
	leg2->AddEntry(h2_data, "Simuluated Data (Source_0_3)", "l");
	leg2->AddEntry(fCombined, "Fit", "l");
	
	leg2->SetTextSize(0.04);
	leg2->SetBorderSize(0);   
	leg2->SetFillStyle(0);
	
	leg2->Draw();
 
	//---------------------------------------------------------------
	// Residual pad for the combined-data fit
	//---------------------------------------------------------------
	pad2_res->cd();
	TH1F *hRes2 = MakeResidualHist("hRes2", h2_data, fCombined, xmin, xmax);
	StyleResidualHist(hRes2, h2_data);
	CenterResidualAxisOnZero(hRes2);
 
	hRes2->Draw("E1");
 
	TLine *zeroLine2 = new TLine(xmin, 0, xmax, 0);
	zeroLine2->SetLineColor(kBlack);
	zeroLine2->SetLineStyle(2);
	zeroLine2->Draw("SAME");
 
	//change to the full canvas and save the figure.
	c->cd(0); 
	c->SaveAs("fittedtest.png");
}
