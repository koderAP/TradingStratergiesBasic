void PairsStrategyWithStopLoss(const vector<Record>& data1, const vector<Record>& data2, int start, int x, int n, double threshold, double stopLossThreshold, vector<Order>& orderStatistics1, vector<Order>& orderStatistics2, ofstream& cashflowFile, ofstream& orderStatisticsFile1, ofstream& orderStatisticsFile2, ofstream& finalPNLFile) {
    double cashInHand1 = 0.0;
    double cashInHand2 = 0.0;
    int position1 = 0;
    int position2 = 0;

    double sumSpread = 0.0;
    double sumSquareSpread = 0.0;

    for (size_t i = start - n; i < start; ++i) {
        double spread = data1[i].price - data2[i].price;
        sumSpread += spread;
        sumSquareSpread += spread * spread;
    }

    sumSpread = sumSpread / n;
    for (size_t i = start; i < data1.size(); ++i) {
        double spread = data1[i].price - data2[i].price;

        sumSpread += (spread / n);
        sumSpread -= ((data1[i - n].price - data2[i - n].price) / n);
        sumSquareSpread += spread * spread;
        sumSquareSpread -= (data1[i - n].price - data2[i - n].price) * (data1[i - n].price - data2[i - n].price);

        double stdDevSpread = sqrt((sumSquareSpread / n) - (sumSpread * sumSpread));
        double zScore = (spread - sumSpread) / stdDevSpread;

        string date = convert_to_date(data1[i]);

        // Generate buy/sell signals based on z-score threshold
        if (zScore > threshold && position1 > -x && position2 < x) {
            orderStatistics1.push_back({date, "SELL", 1, data1[i].price});
            orderStatistics2.push_back({date, "BUY", 1, data2[i].price});
            cashInHand1 += data1[i].price;
            cashInHand2 -= data2[i].price;
            position1 -= 1;
            position2 += 1;
            orderStatisticsFile1 << orderStatistics1.back().date << "," << orderStatistics1.back().direction << "," << orderStatistics1.back().quantity << "," << orderStatistics1.back().price << "\n";
            orderStatisticsFile2 << orderStatistics2.back().date << "," << orderStatistics2.back().direction << "," << orderStatistics2.back().quantity << "," << orderStatistics2.back().price << "\n";
        } else if (zScore < -threshold && position1 < x && position2 > -x) {
            orderStatistics1.push_back({date, "BUY", 1, data1[i].price});
            orderStatistics2.push_back({date, "SELL", 1, data2[i].price});
            cashInHand1 -= data1[i].price;
            cashInHand2 += data2[i].price;
            position1 += 1;
            position2 -= 1;
            orderStatisticsFile1 << orderStatistics1.back().date << "," << orderStatistics1.back().direction << "," << orderStatistics1.back().quantity << "," << orderStatistics1.back().price << "\n";
            orderStatisticsFile2 << orderStatistics2.back().date << "," << orderStatistics2.back().direction << "," << orderStatistics2.back().quantity << "," << orderStatistics2.back().price << "\n";
        }

        if ((zScore > stopLossThreshold && position1 < 0) || (zScore < -stopLossThreshold && position1 > 0) || (zScore > stopLossThreshold && position2 < 0) || (zScore < -stopLossThreshold && position2 > 0)) {
            cashInHand1 += position1 * data1[i].price;
            cashInHand2 += position2 * data2[i].price;
            orderStatistics1.push_back({date, "FORCE_CLOSE", abs(position1), data1[i].price});
            orderStatistics2.push_back({date, "FORCE_CLOSE", abs(position2), data2[i].price});
            position1 = 0;
            position2 = 0;
            orderStatisticsFile1 << orderStatistics1.back().date << "," << orderStatistics1.back().direction << "," << orderStatistics1.back().quantity << "," << orderStatistics1.back().price << "\n";
            orderStatisticsFile2 << orderStatistics2.back().date << "," << orderStatistics2.back().direction << "," << orderStatistics2.back().quantity << "," << orderStatistics2.back().price << "\n";
        }


        cashflowFile << date << "," << fixed << setprecision(2) << cashInHand1 + cashInHand2 << "\n";
    }

    double pnl1 = 0.0;
    double pnl2 = 0.0;

    if (position1 != 0) {
        double finalPrice = data1.back().price;
        pnl1 = position1 * finalPrice;
        pnl1 += cashInHand1;
    }

    if (position2 != 0) {
        double finalPrice = data2.back().price;
        pnl2 = position2 * finalPrice;
        pnl2 += cashInHand2;
    }

    finalPNLFile << fixed << setprecision(2) << pnl1 + pnl2;
}
