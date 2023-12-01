import pandas as pd
import glob
import matplotlib.pyplot as plt
import numpy as np
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


def plot_task_timeline_separated_dfs(df, colormap='tab10', figsize=(15, 8), x_step=2, plot_vertical_lines=True, plot=True):
    """
    Function to plot a task timeline from a dataframe, separating the dataframe into two based on a specific event.

    Parameters:
    df (DataFrame): The dataframe containing task data.
    colormap (str): The name of the matplotlib colormap to use (default: 'tab10').
    figsize (tuple): Figure size (default: (15, 8)).
    x_step (int): Step size for x-axis ticks in milliseconds (default: 2).
    plot_vertical_lines (bool): If True, plots the vertical "I expect to start again around this time" lines.
    plot (bool): If True, plots the figure; otherwise, returns the figure object.

    Returns:
    matplotlib.figure.Figure: The created figure (only if plot is False).
    """
    # Separating the dataframe into two based on a specific event
    df_vertical_lines = df[df['Event'].str.contains("I expect to start again around this time")]
    df_others = df[~df['Event'].str.contains("I expect to start again around this time")]

    unique_tasks = df['Task'].unique()
    switch_points = []
    current_task = None

    for index, row in df_others.iterrows():
        if row['Task'] != current_task:
            switch_points.append((current_task, row['Task'], row['Relative Time (ms)']))
            current_task = row['Task']

    if switch_points:
        switch_points.insert(0, (None, df_others['Task'].iloc[0], df_others['Relative Time (ms)'].iloc[0]))

    max_time = df['Relative Time (ms)'].max()
    colors = plt.get_cmap(colormap)(np.linspace(0, 1, len(unique_tasks)))

    fig, ax = plt.subplots(figsize=figsize)

    # Creating a list to hold the legend handles
    legend_handles = []

    for idx, task in enumerate(unique_tasks):
        task_segments = []
        for i in range(len(switch_points) - 1):
            if switch_points[i][1] == task:
                start_time = switch_points[i][2]
                end_time = switch_points[i + 1][2]
                task_segments.append((start_time, end_time))

        for start_time, end_time in task_segments:
            line = ax.hlines(y=idx + 1, xmin=start_time, xmax=end_time, colors=colors[idx], linewidth=2)
        
        # Add the line to the legend handles
        legend_handles.append(line)

        if plot_vertical_lines:
            restart_events = df_vertical_lines[df_vertical_lines['Task'] == task]
            restart_times = restart_events['Relative Time (ms)'].values
            for restart_time in restart_times:
                ax.axvline(x=restart_time, ymin=0, ymax=1, linestyle='--', color=colors[idx])

    ax.set_xlabel('Relative Time (ms)')
    ax.set_ylabel('Tasks')
    ax.set_title('Task Timeline')
    ax.set_xticks(np.arange(0, max_time + x_step, x_step))
    ax.set_xticklabels(ax.get_xticks(), rotation=90)
    ax.set_yticks(np.arange(1, len(unique_tasks) + 1))
    ax.set_yticklabels(unique_tasks)
    
    # Setting the legend with the correct color for each task
    ax.legend(legend_handles, [task for task in unique_tasks], loc='upper left')
    ax.grid(True)

    if plot:
        plt.show()
    else:
        return fig

# Trying out the function with separated dataframes
fig=plot_task_timeline_separated_dfs(df, plot_vertical_lines=False,plot=False)
fig.savefig("merged_log.png")