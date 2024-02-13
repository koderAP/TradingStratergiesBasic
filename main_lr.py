from jugaad_data.nse import stock_df
from datetime import datetime, timedelta
import time
import os
import sys
import pandas as pd
import numpy as np

def get_stock_data(symbol, start_date, end_date, additional_days):
    end_date = datetime.strptime(end_date, '%d/%m/%Y')
    start_date_with_additional = (datetime.strptime(start_date, '%d/%m/%Y') - timedelta(days=additional_days))
    print(f"Fetching data for {symbol} from {start_date_with_additional} to {end_date}")
    columns_to_remove = ['PREV. CLOSE', 'SERIES', '52W H', 'SYMBOL', '52W L']
    df = stock_df(symbol=symbol, from_date=start_date_with_additional, to_date=end_date, series="EQ")
    df = df.drop(columns=columns_to_remove)
    df = df[['DATE','CLOSE','HIGH','LOW', 'OPEN', 'VWAP', 'NO OF TRADES']]
    return df

def write_to_binary(data, symbol):
    binary_file_name = f"{symbol}_train.bin"
    
    data_sorted = data.sort_values(by='DATE', ascending=True).reset_index(drop=True)

    data_sorted['DATE'] = pd.to_datetime(data_sorted['DATE'], format='%d/%m/%Y').dt.strftime('%Y%m%d').astype(int)  
    data_sorted['CLOSE'] = data_sorted['CLOSE'].astype(np.float64)  # Convert 'CLOSE' column to float64
    data_sorted['HIGH'] = data_sorted['HIGH'].astype(np.float64)  # Convert 'HIGH' column to float64
    data_sorted['LOW'] = data_sorted['LOW'].astype(np.float64)  # Convert 'LOW' column to float64
    data_sorted['OPEN'] = data_sorted['OPEN'].astype(np.float64)  # Convert 'OPEN' column to float64
    data_sorted['VWAP'] = data_sorted['VWAP'].astype(np.float64)  # Convert 'VWAP' column to float64
    data_sorted['NO OF TRADES'] = data_sorted['NO OF TRADES'].astype(np.int64)  # Convert 'NO OF TRADES' column to int64

    np_data = data_sorted.to_records(index=False)
    with open(binary_file_name, 'wb') as binary_file:
        for record in np_data:
            binary_file.write(record[0].tobytes())  # Write date as int
            binary_file.write(record[1].tobytes())  # Write close price as binary
            binary_file.write(record[2].tobytes())  # Write high price as binary
            binary_file.write(record[3].tobytes())  # Write low price as binary
            binary_file.write(record[4].tobytes())  # Write open price as binary
            binary_file.write(record[5].tobytes())  # Write vwap price as binary
            binary_file.write(record[6].tobytes())  # Write no of trades as binary
def write_to_csv(data, symbol):
    data = data.sort_values(by='DATE', ascending=True).reset_index(drop=True)
    csv_file_name = f"{symbol}.csv"
    data.to_csv(csv_file_name, index=False)

args = {}
for key, value in os.environ.items():
    if key in ['symbol', 'n', 'train_start_date', 'train_end_date']:
        args[key] = value

# Check if all required arguments are present
required_args = ['symbol', 'train_start_date', 'train_end_date']
print(args)
if all(arg in args for arg in required_args):
    symbol = args['symbol']
    start_date = args['train_start_date']
    end_date = args['train_end_date']
    additional_days = int(args.get('n', 10)) 
else:
    print("Error: Missing required arguments.")

data = get_stock_data(symbol, start_date, end_date, 10*additional_days)

write_to_binary(data, symbol)
# write_to_csv(data, symbol)
