import serial
import time

# -------------------------------
# Configure serial (adjust COM/baud)
# -------------------------------
ser = serial.Serial(
    port="COM5",        # ⚠ 修改为实际的 USB-TTL 端口，例如 COM3 / ttyUSB0
    baudrate=115200,
    bytesize=8,
    parity="N",
    stopbits=1,
    timeout=0.1
)

def send_cmd(steer, speed):
    """
    Send command to TM4C in format:  steer,speed\n
    Example:   120,-300\n
    Normalized ranges:
      steer: -1000 .. +1000
      speed: -1000 .. +1000
    """
    frame = f"{steer},{speed}\n"
    ser.write(frame.encode("utf-8"))
    print("Sent:", frame.strip())


if __name__ == "__main__":
    print("UART vision sender started...")
    time.sleep(1)

    # Example test pattern
    test_data = [
        (0, 600),        # forward
        (-350, 550),     # forward + left
        (350, 550),      # forward + right
        (0, 0),          # stop
        (0, -400),       # reverse
    ]

    for steer, speed in test_data:
        send_cmd(steer, speed)
        time.sleep(0.2)  # ≈ 20ms per frame (same control frame timing)

    # Continuous loop (example manual control)
    while True:
        try:
            # Here is your vision algorithm result:
            # replace these lines with actual detection output
            x = int(input("steer(x) = "))
            y = int(input("speed(y) = "))

            # clamp range
            x = max(-1000, min(1000, x))
            y = max(-1000, min(1000, y))

            send_cmd(x, y)

        except KeyboardInterrupt:
            ser.close()
            print("\nSerial closed.")
            break
