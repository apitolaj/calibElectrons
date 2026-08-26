#pragma once

//function to calculate the zenith angle of the electron and convert it to radians.
double calculateZenith(TVector3 *dir)
{
	float zenith = TMath::ACos(dir->X() / dir->Mag()) * 180.0 / TMath::Pi();
	return zenith;
}

//function to calculate the azimuth angle of the electron and convert it to radians.
double calculateAzimuth(TVector3 *dir)
{
	float azimuth = TMath::ATan2(dir->Z() , dir->Y());

	//if the azimuth angle is less than 0, add 2 pi to it.
	if (azimuth < 0) 
	{
		azimuth += 2.0 * TMath::Pi();  
	}

	azimuth *= 180.0 / TMath::Pi();
	return azimuth;
}

//function to calculate the distance from the calibration source to the reference source plane vertex.
double calculateDistTPP(MiVector3D* refSourceVertexPos, MiVector3D *calibSourceVertexPos)
{
	//define a a vector to hold the difference in each cartesian coordinate.
	MiVector3D diffVector;
	
	diffVector.setX(0);
	diffVector.setY(refSourceVertexPos->getY()-calibSourceVertexPos->getY());
	diffVector.setZ(refSourceVertexPos->getZ()-calibSourceVertexPos->getZ());

	//take the module of the difference vector to get the total distance.
	return diffVector.getModule();
}

//function to calculate the distance from the calibration source to the triggered optical module vertex.
double calculateDistOM(MiVector3D* OMVertexPos, MiVector3D *calibSourceVertexPos)
{	
	//define a a vector to hold the difference in each cartesian coordinate.
	MiVector3D diffVector;

	diffVector.setX(OMVertexPos->getX()-calibSourceVertexPos->getX());
	diffVector.setY(OMVertexPos->getY()-calibSourceVertexPos->getY());
	diffVector.setZ(OMVertexPos->getZ()-calibSourceVertexPos->getZ());
	
	//take the module of the difference vector to get the total distance.
	return diffVector.getModule();
}

//function to check if the optical module hit lines along the edge of the optical module. 
bool isEdgeTPP(MiVector3D* refSourceVertexPos)
{
	//define two range variables to give the cut (in mm) that will be checked if the vertex lies in. 
	double edgeCutRange = 128;
	double edgeDistFromCenter = 131;
	
	//calculate the reference source vertex y and z positions. 
	double OMTPPy = refSourceVertexPos->getY();
	double OMTPPz = refSourceVertexPos->getZ();

	//lambda expression to check if a value is inbetween the range of the variables above. 
	auto inEdgeBand = [&](double delta)
	{
		//take the absolute value of the input and return a bool after checking if it is in the desired range.
	    double d = std::fabs(delta);
	    return edgeCutRange < d && d < edgeDistFromCenter;
	};

	//loop over each optical module to check if the OM hit is in the edge zone.
	for (int ModuleID = 0; ModuleID < 260; ModuleID++)
	{
		//get the OMy and OMz positions from the mapping in the detectorGeometry.h header.
	    auto [OMy, OMz] = omPositions.at(std::to_string(ModuleID));

	    if (inEdgeBand(OMTPPy - OMy) || inEdgeBand(OMTPPz - OMz))
	    {
			return true;
	    }
	}
	
	return false; 
}
