import matplotlib.pyplot as plt

# Assuming scale is a list of x-axis values
threads = [1, 2, 4, 8, 16, 32, 64, 128, 256]

# Assuming serial, omp, atomic are lists of y-axis values for each series
omp = [4189120.0, 2432710.0, 1340580.0, 745258.0, 401190.0, 219299.0, 120213.0, 66909.1, 36029.3]
atomic = [10059400.0, 9471960.0, 9272050.0, 8903100.0, 8559990.0, 7834680.0, 6993490.0, 5549810.0, 4216120.0]

# Create a new figure
plt.figure()

# Plot each series
plt.plot(threads, omp, label='OMP')
plt.plot(threads, atomic, label='Atomic')

# Add labels and title
plt.xlabel('Threads')
plt.ylabel('Median_TEPS')
plt.title('Line Chart of Median_TEPS')

# Add a legend
plt.legend()

# Display the plot
plt.show()
