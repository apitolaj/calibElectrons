#pragma once
#include <unordered_map>
#include <string>

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

	int eventNumber;

};

bool isCalibElectron_envelope(MiEvent *Event);

bool isCalibElectron_noEnvelope(MiEvent *Event);

bool envelopeInteraction(MiEvent *Event);

float calculateZenith(MiEvent *Event);

float calculateAzimuth(MiEvent *Event);

MiVector3D* calculatePosTPPVector(MiEvent *Event);

void populateChain(TChain &ROOTChain, const char *dirPath);

bool isEnergyBetween600_800keV(MiEvent *Event);

float calculateDistTPP(MiEvent *Event, MiVector3D *calibSourceVertexPos);

float calculateDistOM(MiEvent *Event, MiVector3D *calibrationSourceVertexPos);

double SDEnergyLoss(MiEvent *Event);

double SDInitialEnergy(MiEvent *Event);

bool isEdgeTPP(MiEvent *Event);

inline void makeTrees(TFile* outFile, treeData& trees); 

inline const std::unordered_map<std::string, std::pair<double, double>> omPositions = {
    {"0", {-2460.5, -1554.0}},
    {"1", {-2460.5, -1295.0}},
    {"2", {-2460.5, -1036.0}},
    {"3", {-2460.5, -777.0}},
    {"4", {-2460.5, -518.0}},
    {"5", {-2460.5, -259.0}},
    {"6", {-2460.5, 0.0}},
    {"7", {-2460.5, 259.0}},
    {"8", {-2460.5, 518.0}},
    {"9", {-2460.5, 777.0}},
    {"10", {-2460.5, 1036.0}},
    {"11", {-2460.5, 1295.0}},
    {"12", {-2460.5, 1554.0}},
    {"13", {-2201.5, -1554.0}},
    {"14", {-2201.5, -1295.0}},
    {"15", {-2201.5, -1036.0}},
    {"16", {-2201.5, -777.0}},
    {"17", {-2201.5, -518.0}},
    {"18", {-2201.5, -259.0}},
    {"19", {-2201.5, 0.0}},
    {"20", {-2201.5, 259.0}},
    {"21", {-2201.5, 518.0}},
    {"22", {-2201.5, 777.0}},
    {"23", {-2201.5, 1036.0}},
    {"24", {-2201.5, 1295.0}},
    {"25", {-2201.5, 1554.0}},
    {"26", {-1942.5, -1554.0}},
    {"27", {-1942.5, -1295.0}},
    {"28", {-1942.5, -1036.0}},
    {"29", {-1942.5, -777.0}},
    {"30", {-1942.5, -518.0}},
    {"31", {-1942.5, -259.0}},
    {"32", {-1942.5, 0.0}},
    {"33", {-1942.5, 259.0}},
    {"34", {-1942.5, 518.0}},
    {"35", {-1942.5, 777.0}},
    {"36", {-1942.5, 1036.0}},
    {"37", {-1942.5, 1295.0}},
    {"38", {-1942.5, 1554.0}},
    {"39", {-1683.5, -1554.0}},
    {"40", {-1683.5, -1295.0}},
    {"41", {-1683.5, -1036.0}},
    {"42", {-1683.5, -777.0}},
    {"43", {-1683.5, -518.0}},
    {"44", {-1683.5, -259.0}},
    {"45", {-1683.5, 0.0}},
    {"46", {-1683.5, 259.0}},
    {"47", {-1683.5, 518.0}},
    {"48", {-1683.5, 777.0}},
    {"49", {-1683.5, 1036.0}},
    {"50", {-1683.5, 1295.0}},
    {"51", {-1683.5, 1554.0}},
    {"52", {-1424.5, -1554.0}},
    {"53", {-1424.5, -1295.0}},
    {"54", {-1424.5, -1036.0}},
    {"55", {-1424.5, -777.0}},
    {"56", {-1424.5, -518.0}},
    {"57", {-1424.5, -259.0}},
    {"58", {-1424.5, 0.0}},
    {"59", {-1424.5, 259.0}},
    {"60", {-1424.5, 518.0}},
    {"61", {-1424.5, 777.0}},
    {"62", {-1424.5, 1036.0}},
    {"63", {-1424.5, 1295.0}},
    {"64", {-1424.5, 1554.0}},
    {"65", {-1165.5, -1554.0}},
    {"66", {-1165.5, -1295.0}},
    {"67", {-1165.5, -1036.0}},
    {"68", {-1165.5, -777.0}},
    {"69", {-1165.5, -518.0}},
    {"70", {-1165.5, -259.0}},
    {"71", {-1165.5, 0.0}},
    {"72", {-1165.5, 259.0}},
    {"73", {-1165.5, 518.0}},
    {"74", {-1165.5, 777.0}},
    {"75", {-1165.5, 1036.0}},
    {"76", {-1165.5, 1295.0}},
    {"77", {-1165.5, 1554.0}},
    {"78", {-906.5, -1554.0}},
    {"79", {-906.5, -1295.0}},
    {"80", {-906.5, -1036.0}},
    {"81", {-906.5, -777.0}},
    {"82", {-906.5, -518.0}},
    {"83", {-906.5, -259.0}},
    {"84", {-906.5, 0.0}},
    {"85", {-906.5, 259.0}},
    {"86", {-906.5, 518.0}},
    {"87", {-906.5, 777.0}},
    {"88", {-906.5, 1036.0}},
    {"89", {-906.5, 1295.0}},
    {"90", {-906.5, 1554.0}},
    {"91", {-647.5, -1554.0}},
    {"92", {-647.5, -1295.0}},
    {"93", {-647.5, -1036.0}},
    {"94", {-647.5, -777.0}},
    {"95", {-647.5, -518.0}},
    {"96", {-647.5, -259.0}},
    {"97", {-647.5, 0.0}},
    {"98", {-647.5, 259.0}},
    {"99", {-647.5, 518.0}},
    {"100", {-647.5, 777.0}},
    {"101", {-647.5, 1036.0}},
    {"102", {-647.5, 1295.0}},
    {"103", {-647.5, 1554.0}},
    {"104", {-388.5, -1554.0}},
    {"105", {-388.5, -1295.0}},
    {"106", {-388.5, -1036.0}},
    {"107", {-388.5, -777.0}},
    {"108", {-388.5, -518.0}},
    {"109", {-388.5, -259.0}},
    {"110", {-388.5, 0.0}},
    {"111", {-388.5, 259.0}},
    {"112", {-388.5, 518.0}},
    {"113", {-388.5, 777.0}},
    {"114", {-388.5, 1036.0}},
    {"115", {-388.5, 1295.0}},
    {"116", {-388.5, 1554.0}},
    {"117", {-129.5, -1554.0}},
    {"118", {-129.5, -1295.0}},
    {"119", {-129.5, -1036.0}},
    {"120", {-129.5, -777.0}},
    {"121", {-129.5, -518.0}},
    {"122", {-129.5, -259.0}},
    {"123", {-129.5, 0.0}},
    {"124", {-129.5, 259.0}},
    {"125", {-129.5, 518.0}},
    {"126", {-129.5, 777.0}},
    {"127", {-129.5, 1036.0}},
    {"128", {-129.5, 1295.0}},
    {"129", {-129.5, 1554.0}},
    {"130", {129.5, -1554.0}},
    {"131", {129.5, -1295.0}},
    {"132", {129.5, -1036.0}},
    {"133", {129.5, -777.0}},
    {"134", {129.5, -518.0}},
    {"135", {129.5, -259.0}},
    {"136", {129.5, 0.0}},
    {"137", {129.5, 259.0}},
    {"138", {129.5, 518.0}},
    {"139", {129.5, 777.0}},
    {"140", {129.5, 1036.0}},
    {"141", {129.5, 1295.0}},
    {"142", {129.5, 1554.0}},
    {"143", {388.5, -1554.0}},
    {"144", {388.5, -1295.0}},
    {"145", {388.5, -1036.0}},
    {"146", {388.5, -777.0}},
    {"147", {388.5, -518.0}},
    {"148", {388.5, -259.0}},
    {"149", {388.5, 0.0}},
    {"150", {388.5, 259.0}},
    {"151", {388.5, 518.0}},
    {"152", {388.5, 777.0}},
    {"153", {388.5, 1036.0}},
    {"154", {388.5, 1295.0}},
    {"155", {388.5, 1554.0}},
    {"156", {647.5, -1554.0}},
    {"157", {647.5, -1295.0}},
    {"158", {647.5, -1036.0}},
    {"159", {647.5, -777.0}},
    {"160", {647.5, -518.0}},
    {"161", {647.5, -259.0}},
    {"162", {647.5, 0.0}},
    {"163", {647.5, 259.0}},
    {"164", {647.5, 518.0}},
    {"165", {647.5, 777.0}},
    {"166", {647.5, 1036.0}},
    {"167", {647.5, 1295.0}},
    {"168", {647.5, 1554.0}},
    {"169", {906.5, -1554.0}},
    {"170", {906.5, -1295.0}},
    {"171", {906.5, -1036.0}},
    {"172", {906.5, -777.0}},
    {"173", {906.5, -518.0}},
    {"174", {906.5, -259.0}},
    {"175", {906.5, 0.0}},
    {"176", {906.5, 259.0}},
    {"177", {906.5, 518.0}},
    {"178", {906.5, 777.0}},
    {"179", {906.5, 1036.0}},
    {"180", {906.5, 1295.0}},
    {"181", {906.5, 1554.0}},
    {"182", {1165.5, -1554.0}},
    {"183", {1165.5, -1295.0}},
    {"184", {1165.5, -1036.0}},
    {"185", {1165.5, -777.0}},
    {"186", {1165.5, -518.0}},
    {"187", {1165.5, -259.0}},
    {"188", {1165.5, 0.0}},
    {"189", {1165.5, 259.0}},
    {"190", {1165.5, 518.0}},
    {"191", {1165.5, 777.0}},
    {"192", {1165.5, 1036.0}},
    {"193", {1165.5, 1295.0}},
    {"194", {1165.5, 1554.0}},
    {"195", {1424.5, -1554.0}},
    {"196", {1424.5, -1295.0}},
    {"197", {1424.5, -1036.0}},
    {"198", {1424.5, -777.0}},
    {"199", {1424.5, -518.0}},
    {"200", {1424.5, -259.0}},
    {"201", {1424.5, 0.0}},
    {"202", {1424.5, 259.0}},
    {"203", {1424.5, 518.0}},
    {"204", {1424.5, 777.0}},
    {"205", {1424.5, 1036.0}},
    {"206", {1424.5, 1295.0}},
    {"207", {1424.5, 1554.0}},
    {"208", {1683.5, -1554.0}},
    {"209", {1683.5, -1295.0}},
    {"210", {1683.5, -1036.0}},
    {"211", {1683.5, -777.0}},
    {"212", {1683.5, -518.0}},
    {"213", {1683.5, -259.0}},
    {"214", {1683.5, 0.0}},
    {"215", {1683.5, 259.0}},
    {"216", {1683.5, 518.0}},
    {"217", {1683.5, 777.0}},
    {"218", {1683.5, 1036.0}},
    {"219", {1683.5, 1295.0}},
    {"220", {1683.5, 1554.0}},
    {"221", {1942.5, -1554.0}},
    {"222", {1942.5, -1295.0}},
    {"223", {1942.5, -1036.0}},
    {"224", {1942.5, -777.0}},
    {"225", {1942.5, -518.0}},
    {"226", {1942.5, -259.0}},
    {"227", {1942.5, 0.0}},
    {"228", {1942.5, 259.0}},
    {"229", {1942.5, 518.0}},
    {"230", {1942.5, 777.0}},
    {"231", {1942.5, 1036.0}},
    {"232", {1942.5, 1295.0}},
    {"233", {1942.5, 1554.0}},
    {"234", {2201.5, -1554.0}},
    {"235", {2201.5, -1295.0}},
    {"236", {2201.5, -1036.0}},
    {"237", {2201.5, -777.0}},
    {"238", {2201.5, -518.0}},
    {"239", {2201.5, -259.0}},
    {"240", {2201.5, 0.0}},
    {"241", {2201.5, 259.0}},
    {"242", {2201.5, 518.0}},
    {"243", {2201.5, 777.0}},
    {"244", {2201.5, 1036.0}},
    {"245", {2201.5, 1295.0}},
    {"246", {2201.5, 1554.0}},
    {"247", {2460.5, -1554.0}},
    {"248", {2460.5, -1295.0}},
    {"249", {2460.5, -1036.0}},
    {"250", {2460.5, -777.0}},
    {"251", {2460.5, -518.0}},
    {"252", {2460.5, -259.0}},
    {"253", {2460.5, 0.0}},
    {"254", {2460.5, 259.0}},
    {"255", {2460.5, 518.0}},
    {"256", {2460.5, 777.0}},
    {"257", {2460.5, 1036.0}},
    {"258", {2460.5, 1295.0}},
    {"259", {2460.5, 1554.0}},
};

inline const std::unordered_map<std::string, std::pair<double, double>> sourcePositions = {
    {"Source_0_6", {-2089.0, 1272.0}},
    {"Source_1_6", {-1255.0, 1280.0}},
    {"Source_2_6", {-418.0,  1282.0}},
    {"Source_3_6", { 417.0,  1279.0}},
    {"Source_4_6", { 1253.5, 1284.0}},
    {"Source_5_6", { 2088.0, 1278.0}},

    {"Source_0_5", {-2088.0, 848.0}},
    {"Source_1_5", {-1255.0, 853.0}},
    {"Source_2_5", {-419.0,  857.0}},
    {"Source_3_5", { 417.0,  856.0}},
    {"Source_4_5", { 1254.0, 854.0}},
    {"Source_5_5", { 2087.5, 850.0}},

    {"Source_0_4", {-2088.0, 427.0}},
    {"Source_1_4", {-1254.0, 433.0}},
    {"Source_2_4", {-418.5,  436.0}},
    {"Source_3_4", { 417.5,  431.0}},
    {"Source_4_4", { 1254.0, 428.0}},
    {"Source_5_4", { 2087.0, 422.0}},

    {"Source_0_3", {-2088.0, 0.0}},
    {"Source_1_3", {-1254.0, 0.0}},
    {"Source_2_3", {-418.0,  0.0}},
    {"Source_3_3", { 418.0,  0.0}},
    {"Source_4_3", { 1254.0, 0.0}},
    {"Source_5_3", { 2088.0, 0.0}},

    {"Source_0_2", {-2088.0, -434.0}},
    {"Source_1_2", {-1254.0, -424.0}},
    {"Source_2_2", {-418.0,  -422.0}},
    {"Source_3_2", { 418.5,  -418.0}},
    {"Source_4_2", { 1254.0, -424.0}},
    {"Source_5_2", { 2087.0, -426.0}},

    {"Source_0_1", {-2088.0, -861.0}},
    {"Source_1_1", {-1254.0, -842.0}},
    {"Source_2_1", {-419.0,  -841.0}},
    {"Source_3_1", { 419.5,  -848.0}},
    {"Source_4_1", { 1254.5, -848.0}},
    {"Source_5_1", { 2087.5, -852.0}},

    {"Source_0_0", {-2087.0, -1281.0}},
    {"Source_1_0", {-1252.5, -1276.0}},
    {"Source_2_0", {-419.5,  -1279.0}},
    {"Source_3_0", { 420.0,  -1270.0}},
    {"Source_4_0", { 1255.0, -1275.0}},
    {"Source_5_0", { 2087.0, -1277.0}},
};

//-----------------------------------------------------------------------


bool isEdgeTPP(MiEvent *Event)
{
	double edgeCutRange = 128;
	double edgeDistFromCenter = 131;
	
	double OMTPPy = 0.0;
	double OMTPPz = 0.0;
	
	vector<MiVertex>* vertexVector = Event->getPTD()->getpart(0)->getvertexv();
	for(MiVertex& vertex: *vertexVector)
	{
		if(vertex.getpos()=="calo")
		{
			OMTPPy = vertex.getr()->getY();
			OMTPPz = vertex.getr()->getZ();
			break;
		}
		else if (vertex.getpos()=="xcalo" || vertex.getpos()=="gveto") return false;
	}

	auto inEdgeBand = [&](double delta)
	{
	    double d = std::fabs(delta);
	    return edgeCutRange < d && d < edgeDistFromCenter;
	};

	for (int ModuleID = 0; ModuleID < 260; ModuleID++)
	{
	    auto [OMy, OMz] = omPositions.at(std::to_string(ModuleID));

	    if (inEdgeBand(OMTPPy - OMy) || inEdgeBand(OMTPPz - OMz))
	    {
		return true;
	    }
	}
	
	return false; 
}

double SDEnergyLoss(MiEvent *Event)
{
	double energyLoss = 0.0;
	
	vector<MiSDVisuHit>* visualHitVector=Event->getSD()->getvisuhitv();
	
	if(visualHitVector)
	{
		for(MiSDVisuHit& visuHit : *visualHitVector)
		{
			if(visuHit.getParticleName() == "e-" && visuHit.getParentID() == 0)
			{
				energyLoss += visuHit.getEdep();
			}
		}
	}
	
	return energyLoss;
}

double SDInitialEnergy(MiEvent *Event)
{	
	vector<MiSDParticle>* SDParticleVector = Event->getSD()->getpartv();
	
	vector<MiSDParticle> SDElectrons;
	
	for(MiSDParticle& SDParticle : *SDParticleVector)
	{
		if(SDParticle.getname() == "e-") SDElectrons.push_back(SDParticle);
	}
	
	if(SDElectrons.size() == 1) return SDElectrons[0].getE();
	
	else
	{
		double SDElectronEnergy = SDElectrons[0].getE();
		
		for (int i = 1; i < SDElectrons.size(); i++)
		{
		    if (SDElectrons[i].getE() > SDElectronEnergy) SDElectronEnergy = SDElectrons[i].getE();
		}
		
		return SDElectronEnergy;
	}
}

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
        t->Branch("TPPz"    	    ,   &trees.TPPz     	,  "TPPz/D");
        t->Branch("SDEnergyLoss"    ,   &trees.SDEnergyLoss     ,  "SDEnergyLoss/D");
        t->Branch("isEdgeTPP"  	    ,   &trees.isEdgeTPP       	,  "isEdgeTPP/O");
    }
}

float calculateDistOM(MiEvent *Event, MiVector3D *calibSourceVertexPos)
{

	MiVector3D* OMVertexPos = nullptr;
	MiVector3D* diffVector = new MiVector3D();
	
	vector<MiVertex>* vertexVector=Event->getPTD()->getpart(0)->getvertexv();
	
	for(MiVertex& vertex : *vertexVector)
	{
		if(vertex.getpos()=="calo" || vertex.getpos()=="xcalo" || vertex.getpos()=="gveto")
		{
			OMVertexPos=vertex.getr();
		}
	}
	
	diffVector->setX(OMVertexPos->getX()-calibSourceVertexPos->getX());
	diffVector->setY(OMVertexPos->getY()-calibSourceVertexPos->getY());
	diffVector->setZ(OMVertexPos->getZ()-calibSourceVertexPos->getZ());
	
	float distOM=diffVector->getModule();
	
	delete diffVector;
	
	return distOM;
}
float calculateDistTPP(MiEvent *Event, MiVector3D *calibSourceVertexPos)
{

	MiVector3D* refSourceVertexPos = nullptr;
	MiVector3D* diffVector = new MiVector3D();
	
	vector<MiVertex>* vertexVector=Event->getPTD()->getpart(0)->getvertexv();
	
	for(MiVertex& vertex : *vertexVector)
	{
		if(vertex.getpos()=="reference source plane")
		{
			refSourceVertexPos=vertex.getr();
		}
	}
	
	diffVector->setX(0);
	diffVector->setY(refSourceVertexPos->getY()-calibSourceVertexPos->getY());
	diffVector->setZ(refSourceVertexPos->getZ()-calibSourceVertexPos->getZ());
	
	float distTPP=diffVector->getModule(); 
	return distTPP;
	
}

bool isEnergyinRange(MiEvent *Event, int minEnergy, int maxEnergy)
{

	double E_OM = 0.0;

	std::vector<MiCDCaloHit>* caloHitVector = Event->getPTD()->getpart(0)->getcalohitv();

	for(MiCDCaloHit& hit : *caloHitVector)
	{
		E_OM+=hit.getE();
	}

	if(minEnergy < E_OM && E_OM < maxEnergy)
	{
		return true;
	}
	
	return false;
}

/*/
bool isEnergyBetween600_800keV(MiEvent *Event)
{

	float E_OM = 0;
	std::vector<MiCDCaloHit>* caloHitVector = Event->getPTD()->getpart(0)->getcalohitv();

	for(MiCDCaloHit& hit : *caloHitVector)
	{
		E_OM+=hit.getE();
	}

	if(600 < E_OM && E_OM < 800)
	{
		return true;
	}
	
	return false;
}

bool isCalibElectron_envelope(MiEvent *Event)
{
	MiSD *sd = Event->getSD();
    
	if (sd)
        {
        std::vector<MiSDCaloHit> *ch = sd->getcalohitv();

        	if (ch)
        	{
            		for (auto &hit : *ch)
            		{
                		if (hit.getcategory() == "bi207_calib_source" && hit.getname() == "e-" && hit.getGID()->gettype() == "1066")
              			{
                   			return true;
                		}
            		}
        	}
        }
    	return false;
}

bool isCalibElectron_noEnvelope(MiEvent *Event)
{

	bool isCalibElectron = false;
	bool didInteractEnvelope= false;
	MiSD *sd = Event->getSD();
    
	if (sd)
        {
        	std::vector<MiSDCaloHit> *ch = sd->getcalohitv();

        	if (ch)
        	{
            		for (auto &hit : *ch)
            		{
            			if(hit.getcategory() == "bi207_calib_source" && hit.getname() == "e-")
            			{
            				isCalibElectron=true;
            				
            				if(hit.getGID()->gettype() == "1066")
            				{
            					didInteractEnvelope=true;
            				}
            			}
            		}
        	}
        }
        
        return (isCalibElectron && !didInteractEnvelope);
}

*/

bool envelopeInteraction(MiEvent *Event)
{
	bool didInteractEnvelope= false;
	MiSD *sd = Event->getSD();
    
	if (sd)
        {
        	std::vector<MiSDCaloHit> *ch = sd->getcalohitv();

        	if (ch)
        	{
            		for (auto &hit : *ch)
            		{            				
            			if(hit.getGID()->gettype() == "1066")
            			{
            				didInteractEnvelope=true;
            			}
            			
            		}
        	}
        }
        
        return didInteractEnvelope;
}

float calculateZenith(MiEvent *Event)
{
	TVector3 dir = Event->getPTD()->getpart(0)->getdirectionfromfoil();
	float zenith = TMath::ACos(dir.X() / dir.Mag()) * 180.0 / TMath::Pi();
	return zenith;
}

float calculateAzimuth(MiEvent *Event)
{
	TVector3 dir = Event->getPTD()->getpart(0)->getdirectionfromfoil();
	float azimuth = TMath::ATan2(dir.Z() , dir.Y());
	if (azimuth < 0) 
	{
		azimuth += 2.0 * TMath::Pi();  
	}

	azimuth *= 180.0 / TMath::Pi();
	return azimuth;
}

MiVector3D* calculatePosTPPVector(MiEvent *Event)
{

	MiVector3D* posVector = nullptr;
	
	vector<MiVertex>* vertexVector=Event->getPTD()->getpart(0)->getvertexv();
	
	for(MiVertex& vertex : *vertexVector)
	{
		if(vertex.getpos()=="reference source plane")
		{
			posVector=vertex.getr();
		}
	}
	
	return posVector;
}

void populateChain(TChain &ROOTChain, const char *dirPath) 
{
	TSystemDirectory dir("dirPath", dirPath);
	TList *files = dir.GetListOfFiles();
	
        if (files) 
	{
		TSystemFile *file;
		TString fname;
		TIter next(files);
		
		while ((file = (TSystemFile*)next())) 
		{
			fname = file->GetName();
			
			if (!file->IsDirectory() && fname.EndsWith(".root")) 
			{
				
				TString fullPath = TString(dirPath) + "/" + fname;
				ROOTChain.Add(fullPath);
		    }
		}
	}
}
