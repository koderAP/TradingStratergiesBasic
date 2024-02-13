double calc_sf(double er, int pos, int des, double c1, double c2){
    if (pos == des){
        return 0.5;
    }
    else {
        return ((1 - c1) * calc_sf(er, pos - 1, des, c1, c2) + c1 * ((((2 * er) / (1 + c2)) - 1)/((2 * er) / ((1 + c2)) + 1)));
    }
}

double calc_ama(const vector<Record>& data, vector<double>& sf, int pos, int des, int start){
    if (pos == des){
        return data[des].price;
    }
    else {
        return ((1 - sf[pos - start]) * calc_ama(data, sf, pos - 1, des, start) + sf[pos - start] * data[pos].price);
    }
}

void DMAPlusPlusStrategy(const vector<Record>& data, int start, double x, double p, int n, int maxHoldDays, double c1, double c2, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
    double cashInHand = 0.0;
    int position = 0;
    vector<double>er;
    vector<double>sf;
    vector<double>ama;

    double absPriceChangeSum = 0.0;

    for (size_t i = start - n; i < start; ++i) {
        absPriceChangeSum += abs(data[i].price - data[i - 1].price);
    }


    for (size_t i = start; i < data.size(); ++i){

        absPriceChangeSum += abs(data[i].price - data[i-1].price);
        absPriceChangeSum -= abs(data[i-n].price - data[i-n-1].price);

        double closePrice = data[i].price;

        double priceChange = closePrice - data[i - n].price;

        if (absPriceChangeSum != 0) {
            er.push_back(priceChange / absPriceChangeSum);
        }
        else {
            er.push_back(100000000.0);
        }
    }

    for (size_t i = start; i < data.size(); ++i){

        if (er[i - start] == 100000000.0) {
            sf.push_back(100000000.0);
            continue;
        }

        sf.push_back(calc_sf(er[i - start], i, i - n, c1, c2));
        cout << i << endl;
        cout << sf[i] << endl;
    }    

    
    for (size_t i = start; i < data.size(); ++i){

        if (sf[i - start] == 100000000.0){
            ama.push_back(100000000.0);
            continue;
        }

        ama.push_back(calc_ama(data, sf, i, i - n, start));
        // cout << ama[i] << endl;
    }

    int count = 0;

    for (size_t i = start; i < data.size(); ++i){

        double closePrice = data[i].price;

        string date = convert_to_date(data[i]);

        if (ama[i - start] == 100000000.0){
            continue;
        }

        if (position != 0 && count >= maxHoldDays) {
            cashInHand += position * closePrice;
            orderStatistics.push_back({date, "FORCE_CLOSE", position, closePrice});
            position = 0;
            count = 0;
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }

        if (closePrice > (ama[i - start] * (1 + p / 100)) && position < x) {
            orderStatistics.push_back({date, "BUY", 1, closePrice});
            cashInHand -= closePrice;
            position += 1;
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        } else if (closePrice < (ama[i - start] * (1 - p / 100)) && position > -x) {
            orderStatistics.push_back({date, "SELL", 1, closePrice});
            cashInHand += closePrice;
            position -= 1;
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }
        else {
            count++;
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