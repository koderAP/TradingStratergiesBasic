#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

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
            X[i].insert(X[i].begin(), 1.0);
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

        cout << "----------------------" << endl;
        for (auto i : XtX)
        {
            for (auto j : i)
            {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << "----------------------" << endl;

        XtX_inv = inverse(XtX);

        cout << "----------------------" << endl;
        for (auto i : XtX_inv)
        {
            for (auto j : i)
            {
                cout << j << " ";
            }
            cout << endl;
        }
        cout << "----------------------" << endl;

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
            prediction += coefficients[i + 1] * x[i];
        }
        prediction += coefficients[0];

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
