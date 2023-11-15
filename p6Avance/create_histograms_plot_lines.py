
import pandas as pd
import matplotlib.pyplot as plt
import glob
import re
import os

# Function to read a histogram file and convert the 'bin' column to numeric.
def read_histogram(file_path):
    try:
        # Read the histogram assuming space-separated values without headers.
        data = pd.read_csv(file_path, sep='\s+', header=None, names=['bin', 'count'])
        # Convert 'bin' to numeric.
        data['bin'] = pd.to_numeric(data['bin'])
    except Exception as e:
        print(f"An error occurred while reading {file_path}: {e}")
        return None
    return data

# Function to plot and superpose histograms with log scale using line curves.
def plot_histograms_log_scale(histogram_files):
    plt.figure(figsize=(10, 8))

    # Loop through each file to read and plot data.
    for file_path in histogram_files:
        # Extract information from the file name.
        file_name = os.path.basename(file_path)
        rt_status = "RT Patched" if "RT Patched" in file_name else "No RT Patch"
        thread_number = re.search(r'histogram(\d+)', file_name).group(1)
        label = f'{rt_status} Thread {thread_number}'
        color = 'red' if "RT Patched" in rt_status else 'blue'

        # Read the histogram data.
        histogram = read_histogram(file_path)
        if histogram is not None:
            # Plot as a line curve with log scale on y-axis
            plt.plot(histogram['bin'], histogram['count'], label=label, color=color, alpha=0.7, linewidth=2)

    plt.xlabel('Bin')
    plt.ylabel('Count (log scale)')
    plt.yscale('log')
    plt.title('Superposed Histograms with Log Scale')
    plt.legend()
    plt.tight_layout()
    plt.savefig('superposed_histograms_log_scale_lines.png')
    plt.close()

# Get the list of all histogram files in the current directory.
histogram_files = glob.glob('p6Avance/histogram_combinado/histogram*')

# Call the function to plot the histograms with log scale.
plot_histograms_log_scale(histogram_files)
