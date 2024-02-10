#include "linearregression.h"

int main()
{
    ifstream file("SBIN.csv");
    if (!file.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    string line;
    getline(file, line);

    vector<double> open_values, high_values, low_values, prev_close_values, vwap_values, trades_values, close_values;

    while (getline(file, line))
    {
        stringstream ss(line);
        string cell;
        vector<string> tokens;

        while (getline(ss, cell, ','))
        {
            tokens.push_back(cell);
        }

        open_values.push_back(stod(tokens[2]));
        high_values.push_back(stod(tokens[3]));
        low_values.push_back(stod(tokens[4]));
        prev_close_values.push_back(stod(tokens[5]));
        vwap_values.push_back(stod(tokens[8]));
        trades_values.push_back(stod(tokens[13]));
        close_values.push_back(stod(tokens[7]));
    }

    file.close();

    double first_open_value = open_values[0];
    double first_high_value = high_values[0];
    double first_low_value = low_values[0];
    double first_prev_close_value = prev_close_values[0];
    double first_vwap_value = vwap_values[0];
    double first_trades_value = trades_values[0];
    double first_close_value = close_values[0];

    for (size_t i = 0; i < open_values.size(); ++i)
    {
        open_values[i] /= first_open_value;
        high_values[i] /= first_high_value;
        low_values[i] /= first_low_value;
        prev_close_values[i] /= first_prev_close_value;
        vwap_values[i] /= first_vwap_value;
        trades_values[i] /= first_trades_value;
        close_values[i] /= first_close_value;
    }

    vector<vector<double>> X, X_test;
    vector<double> y, y_test;

    for (size_t i = 100; i < close_values.size(); ++i)
    {
        vector<double> row;
        row.push_back(prev_close_values[i]);
        row.push_back(open_values[i]);
        row.push_back(vwap_values[i - 1]);
        row.push_back(low_values[i - 1]);
        row.push_back(high_values[i - 1]);
        row.push_back(trades_values[i - 1]);
        row.push_back(open_values[i - 1]);
        X.push_back(row);
        y.push_back(close_values[i]);
    }

    for (size_t i = 0; i < 100; ++i)
    {
        vector<double> row;
        row.push_back(prev_close_values[i]);
        row.push_back(open_values[i]);
        row.push_back(vwap_values[i]);
        row.push_back(low_values[i]);
        row.push_back(high_values[i]);
        row.push_back(trades_values[i]);
        row.push_back(open_values[i]);
        X_test.push_back(row);
        y_test.push_back(close_values[i]);
    }

    LinearRegression lr(X, y);
    lr.print_coefficients();

    int buy = 0;
    int sell = 0;
    double balance = 0;
    int k = 0;
    for (size_t i = 99; i > 0; i--)
    {
        double a = lr.predict(X_test[i]) * first_close_value;
        double b = y_test[i] * first_close_value;
        string s = "";
        if (a >= b * 1.002)
        {
            s = "Buy";
        }
        else if (a <= b * 0.998)
        {
            s = "Sell";
        }
        int mex = 4;
        if (s == "Buy")
        {
            buy++;
            if (k < mex)
            {
                balance -= b;
                k++;
            }
        }
        else if (s == "Sell")
        {
            sell++;
            if (k > -1 * mex)
            {
                balance += b;
                k--;
            }
        }
        cout << "Predicted: " << a << ", Actual: " << b << " , Suggestion : " << s << "  ,Current Holdings : " << k << endl;
    }
    balance = balance + k * first_close_value;
    cout << "Buy: " << buy << " & Sell: " << sell << endl;
    cout << "Balance: " << balance << endl;
    cout << "_____________________________________________" << endl;

    for (auto i : X[0])
    {
        cout << i << " ";
    }
    cout << endl;
    for (auto i : X[1])
    {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
