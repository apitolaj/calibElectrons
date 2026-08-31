# calibElectrons
 
This repository was created by Andrew Pitolaj during a summer 2026 Princeton IIP internship at Astrocent under mentorship of Dr. Miroslav Macko. Its purpose is to better understand the behavior of the electrons in the 600-800 keV plateau seen in the energy spectrum of the SuperNEMO detector's bismuth calibration sources.

<b>NOTE:</b> To use this repository, the user must first build the modules SNCuts, Cimrman and MiModule. Installation information for Cimrman can be found at https://github.com/TomasKrizak/CimrmanModule.git. An altered version of the SNCuts module was used that can be found at https://github.com/YaKozina/SNCuts.git. Additionally, extra features for the MiModule were added during development of this repository, so the user should build the forked version found at PLACEHOLDER. 

After installing the necessary modules, the repository can be cloned to the user's directory with:
```bash
   git clone https://github.com/apitolaj/calibElectrons.git
```
<details open>
<summary><h2>Data Generation and Processing</h2></summary>
<br>
  
### Setting Configuration Files
Inside the directory, the user should change into the configuration sub-directory:
   ```bash
      cd configuration
   ```
This subdirectory contains all the simulation and event cut configuration files for Falaise. The default settings in the configuration files may be changed by the user if they desire, but they should be left as is to reproduce the figures seen in this repository. Nonetheless, the user needs to <b> change the paths in the following files </b> to match their own directory: <b> SNCuts-pipeline.conf, p_MiModule_v00.conf, 2D-CMRMN-CPT-GT-pipeline.conf </b>. In simu_setup.conf.template, the user may change how many events they wish to run per simulation; anything larger than 1,000,000 events will be too large as the simulation will take unnecessarily long to run. After all this is completed, inside the configuration subdirectory the user should go deeper into the profiles sub-directory and run the makeProfiles script:
  
Once finished, all configuration is complete and the user can change back to the parent directory.
   ```bash
      cd ../..
   ```

### Making the Source Directories
Now the directories to house all of the files associated with the simulation data for each of the 42 sources will be created. The user should change into the directory_template sub-directory:
   ```bash
      cd directory_template
   ```
The submission and running scripts work together using array jobs. Here you will find 6 scripts:
1. submit_simu_array.sh<br>
2. submit_SNCUTS_array.sh<br>
3. submit_MiModule_array.sh<br>
4. run_simu.sh<br>
5. run_SNCUTS.sh<br>
6. run_MiModule.sh
   
<b>No changes need to be made to them except for the resource allocation in each script </b>. This is dependent on how many events you are simulating at a time. It may take some time experimenting with how many GB and time to allocate for optimal efficiency. After these changes are made, the user should change back to the parent directory and run the makeDirectories script:
   ```bash
      cd ../..
      bash setup.sh
   ```
This creates a new sub directory called SOURCES that houses a copy of the directory_template sub directory for each of the 42 sources, linking each directory to its correct configuration files automatically. 

### Submitting Jobs

Once the previous steps are done, the user is ready to submit the jobs. The file masterSubmit.sh contains directions for selecting which sources are to be simulated with how many jobs in parallel. Once the edits are made to the file, it can be submitted with:
   ```bash
      bash masterSubmit.sh
   ```
The data will be simulated and processed through the full framework developed by YAROSLAVA, generating a ROOT file for each source that was simulated, merging multiple ROOT files together if multiple jobs were ran in parallel. Once the jobs finish, <b>the master submission script can be run again to generate more data</b>. Subsequent analysis scripts used in this repository will automatically chain these ROOT files together, leading to no extra work for the user.

### Running the Data Analysis Script
To create the analysis plots for the generated data, the user must first navigate to the SUBMISSION subdirectory and run the script analysis.sh:
   ```bash
      cd SUBMISSION
      bash submit_analysis.sh
   ```
For each of the 42 sources, this will copy a version of script /calibElectrons/ROOT/analysis_chainROOT.cpp to each sources' respective folder in the /calibelectrons/ANALYSIS subdirectory and run it; the script knows where to find each sources data and to chain together multiple ROOT files if they exist.

### Analysis ROOT file Structure

The script will produce a ROOT file containing a tree for electrons that interacted with the envelope and a tree for electrons that did not. Each tree has the a branch for each of the follow analysis quantities:
<div align="center">
 
| Analysis Quantity | Definition                                                                         |
|------------------ |----------------------------------------------------------------------------------- |
| zenith            | zenith angle of the electrons trajectory                                           |
| azimuth           | azimuth angle of the electrons trajectory                                          |
| DistOM            | Row 1b                                                                             |
| DistTPP           | Row 2b                                                                             |
| TPPy              | Electron trajectory projection point y position                                    |
| TPPz              | Electron trajectory projection point z position                                    |
| isEdgeTPP         | Boolean for if the electron optimal module vertex is along the edge of the module  |
| electronEnergy    | Electron energy                                                                    |  

</div>

<table>
  <tr>
    <td width="35%">
      <img src="https://github.com/user-attachments/assets/c624ed1c-422d-412e-9a16-b4216d37c4ff" width="100%">
    </td>
    <td width="65%">
     <img width="1672" height="941" alt="sourcenumbering" src="https://github.com/user-attachments/assets/efa41097-f84c-4b72-9cd5-4c99cbcd482c" />
    </td>
  </tr>
</table>

### Creating ROOT Plots

After the analysis ROOT file is created, the plotting scripts can be ran. The repository currently has 6 plotting scripts found in the SUBMISSION subdirectory of analysisScirpts:
   ```bash
       polarHisto.sh
       th2dHisto_TPPDistribution.sh
       th2dHisto_zenithTPP.sh
       the2dHisto_zenitDistOM.sh
       submit_curveFitting.sh
   ```
These scripts can be ran individually as is and will produce their respective plot(s) for each simulated source, which is described in more detail in the next section of this README. The scripts call also be ran all at once with:
   ```bash
      bash masterSubmit_plots.sh
   ``` 
The user can view the plots individually in each source's ANALYSIS subdirectory, or they can navigate to /analysisScripts/VIEW where they will find a variety of scripts that can be executed to automatically open each plot of a certain type across all simulated sources for easier viewing. 

</details>

<details open>
<summary> <h2> Plot Discussion </h2></summary>

The plotting scripts above run the following ROOT macros found in the PLOT subdirectory of analysisScripts:
   ```bash
       polarHisto_angles.cpp
       th2dHisto_TPPDistribution.cpp
       th2dHisto_zenithTPP.cpp
       the2dHisto_zenitDistOM.cpp
       curveFittingGauss.cpp
       curveFittingLangau.cpp
   ```

### polarHisto_angles.cpp 

### curveFittingGauss.cpp & curveFittingLangau.cpp

### th2dHisto_TPPDistribution.cpp, th2dHisto_zenithTPP.cpp, the2dHisto_zenitDistOM.cpp






</details>
