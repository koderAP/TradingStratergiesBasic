class LinearRegression
{
private:
    vector<vector<double>> X;
    vector<double> y;
    vector<double> coefficients;
    
    vector<vector<double>> inverse(const vector<vector<double>> &matrix)
    {
        int n = matrix.size();

        vector<vector<double>> augmented(n, vector<double>(2 * n, 0.0));
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                augmented[i][j] = matrix[i][j];
            }
            augmented[i][i + n] = 1.0;
        }

        for (int i = 0; i < n; ++i)
        {
            double pivot = augmented[i][i];
            for (int j = 0; j < 2 * n; ++j)
            {
                augmented[i][j] /= pivot;
            }

            for (int j = 0; j < n; ++j)
            {
                if (j != i)
                {
                    double factor = augmented[j][i];
                    for (int k = 0; k < 2 * n; ++k)
                    {
                        augmented[j][k] -= factor * augmented[i][k];
                    }
                }
            }
        }

        vector<vector<double>> result(n, vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                result[i][j] = augmented[i][j + n];
            }
        }

        return result;
    }

public:
    LinearRegression(vector<vector<double>> &features, vector<double> &targets)
    {
        X = features;
        y = targets;

        size_t num_samples = X.size();
        size_t num_features = X[0].size();

        for (size_t i = 0; i < num_samples; ++i)
        {
            X[i].push_back(1.0);
        }

        vector<vector<double>> XtX(num_features + 1, vector<double>(num_features + 1, 0.0));
        vector<vector<double>> Xt(num_features + 1, vector<double>(num_samples, 0.0));
        vector<vector<double>> XtX_inv(num_features + 1, vector<double>(num_features + 1, 0.0));

        for (size_t i = 0; i < num_samples; ++i)
        {
            for (size_t j = 0; j < num_features + 1; ++j)
            {
                Xt[j][i] = X[i][j];
            }
        }

        for (size_t i = 0; i < num_features + 1; ++i)
        {
            for (size_t j = 0; j < num_features + 1; ++j)
            {
                for (size_t k = 0; k < num_samples; ++k)
                {
                    XtX[i][j] += Xt[i][k] * X[k][j];
                }
            }
        }



        XtX_inv = inverse(XtX);



        vector<vector<double>> check(8, vector<double>(8, 0.0));
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                for (int k = 0; k < 8; ++k)
                {
                    check[i][j] += XtX[i][k] * XtX_inv[k][j];
                }
            }
        }

        for (auto i : check)
        {
            for (auto j : i)
            {
                cout << (j < 1e-8 ? 0:j ) << " ";
            }
            cout << endl;
        }

        vector<vector<double>> XtX_inv_Xt(num_features + 1, vector<double>(num_samples, 0.0));
        for (size_t i = 0; i < num_features + 1; ++i)
        {
            for (size_t j = 0; j < num_samples; ++j)
            {
                for (size_t k = 0; k < num_features + 1; ++k)
                {
                    XtX_inv_Xt[i][j] += XtX_inv[i][k] * Xt[k][j];
                }
            }
        }

        coefficients.resize(num_features + 1, 0.0);
        for (size_t i = 0; i < num_features + 1; ++i)
        {
            for (size_t j = 0; j < num_samples; ++j)
            {
                coefficients[i] += XtX_inv_Xt[i][j] * y[j];
            }
        }
    }

    double predict(const vector<double> &x) const
    {
        double prediction = 0.0;
        for (size_t i = 0; i < x.size(); ++i)
        {
            prediction += coefficients[i] * x[i];
        }
        prediction += coefficients[coefficients.size() - 1];

        return prediction;
    }

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


void LinearRegressionStrategy(string symbol, vector<Record> inputDatatrain, const vector<Record>& inputData, int x, int p, vector<Order>& orderStatistics, ofstream& cashflowFile, ofstream& orderStatisticsFile, ofstream& finalPNLFile, int date_pos, int date_pos_train){
    vector<double> open;
    vector<double> high;
    vector<double> low;
    vector<double> close;
    vector<double> vwap;
    vector<double> numtrades;


    cout<< inputDatatrain[0].date<<" "<<inputDatatrain[inputDatatrain.size()-1].date<<endl;
    cout<< inputData[0].date<<" "<<inputData[inputData.size()-1].date<<endl;

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

    cout<<"Date pos : "<<date_pos<<" Date pos train : "<<date_pos_train<<endl;
    

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
        cout<<inputDatatrain[i].date<<" ";
        for(auto i:row){
            cout<<i<<" ";
        }cout<<"**"<<close[i]<<endl;
        X.push_back(row);
        y.push_back(close[i]);
    }

    LinearRegression lr(X, y);
    lr.print_coefficients();


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

    // first_open_value = open[0];
    // first_high_value = high[0];
    // first_low_value = low[0];
    // first_close_value = close[0];
    // first_vwap_value = vwap[0];
    // first_trades_value = numtrades[0];




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
        cout<<"Date : "<<date<<" Prediction : "<<prediction<<" Actual : "<<closePrice<<endl;
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
    finalPNLFile << pnl;



}