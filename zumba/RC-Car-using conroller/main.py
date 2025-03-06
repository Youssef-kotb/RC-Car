import serial.tools.list_ports
import time
import pygame


# List all available serial ports
ports = serial.tools.list_ports.comports()

print("Available COM Ports:")
for port in ports:
    print(port.device, "-", port.description)



# Working on COM3  always try different ports because it changes everytime you connect
bluetooth_port = ("COM3")
baud_rate = 9600
arduino = serial.Serial(bluetooth_port, baud_rate)


# Initialize pygame for joystick input
pygame.init()

# Check if a joystick is connected
if pygame.joystick.get_count() == 0:
    print("❌ No joystick detected! Connect the PS3 controller and try again.")
    exit()


# Get the number of connected joysticks
joystick_count = pygame.joystick.get_count()

if joystick_count == 0:
    print("❌ No joystick detected! Connect the PS3 controller and try again.")
else:
    print(f"✅ {joystick_count} joystick(s) detected.")


# Initialize the first joystick
joystick = pygame.joystick.Joystick(0)
joystick.init()

# Print joystick name
print(f"✅ Connected to: {joystick.get_name()}")


prev_message = ""

# reading inputs
while True:
    pygame.event.pump()  # Refresh joystick events

    # Read R2 and L2 triggers
    r2 = joystick.get_axis(5)  # R2 trigger (-1 to 1)
    l2 = joystick.get_axis(4)  # L2 trigger (-1 to 1)
    x_axis = joystick.get_axis(0)  # Left joystick X-axis (-1 to 1)

    # Debugging: Print raw values of joystick
    print(f"🎮 Raw R2: {r2:.2f}, L2: {l2:.2f}, X-axis: {x_axis:.2f}")

    # Normalize values for Arduino (0 to 255)
    accel_value = int((r2 + 1) * 127)  # Scale from 0 to 255
    brake_value = int((l2 + 1) * 127)  # Scale from 0 to 255
    steering_value = int((x_axis + 1) * 127)  # Scale from 0 to 255

    message = f"{accel_value},{brake_value},{steering_value}\n"

    # Debugging: Print message before sending
    print(f"🎮 Message to Arduino: {message.strip()}")

    # Ensure the serial connection is open before sending
    if arduino.is_open:
        if prev_message != message.strip():
            try:
                arduino.write(message.encode())
                time.sleep(0.1)
                print(f"✅ Sent to Arduino: {message.strip()}")
                prev_message = message.strip()
            except serial.SerialException as e:
                print(f"❌ Error in writing to Arduino: {e}")
    else:
        print("❌ Serial connection not open. Please check the connection.")

    # Delay to prevent sending data too fast
    pygame.time.delay(100)  # 100ms delay


## Sending data to Arduino via Bluetooth
# message = "Hello Arduino\n"
# arduino.write(message.encode())  # Send message as bytes
# print(f"✅ Sent to Arduino: {message.strip()}")
#
# # Wait for Arduino to process and respond
# time.sleep(2)

# Close the connection
arduino.close()
