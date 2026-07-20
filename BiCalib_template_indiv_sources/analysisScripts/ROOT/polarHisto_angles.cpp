// azimuth_zenith_histo_poly_2files.cpp
//
// Equal-area polar hemisphere histograms for spherical data,
// for TWO input ROOT files shown stacked in a single canvas.
//
// Layout (2x2 grid):
//
//   Row 1 (File A):  [ Left Hemisphere A ] [ Right Hemisphere A ]
//   Row 2 (File B):  [ Left Hemisphere B ] [ Right Hemisphere B ]
//
// Uses TH2Poly with wedge-shaped bins (azimuth x zenith rings).
//
//   Left column  -> Left Hemisphere:
//     centre = zenith 180° (-x)
//     edge   = zenith 90°  (+z)
//     MIRRORED: azimuth increases in the opposite rotational
//               direction so that handedness is consistent at
//               the shared 90° equator edge (i.e. the two disks
//               read like an unfolded sphere).
//
//   Right column -> Right Hemisphere:
//     centre = zenith 0°   (+x)
//     edge   = zenith 90°  (+z)
//
// IMPORTANT:
//
// Uses an AZIMUTHAL EQUAL-AREA projection:
//
//     Right hemisphere:
//         r = sqrt(1 - cos(theta))
//
//     Left hemisphere:
//         r = sqrt(1 + cos(theta))
//
// where theta is zenith in radians.
//
// This guarantees:
//
//     projected area ? spherical solid angle
//
// Therefore a uniform sphere distribution appears
// visually uniform across the disk.
//
// Coordinate system:
//
//   Zenith:
//       0°   = +x
//       90°  = +z
//       180° = -x
//
//   Azimuth:
//       0°   = +y
//       90°  = left
//       180° = -y
//       270° = right
//
// Bin layout:
//
//   N_AZ_BINS  equal azimuth wedges (default 200, i.e. 1.8° each)
//   N_ZEN_BINS equal-area zenith rings (default 100)
//
// Usage:
//
//   .x azimuth_zenith_histo_poly_2files.cpp("fileA.root", "fileB.root")
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
 
const char* TREE_NAME      = "azimuth_zenith";
const char* ZENITH_BRANCH  = "zenith";
const char* AZIMUTH_BRANCH = "azimuth";
 
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
//   have consistent handedness at their shared 90° equator
//   edge ? equivalent to reflecting the disk across the y-axis.
// ------------------------------------------------------------
 
inline double AzToPlotRad(double az_deg, bool mirror = false)
{
    double phi = (az_deg) * TMath::DegToRad();
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
//
// Each bin is a polygon with:
//   - an outer arc  (r_outer, phi_lo -> phi_hi)
//   - a straight edge at phi_hi  (r_outer -> r_inner)
//   - an inner arc  (r_inner, phi_hi -> phi_lo)
//   - a straight edge at phi_lo  (r_inner -> r_outer)
//
// For the innermost ring (touching the pole), r_inner = 0
// so the polygon degenerates to a pie-slice.
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
// Create TH2Poly and populate all bins
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
    // Labels measured from the centre of the displayed hemisphere.
    double displayZeniths[] = {0, 15, 30, 45, 60, 75, 90};
 
    for (double z : displayZeniths)
    {
        double r;
        double labelZenith;
 
        if (rightHemi)
        {
            // Right hemisphere:
            // centre = 0°
            // edge   = 90°
            labelZenith = z;
            r = EqualAreaRadius(labelZenith, true);
        }
        else
        {
            // Left hemisphere:
            // centre = 180°
            // edge   = 90°
            labelZenith = 180.0 - z;
            r = EqualAreaRadius(labelZenith, false);
        }
 
        TEllipse* ring = new TEllipse(0.0, 0.0, r, r);
        ring->SetFillStyle(0);
        ring->SetLineStyle(2);
        ring->SetLineWidth(1);
        ring->SetLineColor(kBlack);
        ring->Draw("same");
 
        // Label every ring except the centre point
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
 
    // Azimuth spokes
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
    }
    dirs[] =
    {
        {  0, "Az 0#circ"},
        { 90, "Az 90#circ" },
        {180, "Az 180#circ"},
        {270, "Az 270#circ"}
    };
 
    double labelR = 1.25;

    for (auto& d : dirs)
    {
        double phi = AzToPlotRad(d.az, mirror);

	double lx = labelR * std::cos(phi);
	double ly = labelR * std::sin(phi) *.85;
	TLatex* lbl = new TLatex(lx, ly, d.text);
	
	// Pick alignment so the label sits just outside the spoke tip,
	// regardless of which way phi points.
	int hAlign = (lx > 0.15) ? 3 : (lx < -0.15) ? 1 : 2;
	int vAlign = (ly > 0.15) ? 1 : (ly < -0.15) ? 3 : 2;
	int align  = 10 * hAlign + vAlign;

        lbl->SetTextSize(0.025);
        lbl->SetTextAlign(align);
        lbl->Draw("same");
    }
 
    TLatex* ttl = new TLatex(0.0, 1.25, title);
    ttl->SetTextAlign(22);
    ttl->SetTextSize(0.04);
    ttl->SetTextFont(62);
    ttl->Draw("same");
}
 
// ------------------------------------------------------------
// Draw one hemisphere into a given pad
//
// mirror = true  -> flip the azimuthal handedness of the
//                   projected disk (used for the left hemi).
// mirror = false -> standard orientation (used for the right
//                   hemi).
// ------------------------------------------------------------
 
void DrawHemisphere(TTree* tree,
                    bool rightHemi,
                    const char* histName,
                    const char* title,
                    TVirtualPad* pad,
                    bool mirror = false)
{
    TH2Poly* poly = BuildPoly(histName, rightHemi, mirror);
 
    Float_t zenith_val;
    Float_t azimuth_val;
 
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus(ZENITH_BRANCH,  1);
    tree->SetBranchStatus(AZIMUTH_BRANCH, 1);
 
    tree->SetBranchAddress(ZENITH_BRANCH,  &zenith_val);
    tree->SetBranchAddress(AZIMUTH_BRANCH, &azimuth_val);
 
    Long64_t nEntries = tree->GetEntries();
 
    std::cout
        << "[INFO] Processing "
        << nEntries
        << " entries for "
        << histName
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
 
        double r   = EqualAreaRadius(z, rightHemi);
        double phi = AzToPlotRad(a, mirror);
 
        poly->Fill(r * std::cos(phi), r * std::sin(phi));
    }
 
    tree->SetBranchStatus("*", 1);
 
    // --------------------------------------------------------
    // Draw into the provided pad
    // --------------------------------------------------------
 
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
        palette->SetX1NDC(0.925);  // left edge of bar (NDC)
        palette->SetX2NDC(0.95);   // right edge
        palette->SetY1NDC(0.10);   // bottom
        palette->SetY2NDC(0.90);   // top
        pad->Modified();
        pad->Update();
    }
 
    DrawPolarDecorations(rightHemi, title, mirror);
 
    pad->Update();
}
 
// ------------------------------------------------------------
// Open one file, grab its tree, and draw both hemispheres
// for that file into the two given pads (left pad = left
// hemisphere, right pad = right hemisphere).
//
// The left hemisphere is drawn with mirror = true so its
// azimuthal handedness is flipped relative to the right,
// making the two disks read like an unfolded sphere.
//
// rowLabel is prepended to each pad's title, e.g. "File A"
// so the two rows are visually distinguishable.
// ------------------------------------------------------------
 
bool DrawFileRow(const char* filename,
                 const char* rowLabel,
                 const char* histPrefix,
                 TVirtualPad* leftPad,
                 TVirtualPad* rightPad)
{
    TFile* f = TFile::Open(filename, "READ");
 
    if (!f || f->IsZombie())
    {
        std::cerr
            << "[ERROR] Cannot open "
            << filename
            << std::endl;
        return false;
    }
 
    TTree* tree = nullptr;
    f->GetObject(TREE_NAME, tree);
 
    if (!tree)
    {
        std::cerr
            << "[ERROR] TTree '"
            << TREE_NAME
            << "' not found in "
            << filename
            << std::endl;
 
        f->Close();
        return false;
    }
 
    std::cout
        << "[INFO] "
        << filename
        << " Entries = "
        << tree->GetEntries()
        << std::endl;
 
    std::string leftHistName  = std::string(histPrefix) + "_left_hemisphere";
    std::string rightHistName = std::string(histPrefix) + "_right_hemisphere";
 
    std::string leftTitle  = std::string(rowLabel) + ": Italian Side.";
    std::string rightTitle = std::string(rowLabel) + ": French Side.";
 
    // Left pad -> Left hemisphere (-x centre), mirrored azimuth
    DrawHemisphere(
        tree,
        false,
        leftHistName.c_str(),
        leftTitle.c_str(),
        leftPad,
        true);   // <-- mirror = true
 
    // Right pad -> Right hemisphere (+x centre), standard azimuth
    DrawHemisphere(
        tree,
        true,
        rightHistName.c_str(),
        rightTitle.c_str(),
        rightPad,
        false);  // <-- mirror = false
 
 	f->Close();
 
    return true;
}
 
// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
 
void polarHisto_angles_ENERGY_PLACEHOLDER1_Source_SOURCE_PLACEHOLDER(
    const char* filenameA = "uniform_sphere_A.root",
    const char* filenameB = "uniform_sphere_B.root")
{
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPalette(kBird);
 
    // --------------------------------------------------------
    // One canvas, 2x2 grid:
    //   pad 1 = File A, left hemisphere
    //   pad 2 = File A, right hemisphere
    //   pad 3 = File B, left hemisphere
    //   pad 4 = File B, right hemisphere
    // --------------------------------------------------------
 
    TCanvas* c =
        new TCanvas("c_both_files", "Hemisphere Plots (2 files)", 2200, 2200);
 
    c->Divide(2, 2);
 
    bool okA = DrawFileRow(
        filenameA,
        "Source SOURCE_PLACEHOLDER (Env., ENERGY_PLACEHOLDER2)",
        "fileA",
        c->GetPad(1),
        c->GetPad(2));
 
    bool okB = DrawFileRow(
        filenameB,
        "Source SOURCE_PLACEHOLDER (No env., ENERGY_PLACEHOLDER2)",
        "fileB",
        c->GetPad(3),
        c->GetPad(4));
 
    if (!okA && !okB)
    {
        std::cerr << "[ERROR] Neither file could be processed. Aborting." << std::endl;
        return;
    }
 
    c->Update();
    c->SaveAs("polarHisto_ENERGY_PLACEHOLDER1_Source_SOURCE_PLACEHOLDER.png");
 
    std::cout
        << "[INFO] Saved polarHisto_ENERGY_PLACEHOLDER1_Source_SOURCE_PLACEHOLDER_histo.png"
        << std::endl;
 
    std::cout
        << "[INFO] Done."
        << std::endl;
}
