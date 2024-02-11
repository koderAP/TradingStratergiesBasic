void RSIStrategy(const vector<Record>& data, int start, double x, int n, double oversoldThreshold, double overboughtThreshold, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
    double cashInHand = 0.0;
    int position = 0;

    double avgGain = 0.0;
    double avgLoss = 0.0;

    for (int i = start - n; i < start; ++i) {
        double priceDiff = data[i].price - data[i - 1].price;

        avgGain = avgGain + max(priceDiff, 0.0);
        avgLoss = avgLoss - min(priceDiff, 0.0);
    }

    avgGain /= n;
    avgLoss /= n;

    string prev = "wh";

    for (size_t i = start; i < data.size(); ++i) {

        double priceDiff = data[i].price - data[i - 1].price;
        double old_pricediff = data[i - n].price - data[i - n - 1].price;

        avgGain = (avgGain * (n) + max(priceDiff, 0.0) - max(old_pricediff, 0.0)) / n;
        avgLoss = (avgLoss * (n) - min(priceDiff, 0.0) + min(old_pricediff, 0.0)) / n;

        double RS = avgGain / avgLoss;
        double RSI = 100.0 - 100.0 / (1.0 + RS);

        if (orderStatistics.size() != 0){
            prev = orderStatistics.back().direction;
        }

        string date = convert_to_date(data[i]);

        if (RSI < oversoldThreshold && position < x) {
            orderStatistics.push_back({date, "BUY", 1, data[i].price});
            cashInHand -= data[i].price;
            position += 1;
        } else if (RSI > overboughtThreshold && position > -x) {
            orderStatistics.push_back({date, "SELL", 1, data[i].price});
            cashInHand += data[i].price;
            position -= 1;
        }
        cashflowFile << date << "," << cashInHand << "\n";
        if ((orderStatistics.size() != 0) && (orderStatistics.back().direction != prev)) {
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }   

    }

    finalPNLFile << "Final PNL: " << cashInHand << "\n";

}