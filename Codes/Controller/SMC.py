import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Define the lengths of the links
L1 = 1.0  # Length of the first link
L2 = 1.0  # Length of the second link

# Define Sliding Mode Control parameters
lambda1 = 10  # Slope of the sliding surface for joint 1
lambda2 = 10  # Slope of the sliding surface for joint 2
eta1 = 0.05  # Control gain for joint 1
eta2 = 0.05  # Control gain for joint 2

# Initialize previous errors for derivative calculation
previous_error1 = 0
previous_error2 = 0

def forward_kinematics(theta1, theta2):
    """
    Compute the (x, y) coordinates of the end effector given joint angles theta1 and theta2.
    """
    x1 = L1 * np.cos(theta1)
    y1 = L1 * np.sin(theta1)
    
    x2 = x1 + L2 * np.cos(theta1 + theta2)
    y2 = y1 + L2 * np.sin(theta1 + theta2)
    
    return (x1, y1), (x2, y2)

def sliding_mode_control(target, current, previous_error, lambda_, eta, dt):
    """
    A basic Sliding Mode Controller that calculates the control action based on error and its derivative.
    """
    # Error between the target and current angle
    error = target - current
    
    # Derivative of the error
    error_derivative = (error - previous_error) / dt
    
    # Sliding surface
    s = lambda_ * error + 0.8* error_derivative
    
    # Control action
    control = eta * np.sign(s)
    
    return control, error

# Initialize figure and axis
fig, ax = plt.subplots()
ax.set_xlim([-2, 2])
ax.set_ylim([-2, 2])
ax.set_aspect('equal')
plt.title('2 DOF Robotic Arm with Sliding Mode Control')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)

# Initialize the lines that will represent the arm
line, = ax.plot([], [], marker='o', linewidth=2.5)

# Initial joint angles (in radians)
theta1 = np.pi / 8  # 45 degrees
theta2 = np.pi / 8  # 45 degrees

# Define target angles for the joints (desired positions)
target_theta1 = np.pi / 4  # Desired angle for joint 1
target_theta2 = np.pi / 4  # Desired angle for joint 2

# Function to initialize the animation
def init():
    line.set_data([], [])
    return line,

# Function to update the frame in the animation
def update(frame):
    global theta1, theta2, previous_error1, previous_error2
    
    # Time step (assume constant for simplicity)
    dt = 0.1
    
    # Apply Sliding Mode Control to joint 1
    control1, previous_error1 = sliding_mode_control(target_theta1, theta1, previous_error1, lambda1, eta1, dt)
    
    # Apply Sliding Mode Control to joint 2
    control2, previous_error2 = sliding_mode_control(target_theta2, theta2, previous_error2, lambda2, eta2, dt)
    
    # Update the joint angles with the control action
    theta1 += control1 * dt
    theta2 += control2 * dt
    
    # Get the positions of the joints
    joint1, end_effector = forward_kinematics(theta1, theta2)

    # Update the line data for the plot
    line.set_data([0, joint1[0], end_effector[0]], [0, joint1[1], end_effector[1]])
    return line,

# Create animation object
ani = FuncAnimation(fig, update, frames=np.arange(0, 200), init_func=init, blit=True, interval=50)

# Show the animation
plt.show()
