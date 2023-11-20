import pandas as pd
import glob

# List of log files
log_files = glob.glob("**/*_log.csv",recursive=True)

# Read and concatenate all log files
all_logs = pd.concat([pd.read_csv(f) for f in log_files])

# Sort by timestamp
all_logs.sort_values(by='Timestamp', inplace=True)

# Process or analyze the merged logs
# ...

# Optionally, save the merged logs
all_logs.to_csv("merged_logs.csv", index=False)
