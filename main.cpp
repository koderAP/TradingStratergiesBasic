#include <iostream>
#include <sstream>
#include "ReadWrite.h"
#include "BasicStrategy.h"
#include "DMA.h"
#include "dma++.h"
#include "MACD.h"
#include "RSI.h"
#include "ADX.h"
#include "LinearRegression.h"
#include "Pairs.h"
#include "PairsStopLoss.h"

using namespace std;

int main(int argc, char *argv[]) {
    
    
    string command = argv[1];

    if (command == "BASIC") {
        cout << "Executing BASIC strategy" << endl;
        string symbol = argv[2];
        int n = stoi(argv[3]);
        int x = stoi(argv[4]);
        string start_date = argv[5];
        string end_date = argv[6];

        string binaryFilePath = symbol +".bin";
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

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        basicStrategy(inputData, date_pos, n, x, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

        cashflowFile.close();
        orderStatisticsFile.close();
        finalPNLFile.close();

    } else if (command == "DMA") {
        cout << "Executing DMA strategy" << endl;

        string symbol = argv[2];
        int n = stoi(argv[3]);
        int x = stoi(argv[4]);
        int p = stoi(argv[5]);
        string start_date = argv[6];
        string end_date = argv[7];

        string binaryFilePath = symbol +".bin";
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

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        DMAStrategy(inputData, date_pos, n, x, p, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

        cashflowFile.close();
        orderStatisticsFile.close();
        finalPNLFile.close();

    } else if (command == "DMA++") {
        cout << "Executing DMA++ strategy" << endl;
        
        string symbol = argv[2];
        double x = stod(argv[3]);
        double p = stod(argv[4]);
        int n = stoi(argv[5]);
        int max_hold_days = stoi(argv[6]);
        float c1 = stof(argv[7]);
        float c2 = stof(argv[8]);
        string start_date = argv[9];
        string end_date = argv[10];

        string binaryFilePath = symbol +".bin";
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

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        DMAPlusPlusStrategy(inputData, date_pos, x, p, n, max_hold_days, c1, c2, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

        cashflowFile.close();
        orderStatisticsFile.close();
        finalPNLFile.close();

    } else if (command == "MACD") {
        cout << "Executing MACD strategy" << endl;

        string symbol = argv[2];
        int x = stoi(argv[3]);
        string start_date = argv[4];
        string end_date = argv[5];
        string binaryFilePath = symbol +".bin";
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

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        MACDStrategy(inputData, date_pos, x, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

        cashflowFile.close();
        orderStatisticsFile.close();
        finalPNLFile.close();

    } else if (command == "RSI") {
        cout << "Executing RSI strategy" << endl;

        string symbol = argv[2];
        int x = stoi(argv[3]);
        int n = stoi(argv[4]);
        int oversoldThreshold = stoi(argv[5]);
        int overboughtThreshold = stoi(argv[6]);
        string start_date = argv[7];
        string end_date = argv[8];

        string binaryFilePath = symbol +".bin";
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

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);

        RSIStrategy(inputData, date_pos, x, n, oversoldThreshold, overboughtThreshold, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

        cashflowFile.close();
        orderStatisticsFile.close();
        finalPNLFile.close();

    } else if (command == "ADX") {
        cout << "Executing ADX strategy" << endl;

        string symbol = argv[2];
        int x = stoi(argv[3]);
        int n = stoi(argv[4]);
        int adx_threshold = stoi(argv[5]);
        string start_date = argv[6];
        string end_date = argv[7];

        string binaryFilePath = symbol +".bin";
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

        int startDateInt = converet_to_int(start_date);

        int date_pos = pos_start(inputData, startDateInt);
        ADXStrategy(inputData, date_pos, x, n, adx_threshold, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile);

        cashflowFile.close();
        orderStatisticsFile.close();
        finalPNLFile.close();

    } else if(command == "LINEAR_REGRESSION"){

        cout << "Executing LINEAR_REGRESSION strategy" << endl;
        string symbol = argv[2];
        int x = stoi(argv[3]);
        int p = stoi(argv[4]);
        string train_start_date = argv[5];
        string train_end_date = argv[6];
        string start_date = argv[7];
        string end_date = argv[8];




        string binaryFilePath = symbol +".bin";
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

        

        int startDateInt = converet_to_int(start_date);
        int date_pos = pos_start(inputData, startDateInt);
        int startDateInt_train = converet_to_int(train_start_date);
        string filenem = symbol + "_train.bin"; 
        vector<Record> inputDatatrain = readBinary(filenem);
        int date_pos_train = pos_start(inputDatatrain, startDateInt_train);
        cout<<train_end_date<<"**"<<date_pos_train<<"##"<<startDateInt_train<<endl;

        LinearRegressionStrategy(symbol, inputDatatrain, inputData, x, p, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile, date_pos, date_pos_train);

    } else if (command == "PAIRS") {
    cout << "Executing Pairs Stop loss strategy" << endl;

        string symbol1 = argv[2];
        string symbol2 = argv[3];
        int x = stoi(argv[4]);
        int n = stoi(argv[5]);
        int threshold = stoi(argv[6]);
        int stop_loss_threshold = -1;

        string start_date;
        string end_date;
        if (argc > 9) {
            stop_loss_threshold = stoi(argv[7]);
            start_date = argv[8];
            end_date = argv[9];
        }
        else {
            start_date = argv[7];
            end_date = argv[8];
        }


        string binaryFilePath1 = symbol1 + ".bin";
        string binaryFilePath2 = symbol2 + ".bin";
        string cashflowFilePath = "daily_cashflow.csv";
        string orderStatisticsFilePath1 = "order_statistics_1.csv";
        string orderStatisticsFilePath2 = "order_statistics_2.csv";
        string finalPNLFilePath = "final_pnl.txt";
        vector<Record> inputData1 = readBinary(binaryFilePath1);
        vector<Record> inputData2 = readBinary(binaryFilePath2);

        vector<Order> orderStatistics1;
        vector<Order> orderStatistics2;
        ofstream cashflowFile(cashflowFilePath);
        ofstream orderStatisticsFile1(orderStatisticsFilePath1);
        ofstream orderStatisticsFile2(orderStatisticsFilePath2);
        ofstream finalPNLFile(finalPNLFilePath);

        cashflowFile << "Date" << "," << "Cashflow" << "\n";
        orderStatisticsFile1 << "Date" << "," << "Order_dir" << "," << "Quantity" << "," << "Price" << "\n";
        orderStatisticsFile2 << "Date" << "," << "Order_dir" << "," << "Quantity" << "," << "Price" << "\n";

        int startDateInt = converet_to_int(start_date);

        int date_pos1 = pos_start(inputData1, startDateInt);
        int date_pos2 = pos_start(inputData2, startDateInt);

        if (stop_loss_threshold != -1) {
            PairsStrategyWithStopLoss(inputData1, inputData2, date_pos1, x, n, threshold, stop_loss_threshold, orderStatistics1, orderStatistics2, cashflowFile, orderStatisticsFile1, orderStatisticsFile2, finalPNLFile);
        } else {
            PairsStrategy(inputData1, inputData2, date_pos1, x, n, threshold, orderStatistics1, orderStatistics2, cashflowFile, orderStatisticsFile1, orderStatisticsFile2, finalPNLFile);
        }

        cashflowFile.close();
        orderStatisticsFile1.close();
        orderStatisticsFile2.close();
        finalPNLFile.close();



    } else {
        cerr << "Invalid command: " << command << endl;
        return 1;
    }

    return 0;
}
