from PIL import Image
import os

# Function to create a mirrored image
def create_mirror_image(image_path):
    # Open the image
    img = Image.open(image_path)
    
    # Create the mirror effect by flipping the image horizontally
    mirrored_img = img.transpose(Image.FLIP_LEFT_RIGHT)
    
    return mirrored_img

# Function to process the images named miner_1.png to miner_11.png
def process_images(input_directory, output_directory):
    # List all files in the directory and process only specific files
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)
    
    for i in range(1, 12):  # Loop through 1 to 11 (inclusive)
        filename = f"miner_a_{i}.png"
        image_path = os.path.join(input_directory, filename)
        
        if os.path.exists(image_path):
            # Create the mirrored image
            mirrored_image = create_mirror_image(image_path)
            
            # Define the output file path
            output_filename = f"miner_b_{i}.png"
            output_path = os.path.join(output_directory, output_filename)
            
            # Save the mirrored image
            mirrored_image.save(output_path)
            print(f"Mirrored image saved: {output_path}")
        else:
            print(f"File not found: {image_path}")

# Directory containing the images
input_directory = "./miner"  # Replace with your directory path
output_directory = "./miner"  # Replace with your desired output directory

# Process the images
process_images(input_directory, output_directory)
