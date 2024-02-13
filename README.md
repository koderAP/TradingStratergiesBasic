# Algorithmic Trading Strategies

## Background

Before diving into the implementation of trading strategies, let's familiarize ourselves with some key concepts:

- **Position**: The quantity of shares held by a trader. It can be long (buy) or short (sell).
- **Price Series**: A sequence of prices of a stock over a period of time.
- **Moving Average**: An indicator used to smooth out price data by creating a constantly updated average price.
- **Exponential Weighted Mean (EWM)**: A type of moving average where more weight is given to recent prices.

## Constraints and Assumptions

Throughout this assignment, we make the following assumptions:

- We only consider the closing price of stocks for analysis, unless specified otherwise.
- Short-selling of stocks is allowed.
- The maximum position a trader can take is limited to a certain value, denoted as `x`.
- All dates are represented in the format "DD/MM/YYYY".

## 1. Momentum Based Strategies

### 1.1 Basic Strategy

#### Parameters
- `n`: Number of past days to consider.
- `x`: Maximum position allowed.

#### Implementation:

 ``` 
 make strategy=BASIC symbol=SBIN n=5 x=2 start_date="b" end_date="a" 
 ```


### 1.2 Trend-based Strategy (n-Day Moving Average - DMA)

#### Parameters
- `n`: Number of past days for calculating the moving average.
- `x`: Maximum position allowed.
- `p`: Standard deviation threshold.

#### Implementation:

```
make strategy=DMA symbol=SBIN n=50 x=3 p=2 start_date="a" end_date="b"
```


### 1.3 Improving DMA


#### Parameters
- `x`: Maximum position allowed.
- `p`: Percent change needed.
- `n`: Number of past days needed to calculate efficiency ratio.
- `max hold days`: Maximum number of days positions can be held.
- `c1`, `c2`: Smoothing factor parameters.

#### Implementation:

```
make strategy="DMA++" symbol=SBIN x=4 p=5 n=14 max_hold_days=28 c1=2 c2=0.2 start_date="a" end_date="b"
```


### 1.4 Using Indicators

#### 1.4.1 Moving Average Convergence Divergence (MACD)

#### Parameters
- `x`: Maximum position allowed.

#### Implementation:

```
make strategy=MACD symbol=SBIN x=3 start_date="a" end_date="b"
```


#### 1.4.2 Relative Strength Index (RSI)

#### Parameters
- `x`: Maximum position allowed.
- `n`: Number of past days for calculating average gain and loss.
- `oversold_threshold`, `overbought_threshold`: RSI threshold values.

#### Implementation:

```
make strategy=RSI symbol=SBIN x=3 n=14 oversold_threshold=30 overbought_threshold=70 start_date="a" end_date="b"
```


#### 1.4.3 Average Directional Index (ADX)

#### Parameters
- `x`: Maximum position allowed.
- `n`: Number of past days for calculating TR, DM, and ATR.
- `adx_threshold`: ADX threshold value.

#### Implementation:

```
make strategy=ADX symbol=SBIN x=3 n=14 adx_threshold=25 start_date="a" end_date="b"
```


# Linear Regression Strategy

In this strategy, we implement a Machine Learning algorithm using Linear Regression to predict the price of a stock at day t. The prediction is based on historical data, and we use the predicted price to make trading decisions.

## Strategy Description

We use the following Linear Regression Equation to predict the closing price of the stock at day t:

`Close_t = β0 + β1Close_{t-1} + β2Open_{t-1} + β3VWAP_{t-1} + β4Low_{t-1} + β5High_{t-1} + β6(No of Trades)_{t-1} + β7*Open_t`


- We learn the parameters (coefficients) using historical data from `train_start_date` to `train_end_date`.
- The predicted price is compared with the actual price at day t.
- If the predicted price is greater than the actual price by ≥ p%, we buy the stock.
- If the predicted price is less than the actual price by ≥ p%, we sell the stock.

## Intuition

If the predicted price is higher than the actual price, we expect the actual price to move towards the predicted price in the future. Therefore, we buy the stock with the hope of selling it later at a higher price.

## Constraints and Assumptions

- `train_end_date` should be earlier than `start_date`.
- Short-selling of stocks is allowed.

## Parameters

- `x`: The maximum position allowed.
- `train_start_date`, `train_end_date`: The dates for training the Linear Regression model.
- `p`: The percent difference required for making trading decisions.

## Implementation:

To run this strategy, use the following command:

```
make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a" train_end_date="b" start_date="c" end_date="d"
```

# Best of All Strategy

In the "Best of All" strategy, we aim to find the strategy that yields the best overall Profit and Loss (PnL) among all the strategies discussed so far. Each strategy will behave independently, and we will run all the strategies in parallel to find the best one. To achieve parallel execution, we will use pthreads or OpenMP.

## Constraints and Assumptions

- For the basic strategy, we always use `n=7`. For DMA, `n=50`. For DMA++, RSI, and ADX, `n=14`.
- Maximum position in all cases will be restricted to `x=5`.
- ADX threshold is set to `25`, oversold threshold to `30`, overbought threshold to `70`, and max hold days to `28`.
- For DMA, `p=2`, while for DMA++, `p=5`.
- For linear regression, one-year past data is used. For example, if the start date is 2nd April 2023 and the end date is 29th May 2023, then the train start date is set as 2nd April 2022 and train end date as 29th May 2022. It is guaranteed that the start date and end date lie in the same year.

## Implementation

To run this strategy, use the following command:

`make strategy="BEST_OF_ALL" symbol=SBIN start_date="a" end_date="b"`

# Mean-Reverting Pairs Trading Strategy

In the Mean-Reverting Pairs Trading Strategy, we implement a trading strategy based on a pair of stocks rather than a single stock. This strategy aims to capitalize on the spread between the prices of two correlated stocks.


## Intuition

The strategy relies on the mean-reverting behavior of the spread between the two stocks. When the z-score is high, indicating that the spread is above its mean, we expect it to return to its mean, prompting us to short the spread. Conversely, when the z-score is low, we expect the spread to increase, leading us to long the spread.

## Constraints and Assumptions

- Short selling is allowed.
- The maximum position allowed in the spread is +/-x.
- Parameters to the Strategy:
  - x: Maximum position allowed
  - n: Lookback period
  - threshold: Z-score threshold

## Implementation

To run this strategy, use the following command:

```
make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 start_date="a" end_date="b"
```

## Additional Features

### Stop-Loss in Pairs Trading Strategy

Incorporates a stop-loss mechanism based on the z-score crossing the stop_loss_threshold.

- Given stop_loss_threshold, close the position when the z-score crosses the threshold.
- Use the same mean and standard deviation used to open the position to calculate the z-score.

To run this strategy with stop-loss, use the following command:

`make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 stop_loss_threshold=4 start_date="a" end_date="b"`

# Algorithmic Trading Strategies Report

In this report, we analyze the performance of various algorithmic trading strategies implemented and backtested on historical stock data. Each strategy aims to generate buy or sell signals based on specific market conditions and indicators.

# Algorithmic Trading Strategies

In this subtask, we implemented various trading strategies and backtested them on historical data. Each strategy aims to generate buy or sell signals based on specific conditions observed in the price series of a stock. We covered momentum-based strategies, trend-based strategies, strategies using technical indicators, linear regression strategy, and mean-reverting pairs trading strategy.

## Key Insights

### Momentum-Based Strategies

#### Basic Strategy
- Simple but inconsistent results
- Performs well in trending markets

#### Trend-Based Strategy (DMA)
- More consistent results
- Filters out noise but less responsive to rapid changes

#### Improved DMA Strategy (DMA++)
- Enhanced performance with stop-loss and smoothing
- More adaptive to market conditions

#### Using Indicators
- MACD: Effective in signaling trend reversals
- RSI: Useful for identifying overbought and oversold conditions
- ADX: Indicates trend strength, helpful in assessing momentum

### Linear Regression Strategy
- Systematic approach to price prediction
- Performs well in certain conditions
- May struggle in volatile markets, parameter tuning can improve accuracy

### Best of All Strategy
- Combines strategies for diversification and risk mitigation
- Best-performing strategy varies with market conditions
- Emphasizes adaptability

### Mean-Reverting Pairs Trading Strategy
- Exploits mean-reverting behavior in correlated stock pairs
- Requires careful parameter selection and risk management
- Stop-loss mechanisms enhance risk-adjusted returns

These insights offer a concise overview of each strategy's performance, characteristics, and considerations for improvement.
