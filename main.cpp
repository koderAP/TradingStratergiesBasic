#include <iostream>
#include <sstream>
#include "ReadWrite.h"
#include "BasicStrategy.h"
#include "DMA.h"
#include "DMA++.h"
#include "MACD.h"
#include "RSI.h"
#include "ADX.h"

using namespace std;

int main(int argc, char *argv[]) {
    
    
    string binaryFilePath = "SBIN.bin";
    string cashflowFilePath = "daily_cashflow.csv";
    string orderStatisticsFilePath = "order_statistics.csv";
    string finalPNLFilePath = "final_pnl.txt";

    vector<Record> inputData = readBinary(binaryFilePath);

    vector<Order> orderStatistics;
    ofstream cashflowFile(cashflowFilePath);
    ofstream orderStatisticsFile(orderStatisticsFilePath);
    ofstream finalPNLFile(finalPNLFilePath);

    cashflowFile << "Date" << "," << "Cashflow" << "\n";
    orderStatisticsFile << "Date" << "," << "Order_dir" << "," << "Quantity" << "," << "Price" << "\n";
    string command = argv[1];

    if (command == "BASIC") {
        cout << "Executing BASIC strategy" << endl;
        string symbol = argv[2];
        int n = stoi(argv[3]);
        int x = stoi(argv[4]);
        string start_date = argv[5];
        string end_date = argv[6];

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        basicStrategy(inputData, date_pos, n, x, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

    } else if (command == "DMA") {
        cout << "Executing DMA strategy" << endl;

        string symbol = argv[2];
        int n = stoi(argv[3]);
        int x = stoi(argv[4]);
        int p = stoi(argv[5]);
        string start_date = argv[6];
        string end_date = argv[7];

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        DMAStrategy(inputData, date_pos, n, x, p, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

    } else if (command == "DMA++") {
        cout << "Executing DMA++ strategy" << endl;

        string symbol = argv[2];
        double x = stod(argv[3]);
        double p = stod(argv[4]);
        int n = stoi(argv[5]);
        int max_hold_days = stoi(argv[6]);
        double c1 = stod(argv[7]);
        double c2 = stod(argv[8]);
        string start_date = argv[9];
        string end_date = argv[10];

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        DMAPlusPlusStrategy(inputData, date_pos, x, p, n, max_hold_days, c1, c2, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

    } else if (command == "MACD") {
        cout << "Executing MACD strategy" << endl;

        string symbol = argv[2];
        int x = stoi(argv[3]);
        string start_date = argv[4];
        string end_date = argv[5];

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        // MACDStrategy(inputData, date_pos, x, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

    } else if (command == "RSI") {
        cout << "Executing RSI strategy" << endl;

        string symbol = argv[2];
        int x = stoi(argv[3]);
        int n = stoi(argv[4]);
        int oversoldThreshold = stoi(argv[5]);
        int overboughtThreshold = stoi(argv[6]);
        string start_date = argv[7];
        string end_date = argv[8];

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        RSIStrategy(inputData, date_pos, x, n, oversoldThreshold, overboughtThreshold, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

    } else if (command == "ADX") {
        cout << "Executing ADX strategy" << endl;

        string symbol = argv[2];
        int x = stoi(argv[3]);
        int n = stoi(argv[4]);
        int adx_threshold = stoi(argv[5]);
        string start_date = argv[6];
        string end_date = argv[7];

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);
        ADXStrategy(inputData, date_pos, x, n, adx_threshold, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

    } else {
        cerr << "Invalid command: " << command << endl;
        return 1;
    }

    cashflowFile.close();
    orderStatisticsFile.close();
    finalPNLFile.close();

    return 0;
}
