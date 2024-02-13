#include <iostream>
#include <vector>
#include <string>

class LinearRegressionBonus
{
private:
    vector<vector<double>> X;
    vector<double> y;
    vector<double> coefficients;

public:
    LinearRegressionBonus(vector<vector<double>> &features, vector<double> &targets)
    {
        X = features;
        y = targets;
        int n = X[0].size();
        coefficients.resize(n, 0.1);
    }

    double loss() const
    {
        size_t num_samples = X.size();
        double error = 0.0;
        for (size_t i = 0; i < num_samples; ++i)
        {
            double predicted = predict(X[i]);
            double diff = predicted - y[i];
            error += diff * diff;
        }
        return error / (2 * num_samples);
    }

    vector<double> gradient() const
    {
        size_t num_features = X[0].size();
        size_t num_samples = X.size();
        vector<double> grad(num_features, 0.0);

        for (size_t i = 0; i < num_samples; ++i)
        {
            double predicted = predict(X[i]);
            double diff = predicted - y[i];
            for (size_t j = 0; j < num_features; ++j)
            {
                grad[j] += diff * X[i][j];
            }
        }

        for (size_t j = 0; j < num_features; ++j)
        {
            grad[j] /= num_samples;
        }

        return grad;
    }

    void gradient_descent(double learning_rate, int iterations)
    {
        size_t num_features = X[0].size();

        for (int iter = 0; iter < iterations; ++iter)
        {
            vector<double> grad = gradient();
            for (size_t j = 0; j < num_features; ++j)
            {
                coefficients[j] -= learning_rate * grad[j];
            }
        }
    }

    // Function to predict y for given x
    double predict(const vector<double> &x) const
    {
        double prediction = 0.0;
        size_t num_features = x.size();
        for (size_t i = 0; i < num_features; ++i)
        {
            prediction += coefficients[i] * x[i];
        }
        return prediction;
    }

    // Function to print coefficients
    void print_coefficients() const
    {
        cout << "Coefficients: ";
        for (double coeff : coefficients)
        {
            cout << coeff << " ";
        }
        cout << endl;
    }
};

void BonusStratergy(string symbol, vector<Record> inputDatatrain, const vector<Record>& inputData, int x, int p, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile, int date_pos, int date_pos_train, double learning_rate=0.1, int iteration=100000){
    vector<double> open;
    vector<double> high;
    vector<double> low;
    vector<double> close;
    vector<double> vwap;
    vector<double> numtrades;


    // cout<< inputDatatrain[0].date<<" "<<inputDatatrain[inputDatatrain.size()-1].date<<endl;
    // cout<< inputData[0].date<<" "<<inputData[inputData.size()-1].date<<endl;

    for (size_t i = 0; i < inputDatatrain.size(); ++i)
    {
        open.push_back(inputDatatrain[i].open);
        high.push_back(inputDatatrain[i].high);
        low.push_back(inputDatatrain[i].low);
        close.push_back(inputDatatrain[i].price);
        vwap.push_back(inputDatatrain[i].vwap);
        numtrades.push_back(inputDatatrain[i].numtrade);
    }


    double first_open_value = open[0];
    double first_high_value = high[0];
    double first_low_value = low[0];
    double first_close_value = close[0];
    double first_vwap_value = vwap[0];
    double first_trades_value = numtrades[0];


    for (size_t i = 0; i < open.size(); ++i)
    {
        open[i] /= first_open_value;
        high[i] /= first_high_value;
        low[i] /= first_low_value;
        close[i] /= first_close_value;
        vwap[i] /= first_vwap_value;
        numtrades[i] /= first_trades_value;
    }



    vector<vector<double>> X;
    vector<double> y;

    

    for (size_t i = date_pos_train; i < close.size(); ++i)
    {
        vector<double> row;
        row.push_back(close[i - 1]);
        row.push_back(vwap[i - 1]);
        row.push_back(low[i - 1]);
        row.push_back(high[i - 1]);
        row.push_back(numtrades[i - 1]);
        row.push_back(open[i - 1]);
        row.push_back(open[i]);

        X.push_back(row);
        y.push_back(close[i]);
    }


    LinearRegressionBonus lr(X, y);
    lr.gradient_descent(learning_rate, iteration);


    double cashInHand = 0.0;
    int position = 0;

    open = {};
    high = {};
    low = {};
    close = {};
    vwap = {};
    numtrades = {};

    for (size_t i = 0; i < inputData.size(); ++i)
    {
        open.push_back(inputData[i].open);
        high.push_back(inputData[i].high);
        low.push_back(inputData[i].low);
        close.push_back(inputData[i].price);
        vwap.push_back(inputData[i].vwap);
        numtrades.push_back(inputData[i].numtrade);
    }
        cout<<inputDatatrain[date_pos_train].date<<" "<<inputDatatrain[inputDatatrain.size()-1].date<<endl;



    for (size_t i = 0; i < open.size(); ++i)
    {
        open[i] /= first_open_value;
        high[i] /= first_high_value;
        low[i] /= first_low_value;
        close[i] /= first_close_value;
        vwap[i] /= first_vwap_value;
        numtrades[i] /= first_trades_value;
    }

    vector<vector<double>> X_test;
    vector<double> y_test;


    for (size_t i = date_pos; i < close.size(); ++i)
    {
        vector<double> row;
        row.push_back(close[i - 1]);
        row.push_back(vwap[i - 1]);
        row.push_back(low[i - 1]);
        row.push_back(high[i - 1]);
        row.push_back(numtrades[i - 1]);
        row.push_back(open[i - 1]);
        row.push_back(open[i]);
        X_test.push_back(row);
        y_test.push_back(close[i]);
    }


    for (size_t i = 0; i < X_test.size(); ++i)
    {
        double prediction = lr.predict(X_test[i])*first_close_value;
        double closePrice = close[i+date_pos]*first_close_value;

        string date = convert_to_date(inputData[i+date_pos]);
        double brate = (100.0 + p)/100;
        double srate = (100.0 - p)/100;

        if (prediction >= closePrice*brate && position < x)
        {

            orderStatistics.push_back({date, "BUY", 1, closePrice});
            cashInHand -= closePrice;
            position += 1;
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }
        else if (prediction <= closePrice*(srate) && position > -x)
        {

            orderStatistics.push_back({date, "SELL", 1, closePrice});
            cashInHand += closePrice;
            position -= 1;
            orderStatisticsFile << orderStatistics.back().date << "," << orderStatistics.back().direction << "," << orderStatistics.back().quantity << "," << orderStatistics.back().price << "\n";
        }

        cashflowFile << date << "," << cashInHand << "\n";
    }


    double finalPrice = close.back()*first_close_value;
    double pnl = position * finalPrice;
    pnl += cashInHand;
    cout<<position<<" "<<finalPrice<<" "<<cashInHand<<endl;
    finalPNLFile << fixed << setprecision(2) << pnl;

}

    
          