void ADXStrategy(const vector<Record>& data, int start, double x, int n, double adx_threshold, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
    double cashInHand = 0.0;
    int position = 0;
    
    vector<double> tr;
    vector<double> dm_plus;
    vector<double> dm_minus;
    vector<double> atr;
    vector<double> di_plus;
    vector<double> di_minus;
    vector<double> dx;
    vector<double> adx;
    vector<double> dm_atr;



    for (int i = start - 2 * n; i < data.size(); ++i) {
        double tr_high_low = data[i].high - data[i].low;
        double tr_high_prev_close = data[i].high - data[i].price;
        double tr_low_prev_close = data[i].low - data[i - 1].price;
        tr.push_back(max({tr_high_low, tr_high_prev_close, tr_low_prev_close}));

        dm_plus.push_back(max(0.0, data[i].high - data[i - 1].high));
        dm_minus.push_back(max(0.0, data[i].low - data[i - 1].low));
    }

    for (int i = start - n; i < data.size(); ++i) {
        double atr_temp = tr[i - n];
        for (int j = n - 1; j >= 0; j--){
            atr_temp = (1 / (n + 1)) * (tr[i - j] - atr_temp) + atr_temp;
        }
        atr.push_back(atr_temp);
    }

    for (int i = start; i < data.size(); ++i) {
        double di_plus_temp = dm_plus[i - n] / atr[i - start];
        for (int j = n - 1; j >= 0; --j){
            di_plus_temp = (1 / (n + 1)) * (dm_plus[i - j] / atr[i - start + n - j] - di_plus_temp) + di_plus_temp;
        }
        di_plus.push_back(di_plus_temp);
    }

    for (int i = start; i < data.size(); ++i) {
        double di_minus_temp = dm_minus[i - n] / atr[i - start];
        for (int j = n - 1; j >= 0; --j){
            di_minus_temp = (1 / (n + 1)) * (dm_minus[i - j] / atr[i - start + n - j] - di_minus_temp) + di_minus_temp;
        }
        di_minus.push_back(di_minus_temp);
    }

    for (int i = 0; i < di_plus.size(); ++i) {
        double dx_temp = (di_plus[i] - di_minus[i]) * 100 / (di_plus[i] + di_minus[i]);
        dx.push_back(dx_temp);
    }

    for (int i = start; i < data.size(); ++i) {
        double adx_temp = dx[0];
        for (int j = n - 1; j >= 0; --j){
            adx_temp = (1 / (n + 1)) * (dx[i - j] - adx_temp) + adx_temp;
        }
        adx.push_back(adx_temp);
    }

    string prev = "wh";

    for (int i = start; i < data.size(); ++i) {

        if (orderStatistics.size() != 0){
            prev = orderStatistics.back().direction;
        }

        string date = convert_to_date(data[i]);

        if (adx[i - start] > adx_threshold && position < x) {
            orderStatistics.push_back({date, "BUY", 1, data[i].price});
            cashInHand -= data[i].price;  
            position += 1;  
        } else if (adx[i] < adx_threshold && position > -x) {
            orderStatistics.push_back({date, "SELL", 1, data[i].price});
            cashInHand += data[i].price; 
            position -= 1;
        }

        cashflowFile << date << "," << fixed << setprecision(2) << cashInHand << "\n";

        if ((orderStatistics.size() != 0) && (orderStatistics.back().direction != prev)) {
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }

    }

    finalPNLFile << "Final PNL: " << fixed << setprecision(2) << cashInHand << "\n";

}