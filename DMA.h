void DMAStrategy(const vector<Record>& data, int start, int n, double x, double p, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
    double cashInHand = 0.0;
    int position = 0;

    double sum = 0.0;

    for (size_t i = start - n; i < start; i++) {
        sum += data[i].price;
    }

    double dma = sum / n;

    double sumSquare = 0.0;

    for (size_t i = start - n; i < start; i++){
        sumSquare += data[i].price * data[i].price;
    }

    for (size_t i = start; i < data.size(); ++i) {
        double closePrice = data[i].price;

        string date = convert_to_date(data[i]);

        dma += (data[i].price / n);
        dma -= (data[i - n].price / n);

        sumSquare += data[i].price * data[i].price;
        sumSquare -= data[i - n].price * data[i - n].price;

        double sd = sqrt((sumSquare / n) - dma * dma);
        
        if (closePrice > (dma + p * sd) && position < x) {
            orderStatistics.push_back({date, "BUY", 1, closePrice});
            cashInHand -= closePrice;  
            position += 1;  
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        } else if (closePrice < (dma - p * sd) && position > -x) {
            orderStatistics.push_back({date, "SELL", 1, closePrice});
            cashInHand += closePrice; 
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