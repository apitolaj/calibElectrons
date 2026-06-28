#pragma once
#include <unordered_map>
#include <string>

bool isCalibElectron_envelope(MiEvent *Event);

bool isCalibElectron_noEnvelope(MiEvent *Event);

float calculateZenith(MiEvent *Event);

float calculateAzimuth(MiEvent *Event);

MiVector3D* calculatePosTPPVector(MiEvent *Event);

void populateChain(TChain &ROOTChain, const char *dirPath);

bool isEnergyBetween600_800keV(MiEvent *Event);

float calculateDistTPP(MiEvent *Event, MiVector3D *calibSourceVertexPos);

float calculateDistOM(MiEvent *Event, MiVector3D *calibrationSourceVertexPos);

//-----------------------------------------------------------------------

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


