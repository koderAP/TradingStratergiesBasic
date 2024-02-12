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
    columns_to_remove = ['PREV. CLOSE', 'SERIES', '52W H', 'VWAP', 'SYMBOL', '52W L']
    df = stock_df(symbol=symbol, from_date=start_date_with_additional, to_date=end_date, series="EQ")
    df = df.drop(columns=columns_to_remove)
    df = df[['DATE','CLOSE','HIGH','LOW']]
    return df

def write_to_binary(data, symbol):
    binary_file_name = f"{symbol}.bin"
    
    data_reversed = data.iloc[::-1]
    
    data_reversed['DATE'] = pd.to_datetime(data_reversed['DATE'], format='%d/%m/%Y').dt.strftime('%Y%m%d').astype(int)  
    data_reversed['CLOSE'] = data_reversed['CLOSE'].astype(np.float64)  # Convert 'CLOSE' column to float64
    data_reversed['HIGH'] = data_reversed['HIGH'].astype(np.float64)  # Convert 'HIGH' column to float64
    data_reversed['LOW'] = data_reversed['LOW'].astype(np.float64)  # Convert 'LOW' column to float64
    
    np_data = data_reversed.to_records(index=False)
    with open(binary_file_name, 'wb') as binary_file:
        for record in np_data:
            binary_file.write(record[0].tobytes())  # Write date as int
            binary_file.write(record[1].tobytes())  # Write close price as binary
            binary_file.write(record[2].tobytes())  # Write high price as binary
            binary_file.write(record[3].tobytes())  # Write low price as binary

    return




args = {}
for arg in sys.argv[1:]:
    if '=' in arg:
        try:
            key, value = arg.split('=')
            args[key] = value
        except ValueError:
            print(f"Invalid argument format: {arg}")
    else:
        print(f"Invalid argument format: {arg}")

required_args = ['symbol', 'n', 'start_date', 'end_date']

if all(arg in args for arg in required_args):
    symbol = args['symbol']
    start_date = args['start_date']
    end_date = args['end_date']
    additional_days = int(args['n'])
else:
    print("Error: Missing required arguments.")



data = get_stock_data(symbol, start_date, end_date, 100*additional_days)

write_to_binary(data, symbol)

