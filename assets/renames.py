
import os

# Function to rename the files
def rename_files(input_directory):
    for i in range(1, 12):  # Loop through 1 to 11 (inclusive)
        old_filename = f"miner_{i}.png"
        old_file_path = os.path.join(input_directory, old_filename)
        
        if os.path.exists(old_file_path):
            # New filename with the "a_" prefix
            new_filename = f"miner_a_{i}.png"
            new_file_path = os.path.join(input_directory, new_filename)
            
            # Rename the file
            os.rename(old_file_path, new_file_path)
            print(f"Renamed: {old_filename} -> {new_filename}")
        else:
            print(f"File not found: {old_filename}")

# Directory containing the images
input_directory = "./miner"  # Replace with your directory path

# Rename the files
rename_files(input_directory)
