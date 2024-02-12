from jugaad_data.nse import stock_df
from datetime import datetime, timedelta
import time
import os
import sys
import pandas as pd
import numpy as np

def get_stock_data(symbol1, symbol2, start_date, end_date, additional_days):
    end_date = datetime.strptime(end_date, '%d/%m/%Y')
    start_date_with_additional = (datetime.strptime(start_date, '%d/%m/%Y') - timedelta(days=additional_days))
    columns_to_remove = ['PREV. CLOSE', 'SERIES', '52W H', 'VWAP', 'SYMBOL', '52W L']
    df1 = stock_df(symbol=symbol1, from_date=start_date_with_additional, to_date=end_date, series="EQ")
    df1 = df1.drop(columns=columns_to_remove)
    df1 = df1[['DATE','CLOSE','HIGH','LOW']]

    df2 = stock_df(symbol=symbol2, from_date=start_date_with_additional, to_date=end_date, series="EQ")
    df2 = df2.drop(columns=columns_to_remove)
    df2 = df2[['DATE','CLOSE','HIGH','LOW']]

    return df1, df2

def write_to_binary(data, symbol):
    binary_file_name = f"{symbol}.bin"
    grouped_data = data.groupby('DATE').agg({'CLOSE': 'mean', 'HIGH': 'mean', 'LOW': 'mean'}).reset_index()

    data_sorted = grouped_data.sort_values(by='DATE', ascending=True).reset_index(drop=True)

    data_sorted['DATE'] = pd.to_datetime(data_sorted['DATE'], format='%d/%m/%Y').dt.strftime('%Y%m%d').astype(int)  
    data_sorted['CLOSE'] = data_sorted['CLOSE'].astype(np.float64)  # Convert 'CLOSE' column to float64
    data_sorted['HIGH'] = data_sorted['HIGH'].astype(np.float64)  # Convert 'HIGH' column to float64
    data_sorted['LOW'] = data_sorted['LOW'].astype(np.float64)  # Convert 'LOW' column to float64
    
    np_data = data_sorted.to_records(index=False)
    with open(binary_file_name, 'wb') as binary_file:
        for record in np_data:
            binary_file.write(record[0].tobytes())  # Write date as int
            binary_file.write(record[1].tobytes())  # Write close price as binary
            binary_file.write(record[2].tobytes())  # Write high price as binary
            binary_file.write(record[3].tobytes())  # Write low price as binary

def write_to_csv(data, symbol):
    grouped_data = data.groupby('DATE').agg({'CLOSE': 'mean', 'HIGH': 'mean', 'LOW': 'mean'}).reset_index()

    sorted_data = grouped_data.sort_values(by='DATE', ascending=True).reset_index(drop=True)

    csv_file_name = f"{symbol}.csv"
    sorted_data.to_csv(csv_file_name, index=False)


args = {}
for key, value in os.environ.items():
    if key in ['symbol1', 'symbol2', 'n', 'start_date', 'end_date']:
        args[key] = value

# Check if all required arguments are present
required_args = ['symbol1', 'symbol2', 'n', 'start_date', 'end_date']

if all(arg in args for arg in required_args):
    symbol1 = args['symbol1']
    symbol2 = args['symbol2']
    start_date = args['start_date']
    end_date = args['end_date']
    additional_days = int(args['n'])
else:
    print("Error: Missing required arguments.")

data1, data2 = get_stock_data(symbol1, symbol2, start_date, end_date, 10 + additional_days)

write_to_binary(data1, symbol1)
# write_to_csv(data1, symbol1)

write_to_binary(data2, symbol2)
# write_to_csv(data2, symbol2)
