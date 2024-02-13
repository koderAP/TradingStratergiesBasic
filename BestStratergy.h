// #include <iostream>
#include <fstream>
#include <vector>
#include <omp.h> 
#include <cstdlib>

using namespace std;

struct StrategyParams {
    string command;
    string symbol;
    string start_date;
    string end_date;
    double pnl;
};

void execute_strategy(StrategyParams* strategyParams) {
    string command = strategyParams->command;
    string symbol = strategyParams->symbol;
    string start_date = strategyParams->start_date;
    string end_date = strategyParams->end_date;

    double pnl = 0.0; 
    string cashflowFilePath = "daily_cashflow_" + command + ".csv";
    string orderStatisticsFilePath = "order_statistics_" + command + ".csv";
    string finalPNLFilePath = "final_pnl_" + command + ".txt";
    string binaryFilePath = symbol + ".bin";

    ofstream cashflowFile(cashflowFilePath);
    ofstream orderStatisticsFile(orderStatisticsFilePath);
    ofstream finalPNLFile(finalPNLFilePath);


    if (command == "BASIC") {
        cout << "Executing BASIC strategy" << endl;
        int n = 7;
        int x = 5;
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


    } else if (command == "DMA") {
        cout << "Executing DMA strategy" << endl;

        int n = 50;
        int p = 2;
        int x = 5;

        string binaryFilePath = symbol +".bin";
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

    } else if (command == "DMA++") {
        cout << "Executing DMA++ strategy" << endl;

        double p = 5;
        int n = 14;
        int max_hold_days = 28;
        float c2 = 2;
        float c1 = 0.2;
        float x = 5;


        string binaryFilePath = symbol +".bin";


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


    } else if (command == "MACD") {
        cout << "Executing MACD strategy" << endl;

        int x = 5;
        string binaryFilePath = symbol +".bin";


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

    } else if (command == "RSI") {
        cout << "Executing RSI strategy" << endl;

        int x = 5;
        int n = 14;
        int oversoldThreshold = 30;
        int overboughtThreshold = 70;

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

    } else if (command == "ADX") {
        cout << "Executing ADX strategy" << endl;

        int x = 5;
        int n = 14;
        int adx_threshold = 25;

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

    } else if (command == "LINEAR_REGRESSION") {
        cout << "Executing LINEAR_REGRESSION strategy" << endl;
        int x = 5;
        int p = 2;
        
        vector<Record> inputData = readBinary(binaryFilePath);

        vector<Order> orderStatistics;
        ofstream cashflowFile(cashflowFilePath);
        ofstream orderStatisticsFile(orderStatisticsFilePath);
        ofstream finalPNLFile(finalPNLFilePath);

        cashflowFile << "Date" << "," << "Cashflow" << "\n";
        orderStatisticsFile << "Date" << "," << "Order_dir" << "," << "Quantity" << "," << "Price" << "\n";

        int startDateInt = converet_to_int(start_date);
        int trainstartint = startDateInt - 10000;
        int endDateInt = converet_to_int(end_date);
        int trainendtint = endDateInt - 10000;
        string train_start_date = convert_to_tarik(trainstartint);
        string train_end_date = convert_to_tarik(trainendtint);


        int date_pos = pos_start(inputData, startDateInt);
        int startDateInt_train = converet_to_int(train_start_date);
        string filenem = symbol + ".bin"; 
        vector<Record> inputDatatrain = readBinary(filenem);
        int date_pos_train = pos_start(inputDatatrain, startDateInt_train);
        int date_pos_end_train = pos_start(inputDatatrain, trainendtint);
        cout<<train_end_date<<"**"<<date_pos_train<<"##"<<startDateInt_train<<endl;

        LinearRegressionStrategy_(symbol, inputDatatrain, inputData, x, p, orderStatistics, cashflowFile, orderStatisticsFile, finalPNLFile, date_pos, date_pos_train, date_pos_end_train);
    }


    cashflowFile.close();
    orderStatisticsFile.close();
    finalPNLFile.close();
}

void bestStrategy(string symbol, string start_date, string end_date){
    const int NUM_STRATEGIES = 7;
    vector<StrategyParams> strategyParams(NUM_STRATEGIES);

    strategyParams[0] = {"BASIC", symbol, start_date, end_date};
    strategyParams[1] = {"DMA", symbol, start_date, end_date};
    strategyParams[2] = {"DMA++", symbol, start_date, end_date};
    strategyParams[3] = {"MACD", symbol, start_date, end_date};
    strategyParams[4] = {"RSI", symbol, start_date, end_date};
    strategyParams[5] = {"ADX", symbol, start_date, end_date};
    strategyParams[6] = {"LINEAR_REGRESSION", symbol, start_date, end_date};

    #pragma omp parallel for
    for (int i = 0; i < NUM_STRATEGIES; ++i) {
        execute_strategy(&strategyParams[i]);
    }

    double maxPnL = -1e20;
    string bestStrategy;
    for (int i = 0; i < NUM_STRATEGIES; ++i) {
        string pnlFilePath = "final_pnl_" + strategyParams[i].command + ".txt";
        ifstream pnlFile(pnlFilePath);
        double pnl;
        if (pnlFile.is_open()) {
            pnlFile >> pnl;
            pnlFile.close();
            if (pnl > maxPnL) {
                maxPnL = pnl;
                bestStrategy = strategyParams[i].command;
            }
        } else {
            cerr << "Error opening PnL file: " << pnlFilePath << endl;
        }
    }

    string cashflowFilePathSrc = "daily_cashflow_" + bestStrategy + ".csv";
    string orderStatisticsFilePathSrc = "order_statistics_" + bestStrategy + ".csv";
    string finalPNLFilePathSrc = "final_pnl_" + bestStrategy + ".txt";
    string finalPNLFilePathDest = "final_pnl.txt";
    string cashflowFilePathDest = "daily_cashflow.csv";
    string orderStatisticsFilePathDest = "order_statistics.csv";

    ifstream cashflowFileSrc(cashflowFilePathSrc);
    ifstream orderStatisticsFileSrc(orderStatisticsFilePathSrc);
    ifstream finalPNLFileSrc(finalPNLFilePathSrc);
    ofstream finalPNLFileDest(finalPNLFilePathDest);
    ofstream cashflowFileDest(cashflowFilePathDest);
    ofstream orderStatisticsFileDest(orderStatisticsFilePathDest);

    if (cashflowFileSrc.is_open()) {
        cashflowFileDest << cashflowFileSrc.rdbuf();
        cashflowFileSrc.close();
    } else {
        cerr << "Error opening daily cashflow file: " << cashflowFilePathSrc << endl;
    }
    if (orderStatisticsFileSrc.is_open()) {
        orderStatisticsFileDest << orderStatisticsFileSrc.rdbuf();
        orderStatisticsFileSrc.close();
    } else {
        cerr << "Error opening order statistics file: " << orderStatisticsFilePathSrc << endl;
    }
    if (finalPNLFileSrc.is_open()) {
        finalPNLFileDest << finalPNLFileSrc.rdbuf();
        finalPNLFileSrc.close();
    } else {
        cerr << "Error opening final PnL file: " << finalPNLFilePathSrc << endl;
    }


    for (int i = 0; i < NUM_STRATEGIES; ++i) {
        string pnlFilePath = "final_pnl_" + strategyParams[i].command + ".txt";
        string cashflowFilePath = "daily_cashflow_" + strategyParams[i].command + ".csv";
        string orderStatisticsFilePath = "order_statistics_" + strategyParams[i].command + ".csv";
        remove(pnlFilePath.c_str());
        remove(cashflowFilePath.c_str());
        remove(orderStatisticsFilePath.c_str());
    }
    finalPNLFileDest.close();
    cashflowFileDest.close();
    orderStatisticsFileDest.close();

    string filee = symbol+".bin";
    remove(filee.c_str());

    
    cout << "Best Strategy: " << bestStrategy << ", with PnL: " << maxPnL << endl;




}
