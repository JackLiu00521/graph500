# Define a function to parse the text and return a list of numbers
def get_bfs_median_TEPS(text):
  # Initialize an empty list to store the numbers
  numbers = []
  # Split the text by lines
  lines = text.split("\n")
  # Loop through the lines
  for line in lines:
    # Strip any whitespace from the line
    line = line.strip()
    # Check if the line starts with "bfs  median_TEPS:"
    if line.startswith("bfs  median_TEPS:"):
      # Split the line by colon and get the second part
      number = line.split(":")[1]
      # Try to convert the number to a float and append it to the list
      try:
        number = float(number)
        numbers.append(number)
      except ValueError:
        # If the conversion fails, ignore the line
        pass
  # Return the list of numbers
  return numbers
  
  
text =
"""
put all results here
"""

print(get_bfs_median_TEPS(text))
print(len(get_bfs_median_TEPS(text)))