void basicStrategy(const vector<Record>& data, int start,  int n, int x, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
    double cashInHand = 0.0;
    int position = 0;
    n++;

    int inc = 1;
    int dec = 1;

    for (size_t i = start - n + 1; i < start; ++i) {
        if (data[i].price > data[i - 1].price) {
            inc += 1;
            dec = 1;
        }
        else if (data[i].price < data[i - 1].price) {
            dec += 1;
            inc = 1;
        }
    }

    for (size_t i = start; i < data.size(); ++i) {
        double closePrice = data[i].price;

        if (data[i].price > data[i - 1].price) {
            inc += 1;
            dec = 1;
        }
        else if (data[i].price < data[i - 1].price) {
            dec += 1;
            inc = 1;
        }

        string date = convert_to_date(data[i]);

        if (inc >= n && position < x) {
            orderStatistics.push_back({date, "BUY", 1, closePrice});
            cashInHand -= closePrice;  
            position += 1;  
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        } 
        else if (dec >= n && position > -x) {
            orderStatistics.push_back({date, "SELL", 1, closePrice});
            cashInHand += closePrice; 
            position -= 1;
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }

        // Write cash flow for the day
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
