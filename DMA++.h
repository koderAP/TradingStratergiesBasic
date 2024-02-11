void DMAPlusPlusStrategy(const vector<Record>& data, int start, double x, double p, int n, int maxHoldDays, double c1, double c2, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
    double cashInHand = 0.0;
    int position = 0;

    vector<double>er;
    vector<double>sf;
    vector<double>ama;

    double absPriceChangeSum = 0.0;

    string prev = "wh";

    for (size_t i = start - n + 1; i <= start; ++i) {
        absPriceChangeSum += abs(data[i].price - data[i - 1].price);
    }

    double closePrice = data[start].price;
    double priceChange = closePrice - data[start - n].price;

    if (absPriceChangeSum != 0) {
        er.push_back(priceChange / absPriceChangeSum);
    }
    else {
        er.push_back(1000000000.0);
    }

    for (size_t i = start + 1; i < data.size(); ++i){

        absPriceChangeSum += abs(data[i].price - data[i-1].price);
        absPriceChangeSum -= abs(data[i-n].price - data[i-n-1].price);

        closePrice = data[i].price;

        priceChange = closePrice - data[i - n].price;

        if (absPriceChangeSum != 0) {
            er.push_back(priceChange / absPriceChangeSum);
        }
        else {
            er.push_back(1000000000.0);
        }
    }

    sf.push_back(0.5);

    for (size_t i = start + 1; i < data.size(); ++i){

        sf.push_back(sf[i - start - 1] + c1 * ((((2 * er[i - start]) / (1 + c2)) - 1)/(((2 * er[i - start]) / (1 + c2)) + 1) - sf[i - start - 1]));

        double rsf = (1 - c1);
        double asf = ((2 * er[i - start] - 1 - c2) / (2 * er[i - start] + 1 + c2));

        sf.push_back((pow(rsf, (i - start))) * 0.5 + c1 * asf * (1 - pow(rsf, (i - start))) / (1 - rsf));

    }    

    
    ama.push_back(data[start].price);

    for (size_t i = start + 1; i < data.size(); ++i){

        ama.push_back(ama[i - start - 1] + sf[i - start] * (data[i].price - ama[i - start - 1]));

    }

    for (size_t i = 0; i < ama.size(); ++i){

        closePrice = data[i + start].price;

        if (orderStatistics.size() != 0){
            prev = orderStatistics.back().direction;
        }

        string date = convert_to_date(data[i + start]);

        if (position != 0 && i - orderStatistics.back().quantity >= maxHoldDays) {
            cashInHand += position * closePrice;
            orderStatistics.push_back({date, "FORCE_CLOSE", position, closePrice});
            position = 0;
        }

        if (closePrice > (ama[i] * (1 + p / 100)) && position < x) {
            orderStatistics.push_back({date, "BUY", 1, closePrice});
            cashInHand -= closePrice;
            position += 1;
        } else if (closePrice < (ama[i] * (1 - p / 100)) && position > -x) {
            orderStatistics.push_back({date, "SELL", 1, closePrice});
            cashInHand += closePrice;
            position -= 1;
        }

        cashflowFile << date << "," << cashInHand << "\n";
        if ((orderStatistics.size() != 0) && (orderStatistics.back().direction != prev)) {
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }   

    }

    finalPNLFile << "Final PNL: " << cashInHand << "\n";
}