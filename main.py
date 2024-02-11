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

def write_to_binary_and_csv(data, symbol):
    binary_file_name = f"{symbol}.bin"
    csv_file_name = f"{symbol}.csv"
    start_time = time.time()
    
    # Reverse the DataFrame
    data_reversed = data.iloc[::-1]
    
    # Convert date to YYYYMMDD format and then to int
    data_reversed['DATE'] = pd.to_datetime(data_reversed['DATE'], format='%d/%m/%Y').dt.strftime('%d%m%Y').astype(int)  
    data_reversed['CLOSE'] = data_reversed['CLOSE'].astype(np.float64)  # Convert 'CLOSE' column to float64
    data_reversed['HIGH'] = data_reversed['HIGH'].astype(np.float64)  # Convert 'HIGH' column to float64
    data_reversed['LOW'] = data_reversed['LOW'].astype(np.float64)  # Convert 'LOW' column to float64
    
    # Save binary file
    np_data = data_reversed.to_records(index=False)
    with open(binary_file_name, 'wb') as binary_file:
        for record in np_data:
            binary_file.write(record[0].tobytes())  # Write date as int
            binary_file.write(record[1].tobytes())  # Write close price as binary
            binary_file.write(record[2].tobytes())  # Write high price as binary
            binary_file.write(record[3].tobytes())  # Write low price as binary
    
    # Save CSV file
    data_reversed.to_csv(csv_file_name, index=False)
    
    end_time = time.time()

    try:
        binary_file_size = os.path.getsize(binary_file_name)
        csv_file_size = os.path.getsize(csv_file_name)
    except FileNotFoundError:
        print("File not found.")
    except OSError:
        print("OS error occurred.")

    return end_time - start_time, binary_file_size, csv_file_size


# ARGUMENTS FOR MAKE
# symbol = sys.argv[1]
# n = int(sys.argv[2])
# x = float(sys.argv[3])
# start_date = sys.argv[4]
# end_date = sys.argv[5]
###################

data = get_stock_data("SBIN", "22/03/2022", "22/01/2024", 100)

# Write to binary and CSV format
time_taken, binary_file_size, csv_file_size = write_to_binary_and_csv(data, "SBIN")

print(f"Binary: Time taken - {time_taken} seconds, Binary file size - {binary_file_size} bytes")
print(f"CSV: CSV file size - {csv_file_size} bytes")
