#include "ReadWrite.h"

void ADXStrategy(const vector<Record>& data, int start, double x, int n, double adx_threshold,
                 vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile) {
    vector<double> tr;
    vector<double> dm_plus;
    vector<double> dm_minus;
    vector<double> atr;
    vector<double> di_plus;
    vector<double> di_minus(data.size());
    vector<double> dx(data.size());
    vector<double> adx(data.size());

    // Calculate True Range (TR), DM+, and DM- for each day
    for (size_t i = start; i < data.size(); ++i) {
        double tr_high_low = data[i].high - data[i].low;
        double tr_high_prev_close = data[i].high - data[i].price;
        double tr_low_prev_close = data[i].low - data[i - 1].price;
        tr.push_back(max({tr_high_low, tr_high_prev_close, tr_low_prev_close}));

        dm_plus.push_back(max(0.0, data[i].high - data[i - 1].high));
        dm_minus.push_back(max(0.0, data[i].low - data[i - 1].low));
    }

    atr.push_back(tr[0]);

    for (size_t i = start + 1; i < data.size(); ++i) {
        atr.push_back((2 / (n + 1)) * (tr[i - n] - atr[i - n - 1]) + atr[i - n - 1]);
    }

    for (size_t i = start; i < data.size(); ++i){
        
    }

        double sum_tr = 0.0;
        double sum_dm_plus = 0.0;
        double sum_dm_minus = 0.0;

        // Calculate sum of TR, DM+ and DM- for the past n days
        for (size_t j = i - n; j <= i; ++j) {
            sum_tr += tr[j];
            sum_dm_plus += dm_plus[j];
            sum_dm_minus += dm_minus[j];
        }

        // Calculate ATR, DI+, and DI-
        atr[i] = sum_tr / n;
        di_plus[i] = sum_dm_plus / atr[i];
        di_minus[i] = sum_dm_minus / atr[i];
    }

    // Calculate DX for each day
    for (size_t i = n + 1; i < data.size(); ++i) {
        dx[i] = abs((di_plus[i] - di_minus[i]) / (di_plus[i] + di_minus[i])) * 100;
    }

    // Calculate ADX as EWM of DX
    for (size_t i = 2 * n + 1; i < data.size(); ++i) {
        adx[i] = (adx[i - 1] * (n - 1) + dx[i]) / n;
    }

    // Generate buy/sell signals based on ADX threshold
    for (size_t i = 2 * n + 1; i < data.size(); ++i) {
        if (adx[i] > adx_threshold && adx[i - 1] <= adx_threshold) {
            orderStatistics.push_back({"BUY", 1, data[i]});
        } else if (adx[i] < adx_threshold && adx[i - 1] >= adx_threshold) {
            orderStatistics.push_back({"SELL", 1, data[i]});
        }

        cashflowFile << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        orderStatisticsFile << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
    }
}