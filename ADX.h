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



    for (int i = start; i < data.size(); ++i) {
        double tr_high_low = data[i].high - data[i].low;
        double tr_high_prev_close = data[i].high - data[i].price;
        double tr_low_prev_close = data[i].low - data[i - 1].price;
        tr.push_back(max({tr_high_low, tr_high_prev_close, tr_low_prev_close}));

        dm_plus.push_back(max(0.0, data[i].high - data[i - 1].high));
        dm_minus.push_back(max(0.0, data[i].low - data[i - 1].low));
    }

    atr.push_back(tr[0]);
    double alpha = 2.0 / (n + 1);

    for (int i = start + 1; i < data.size(); ++i) {
        atr.push_back(alpha * (tr[i - start] - atr[i - start - 1]) + atr[i - start - 1]);
    }

    di_plus.push_back(dm_plus[0] / atr[0]);

    for (int i = start + 1; i < data.size(); ++i) {
        di_plus.push_back(alpha * ((dm_plus[i - start] / atr[i - start]) - di_plus[i - start - 1]) + di_plus[i - start - 1]);
    }

    di_minus.push_back(dm_minus[0] / atr[0]);

    for (int i = start + 1; i < data.size(); ++i) {
        di_minus.push_back(alpha * ((dm_minus[i - start] / atr[i - start]) - di_minus[i - start - 1]) + di_minus[i - start - 1]);
    }


    for (int i = 0; i < di_minus.size(); i++){
        if (di_plus[i] + di_minus[i] == 0){
            dx.push_back(1000000.0);
            continue;
        }
        dx.push_back((di_plus[i] - di_minus[i]) / (di_plus[i] + di_minus[i]) * 100);
    }

    adx.push_back(dx[0]);

    for (int i = 1; i < dx.size(); ++i) {
        if (dx[i] == 1000000.0){
            adx.push_back(1000000.0);
            continue;
        }
        adx.push_back(alpha * (dx[i] - adx[i - 1]) + adx[i - 1]);
    }

    for (int i = start; i < data.size(); ++i) {

        string date = convert_to_date(data[i]);

        if (adx[i - start] == 1000000.0){
            cashflowFile << date << "," << fixed << setprecision(2) << cashInHand << "\n";
            continue;
        }

        if (adx[i - start] >= adx_threshold && position < x) {
            orderStatistics.push_back({date, "BUY", 1, data[i].price});
            cashInHand -= data[i].price;  
            position += 1; 
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n"; 
        } else if (adx[i - start] <= adx_threshold && position > -x) {
            orderStatistics.push_back({date, "SELL", 1, data[i].price});
            cashInHand += data[i].price; 
            position -= 1;
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }

        cashflowFile << date << "," << fixed << setprecision(2) << cashInHand << "\n";

    }

    if (position != 0) {
        double finalPrice = data.back().price;
        double pnl = position * finalPrice;
        pnl += cashInHand;
        finalPNLFile << fixed << setprecision(2) << pnl;
    } else {
        finalPNLFile << 0.0;
    }
}