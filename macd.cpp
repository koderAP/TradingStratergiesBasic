#include "macd.h"


vector<double> readPricesFromCSV(const string& filename) {
    vector<double> prices;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            string date, open, high, low, close, adjClose, volume;
            getline(ss, date, ',');
            getline(ss, open, ',');
            getline(ss, high, ',');
            getline(ss, low, ',');
            getline(ss, close, ',');
            getline(ss, adjClose, ',');
            getline(ss, volume, ',');
            prices.push_back(stod(close));
        }
        file.close();
    } else {
        cout << "Failed to open file: " << filename << endl;
    }
    
    return prices;
}


int main() {
    vector<double> prices = readPricesFromCSV("SBIN.csv");
    reverse(prices.begin(), prices.end()); // Reverse the order of prices

    MACDindicator macd(prices);

    macd.simulateTrading();

    return 0;
}
