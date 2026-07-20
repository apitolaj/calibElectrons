void plotEnergySpectra(const char *inputtedROOTFile1, const char *inputtedROOTFile2,const char *inputtedROOTFile3) {

    //--------------------------------------------------------------
    // Open files and grab trees
    //--------------------------------------------------------------
    TFile *f1 = new TFile(inputtedROOTFile1);
    if (!f1 || f1->IsZombie()) {
        std::cerr << "Error: cannot open file 1" << std::endl;
        return;
    }
    TTree *t1_envelope   = (TTree*)f1->Get("envelope");
    TTree *t1_noEnvelope = (TTree*)f1->Get("noEnvelope");

    TFile *f2 = new TFile(inputtedROOTFile2);
    if (!f2 || f2->IsZombie()) {
        std::cerr << "Error: cannot open file 2" << std::endl;
        return;
    }
    TTree *t2_envelope   = (TTree*)f2->Get("envelope");
    TTree *t2_noEnvelope = (TTree*)f2->Get("noEnvelope");

    TFile *f3 = new TFile(inputtedROOTFile3);
    if (!f3 || f3->IsZombie()) {
        std::cerr << "Error: cannot open file 3" << std::endl;
        return;
    }
    TTree *t3_envelope   = (TTree*)f3->Get("envelope");
    TTree *t3_noEnvelope = (TTree*)f3->Get("noEnvelope");

    //--------------------------------------------------------------
    // Canvas: 3 panels side by side
    //--------------------------------------------------------------
    TCanvas *c = new TCanvas("c", "Energy Spectra", 1800, 500);
    c->Divide(3, 1);

    TLegend *leg;

    //================================================================
    // PANEL 1: envelope only, 3 files overlaid
    //================================================================
    c->cd(1);

    TH1F *h1_env = new TH1F("h1_env", "Envelope;Energy [keV];Counts", 100, 0, 1200);
    t1_envelope->Draw("electronEnergy>>h1_env");
    h1_env->SetDirectory(0);
    h1_env->SetLineColor(kRed);
    h1_env->Draw("HIST");

    TH1F *h2_env = new TH1F("h2_env", "h2_env", 100, 0, 1200);
    t2_envelope->Draw("electronEnergy>>h2_env");
    h2_env->SetDirectory(0);
    h2_env->SetLineColor(kBlue);
    h2_env->Draw("HIST SAME");

    TH1F *h3_env = new TH1F("h3_env", "h3_env", 100, 0, 1200);
    t3_envelope->Draw("electronEnergy>>h3_env");
    h3_env->SetDirectory(0);
    h3_env->SetLineColor(kGreen+2);
    h3_env->Draw("HIST SAME");

    leg = new TLegend(0.65, 0.7, 0.9, 0.9);
    leg->AddEntry(h1_env, "File 1", "l");
    leg->AddEntry(h2_env, "File 2", "l");
    leg->AddEntry(h3_env, "File 3", "l");
    leg->Draw();

    //================================================================
    // PANEL 2: noEnvelope only, 3 files overlaid
    //================================================================
    c->cd(2);

    TH1F *h1_noEnv = new TH1F("h1_noEnv", "No Envelope;Energy [keV];Counts", 100, 0, 1200);
    t1_noEnvelope->Draw("electronEnergy>>h1_noEnv");
    h1_noEnv->SetDirectory(0);
    h1_noEnv->SetLineColor(kRed);
    h1_noEnv->Draw("HIST");

    TH1F *h2_noEnv = new TH1F("h2_noEnv", "h2_noEnv", 100, 0, 1200);
    t2_noEnvelope->Draw("electronEnergy>>h2_noEnv");
    h2_noEnv->SetDirectory(0);
    h2_noEnv->SetLineColor(kBlue);
    h2_noEnv->Draw("HIST SAME");

    TH1F *h3_noEnv = new TH1F("h3_noEnv", "h3_noEnv", 100, 0, 1200);
    t3_noEnvelope->Draw("electronEnergy>>h3_noEnv");
    h3_noEnv->SetDirectory(0);
    h3_noEnv->SetLineColor(kGreen+2);
    h3_noEnv->Draw("HIST SAME");

    leg = new TLegend(0.65, 0.7, 0.9, 0.9);
    leg->AddEntry(h1_noEnv, "File 1", "l");
    leg->AddEntry(h2_noEnv, "File 2", "l");
    leg->AddEntry(h3_noEnv, "File 3", "l");
    leg->Draw();

    //================================================================
    // PANEL 3: combined (envelope + noEnvelope), 3 files overlaid
    //================================================================
    c->cd(3);

    TH1F *h1_comb = new TH1F("h1_comb", "Combined;Energy [keV];Counts", 100, 0, 1200);
    t1_envelope->Draw("electronEnergy>>h1_comb");
    t1_noEnvelope->Draw("electronEnergy>>+h1_comb");
    h1_comb->SetDirectory(0);
    h1_comb->SetLineColor(kRed);
    h1_comb->Draw("HIST");

    TH1F *h2_comb = new TH1F("h2_comb", "h2_comb", 100, 0, 1200);
    t2_envelope->Draw("electronEnergy>>h2_comb");
    t2_noEnvelope->Draw("electronEnergy>>+h2_comb");
    h2_comb->SetDirectory(0);
    h2_comb->SetLineColor(kBlue);
    h2_comb->Draw("HIST SAME");

    TH1F *h3_comb = new TH1F("h3_comb", "h3_comb", 100, 0, 1200);
    t3_envelope->Draw("electronEnergy>>h3_comb");
    t3_noEnvelope->Draw("electronEnergy>>+h3_comb");
    h3_comb->SetDirectory(0);
    h3_comb->SetLineColor(kGreen+2);
    h3_comb->Draw("HIST SAME");

    leg = new TLegend(0.65, 0.7, 0.9, 0.9);
    leg->AddEntry(h1_comb, "File 1", "l");
    leg->AddEntry(h2_comb, "File 2", "l");
    leg->AddEntry(h3_comb, "File 3", "l");
    leg->Draw();

    //--------------------------------------------------------------
    c->SaveAs("energy_spectra_all.png");
}
