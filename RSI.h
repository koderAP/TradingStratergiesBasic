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

    for (size_t i = start; i < data.size(); ++i) {

        double priceDiff = data[i].price - data[i - 1].price;
        double old_pricediff = data[i - n].price - data[i - n - 1].price;

        avgGain = (avgGain * (n) + max(priceDiff, 0.0) - max(old_pricediff, 0.0)) / n;
        avgLoss = (avgLoss * (n) - min(priceDiff, 0.0) + min(old_pricediff, 0.0)) / n;

        double RS = avgGain / avgLoss;
        double RSI = 100.0 - 100.0 / (1.0 + RS);
        string date = convert_to_date(data[i]);

        if (RSI < oversoldThreshold && position < x) {
            orderStatistics.push_back({date, "BUY", 1, data[i].price});
            cashInHand -= data[i].price;
            position += 1;
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        } else if (RSI > overboughtThreshold && position > -x) {
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