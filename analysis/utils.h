void BinLogX(TH1 *h) {
    TAxis *axis = h->GetXaxis();
    int bins = axis->GetNbins();

    Axis_t from = TMath::Log10(axis->GetXmin());
    Axis_t to = TMath::Log10(axis->GetXmax());
    Axis_t width = (to - from) / bins;
    Axis_t *new_bins = new Axis_t[bins + 1];

    for (int i = 0; i <= bins; i++) {
        new_bins[i] = TMath::Power(10, from + i * width);
    }
    axis->Set(bins, new_bins);
    delete[] new_bins;
}

void NormBinByBin(TH1 *h, const Double_t commonSF = 1.) {
    for (int i = 1; i <= h->GetXaxis()->GetNbins(); i++) {
        Double_t val = h->GetBinContent(i);
        Double_t bincenter = h->GetBinCenter(i);
        Double_t binsize = h->GetBinWidth(i);
        Double_t newval = val * commonSF / binsize;
        h->SetBinContent(i, newval);
    }
}
