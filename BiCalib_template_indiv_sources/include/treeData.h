#pragma once

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
	bool isEdgeTPP;
	double SDEnergyLoss;
	double SDInitialEnergy;
	
	double electronEnergy;

	int eventNumber;

};


inline void makeTrees(TFile* outFile, treeData& trees) 
{

    outFile->cd();

    trees.envelope   = new TTree("envelope", "envelope");
    trees.noEnvelope = new TTree("noEnvelope", "noEnvelope");

    for (TTree* t : {trees.envelope, trees.noEnvelope}) 
	{
        t->Branch("eventNum"	    ,   &trees.eventNumber	,  "eventNumber/I");
        t->Branch("azimuth" 	    ,   &trees.azimuth    	,  "azimuth/D");
        t->Branch("zenith" 	    ,   &trees.zenith    	,  "zenith/D");
	t->Branch("DistOM"  	    ,   &trees.DistOM    	,  "DistOM/D");
        t->Branch("DistTPP" 	    ,   &trees.DistTPP   	,  "DistTPP/D");
        t->Branch("TPPy"   	    ,   &trees.TPPy     	,  "TPPy/D");
        t->Branch("TPPz"    	    ,   &trees.TPPz     	,  "TPPz/D");;
        t->Branch("isEdgeTPP"  	    ,   &trees.isEdgeTPP       	,  "isEdgeTPP/O");
        t->Branch("electronEnergy"  ,   &trees.electronEnergy   ,  "electronEnergy/D");
    }
}
