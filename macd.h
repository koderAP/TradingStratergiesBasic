#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class MACDindicator {
private:
    vector<double> prices;
    vector<double> shortEWM;
    vector<double> longEWM;
    vector<double> MACD;
    vector<double> Signal;

    void calculateSEWM(int days) {
        double alpha = 2.0 / (days + 1);
        shortEWM.resize(prices.size());        
        shortEWM[0] = prices[0]; 
        for (size_t i = 1; i < prices.size(); ++i) {
            shortEWM[i] = alpha * (prices[i] - shortEWM[i - 1]) + shortEWM[i - 1];
        }
    }

    void calculateLEWM(int days) {
        double alpha = 2.0 / (days + 1);
        longEWM.resize(prices.size());
        longEWM[0] = prices[0];
        for (size_t i = 1; i < prices.size(); ++i) {
            longEWM[i] = alpha * (prices[i] - longEWM[i - 1]) + longEWM[i - 1];
        }
    }


    void calculateMACD() {
        MACD.resize(prices.size());
        for (size_t i = 0; i < prices.size(); ++i) {
            MACD[i] = shortEWM[i] - longEWM[i];
        }
    }

    void calculateSignal() {
        Signal.resize(prices.size());
        Signal[0] = MACD[0];
        double alpha = 2.0 / (9 + 1);
        for (size_t i = 0; i < prices.size(); ++i) {
            Signal[i] = alpha * (MACD[i] - Signal[i - 1]) + Signal[i - 1];
        }
    }

public:
    MACDindicator(const vector<double>& prices) : prices(prices) {
        calculateSEWM(12); 
        calculateLEWM(26);
        calculateMACD(); 
        calculateSignal(); 
    }

    void simulateTrading() {
        int shares = 0;
        for (size_t i = 26; i < prices.size(); ++i) {

            if (MACD[i] > Signal[i] && shares < 4) {
                shares++;
                cout << "Buy 1 share at day " << i <<" Price "<< prices[i]<< endl;
            } else if (MACD[i] < Signal[i] && shares > -4) {
                shares--;
                cout << "Sell 1 share at day " << i <<" Price "<< prices[i]<< endl;
            } else {
                cout << "Hold at day " << i << endl;
            }
        }
    }
};