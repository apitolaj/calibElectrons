// azimuth_zenith_histo_poly_2files_ratio.cpp
//
// Equal-area polar hemisphere histograms for spherical data.
//
// This version:
//   1) Reproduces the ORIGINAL 2x2 canvas exactly as before
//      (envelope / noEnvelope rows, left/right hemisphere columns,
//      raw event counts), saved to the original output filename.
//
//   2) ADDS a second, separate 2x2 canvas of RATIO plots, saved to
//      a different output filename:
//
//        Row 1: Ratio1 = envelope(all energies) / [envelope + noEnvelope](all energies)
//        Row 2: Ratio2 = envelope(electronEnergy in [600,800]) / [envelope + noEnvelope](all energies)
//
//      Both ratios use the SAME denominator D = envelope(all E) + noEnvelope(all E),
//      built by summing the already-filled envelope/noEnvelope polys bin-by-bin.
//
// Uses TH2Poly with wedge-shaped bins (azimuth x zenith rings), same
// equal-area projection and decorations as the original script.
//
// Usage:
//
//   .x azimuth_zenith_histo_poly_2files_ratio.cpp("file.root")
//
// ============================================================
 
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
 
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
 
// ------------------------------------------------------------
// Configuration
// ------------------------------------------------------------
 
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
 
// ------------------------------------------------------------
// Azimuth conversion
//
// mirror = false (right hemisphere): azimuth increases
//   counter-clockwise as seen by the viewer.
//
// mirror = true  (left hemisphere):  azimuth increases
//   clockwise as seen by the viewer, so that the two disks
//   have consistent handedness at their shared 90 deg equator
//   edge -- equivalent to reflecting the disk across the y-axis.
// ------------------------------------------------------------
 
inline double AzToPlotRad(double az_deg, bool mirror = false)
{
    double phi = (90.0 + az_deg) * TMath::DegToRad();
    return mirror ? TMath::Pi() - phi : phi;
}
 
// ------------------------------------------------------------
// Equal-area radius
// ------------------------------------------------------------
 
double EqualAreaRadius(double zenith_deg, bool rightHemi)
{
    double theta = zenith_deg * TMath::DegToRad();
 
    if (rightHemi)
        return std::sqrt(1.0 - std::cos(theta));
 
    return std::sqrt(1.0 + std::cos(theta));
}
 
// ------------------------------------------------------------
// Build TH2Poly with wedge bins
// ------------------------------------------------------------
 
void AddWedgeBin(TH2Poly* poly,
                 bool rightHemi,
                 double zenLo,
                 double zenHi,
                 double azLo,
                 double azHi,
                 bool mirror = false)
{
    double r_inner, r_outer;
 
    if (rightHemi)
    {
        r_inner = EqualAreaRadius(zenLo, true);
        r_outer = EqualAreaRadius(zenHi, true);
    }
    else
    {
        r_inner = EqualAreaRadius(zenLo, false);
        r_outer = EqualAreaRadius(zenHi, false);
 
        if (r_inner > r_outer)
            std::swap(r_inner, r_outer);
    }
 
    double phi_lo = AzToPlotRad(azLo, mirror);
    double phi_hi = AzToPlotRad(azHi, mirror);
 
    std::vector<double> px, py;
 
    // Outer arc: phi_lo -> phi_hi
    for (int k = 0; k <= ARC_STEPS; ++k)
    {
        double t   = static_cast<double>(k) / ARC_STEPS;
        double phi = phi_lo + t * (phi_hi - phi_lo);
 
        px.push_back(r_outer * std::cos(phi));
        py.push_back(r_outer * std::sin(phi));
    }
 
    // Inner arc: phi_hi -> phi_lo
    if (r_inner < 1e-9)
    {
        // Degenerate: pole point
        px.push_back(0.0);
        py.push_back(0.0);
    }
    else
    {
        for (int k = ARC_STEPS; k >= 0; --k)
        {
            double t   = static_cast<double>(k) / ARC_STEPS;
            double phi = phi_lo + t * (phi_hi - phi_lo);
 
            px.push_back(r_inner * std::cos(phi));
            py.push_back(r_inner * std::sin(phi));
        }
    }
 
    poly->AddBin(static_cast<int>(px.size()),
                 px.data(),
                 py.data());
}
 
// ------------------------------------------------------------
// Create TH2Poly and populate all bins (geometry only, no fill)
//
// IMPORTANT: called with identical rightHemi/mirror/N_AZ_BINS/
// N_ZEN_BINS, this produces the SAME bin ordering every time, so
// two polys built this way are bin-index-aligned and can be
// safely divided/summed bin-by-bin.
// ------------------------------------------------------------
 
TH2Poly* BuildPoly(const char* name, bool rightHemi, bool mirror = false)
{
    TH2Poly* poly =
        new TH2Poly(name, name, -1.05, 1.05, -1.05, 1.05);
 
    poly->SetFloat();
    double azStep = 360.0 / N_AZ_BINS;
    double muStep = 1.0 / N_ZEN_BINS;
 
    for (int iz = 0; iz < N_ZEN_BINS; ++iz)
    {
        double muLo, muHi;
 
        if (rightHemi)
        {
            muHi = 1.0 - iz * muStep;
            muLo = 1.0 - (iz + 1) * muStep;
        }
        else
        {
            muHi = -iz * muStep;
            muLo = -(iz + 1) * muStep;
        }
 
        double zenLo = std::acos(muHi) * TMath::RadToDeg();
        double zenHi = std::acos(muLo) * TMath::RadToDeg();
 
        for (int ia = 0; ia < N_AZ_BINS; ++ia)
        {
            double azLo = ia * azStep;
            double azHi = (ia + 1) * azStep;
 
            AddWedgeBin(
                poly,
                rightHemi,
                zenLo,
                zenHi,
                azLo,
                azHi,
                mirror);
        }
    }
 
    return poly;
}
 
// ------------------------------------------------------------
// Decorations
// ------------------------------------------------------------
 
void DrawPolarDecorations(bool rightHemi, const char* title, bool mirror = false)
{
    double displayZeniths[] = {0, 15, 30, 45, 60, 75, 90};
 
    for (double z : displayZeniths)
    {
        double r;
        double labelZenith;
 
        if (rightHemi)
        {
            labelZenith = z;
            r = EqualAreaRadius(labelZenith, true);
        }
        else
        {
            labelZenith = 180.0 - z;
            r = EqualAreaRadius(labelZenith, false);
        }
 
        TEllipse* ring = new TEllipse(0.0, 0.0, r, r);
        ring->SetFillStyle(0);
        ring->SetLineStyle(2);
        ring->SetLineWidth(1);
        ring->SetLineColor(kBlack);
        ring->Draw("same");
 
        if (z > 0.0)
        {
            TLatex* lbl =
                new TLatex(r - 0.08,
                           0.01,
                           Form("%.0f#circ", labelZenith));
 
            lbl->SetTextSize(0.022);
            lbl->Draw("same");
        }
    }
 
    for (int deg = 0; deg < 360; deg += 30)
    {
        double phi = AzToPlotRad(deg, mirror);
 
        TLine* spoke =
            new TLine(0.0, 0.0,
                      std::cos(phi),
                      std::sin(phi));
 
        spoke->SetLineStyle(2);
        spoke->SetLineWidth(1);
        spoke->SetLineColor(kBlack);
        spoke->Draw("same");
    }
 
    struct
    {
        double az;
        const char* text;
        int align;
    }
    dirs[] =
    {
        {  0, "Az 0#circ (+y)", 21 },
        { 90, "Az 90#circ",     32 },
        {180, "Az 180#circ",    23 },
        {270, "Az 270#circ",    12 }
    };
 
    double labelR = mirror ? 1.23 : 1.05;
 
    for (auto& d : dirs)
    {
        double phi = AzToPlotRad(d.az, mirror);
 
	double lx = labelR * std::cos(phi);
	double ly = mirror ? labelR * std::sin(phi) * 0.85 : labelR * std::sin(phi);
	TLatex* lbl = new TLatex(lx, ly, d.text);
 
        lbl->SetTextSize(0.025);
        lbl->SetTextAlign(d.align);
        lbl->Draw("same");
    }
 
    TLatex* ttl = new TLatex(0.0, 1.25, title);
    ttl->SetTextAlign(22);
    ttl->SetTextSize(0.04);
    ttl->SetTextFont(62);
    ttl->Draw("same");
}
 
// ------------------------------------------------------------
// FillHemispherePoly
//
// Builds the wedge geometry and fills it from the tree.
// No drawing/styling here -- this makes the poly reusable both
// for the "as is" plots AND as an input to ratio/sum arithmetic.
//
// If useEnergyCut is true, only entries with ENERGY_BRANCH in
// [eLo, eHi] are filled (electronEnergy is a Double_t branch).
// ------------------------------------------------------------
 
TH2Poly* FillHemispherePoly(TTree* tree,
                            bool rightHemi,
                            const char* histName,
                            bool mirror,
                            bool useEnergyCut,
                            double eLo,
                            double eHi)
{
    TH2Poly* poly = BuildPoly(histName, rightHemi, mirror);
 
    double  zenith_val;
    double  azimuth_val;
    double energy_val = 0.0;
 
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus(ZENITH_BRANCH,  1);
    tree->SetBranchStatus(AZIMUTH_BRANCH, 1);
 
    tree->SetBranchAddress(ZENITH_BRANCH,  &zenith_val);
    tree->SetBranchAddress(AZIMUTH_BRANCH, &azimuth_val);
 
    if (useEnergyCut)
    {
        tree->SetBranchStatus(ENERGY_BRANCH, 1);
        tree->SetBranchAddress(ENERGY_BRANCH, &energy_val);
    }
 
    Long64_t nEntries = tree->GetEntries();
 
    std::cout
        << "[INFO] Processing "
        << nEntries
        << " entries for "
        << histName
        << (useEnergyCut ? " (energy cut applied)" : "")
        << std::endl;
 
    for (Long64_t i = 0; i < nEntries; ++i)
    {
        tree->GetEntry(i);
 
        double z = zenith_val;
        double a = azimuth_val;
 
        if (rightHemi)
        {
            if (z < 0.0 || z > 90.0)
                continue;
        }
        else
        {
            if (z < 90.0 || z > 180.0)
                continue;
        }
 
        if (useEnergyCut)
        {
            if (energy_val < eLo || energy_val > eHi)
                continue;
        }
 
        double r   = EqualAreaRadius(z, rightHemi);
        double phi = AzToPlotRad(a, mirror);
 
        poly->Fill(r * std::cos(phi), r * std::sin(phi));
    }
 
    tree->SetBranchStatus("*", 1);
 
    return poly;
}
 
// ------------------------------------------------------------
// StyleAndDrawPoly
//
// Takes an already-filled TH2Poly and draws/decorates it into
// the given pad. Works equally well for raw-count polys and for
// ratio polys (the color axis just auto-ranges to the content).
// ------------------------------------------------------------
 
void StyleAndDrawPoly(TH2Poly* poly,
                      const char* title,
                      TVirtualPad* pad,
                      bool rightHemi,
                      bool mirror)
{
    pad->cd();
    pad->SetRightMargin(0.15);
 
    poly->GetXaxis()->SetLabelOffset(999);
    poly->GetYaxis()->SetLabelOffset(999);
    poly->GetXaxis()->SetTickLength(0);
    poly->GetYaxis()->SetTickLength(0);
    poly->GetXaxis()->SetAxisColor(0);
    poly->GetYaxis()->SetAxisColor(0);
    pad->SetFrameLineColor(0);
    pad->SetFrameLineWidth(0);
 
    poly->Draw("COLZ");
 
    pad->Update();
 
    TPaletteAxis* palette =
        (TPaletteAxis*)poly->GetListOfFunctions()->FindObject("palette");
 
    if (palette)
    {
        palette->SetX1NDC(0.925);
        palette->SetX2NDC(0.95);
        palette->SetY1NDC(0.10);
        palette->SetY2NDC(0.90);
        pad->Modified();
        pad->Update();
    }
 
    DrawPolarDecorations(rightHemi, title, mirror);
 
    pad->Update();
}
 
// ------------------------------------------------------------
// SumPolys
//
// Bin-by-bin sum of two bin-aligned TH2Poly histograms
// (i.e. built with identical BuildPoly geometry/ordering).
// ------------------------------------------------------------
 
TH2Poly* SumPolys(TH2Poly* a, TH2Poly* b, const char* name)
{
    TH2Poly* sum = (TH2Poly*)a->Clone(name);
 
    int nbins = a->GetNumberOfBins();
    for (int i = 1; i <= nbins; ++i)
    {
        double va = a->GetBinContent(i);
        double vb = b->GetBinContent(i);
        sum->SetBinContent(i, va + vb);
    }
 
    return sum;
}
 
// ------------------------------------------------------------
// DivideHemispheres
//
// Bin-by-bin ratio of two bin-aligned TH2Poly histograms.
// Empty denominator bins are set to 0 rather than propagating
// NaN/inf, so the color map stays well-behaved.
// ------------------------------------------------------------
 
TH2Poly* DivideHemispheres(TH2Poly* num, TH2Poly* den, const char* name)
{
    TH2Poly* ratio = (TH2Poly*)num->Clone(name);
 
    int nbins = num->GetNumberOfBins();
    for (int i = 1; i <= nbins; ++i)
    {
        double n = num->GetBinContent(i);
        double d = den->GetBinContent(i);
        ratio->SetBinContent(i, d != 0.0 ? n / d : 0.0);
    }
 
    return ratio;
}
 
// ------------------------------------------------------------
// DrawTreeRow
//
// Fills + draws both hemispheres for one tree into the two given
// pads, EXACTLY as in the original script. Also hands back the
// filled polys (before any further use) so the caller can reuse
// their bin contents later for ratio/sum arithmetic without
// re-scanning the tree.
// ------------------------------------------------------------
 
bool DrawTreeRow(TFile* f,
                 const char* treeName,
                 const char* rowLabel,
                 const char* histPrefix,
                 TVirtualPad* leftPad,
                 TVirtualPad* rightPad,
                 TH2Poly*& leftPolyOut,
                 TH2Poly*& rightPolyOut)
{
    TTree* tree = nullptr;
    f->GetObject(treeName, tree);
 
    if (!tree)
    {
        std::cerr
            << "[ERROR] TTree '"
            << treeName
            << "' not found in file"
            << std::endl;
        return false;
    }
 
    std::cout
        << "[INFO] Tree '"
        << treeName
        << "' Entries = "
        << tree->GetEntries()
        << std::endl;
 
    std::string leftHistName  = std::string(histPrefix) + "_left_hemisphere";
    std::string rightHistName = std::string(histPrefix) + "_right_hemisphere";
 
    std::string leftTitle  = std::string(rowLabel) + ": Italian Side.";
    std::string rightTitle = std::string(rowLabel) + ": French Side.";
 
    leftPolyOut = FillHemispherePoly(tree, false, leftHistName.c_str(), true, false, 0.0, 0.0);
    StyleAndDrawPoly(leftPolyOut, leftTitle.c_str(), leftPad, false, true);
 
    rightPolyOut = FillHemispherePoly(tree, true, rightHistName.c_str(), false, false, 0.0, 0.0);
    StyleAndDrawPoly(rightPolyOut, rightTitle.c_str(), rightPad, true, false);
 
    return true;
}
 
// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
 
void polarHistoTEST_angles_2_4(const char* filename)
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
 
    TCanvas* c =
        new TCanvas("c_both_trees", "Hemisphere Plots (2 trees)", 2200, 2200);
 
    c->Divide(2, 2);
 
    TH2Poly *envLeft = nullptr, *envRight = nullptr;
    TH2Poly *noenvLeft = nullptr, *noenvRight = nullptr;
 
    bool okEnv = DrawTreeRow(
        f,
        "envelope",
        "Source 2_4 (Env., placeholder)",
        "env",
        c->GetPad(1),
        c->GetPad(2),
        envLeft,
        envRight);
 
    bool okNoEnv = DrawTreeRow(
        f,
        "noEnvelope",
        "Source 2_4 (No env., placeholder)",
        "noenv",
        c->GetPad(3),
        c->GetPad(4),
        noenvLeft,
        noenvRight);
 
    if (!okEnv && !okNoEnv)
    {
        std::cerr << "[ERROR] Neither tree could be processed. Aborting." << std::endl;
        f->Close();
        return;
    }
 
    c->Update();
    c->SaveAs("polarHisto_Source_2_4.png");
 
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
 
        TCanvas* cRatio =
            new TCanvas("c_ratios", "Hemisphere Ratio Plots", 2200, 2200);
 
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
        cRatio->SaveAs("polarHisto_Source_2_4_ratios.png");
    }
    else
    {
        std::cerr
            << "[WARN] Skipping ratio plots -- both 'envelope' and 'noEnvelope' "
            << "trees are required."
            << std::endl;
    }
 
    f->Close();
 
    std::cout << "[INFO] Done." << std::endl;
}

