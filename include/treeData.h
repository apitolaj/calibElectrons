#pragma once

//custom data structure that is used to store the electron characteristics.
struct treeData
{
	TTree* envelope;
	TTree* noEnvelope;

	double azimuth;
	double zenith;
	double DistOM;
	double DistTPP;
	double TPPy;
	double TPPz;
	double SDEnergyLoss;
	double SDInitialEnergy;
	double electronEnergy;

	int eventNumber;

	bool isEdgeTPP;
};

//inline function to create to a ROOT file two data trees that follow the treeData data structure. 
inline void makeTrees(TFile* outFile, treeData& trees) 
{
	//change into the inputted ROOT file.
    outFile->cd();

	//reasign the null value asssociated with the trees treeData structure to actual trees.
    trees.envelope   = new TTree("envelope", "envelope");
    trees.noEnvelope = new TTree("noEnvelope", "noEnvelope");

	//for both trees, create branches that mimic the variables from the treeData structure. 
    for (TTree* t : {trees.envelope, trees.noEnvelope}) 
	{
        t->Branch("eventNum"	    ,   &trees.eventNumber	    ,  "eventNumber/I");
        t->Branch("azimuth" 	    ,   &trees.azimuth    	    ,  "azimuth/D");
        t->Branch("zenith" 	        ,   &trees.zenith    	    ,  "zenith/D");
		t->Branch("DistOM"  	    ,   &trees.DistOM    	    ,  "DistOM/D");
        t->Branch("DistTPP" 	    ,   &trees.DistTPP   	    ,  "DistTPP/D");
        t->Branch("TPPy"	        ,   &trees.TPPy     	    ,  "TPPy/D");
        t->Branch("TPPz"    	    ,   &trees.TPPz     	    ,  "TPPz/D");
        t->Branch("isEdgeTPP"  	    ,   &trees.isEdgeTPP       	,  "isEdgeTPP/O");
        t->Branch("electronEnergy"  ,   &trees.electronEnergy   ,  "electronEnergy/D");
    }
}
