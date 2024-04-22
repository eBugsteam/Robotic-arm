import cv2
import mediapipe as mp
import math

# Function to calculate distance between two points
def calculate_distance(point1, point2):
    return math.sqrt((point2[0] - point1[0])**2 + (point2[1] - point1[1])**2)

# Function to detect landmarks on hand
def detect_landmarks(image):
    with mp.solutions.hands.Hands(static_image_mode=False, max_num_hands=1, min_detection_confidence=0.5) as hands:
        image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = hands.process(image_rgb)
        if results.multi_hand_landmarks:
            hand_landmarks = results.multi_hand_landmarks[0].landmark
            return [(lm.x, lm.y) for lm in hand_landmarks]
        else:
            return None

# Function to calibrate the distance range
def calibrate_distance():
    cap = cv2.VideoCapture(0)
    max_distance = 0
    min_distance = float('inf')

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Unable to capture video.")
            break

        # Detect landmarks
        landmarks = detect_landmarks(frame)

        if landmarks:
            # Specify landmarks for distance calculation (e.g., index finger and thumb)
            landmark_index = landmarks[mp.solutions.hands.HandLandmark.INDEX_FINGER_TIP]
            landmark_thumb = landmarks[mp.solutions.hands.HandLandmark.THUMB_TIP]

            # Calculate distance between landmarks
            distance = calculate_distance(landmark_index, landmark_thumb)

            # Update maximum and minimum distances
            max_distance = max(max_distance, distance)
            min_distance = min(min_distance, distance)

            # Display distance on frame
            cv2.putText(frame, f"Max Distance: {max_distance:.2f} pixels, Min Distance: {min_distance:.2f} pixels", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)

        cv2.imshow('Calibration', frame)
        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('c'):
            break

    cap.release()
    cv2.destroyAllWindows()
    return min_distance, max_distance

# Main function
def main():
    # Calibration
    min_distance, max_distance = calibrate_distance()
    # Calculate scaling factor
    scale_factor = 10 / (max_distance - min_distance)

    cap = cv2.VideoCapture(0)

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            print("Unable to capture video.")
            break

        # Detect landmarks
        landmarks = detect_landmarks(frame)

        if landmarks:
            # Specify landmarks for distance calculation (e.g., index finger and thumb)
            landmark_index = landmarks[mp.solutions.hands.HandLandmark.INDEX_FINGER_TIP]
            landmark_thumb = landmarks[mp.solutions.hands.HandLandmark.THUMB_TIP]

            # Calculate distance between landmarks
            distance = calculate_distance(landmark_index, landmark_thumb)

            # Scale the distance to a range of 0 to 10
            scaled_distance = (distance - min_distance) * scale_factor

            # Display scaled distance on frame
            cv2.putText(frame, f"Scaled Distance: {scaled_distance:.2f}", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)

        cv2.imshow('Hand Landmarks', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
