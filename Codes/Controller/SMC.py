import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Define the lengths of the links
L1 = 1.0  # Length of the first link
L2 = 1.0  # Length of the second link

# SMC parameters
lambda_smc = 0.09  # Sliding surface gain
k_smc = 1  # Control gain for sliding mode

# Function for forward kinematics
def forward_kinematics(theta1, theta2):
    x1 = L1 * np.cos(theta1)
    y1 = L1 * np.sin(theta1)
    
    x2 = x1 + L2 * np.cos(theta1 + theta2)
    y2 = y1 + L2 * np.sin(theta1 + theta2)
    
    return (x1, y1), (x2, y2)

def smc_control(target, current, current_velocity, lambda_smc, k_smc):
    """
    Sliding Mode Control law.
    """
    # Calculate the error and the sliding surface
    error = target - current
    sliding_surface = lambda_smc * error + current_velocity
    
    # Control law based on the sliding surface
    control_action = -k_smc * np.sign(sliding_surface)
    
    return control_action, sliding_surface

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
line, = ax.plot([], [], marker='o')

# Initial joint angles and velocities (in radians and radians/sec)
theta1 = np.pi / 4  # 45 degrees
theta2 = np.pi / 4  # 45 degrees
theta1_dot = 0.0  # Angular velocity for joint 1
theta2_dot = 0.0  # Angular velocity for joint 2

# Define target angles for the joints (desired positions)
target_theta1 = np.pi / 2  # Desired angle for joint 1
target_theta2 = np.pi / 2  # Desired angle for joint 2

# Function to initialize the animation
def init():
    line.set_data([], [])
    return line,

# Function to update the frame in the animation
def update(frame):
    global theta1, theta2, theta1_dot, theta2_dot
    
    # Time step (assume constant for simplicity)
    dt = 0.1

    # Apply SMC to joint 1
    control1, sliding_surface1 = smc_control(target_theta1, theta1, theta1_dot, lambda_smc, k_smc)
    
    # Apply SMC to joint 2
    control2, sliding_surface2 = smc_control(target_theta2, theta2, theta2_dot, lambda_smc, k_smc)
    
    # Update the joint velocities (integrating the control action)
    theta1_dot += control1 * dt
    theta2_dot += control2 * dt
    
    # Update the joint angles based on the velocities
    theta1 += theta1_dot * dt
    theta2 += theta2_dot * dt
    
    # Get the positions of the joints
    joint1, end_effector = forward_kinematics(theta1, theta2)

    # Update the line data for the plot
    line.set_data([0, joint1[0], end_effector[0]], [0, joint1[1], end_effector[1]])
    return line,

# Create animation object
ani = FuncAnimation(fig, update, frames=np.arange(0, 200), init_func=init, blit=True, interval=50)

# Show the animation
plt.show()
