#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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

        cout << inc << endl;
        cout << date << endl;
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

int main() {
    string binaryFilePath = "SBIN.bin";
    string cashflowFilePath = "daily_cashflow.csv";
    string orderStatisticsFilePath = "order_statistics.csv";
    string finalPNLFilePath = "final_pnl.txt";

    vector<Record> inputData = readBinary(binaryFilePath);

    vector<Order> orderStatistics;
    ofstream cashflowFile(cashflowFilePath);
    ofstream orderStatisticsFile(orderStatisticsFilePath);
    ofstream finalPNLFile(finalPNLFilePath);

    int n = 7;  
    double x = 5.0; 

    int date = 22032022;
    int date_pos = pos_start(inputData, date);

    basicStrategy(inputData, date_pos, n, x, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

    cashflowFile.close();
    orderStatisticsFile.close();
    finalPNLFile.close();

    return 0;
}
