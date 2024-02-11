void basicStrategy(const vector<Record>& data, int start,  int n, double x, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
    double cashInHand = 0.0;
    int position = 0;

    string prev = "wh";

    int inc = 1;
    int dec = 1;

    for (size_t i = start - n + 1; i < start; ++i){
        if (data[i].price - data[i - 1].price > 0) {
            inc += 1;
            dec = 1;
        }
        else if (data[i].price - data[i - 1].price < 0) {
            dec += 1;
            inc = 1;
        }
    }

    for (size_t i = start; i < data.size(); ++i) {
        double closePrice = data[i].price;

        if (orderStatistics.size() != 0){
            prev = orderStatistics.back().direction;
        }

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
        } else if (dec >= n && position > -x) {
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