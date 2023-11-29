import pandas as pd
import glob

# List of log files
log_files = glob.glob("**/*_log.csv",recursive=True)

# Read and concatenate all log files
all_logs = pd.concat([pd.read_csv(f) for f in log_files])

# Sort by timestamp
all_logs.sort_values(by='Timestamp', inplace=True)

df = all_logs.copy()

# Convert the 'Timestamp' column to datetime with nanoseconds resolution
df['Timestamp'] = pd.to_datetime(df['Timestamp'])

# Calculate the difference in milliseconds relative to the earliest timestamp
df['Relative Time (ms)'] = (df['Timestamp'] - df['Timestamp'].min()).dt.total_seconds() * 1000
# Process or analyze the merged logs
# ...

# Optionally, save the merged logs
df.to_csv("merged_log.csv", index=False)
