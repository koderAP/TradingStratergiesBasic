#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Order {
    string date;
    string direction;
    int quantity;
    double price;
};

struct Record {
    int date;   // Date in integer format (DDMMYYYY)
    double price; // Price in double format
    double high;
    double low;
};

vector<Record> readBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening binary file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    // Determine the size of the file
    file.seekg(0, ios::end);
    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);

    // Calculate the number of records (each record is 12 bytes: 4 bytes for date and 8 bytes for price)
    int numRecords = fileSize / sizeof(Record);

    // Create a vector to store the records
    vector<Record> records(numRecords);

    // Read the binary data into the vector of records
    file.read(reinterpret_cast<char*>(records.data()), fileSize);

    // Clean up
    file.close();

    return records;
}

int pos_start(const vector<Record>& data, int start_date){
    for(int i = 0; i < data.size(); ++i){
        if (data[i].date == start_date){
            return i;
        }
    }
    return -1;
}

string convert_to_date(Record a){
    int day = a.date / 1000000;
    int month = (a.date / 10000) % 100;
    int year = a.date % 10000;

    string date = to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    return date;
}

void RSIStrategy(const vector<Record>& data, int start, double x, int n, double oversoldThreshold, double overboughtThreshold,
                 vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
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

int main() {
    string binaryFilePath = "SBIN.bin";

    string cashflowFilePathRSI = "daily_cashflow_RSI.csv";
    string orderStatisticsFilePathRSI = "order_statistics_RSI.csv";
    string finalPNLFilePathRSI = "final_pnl_RSI.txt";

    vector<Record> inputData = readBinary(binaryFilePath);

    vector<Order> orderStatisticsRSI;
    ofstream cashflowFileRSI(cashflowFilePathRSI);
    ofstream orderStatisticsFileRSI(orderStatisticsFilePathRSI);
    ofstream finalPNLFileRSI(finalPNLFilePathRSI);

    double xRSI = 3.0;
    int nRSI = 14;
    double oversoldThresholdRSI = 30.0;
    double overboughtThresholdRSI = 70.0;

    int date = 1022023;
    int date_pos = pos_start(inputData, date);

    RSIStrategy(inputData, date_pos, xRSI, nRSI, oversoldThresholdRSI, overboughtThresholdRSI,
                orderStatisticsRSI, cashflowFileRSI, orderStatisticsFileRSI, finalPNLFileRSI);

    cashflowFileRSI.close();
    orderStatisticsFileRSI.close();
    finalPNLFileRSI.close();

    return 0;
}
