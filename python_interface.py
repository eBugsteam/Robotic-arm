import tkinter as tk
from tkinter import Scale
import serial

# Function to send servo positions to Arduino
def send_positions():
    # Get position values from sliders
    servo1_pos = servo1_slider.get()
    servo2_pos = servo2_slider.get()
    # Format positions as bytes
    data = f"{servo1_pos},{servo2_pos}\n".encode()
    # Send data to Arduino
    ser.write(data)

# Initialize serial connection to Arduino
ser = serial.Serial('COM3', 9600)  # Change 'COM3' to your Arduino's port

# Create main window
root = tk.Tk()
root.title("Servo Control")

# Create servo 1 slider
servo1_label = tk.Label(root, text="Servo 1")
servo1_label.grid(row=0, column=0)
servo1_slider = Scale(root, from_=0, to=180, orient="horizontal")
servo1_slider.grid(row=0, column=1)

# Create servo 2 slider
servo2_label = tk.Label(root, text="Servo 2")
servo2_label.grid(row=1, column=0)
servo2_slider = Scale(root, from_=0, to=180, orient="horizontal")
servo2_slider.grid(row=1, column=1)

# Create button to send servo positions
send_button = tk.Button(root, text="Send Positions", command=send_positions)
send_button.grid(row=2, columnspan=2)

# Run the Tkinter event loop
root.mainloop()
