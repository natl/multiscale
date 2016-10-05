{
gROOT->Reset();
gStyle->SetPalette(1);
gROOT->SetStyle("Plain");
Double_t scale;

TCanvas* pCanvas = new TCanvas();


gStyle->SetOptTitle(0); 


//Get Histos
TH1D* tl050 = TFile("livermore_100kev_100k_tracklength_50um.root").GetDirectory("hists;1")->GetObjectUnchecked("4;1");
TH1D* tl100 = TFile("livermore_100kev_100k_tracklength_100um.root").GetDirectory("hists;1")->GetObjectUnchecked("4;1");
TH1D* tl200 = TFile("livermore_100kev_100k_tracklength_200um.root").GetDirectory("hists;1")->GetObjectUnchecked("4;1");
TH1D* tl400 = TFile("livermore_100kev_100k_tracklength_200um.root").GetDirectory("hists;1")->GetObjectUnchecked("4;1");


tl050->SetLineColor(kRed);
tl100->SetLineColor(kBlue);
tl200->SetLineColor(kBlack);
tl400->SetLineColor(kOrange);


tl050->SetStats(false);
tl100->SetStats(false);
tl200->SetStats(false);
tl400->SetStats(false);

tl050->Draw();
tl050->GetXaxis()->SetTitle("Track length (um)");
tl050->GetYaxis()->SetTitle("Frequency");

tl100->Draw("SAME");
tl200->Draw("SAME");
tl400->Draw("SAME");


leg = new TLegend(0.1,0.9,0.4,0.8);
leg->AddEntry(tl050,"50um box","l");
leg->AddEntry(tl100,"100um box","l");
leg->AddEntry(tl200,"200um box","l");
leg->AddEntry(tl400,"400um box","l");

leg->Draw();

//pCanvas->SetLogx();
//pCanvas->SetLogy();
}

