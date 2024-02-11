#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
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

void trendBasedStrategy(const vector<Record>& data, int start, int n, double x, double p, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
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

    string prev = "wh";
    for (size_t i = start; i < data.size(); ++i) {
        double closePrice = data[i].price;

        string date = convert_to_date(data[i]);

        dma += (data[i].price / n);
        dma -= (data[i - n].price / n);

        sumSquare += data[i].price * data[i].price;
        sumSquare -= data[i - n].price * data[i - n].price;

        double sd = sqrt((sumSquare / n) - dma * dma);

        cout << "dma : " << dma << " sd : " << sd << endl;

        if (orderStatistics.size() != 0){
            prev = orderStatistics.back().direction;
        }
        if (closePrice > (dma + p * sd) && position < x) {
            orderStatistics.push_back({date, "BUY", 1, closePrice});
            cashInHand -= closePrice;  
            position += 1;  
        } else if (closePrice < (dma - p * sd) && position > -x) {
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


int main() {
    string binaryFilePath = "SBIN.bin";

    string cashflowFilePathTrend = "daily_cashflow_trend.csv";
    string orderStatisticsFilePathTrend = "order_statistics_trend.csv";
    string finalPNLFilePathTrend = "final_pnl_trend.txt";

    vector<Record> inputData = readBinary(binaryFilePath);
    
    vector<Order> orderStatisticsTrend;
    ofstream cashflowFileTrend(cashflowFilePathTrend);
    ofstream orderStatisticsFileTrend(orderStatisticsFilePathTrend);
    ofstream finalPNLFileTrend(finalPNLFilePathTrend);

    int nTrend = 50;  
    double xTrend = 3.0; 
    double pTrend = 2.0; 

    int date = 1022023;

    date = pos_start(inputData, date);

    trendBasedStrategy(inputData, date, nTrend, xTrend, pTrend, orderStatisticsTrend, cashflowFileTrend, orderStatisticsFileTrend, finalPNLFileTrend);

    cashflowFileTrend.close();
    orderStatisticsFileTrend.close();
    finalPNLFileTrend.close();

    return 0;
}
