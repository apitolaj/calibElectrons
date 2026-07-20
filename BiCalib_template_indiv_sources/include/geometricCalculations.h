#pragma once

double calculateZenith(TVector3 *dir)
{
	float zenith = TMath::ACos(dir->X() / dir->Mag()) * 180.0 / TMath::Pi();
	return zenith;
}

double calculateAzimuth(TVector3 *dir)
{
	float azimuth = TMath::ATan2(dir->Z() , dir->Y());
	if (azimuth < 0) 
	{
		azimuth += 2.0 * TMath::Pi();  
	}

	azimuth *= 180.0 / TMath::Pi();
	return azimuth;
}

double calculateDistTPP(MiVector3D* refSourceVertexPos, MiVector3D *calibSourceVertexPos)
{
	MiVector3D diffVector;
	
	diffVector.setX(0);
	diffVector.setY(refSourceVertexPos->getY()-calibSourceVertexPos->getY());
	diffVector.setZ(refSourceVertexPos->getZ()-calibSourceVertexPos->getZ());

	return diffVector.getModule();
}

double calculateDistOM(MiVector3D* OMVertexPos, MiVector3D *calibSourceVertexPos)
{	
	MiVector3D diffVector;

	diffVector.setX(OMVertexPos->getX()-calibSourceVertexPos->getX());
	diffVector.setY(OMVertexPos->getY()-calibSourceVertexPos->getY());
	diffVector.setZ(OMVertexPos->getZ()-calibSourceVertexPos->getZ());
	
	double distOM=diffVector.getModule();
	
	return distOM;
}

bool isEdgeTPP(MiVector3D* refSourceVertexPos)
{
	double edgeCutRange = 128;
	double edgeDistFromCenter = 131;
	
	double OMTPPy = refSourceVertexPos->getY();
	double OMTPPz = refSourceVertexPos->getZ();

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
