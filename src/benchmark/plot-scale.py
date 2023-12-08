import matplotlib.pyplot as plt

# Assuming scale is a list of x-axis values
scale = range(10, 21)

# Assuming serial, omp, atomic are lists of y-axis values for each series
serial = [71696100.0, 64452400.0, 54965200.0, 45870600.0, 43164400.0, 33418500.0, 29635200.0, 20176000.0, 17515100.0, 17762000.0, 17889900.0]
omp = [28979.2, 54868.6, 101214.0, 169622.0, 277440.0, 407389.0, 595892.0, 769373.0, 884890.0, 975859.0, 1009530.0]
atomic = [4984540.0, 6770100.0, 8274980.0, 8835330.0, 8991160.0, 8501110.0, 7439720.0, 5875410.0, 4693250.0, 4167500.0, 3762110.0]

# Create a new figure
plt.figure()

# Plot each series
plt.plot(scale, serial, label='Serial')
plt.plot(scale, omp, label='OMP-16threads')
plt.plot(scale, atomic, label='Atomic-16threads')

# Add labels and title
plt.xlabel('Scale')
plt.ylabel('Median_TEPS')
plt.title('Line Chart of Median_TEPS')

# Add a legend
plt.legend()

# Display the plot
plt.show()
