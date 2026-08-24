#include "../../include/curveFitting.h"

//normalization function
void Normalize(TH1 *h)
{
	if (!h) return;
	
	double integral = h->Integral();
	
	if (integral > 0) h->Scale(1.0 / integral);
}

void curveFitting(const char *inputtedROOTFile1)
{
	//check if the inputted file is valid
	TFile *f1 = new TFile(inputtedROOTFile1);
	if (!f1 || f1->IsZombie())
	{
		std::cerr << "Error: cannot open file 1" << std::endl;return;
	}
 
	//create a canvas with two panels
	TCanvas *c = new TCanvas("c", "Energy Spectra", 1200, 500);
	c->Divide(2, 1); 
 
	c->cd(1);
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
 
	//get the envelope tree from the root file and create a normalized histogram of the electron energy spectrum; this is not drawn on the canvas but used later for fitting the total electron energy spectrum.
	TTree *t1_envelope = (TTree*)f1->Get("envelope");
	TH1F *h1_env = new TH1F("h1_env", "Bi207 Calibration Source Energy Spectrum (envelope electrons);Energy [keV];Normalized counts", 100, 0, 1200);
	
	t1_envelope->Draw("electronEnergy>>h1_env", "", "goff");
	Normalize(h1_env);
 
	//change to the next pad on the canvas.
	c->cd(2);
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
 
	//change to the full canvas and save the figure.
	c->cd(0); 
	c->SaveAs("fittedtest.png");
}
