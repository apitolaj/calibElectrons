#pragma once

#include <cmath>      
#include "TMath.h"    

static constexpr double I_482keV_  = 1.52;
static constexpr double I_555keV_  = 0.44;
static constexpr double I_567keV_  = 0.15;
static constexpr double I_976keV_  = 7.03;
static constexpr double I_1049keV_ = 1.84;
static constexpr double I_1061keV_ = 0.54;

inline double sixGaussianSingleFit(double* x,  double* par)
{
	double PDF    = 0.0;
	
	//gaussians for the first peak
	double gauss1 = 0.0;
	double gauss2 = 0.0;
	double gauss3 = 0.0;
	
	//gaussians for the second peak
	double gauss4 = 0.0;
	double gauss5 = 0.0;
	double gauss6 = 0.0;
	
	double n1   = par[0];
	double mu1  = par[1];
	double sig1 = par[2];
	
/*	double n2   = (I_555keV_ / I_482keV_) * n1;
	double mu2  = (555.0 / 482.0) * mu1;
	double sig2 = sqrt(555.0 / 482.0) * sig1;
	
	double n3   = (I_567keV_ / I_482keV_) * n1;
	double mu3  = (567.0 / 482.0) * mu1;
	double sig3 = sqrt(567.0 / 482.0) * sig1;
	

	double n4   = (I_976keV_ / I_482keV_) * n1;
	double mu4  = (976.0 / 482.0) * mu1;
	double sig4 = sqrt(976.0 / 482.0) * sig1;
	
	double n5   = (I_1049keV_ / I_482keV_) * n1;
	double mu5  = (1049.0 / 482.0) * mu1;
	double sig5 = sqrt(1049.0 / 482.0) * sig1;
	
	double n6   = (I_1061keV_ / I_482keV_) * n1;
	double mu6  = (1061.0 / 482.0) * mu1;
	double sig6 = sqrt(1061.0 / 482.0) * sig1;
	*/
	
 	double n2   = (I_555keV_ / I_482keV_) * n1;
	double mu2  = (555.0 / 482.0) * mu1;
	double sig2 = sqrt(555.0 / 482.0) * sig1;
	
	double n3   = (I_567keV_ / I_482keV_) * n1;
	double mu3  = (567.0 / 482.0) * mu1;
	double sig3 = sqrt(567.0 / 482.0) * sig1;
	
	double n4   = par[3];
//	double n4   = (I_976keV_ / I_482keV_) * n1;
	double mu4  = par[4];
//	double mu4  = (976.0 / 482.0) * mu1;
	double sig4 = sqrt(976.0 / 482.0) * sig1;
	
	double n5   = (I_1049keV_ / I_976keV_) * n4;
	double mu5  = (1049.0 / 976.0) * mu4;
	double sig5 = sqrt(1049.0 / 482.0) * sig1;
	
	double n6   = (I_1061keV_ / I_976keV_) * n4;
	double mu6  = (1061.0 / 976.0) * mu4;
	double sig6 = sqrt(1061.0 / 482.0) * sig1;
	
	// Calculation of exponents of each Gaussian
	double arg1 = (sig1 != 0.0) ? (x[0] - mu1) / (sig1) : 0.0;	
	double arg2 = (sig2 != 0.0) ? (x[0] - mu2) / (sig2) : 0.0;
	double arg3 = (sig3 != 0.0) ? (x[0] - mu3) / (sig3) : 0.0;
	double arg4 = (sig4 != 0.0) ? (x[0] - mu4) / (sig4) : 0.0;	
	double arg5 = (sig5 != 0.0) ? (x[0] - mu5) / (sig5) : 0.0;
	double arg6 = (sig6 != 0.0) ? (x[0] - mu6) / (sig6) : 0.0;
  
	// Calculate gaussians
	gauss1 = exp(-0.5 * arg1 * arg1) / ( sig1 * sqrt(2.0 * TMath::Pi()));
	gauss2 = exp(-0.5 * arg2 * arg2) / ( sig2 * sqrt(2.0 * TMath::Pi()));
	gauss3 = exp(-0.5 * arg3 * arg3) / ( sig3 * sqrt(2.0 * TMath::Pi()));
	gauss4 = exp(-0.5 * arg4 * arg4) / ( sig4 * sqrt(2.0 * TMath::Pi()));
	gauss5 = exp(-0.5 * arg5 * arg5) / ( sig5 * sqrt(2.0 * TMath::Pi()));
	gauss6 = exp(-0.5 * arg6 * arg6) / ( sig6 * sqrt(2.0 * TMath::Pi()));
	
	PDF = n1*gauss1 + n2*gauss2 + n3*gauss3 + n4*gauss4 + n5*gauss5 + n6*gauss6;
  
  	return PDF;
}

// ---------------------------------------------------------------------
// Manual Landau (x) Gaussian convolution ? self contained, no ROOT
// TF1Convolution / FFT machinery, no global state, no null-pointer risk.
//
// x     = point at which to evaluate the convolved shape
// mpv   = most probable value (peak position) of the underlying Landau
// eta   = Landau width parameter
// sigma = Gaussian smearing width
// ---------------------------------------------------------------------
inline double langauShape(double x, double mpv, double eta, double sigma)
{
	double invsq2pi = 0.3989422804014; // 1/sqrt(2*pi)
	double mpshift  = -0.22278298;     // Landau MPV shift correction
	double np       = 6000.0;           // number of integration steps
	double sc       = 10.0;             // integration range in Gaussian sigmas
 
	if (eta <= 0.0 || sigma <= 0.0) return 0.0;
 
	double mpc = mpv - mpshift * eta;
 
	double xlow = x - sc * sigma;
	double xupp = x + sc * sigma;
	double step = (xupp - xlow) / np;
 
	double sum = 0.0;
	for (int i = 1; i <= np / 2; i++) {
		double xx = xlow + (i - 0.5) * step;
		double fland = TMath::Landau(xx, mpc, eta) / eta;
		sum += fland * TMath::Gaus(x, xx, sigma);
 
		xx = xupp - (i - 0.5) * step;
		fland = TMath::Landau(xx, mpc, eta) / eta;
		sum += fland * TMath::Gaus(x, xx, sigma);
	}
 
	return step * sum * invsq2pi / sigma;
}


inline double sixLangauSingleFit(double* x, double* par)
{
	double PDF = 0.0;

	double n1   = par[0];
	double mu1  = par[1];
	double sig1 = par[2];
	double eta1 = par[3];

	double n2   = (I_555keV_ / I_482keV_) * n1;
	double mu2  = (555.0 / 482.0) * mu1;
	double sig2 = sqrt(555.0 / 482.0) * sig1;
	double eta2 = sqrt(555.0 / 482.0) * eta1;

	double n3   = (I_567keV_ / I_482keV_) * n1;
	double mu3  = (567.0 / 482.0) * mu1;
	double sig3 = sqrt(567.0 / 482.0) * sig1;
	double eta3 = sqrt(567.0 / 482.0) * eta1;

	//double n4   = (I_976keV_ / I_482keV_) * n1;
	
	double n4   = par[4];
	double mu4  = par[5];
	double sig4 = par[6];
	double eta4 = par[7];

	double n5   = (I_1049keV_ / I_976keV_) * n4;
	double mu5  = (1049.0 / 976.0) * mu4;
	double sig5 = sqrt(1049.0 / 976.0) * sig4;
	double eta5 = sqrt(1049.0 / 976.0) * eta4;

	double n6   = (I_1061keV_ / I_976keV_) * n4;
	double mu6  = (1061.0 / 976.0) * mu4;
	double sig6 = sqrt(1061.0 / 976.0) * sig4;
	double eta6 = sqrt(1061.0 / 976.0) * eta4;

	// Parameter arrays for each TF1Convolution: [LandauConst, MPV, eta, GausConst, GausMean, GausSigma]
	double p1[6] = {1.0, mu1, eta1, 1.0, 0.0, sig1};
	double p2[6] = {1.0, mu2, eta2, 1.0, 0.0, sig2};
	double p3[6] = {1.0, mu3, eta3, 1.0, 0.0, sig3};
	double p4[6] = {1.0, mu4, eta4, 1.0, 0.0, sig4};
	double p5[6] = {1.0, mu5, eta5, 1.0, 0.0, sig5};
	double p6[6] = {1.0, mu6, eta6, 1.0, 0.0, sig6};
	
	double langau1 = n1 * langauShape(x[0], mu1, eta1, sig1);
	double langau2 = n2 * langauShape(x[0], mu2, eta2, sig2);
	double langau3 = n3 * langauShape(x[0], mu3, eta3, sig3);
	double langau4 = n4 * langauShape(x[0], mu4, eta4, sig4);
	double langau5 = n5 * langauShape(x[0], mu5, eta5, sig5);
	double langau6 = n6 * langauShape(x[0], mu6, eta6, sig6);


	PDF = langau1 + langau2 + langau3 + langau4 + langau5 + langau6;

	return PDF;
}
