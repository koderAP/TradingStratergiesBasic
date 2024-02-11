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

void dmaPlusPlusStrategy(const vector<Record>& data, int start, double x, double p, int n, int maxHoldDays, double c1, double c2,
                         vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile) {
    double cashInHand = 0.0;
    int position = 0;

    vector<double>er;
    vector<double>sf;
    vector<double>ama;

    double absPriceChangeSum = 0.0;

    string prev = "wh";

    for (size_t i = start - n + 1; i <= start; ++i) {
        absPriceChangeSum += abs(data[i].price - data[i - 1].price);
    }

    double closePrice = data[start].price;
    double priceChange = closePrice - data[start - n].price;

    if (absPriceChangeSum != 0) {
        er.push_back(priceChange / absPriceChangeSum);
    }
    else {
        er.push_back(1000000000.0);
    }

    for (size_t i = start + 1; i < data.size(); ++i){

        absPriceChangeSum += abs(data[i].price - data[i-1].price);
        absPriceChangeSum -= abs(data[i-n].price - data[i-n-1].price);

        closePrice = data[i].price;

        priceChange = closePrice - data[i - n].price;

        if (absPriceChangeSum != 0) {
            er.push_back(priceChange / absPriceChangeSum);
        }
        else {
            er.push_back(1000000000.0);
        }
    }

    sf.push_back(0.5);

    for (size_t i = start + 1; i < data.size(); ++i){

        sf.push_back(sf[i - start - 1] + c1 * ((((2 * er[i - start]) / (1 + c2)) - 1)/(((2 * er[i - start]) / (1 + c2)) + 1) - sf[i - start - 1]));

        double rsf = (1 - c1);
        double asf = ((2 * er[i - start] - 1 - c2) / (2 * er[i - start] + 1 + c2));

        sf.push_back((pow(rsf, (i - start))) * 0.5 + c1 * asf * (1 - pow(rsf, (i - start))) / (1 - rsf));

    }    

    
    ama.push_back(data[start].price);

    for (size_t i = start + 1; i < data.size(); ++i){

        ama.push_back(ama[i - start - 1] + sf[i - start] * (data[i].price - ama[i - start - 1]));

    }

    for (size_t i = 0; i < ama.size(); ++i){

        cout << ama[i] << endl;
    
    }
    for (size_t i = 0; i < ama.size(); ++i){

        closePrice = data[i + start].price;

        if (orderStatistics.size() != 0){
            prev = orderStatistics.back().direction;
        }

        string date = convert_to_date(data[i + start]);

        if (position != 0 && i - orderStatistics.back().quantity >= maxHoldDays) {
            cashInHand += position * closePrice;
            orderStatistics.push_back({date, "FORCE_CLOSE", position, closePrice});
            position = 0;
        }

        if (closePrice > (ama[i] * (1 + p / 100)) && position < x) {
            orderStatistics.push_back({date, "BUY", 1, closePrice});
            cashInHand -= closePrice;
            position += 1;
        } else if (closePrice < (ama[i] * (1 - p / 100)) && position > -x) {
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

    string cashflowFilePathDMAPlusPlus = "daily_cashflow_dma++.csv";
    string orderStatisticsFilePathDMAPlusPlus = "order_statistics_dma++.csv";
    string finalPNLFilePathDMAPlusPlus = "final_pnl_dma++.txt";

    vector<Record> inputData = readBinary(binaryFilePath);

    vector<Order> orderStatisticsDMAPlusPlus;
    ofstream cashflowFileDMAPlusPlus(cashflowFilePathDMAPlusPlus);
    ofstream orderStatisticsFileDMAPlusPlus(orderStatisticsFilePathDMAPlusPlus);
    ofstream finalPNLFileDMAPlusPlus(finalPNLFilePathDMAPlusPlus);

    double xDMAPlusPlus = 4.0;
    double pDMAPlusPlus = 5.0;
    int nDMAPlusPlus = 14;
    int maxHoldDaysDMAPlusPlus = 28;
    double c1DMAPlusPlus = 2.0;
    double c2DMAPlusPlus = 0.2;

    int date = 1022023;
    int date_pos = pos_start(inputData, date);

    dmaPlusPlusStrategy(inputData, date_pos, xDMAPlusPlus, pDMAPlusPlus, nDMAPlusPlus, maxHoldDaysDMAPlusPlus, c1DMAPlusPlus, c2DMAPlusPlus,
                        orderStatisticsDMAPlusPlus, cashflowFileDMAPlusPlus, orderStatisticsFileDMAPlusPlus, finalPNLFileDMAPlusPlus);

    cashflowFileDMAPlusPlus.close();
    orderStatisticsFileDMAPlusPlus.close();
    finalPNLFileDMAPlusPlus.close();

    return 0;
}
