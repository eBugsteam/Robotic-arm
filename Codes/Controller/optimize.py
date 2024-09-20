import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Define the lengths of the links
L1 = 1.0  # Length of the first link
L2 = 1.0  # Length of the second link

# Define target angles for the joints (desired positions)
target_theta1 = np.pi / 2  # Desired angle for joint 1
target_theta2 = np.pi / 2  # Desired angle for joint 2

def forward_kinematics(theta1, theta2):
    """
    Compute the (x, y) coordinates of the end effector given joint angles theta1 and theta2.
    """
    x1 = L1 * np.cos(theta1)
    y1 = L1 * np.sin(theta1)
    
    x2 = x1 + L2 * np.cos(theta1 + theta2)
    y2 = y1 + L2 * np.sin(theta1 + theta2)
    
    return (x1, y1), (x2, y2)

def sliding_mode_control(target, current, lambda_, eta, dt):
    """
    A basic Sliding Mode Controller that calculates the control action based on error.
    """
    # Error between the target and current angle
    error = target - current
    
    # Sliding surface
    s = lambda_ * error
    
    # Control action
    control = eta * np.sign(s)
    
    return control

def simulate_arm(params):
    """
    Simulate the robotic arm with given parameters and return the total error.
    """
    lambda1, lambda2, eta1, eta2 = params
    
    # Initial joint angles (in radians)
    theta1 = np.pi / 8  # 45 degrees
    theta2 = np.pi / 8  # 45 degrees
    
    total_error = 0
    dt = 0.1
    
    for _ in range(200):
        # Apply Sliding Mode Control to joint 1
        control1 = sliding_mode_control(target_theta1, theta1, lambda1, eta1, dt)
        
        # Apply Sliding Mode Control to joint 2
        control2 = sliding_mode_control(target_theta2, theta2, lambda2, eta2, dt)
        
        # Update the joint angles with the control action
        theta1 += control1 * dt
        theta2 += control2 * dt
        
        # Calculate the error
        error1 = target_theta1 - theta1
        error2 = target_theta2 - theta2
        total_error += np.abs(error1) + np.abs(error2)
    
    return total_error

def gradient_descent(simulate_arm, initial_params, learning_rate=0.01, iterations=1000):
    """
    Perform gradient descent to minimize the total error.
    """
    params = np.array(initial_params)
    for _ in range(iterations):
        gradients = np.zeros_like(params)
        for i in range(len(params)):
            params[i] += 1e-5
            loss1 = simulate_arm(params)
            params[i] -= 2 * 1e-5
            loss2 = simulate_arm(params)
            params[i] += 1e-5
            gradients[i] = (loss1 - loss2) / (2 * 1e-5)
        
        params -= learning_rate * gradients
    
    return params

# Initial parameters
initial_params = [1.0, 1.0, 1.0, 1.0]

# Perform gradient descent to find the best parameters
best_params = gradient_descent(simulate_arm, initial_params)
print("Best parameters found:", best_params)

# Use the best parameters to simulate the arm
lambda1, lambda2, eta1, eta2 = best_params

# Initialize figure and axis
fig, ax = plt.subplots()
ax.set_xlim([-2, 2])
ax.set_ylim([-2, 2])
ax.set_aspect('equal')
plt.title('2 DOF Robotic Arm with Optimized Sliding Mode Control')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)

# Initialize the lines that will represent the arm
line, = ax.plot([], [], marker='o')

# Initial joint angles (in radians)
theta1 = np.pi / 8  # 45 degrees
theta2 = np.pi / 8  # 45 degrees

# Function to initialize the animation
def init():
    line.set_data([], [])
    return line,

# Function to update the frame in the animation
def update(frame):
    global theta1, theta2
    
    # Time step (assume constant for simplicity)
    dt = 0.1
    
    # Apply Sliding Mode Control to joint 1
    control1 = sliding_mode_control(target_theta1, theta1, lambda1, eta1, dt)
    
    # Apply Sliding Mode Control to joint 2
    control2 = sliding_mode_control(target_theta2, theta2, lambda2, eta2, dt)
    
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
