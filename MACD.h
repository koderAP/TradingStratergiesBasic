void MACDStrategy(const vector<Record>& data, int start, double x, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
    vector<double> prices;
    for (const auto& record : data) {
        prices.push_back(record.price);
    }

    vector<double> shortEWM;
    vector<double> longEWM;
    vector<double> MACD;
    vector<double> Signal;

    // Calculate short and long EWMs
    double alpha_short = 2.0 / (12 + 1);
    double alpha_long = 2.0 / (26 + 1);

    shortEWM.push_back(data[start].price);
    longEWM.push_back(data[start].price);

    for (size_t i = start + 1; i < prices.size(); ++i) {
        shortEWM.push_back(alpha_short * (prices[i] - shortEWM[i - start - 1]) + shortEWM[i - start - 1]);
    }

    for (size_t i = start + 1; i < prices.size(); ++i) {
        longEWM.push_back(alpha_long * (prices[i] - longEWM[i - start - 1]) + longEWM[i - start - 1]);
    }

    for (size_t i = 0; i < shortEWM.size(); ++i) {
        MACD.push_back(shortEWM[i] - longEWM[i]);
    }

    Signal.push_back(MACD[0]);
    double alpha_signal = 2.0 / (9 + 1);
    for (size_t i = start + 1; i < prices.size(); ++i) {
        Signal.push_back(alpha_signal * (MACD[i - start] - Signal[i - start - 1]) + Signal[i - start - 1]);
    }
    double cashInHand = 0.0;
    int position = 0;

    for (size_t i = start; i < prices.size(); ++i) {
        double closePrice = prices[i];
        string date = convert_to_date(data[i]);

        // Check for buy/sell signals based on MACD and Signal
        if (MACD[i - start] > Signal[i - start] && position < x) {
            orderStatistics.push_back({date, "BUY", 1, closePrice});
            cashInHand -= closePrice;  
            position += 1;  
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        } else if (MACD[i - start] < Signal[i - start] && position > -x) {
            orderStatistics.push_back({date, "SELL", 1, closePrice});
            cashInHand += closePrice; 
            position -= 1;
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }

        cashflowFile << date << "," << fixed << setprecision(2) << cashInHand << "\n";
    }

    if (position != 0) {
        double finalPrice = prices.back();
        double pnl = position * finalPrice;
        pnl += cashInHand;
        finalPNLFile << fixed << setprecision(2) << pnl;
    } else {
        finalPNLFile << 0.0;
    }
}
