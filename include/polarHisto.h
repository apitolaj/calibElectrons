#pragma once

//function to take azimuth angle of the electron and convert it to radians accounting for hemisphere differences.
//this work uses the convention that the azimuth angle increases in the postive direction CCW as viewed from the +x French side. 
inline double AzToPlotRad(double az_deg, bool mirror = false)
{
	//convert the inputted azimuth angle to degrees.
	double phi = (az_deg) * TMath::DegToRad();
	
	//if mirroring to the Italian hemisphere, we reflect across the yz plane; this warrants Pi() - phi as this flips the sign of cos but maintains the sign of sin.
	return mirror ? TMath::Pi() - phi : phi;
}

//function to take zenith angle of the electron and convert it to a radius value on the polar plot.
//this function is used by the AddWedgeBin function to construct the upper and lower radial bounds for each bin (i.e., "wedge") on the polar plot to ensure each bin on the 2D plot covers the same 3D solid angle.
double EqualAreaRadius(double zenith_deg, bool rightHemi)
{
	//conversion of the inputted zenith value to radians.
	double theta = zenith_deg * TMath::DegToRad();
	
	//depending on the hemisphere the electron is in, calculate and return the correct radius value.
	//the explanation and derivation of the following formulas are given in the accompanying project report in the calibElectrons github repository.
 	if (rightHemi)
        	return std::sqrt(1.0 - std::cos(theta));
 
    	return std::sqrt(1.0 + std::cos(theta));
}
 
//function to create a wedge bin on the polar plot.
void AddWedgeBin(TH2Poly* poly, bool rightHemi, double zenLo, double zenHi, double azLo, double azHi, bool mirror = false)
{

	//declare the inner and outer radius bounds of the bin and calculate their value according to the EqualAreaRadius function.
	double r_inner, r_outer;
 	
 	//check if the electron is in the right or left hemisphere before the calculation to ensure the correct formula is used in the EqualAreaRadius function.
 	if (rightHemi)
	{
        	r_inner = EqualAreaRadius(zenLo, true);
        	r_outer = EqualAreaRadius(zenHi, true);
	}
	else	
	{
        	r_inner = EqualAreaRadius(zenLo, false);
        	r_outer = EqualAreaRadius(zenHi, false); 
	}
	
	//ensure that r_outer is larger than r_inner.
	if (r_inner > r_outer) std::swap(r_inner, r_outer);
 
 	//declare and convert the phi coordinate bounds to radians from the inputted azimuth values.
	double phi_lo = AzToPlotRad(azLo, mirror);
	double phi_hi = AzToPlotRad(azHi, mirror);
 
 	//declaration of a standard Cartesian x, y vector; this will be used to store positions of the points that lay on the inner and outer wedge arc. 
	std::vector<double> px, py;
 
	//create the outer arc of the bin.
	//loop to fill the arc with points; the fineness of the arc is determined by the arc steps, which the user has flexibility to change at the beginning of the script.
	for (int k = 0; k <= ARC_STEPS; ++k)
	{
		//creation of a positive value that is <=1.
		double t   = static_cast<double>(k) / ARC_STEPS;
		
		//utilization of the t value to pick an incremental increase in phi from phi_lo. When k=ARC_STEPS at the end of the loop, phi=phi_hi.
		double phi = phi_lo + t * (phi_hi - phi_lo);
	 
	 	//append to the Cartesian vector a point that is on the outer arc; this is a simple calculation done by finding the x and y components of r_outer with the incremented phi angle.  
		px.push_back(r_outer * std::cos(phi));
		py.push_back(r_outer * std::sin(phi));
	}
 
	//create the inner arc of the bin.
	//check if r_inner is negligibly small and simply treat the "arc" as a point on the origin; this is done as the origin is a degenerate point that multiple bins will share. 
	if (r_inner < 1e-9)
	{
		px.push_back(0.0);
		py.push_back(0.0);
	}
	
	//if the inner radius is larger then the critical size value, calculate points on the arc identically to how was done with the outer arc and store them in the Cartesian x, y vector.
	else
	{
        	for (int k = 0; k <= ARC_STEPS; ++k)
        	{
			double t   = static_cast<double>(k) / ARC_STEPS;
			double phi = phi_hi - t * (phi_hi - phi_lo);
			 
			px.push_back(r_inner * std::cos(phi));
			py.push_back(r_inner * std::sin(phi));
        	}
	}
 
 	//utilize the ROOT TH2DPoly class to draw the wedge with the data points from the Cartesian vector. 
	poly->AddBin(static_cast<int>(px.size()), px.data(), py.data());
	
}
 
//function to create the histogram object and create its wedge bins.
TH2Poly* BuildPoly(const char* name, bool rightHemi, bool mirror = false)
{
	//create a TH2Poly histogram object and set its name and title.
	TH2Poly* poly = new TH2Poly();
	poly->SetName(name);
	poly->SetTitle(name);
	
	//create an incremental step variable for the azimuth direction, which spans a full 360 degrees.
	double azStep = 360.0 / N_AZ_BINS;
	
	//create an incremental step variable for the zenith direction; the range of cos(zenith) for a single hemipshere (0 < zenith < 90) is used to ensure each bin covers equal solid angle.
	double muStep = 1.0 / N_ZEN_BINS;
 
 	//begin looping over all the zenith bins, starting with the zenith direction.
 	for (int iz = 0; iz < N_ZEN_BINS; ++iz)
	{
		//declare two variables for the upper and lower bound for zenith in the cos(zenith) space.
		double muLo, muHi;
 
 		//set the values for muLo and muHi depending on which hemisphere the electron is in. 
		if (rightHemi)
		{
			//set muHi and muLo to be offset by one muStep with muHi > muLo; the values are anchored at 1.0 as this is the maximum value of cos(zenith) in the right hemisphere.
	     		muHi = 1.0 - iz * muStep;
			muLo = 1.0 - (iz + 1) * muStep;
		}
		else
		{
			//set muHi and muLo to be offset by one muStep with muHi > muLo; the values are anchored at 0 as this is the maximum value of cos(zenith) in the left hemisphere.
			muHi = -iz * muStep;
			muLo = -(iz + 1) * muStep;
		}
	 
	 	//calculate zenHi and zenLo from muHi and muLo, respectively, by utilizing arccos; the angle is converted to degrees.
		double zenLo = std::acos(muHi) * TMath::RadToDeg();
		double zenHi = std::acos(muLo) * TMath::RadToDeg();
	 	
	 	//inside the zenith bin loop, loop over all the azimuth bins.
		for (int ia = 0; ia < N_AZ_BINS; ++ia)
		{
			//define azLo and azHi analogously to how muHi and muLo are; the values are already in degrees and do not need to be converted. 
			double azLo = ia * azStep;
			double azHi = (ia + 1) * azStep;
	 
	 		//call the AddWedgeBin function to create the bin and add it to the histogram.
			AddWedgeBin(poly, rightHemi, zenLo, zenHi, azLo, azHi, mirror);
		}
	}
 
 	//return the final output that looped over all the zenith and azimuth steps to create a full histogram. 
	return poly;
}

//function to draw cosmetic axis grids and labels for the histogram.
void DrawPolarDecorations(bool rightHemi, const char* title, bool mirror = false)
{

	//create an array of the desired zenith labels; these will be reflected to the corresponding 90->180 values for the left hemisphere.
	double displayZeniths[] = {0, 15, 30, 45, 60, 75, 90};
 
	//loop over each zenith label in the array.
	for (double z : displayZeniths)
	{
		//define a variable to hold the zenith label and one to hold the radius value on the plot that the label should be. 
		double r;
		double labelZenith;
 
		if (rightHemi)
		{
			//if on the right hemipshere, set the zenith label to the value in the array and calculate the radius value.
		    labelZenith = z;
		    r = EqualAreaRadius(labelZenith, true);
		}
		
		else
		{
			//if not on the right hemisphere, calculate the reflected zenith label value and calculate the radius value. 
		    labelZenith = 180.0 - z;
		    r = EqualAreaRadius(labelZenith, false);
		}
	 
		//create and draw an ring (circle) centered at the origin that has a radius of the previous calculated value.
		TEllipse* ring = new TEllipse(0.0, 0.0, r, r);
		ring->SetFillStyle(0);
		ring->SetLineStyle(2);
		ring->SetLineWidth(1);
		ring->SetLineColor(kBlack);
		ring->Draw("same");
	 
		//draw the label if the zenith value is not equal to 0.
		if (z > 0.0)
		{
			if(z == 15 || z == 45 | z==75 || z==90) continue;

			double lx = r - 0.1;
			if (rightHemi) lx = -(r - 0.1);   // reflect across y-axis on the right hemisphere

			TLatex* lbl = new TLatex(lx, 0.01, Form("%.0f#circ", labelZenith));
			lbl->SetTextSize(0.035);
			lbl->SetTextColor(kWhite);			
			lbl->Draw("same");
		}
	}
 
	//draw the spokes to the plot at 30 degree intervals. 
	for (int deg = 0; deg < 360; deg += 30)
	{
		//define the phi value that the spoke runs through.
		double phi = AzToPlotRad(deg, mirror);

		//draw a line between the origin and the point about the rings at the phi value. 
		TLine* spoke = new TLine(0.0, 0.0, std::cos(phi), std::sin(phi));
		spoke->SetLineStyle(2);
		spoke->SetLineWidth(1);
		spoke->SetLineColor(kBlack);
		spoke->Draw("same");
	}
 
	//create an array of a custum data structure to hold the azimuth value, text label, and alignment value.
	struct
	{
		double az;
		const char* text;
		int align;
	}

	dirs[] =
	{ 
		{  0, "Az 0#circ"     , 21},
		{ 90, "Az 90#circ"    , 21},
		{180, "Az 180#circ"   , 21},
		{270, "Az 270#circ"   , 21}
	};
 
	double labelR = 1.2;

	//tunable vertical pull-in factors ? separate for top (Az90) and bottom (Az270) labels
	double topScale    = mirror ? 0.90 : 0.90;   // right hemi labels were too high -> pull in more (0.85)
	double bottomScale = mirror ? 0.95 : 0.95;   // left hemi bottom -> push down a touch (increase toward 1.0)

	for (auto& d : dirs)
	{
		double phi = AzToPlotRad(d.az, mirror);

		double lx = labelR * std::cos(phi);
		double sinPhi = std::sin(phi);

		double ly;
		if (sinPhi > 1e-9)        ly = labelR * sinPhi * topScale;     // Az 90 (top)
		else if (sinPhi < -1e-9)  ly = labelR * sinPhi * bottomScale;  // Az 270 (bottom)
		else                      ly = 0.0;                            // Az 0 / Az 180 (unaffected)

		TLatex* lbl = new TLatex(lx, ly, d.text);
		lbl->SetTextSize(0.035);
		lbl->SetTextAlign(d.align);
		lbl->SetTextColor(kBlack);
		lbl->Draw("same");
	}
 
	//create and draw the histogram title.
	TLatex* ttl = new TLatex(0.0, 1.25, title);
	ttl->SetTextAlign(22);
	ttl->SetTextSize(0.038);
	ttl->SetTextFont(62);
	ttl->Draw("same");
} 
 
//function to fill the hemisphere plot with entries.
TH2Poly* FillHemispherePoly(TTree* tree, bool rightHemi, const char* histName, bool mirror, bool useEnergyCut, double eLo, double eHi)
{
	//create a polygon object and define variables for the zenith, azimuth, and energy values for the function's inputted tree entries. 
	TH2Poly* poly = BuildPoly(histName, rightHemi, mirror);
 
	double  zenith_val;
	double  azimuth_val;
	double energy_val = 0.0;
 	
	//only set the azimuth, zenith, and energy (if applicable) branches active and tie via a reference their values to the previously defined variables.
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
 
	//get the total number of entries in the tree and print to the console that they are being processed. 
	Long64_t nEntries = tree->GetEntries();

	std::cout << "[INFO] Processing " << nEntries << " entries for " << histName << (useEnergyCut ? " (energy cut applied)" : "") << std::endl;
 
	//loop over each entry in the tree and fill it to a bin in the histogram plot.
	for (Long64_t i = 0; i < nEntries; ++i)
	{
	
		//get the tree entry.
		tree->GetEntry(i);
		
		double z = zenith_val;
		double a = azimuth_val;
		 
		//verify that the entry's zenith values are consistent with the hemisphere it is in, skipping the event competely if not. 
		if (rightHemi)
		{
			if (z < 0.0 || z > 90.0) continue;
		}
		
		else
		{
			if (z < 90.0 || z > 180.0) continue;
		}
		
		//verify that the entry is in the desired energy range, skipping the evernt completely if not.
		if (useEnergyCut)
		{
			if (energy_val < eLo || energy_val > eHi) continue;
		}
	 
		//calculate the radius and phi value of the event on the histogram geometry, and then fill a bin at this location on the plot. 
		double r   = EqualAreaRadius(z, rightHemi);
		double phi = AzToPlotRad(a, mirror);
		 
		poly->Fill(r * std::cos(phi), r * std::sin(phi));
	}

	//after the fill loop, set all branch back on.
	tree->SetBranchStatus("*", 1);
 
	return poly;
}
 
//function to draw a given polygon histogram object on a canvas pad.
void StyleAndDrawPoly(TH2Poly* poly, const char* title, TVirtualPad* pad, bool rightHemi, bool mirror)
{

	//switch into the canvas pad and draw the polygon object ensuring that any object outer borders are invisible.
	pad->cd();
	pad->SetRightMargin(0.25);
	pad->SetLeftMargin(0.13);
	pad->SetTopMargin(0.20);
	 
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
	 
	//create the histogram color-key legend and draw it with modified dimensions.
	TPaletteAxis* palette = (TPaletteAxis*)poly->GetListOfFunctions()->FindObject("palette");
	 
	if (palette)
	{
		palette->SetX1NDC(0.875);
		palette->SetX2NDC(0.90);
		palette->SetY1NDC(0.10);
		palette->SetY2NDC(0.90);
		pad->Modified();
		pad->Update();
	}
	 
	//draw the custom circle rings/spokes/labels for the plot.
	DrawPolarDecorations(rightHemi, title, mirror);
	 
	pad->Update();
}

//function to take two polygon histogram objects and add each bin.
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
 
//function to take two polygon histogram objects and divide each bin.
TH2Poly* DivideHemispheres(TH2Poly* num, TH2Poly* den, const char* name)
{
	TH2Poly* ratio = (TH2Poly*)num->Clone(name);
	 
	int nbins = num->GetNumberOfBins();
	
	for (int i = 1; i <= nbins; ++i)
	{
		double n = num->GetBinContent(i);
		double d = den->GetBinContent(i);
		
		//ensure that a divide by 0 error will not occur.
		ratio->SetBinContent(i, d != 0.0 ? n / d : 0.0);
	}
	 
	return ratio;
}

//function to put draw the left and right histograms next to each other.
bool DrawTreeRow(TFile* f, const char* treeName, const char* rowLabel, const char* histPrefix, TVirtualPad* leftPad, TVirtualPad* rightPad, TH2Poly*& leftPolyOut, TH2Poly*& rightPolyOut)
{
	TTree* tree = nullptr;
	f->GetObject(treeName, tree);

	if (!tree)
	{
		std::cerr << "[ERROR] TTree '" << treeName << "' not found in file" << std::endl;
		return false;
	}

	std::cout << "[INFO] Tree '" << treeName << "' Entries = " << tree->GetEntries() << std::endl;

	std::string leftHistName  = std::string(histPrefix) + "_left_hemisphere";
	std::string rightHistName = std::string(histPrefix) + "_right_hemisphere";

	std::string leftTitle  = std::string(rowLabel) + ": Italian Side.";
	std::string rightTitle = std::string(rowLabel) + ": French Side.";

	// --- Fill BOTH histograms first, don't draw yet ---
	leftPolyOut  = FillHemispherePoly(tree, false, leftHistName.c_str(),  true,  false, 0.0, 0.0);
	rightPolyOut = FillHemispherePoly(tree, true,  rightHistName.c_str(), false, false, 0.0, 0.0);

	// --- Compute a shared color scale from both ---
	double globalMax = std::max(leftPolyOut->GetMaximum(), rightPolyOut->GetMaximum());
	double globalMin = 0.0;  // force min to 0 since these are counts; avoids empty-bin weirdness

	leftPolyOut->SetMinimum(globalMin);
	leftPolyOut->SetMaximum(globalMax);
	rightPolyOut->SetMinimum(globalMin);
	rightPolyOut->SetMaximum(globalMax);

	// --- Now draw both with matching scales ---
	StyleAndDrawPoly(leftPolyOut, leftTitle.c_str(), leftPad, false, true);
	StyleAndDrawPoly(rightPolyOut, rightTitle.c_str(), rightPad, true, false);

	return true;
}
 
