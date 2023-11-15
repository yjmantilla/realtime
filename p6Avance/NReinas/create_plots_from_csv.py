
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import datetime

# Function to create and save plots from CSV file
def create_and_save_plots(csv_file_path):
    # Read the CSV file into a DataFrame
    data = pd.read_csv(csv_file_path)

    # Convert the 'time' column to datetime objects assuming it's in Unix time
    data['time'] = pd.to_datetime(data['time'], unit='s')

    # Calculate total seconds and average temperature
    data['total_seconds'] = data['seconds'] + data['microseconds'] * 1e-6
    data['avg_temp'] = (data['cpu_temp'] + data['gpu_temp']) / 2

    # Calculate relative time in seconds and then convert to minutes for the x-axis
    data['relative_time'] = (data['time'] - data['time'].iloc[0]).dt.total_seconds()
    data['experiment_time_minutes'] = data['relative_time'] / 60

    # Plot 1: CPU and GPU temperatures over experiment time in minutes
    fig, ax1 = plt.subplots()
    color = 'tab:red'
    ax1.set_xlabel('Experiment Time (minutes)')
    ax1.set_ylabel('CPU Temperature (C)', color=color)
    ax1.plot(data['experiment_time_minutes'], data['cpu_temp'], color=color)
    ax1.tick_params(axis='y', labelcolor=color)
    ax2 = ax1.twinx()
    color = 'tab:blue'
    ax2.set_ylabel('GPU Temperature (C)', color=color)
    ax2.plot(data['experiment_time_minutes'], data['gpu_temp'], color=color)
    ax2.tick_params(axis='y', labelcolor=color)
    plt.title('CPU and GPU Temperature Over Experiment Time')
    plt.savefig('cpu_gpu_temperature_plot.png')
    plt.close()

    # Plot 2: Total seconds and average temperature over experiment time in minutes
    fig, ax3 = plt.subplots()
    color = 'tab:green'
    ax3.set_xlabel('Experiment Time (minutes)')
    ax3.set_ylabel('Time in Seconds', color=color)
    ax3.plot(data['experiment_time_minutes'], data['total_seconds'], color=color)
    ax3.tick_params(axis='y', labelcolor=color)
    ax4 = ax3.twinx()
    color = 'tab:purple'
    ax4.set_ylabel('Average Temperature (C)', color=color)
    ax4.plot(data['experiment_time_minutes'], data['avg_temp'], color=color)
    ax4.tick_params(axis='y', labelcolor=color)
    plt.title('Elapsed Time and Average Temperature Over Experiment Time')
    plt.savefig('elapsed_time_average_temperature_plot.png')
    plt.close()

# Replace 'your_csv_file.csv' with your actual CSV file path
csv_file_path = 'p6Avance/multithread_output-NoRT.csv'
create_and_save_plots(csv_file_path)
