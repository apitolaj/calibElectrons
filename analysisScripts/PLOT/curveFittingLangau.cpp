#include "../../../include/curveFitting.h"
 
//normalization function
void Normalize(TH1 *h)
{
	if (!h) return;
	
	double integral = h->Integral();
	
	if (integral > 0) h->Scale(1.0 / integral);
}
 
void DrawNormalizedChi2(TFitResultPtr fitResult, double x1 = 0.30, double y1 = 0.52, double x2 = 0.58, double y2 = 0.62)
{
	double chi2 = fitResult->Chi2();
	int ndf = fitResult->Ndf();

	double normChi2 = (ndf > 0) ? chi2 / ndf : 0.0;

	TPaveText *box = new TPaveText(x1, y1, x2, y2, "NDC");

	box->SetFillStyle(0);
	box->SetBorderSize(0);
	box->SetTextAlign(12);
	box->SetTextSize(0.04);

	box->AddText(Form("#chi^{2}/NDF = %.3f", normChi2));

	box->Draw();
}

TH1F* MakePullHist(const char *name, TH1F *data, TF1 *fitFunc, double xmin, double xmax)
{
	TH1F *hPull = (TH1F*)data->Clone(name);
	hPull->Reset();

	hPull->SetTitle(";Energy [keV];  Fit [#sigma] - Data");

	for (int i = 1; i <= data->GetNbinsX(); ++i)
	{
		double x = data->GetBinCenter(i);

		if (x < xmin || x > xmax) continue;

		double binLow = data->GetXaxis()->GetBinLowEdge(i);

		double binHigh = data->GetXaxis()->GetBinUpEdge(i);

		double binWidth =binHigh - binLow;

		double observed = data->GetBinContent(i);

		double expected = fitFunc->Integral(binLow, binHigh) / binWidth;

		double error = data->GetBinError(i);

		if (error <= 0.0) continue;

		double pull = (expected -observed) / error;

		hPull->SetBinContent(i, pull);
	}

	return hPull;
}
 
void StyleMainAndResidualPads(TPad *padMain, TPad *padRes)
{
	padMain->SetBottomMargin(0.02);
	padMain->SetLeftMargin(0.12);
 
	padRes->SetTopMargin(0.03);
	padRes->SetBottomMargin(0.35);
	padRes->SetLeftMargin(0.12);
}
 
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
 
	double range = maxDev * paddingFactor;
	
	hRes->SetMinimum(-range);
	hRes->SetMaximum(range);
}
 
void StyleResidualHist(TH1F *hRes, TH1F *refHist)
{
	hRes->GetXaxis()->SetLabelSize(0.08);
	hRes->GetXaxis()->SetTitleSize(0.1);
	hRes->GetXaxis()->SetTitleOffset(1.1);
 
	hRes->GetYaxis()->SetLabelSize(0.08);
	hRes->GetYaxis()->SetTitleSize(0.08);
	hRes->GetYaxis()->SetTitleOffset(0.65);
	hRes->GetYaxis()->SetNdivisions(505);
 
	hRes->SetMarkerStyle(20);
	hRes->SetMarkerSize(0.4);
	hRes->SetLineColor(kBlack);
	
	hRes->SetMinimum(-0.005);
	hRes->SetMaximum(0.005);
 
	// hide the main plot's x-axis labels/title since the residual pad shows them
	refHist->GetXaxis()->SetLabelSize(0);
	refHist->GetXaxis()->SetTitleSize(0);
}
 
void curveFittingLangau_SOURCE_PLACEHOLDER(const char *inputtedROOTFile1)
{
	gStyle->SetOptStat(0);
	
	//check if the inputted file is valid
	TFile *f1 = new TFile(inputtedROOTFile1);
	if (!f1 || f1->IsZombie())
	{
		std::cerr << "Error: cannot open file 1" << std::endl;return;
	}
 
	TCanvas *c = new TCanvas("c", "Energy Spectra", 1200, 700);
 
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
	TH1F *h1_noEnv = new TH1F("h1_noEnv", "Bi207 Calibration Source Energy Spectrum (non-envelope electrons);Energy [keV];Counts", 200, 0, 1200);
	
	t1_noEnvelope->Draw("electronEnergy>>h1_noEnv", "", "goff");
	h1_noEnv->SetLineColor(kRed);
		
//	Normalize(h1_noEnv);
 
	//domain of the fitting function
	double xmin = 50.0, xmax = 1150.0;
	
	//creation of the to-be-fitted six gaussian function for the noEnvelope histogram; 5 free parameters are given.
	TF1 *fitFunc = new TF1("fitFunc", sixLangauSingleFit, xmin, xmax, 8);

// ---- Set all 8 starting parameters ----

	double totalCounts = h1_noEnv->Integral()
	;
 	fitFunc->SetParNames("n1", "mu1", "sig1", "eta1", "n4", "mu4", "sig4", "eta4");
	fitFunc->SetParameters(0.2 * totalCounts, 450.0, 20.0, 10, 0.5 * totalCounts, 960.0, 20, 10);

	//boundary limits of the parameters.
	fitFunc->SetParLimits(0, 0.0, 2* totalCounts);
	fitFunc->SetParLimits(1, 400.0, 500.0);
	fitFunc->SetParLimits(2, 10.0, 35.0);
	fitFunc->SetParLimits(3, 0.0, 20.0);
	fitFunc->SetParLimits(4, 0.001, 5 * totalCounts);   // n4 > 0
	fitFunc->SetParLimits(5, 900, 1000);   
	fitFunc->SetParLimits(6, 0.001, 100);   // sig4 > 0
	fitFunc->SetParLimits(7, 0.001, 100);   // eta4 > 0
	
	//resulting fitted six gaussian function
	TFitResultPtr fitResult = h1_noEnv->Fit(fitFunc, "RS+I");
	
	//save the values of the fitted parameters in a data structure.
	std::vector<double> fixedParams(8);
	fitFunc->GetParameters(fixedParams.data());
	
	//draw the histogram and the fitted function in a reasonable axis range.
	h1_noEnv->Draw("E1");
	
	gPad->Update();
	
	TPaveText *title1 = (TPaveText*)gPad->GetPrimitive("title");
	if (title1)
	{
		title1->SetTextSize(0.035);
		title1->SetTextFont(62);
	}
	
	fitFunc->SetLineColor(kBlack);
	fitFunc->SetLineWidth(2);
	fitFunc->Draw("SAME");
	
	h1_noEnv->SetMaximum(1.2 * h1_noEnv->GetMaximum());
	
	DrawNormalizedChi2(fitResult);
 
	//create and draw the legend in the top left corner of the plot.
	TLegend *leg = new TLegend(0.17, 0.72, 0.42, 0.90);
	
	leg->AddEntry(h1_noEnv, "Simulated Data (SOURCE_PLACEHOLDER)", "l");
	leg->AddEntry(fitFunc, "Langau Fit", "l");
	leg->SetTextSize(0.04);
	leg->SetBorderSize(0);   
	leg->SetFillStyle(0);
	
	leg->Draw();
 
	//---------------------------------------------------------------
	// Residual pad for the noEnvelope fit
	//---------------------------------------------------------------
	pad1_res->cd();
	TH1F *hRes1 = MakePullHist("hRes1", h1_noEnv, fitFunc, xmin, xmax);
	StyleResidualHist(hRes1, h1_noEnv);
	CenterResidualAxisOnZero(hRes1);
 
	hRes1->Draw("P");
 
	TLine *zeroLine1 = new TLine(xmin, 0, xmax, 0);
	zeroLine1->SetLineColor(kBlack);
	zeroLine1->SetLineStyle(2);
	zeroLine1->Draw("SAME");
 
	//get the envelope tree from the root file and create a normalized histogram of the electron energy spectrum; this is not drawn on the canvas but used later for fitting the total electron energy spectrum.
	TTree *t1_envelope = (TTree*)f1->Get("envelope");
	TH1F *h1_env = new TH1F("h1_env", "Bi207 Calibration Source Energy Spectrum (envelope electrons);Energy [keV];Counts", 200, 0, 1200);

	t1_envelope->Draw("electronEnergy>>h1_env", "", "goff");

//	Normalize(h1_env);
 
	//change to the next main pad on the canvas.
	pad2->cd();
	//create a copy of the noEnvelope histogram and add to it the envelope histogram.
	TH1F *h2_data = (TH1F*)h1_noEnv->Clone("h2_data");
	h2_data->GetListOfFunctions()->Clear();   // drop the inherited noEnvelope fit function
	h2_data->Add(h1_env); 
	
	h2_data->SetTitle("Bi207 Calibration Source Energy Spectrum (all electrons);Energy [keV];Normalized counts");
	h2_data->SetLineColor(kBlue);
	
	//creation of one single function of the sum of the previous fixed-six guassian shape and the to-be-fitted envelope signature.
	
	TF1 *fCombined = new TF1("fCombined",
	
	[h1_env, fixedParams](double *x, double *p) mutable
	{
		double langauPart = sixLangauSingleFit(x, fixedParams.data());

		int bin = h1_env->GetXaxis()->FindBin(x[0]);

		bin = std::max(1, std::min(bin, h1_env->GetNbinsX()));

		double binWidth = h1_env->GetXaxis()->GetBinWidth(bin);

		double histPart = p[0] * h1_env->GetBinContent(bin) / binWidth;

		return langauPart + histPart;
	},
	xmin,
	xmax,
	1
	);
 
	//set the name of the free parameter and its bounds.
	fCombined->SetParName(0, "envelopeCoeff");
	fCombined->SetParLimits(0, 0.0, 10.0);
 
	//fit the total function over the total electron spectrum.
	TFitResultPtr fitResult2 = h2_data->Fit(fCombined, "RS+I");
 
	//draw the total electron spectrum histogram and the fitted function on the canvas on a reasonable axis range.
	h2_data->Draw("E1");
	
	gPad->Update();
	TPaveText *title2 = (TPaveText*)gPad->GetPrimitive("title");
	if (title2) 
	{
		title2->SetTextSize(0.035);
		title2->SetTextFont(62);
	}
	
	fCombined->SetLineColor(kBlack);
	fCombined->SetLineWidth(2);
	fCombined->Draw("SAME");
	
	h2_data->SetMaximum(1.2 * h2_data->GetMaximum());
	
	DrawNormalizedChi2(fitResult2);
 
	//draw the legend of the total energy spectrum on the top left corner of the plot.
	TLegend *leg2 = new TLegend(0.17, 0.72, 0.42, 0.90);
	leg2->AddEntry(h2_data, "Simuluated Data (SOURCE_PLACEHOLDER)", "l");
	leg2->AddEntry(fCombined, "Langau Fit", "l");
	
	leg2->SetTextSize(0.04);
	leg2->SetBorderSize(0);   
	leg2->SetFillStyle(0);
	
	leg2->Draw();
 
	//---------------------------------------------------------------
	// Residual pad for the combined-data fit
	//---------------------------------------------------------------
	pad2_res->cd();
	TH1F *hRes2 = MakePullHist("hRes2", h2_data, fCombined, xmin, xmax);
	StyleResidualHist(hRes2, h2_data);
	CenterResidualAxisOnZero(hRes2);
 
	hRes2->Draw("P");
 
	TLine *zeroLine2 = new TLine(xmin, 0, xmax, 0);
	zeroLine2->SetLineColor(kBlack);
	zeroLine2->SetLineStyle(2);
	zeroLine2->Draw("SAME");
 
	//change to the full canvas and save the figure.
	c->cd(0); 
	c->SaveAs("fittedLangau_SOURCE_PLACEHOLDER.png");
}
