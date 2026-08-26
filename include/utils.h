#pragma once

//function to link together multiple ROOT files into one chain object.
void populateChain(TChain &ROOTChain, const char *dirPath) 
{
	//take the inputted directory path from the function call and generate a list of files in that location.
	TSystemDirectory dir("dirPath", dirPath);
	TList *files = dir.GetListOfFiles();
	
	//if the file list is not empty, chain the ROOT files in it together. 
    if (files) 
	{
		TSystemFile *file;
		TString fname;
		TIter next(files);
		
		//while loop to iterate through all the files in the directory. 
		while ((file = (TSystemFile*)next())) 
		{
			fname = file->GetName();
			
			//check that the file is not a directory and that it ends with .root.
			if (!file->IsDirectory() && fname.EndsWith(".root")) 
			{
				//append the root file to the chain.
				TString fullPath = TString(dirPath) + "/" + fname;
				ROOTChain.Add(fullPath);
		    }
		}
	}
}
