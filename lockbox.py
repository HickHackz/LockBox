# This code is used to create WiFi time trigger, which can be used for multiple applications.
# I built a lockbox with a latching solenoid which only opens during certain times throughout the week.
# This helped us to stay focued on school since we could lock the TV remote in the box, and only watch TV late in the evenings when the box was open


import RPi.GPIO as GPIO

from time import sleep
from datetime import datetime

def wait_pin_change(button):
    # wait for pin to change value
    # it needs to be stable for a continuous 20ms
    cur_value = GPIO.input(button)
    active = 0
    while active < 20:
        if GPIO.input(button) != cur_value:
            active += 1
        else:
            active = 0
        sleep(.001)
        
button = 3  # Push button Input
out = 2     # Output Trigger

ledState = False
GPIO.setmode(GPIO.BCM)
GPIO.setup(button, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(out, GPIO.OUT)

while True:
    weekday = datetime.today().weekday()
    hour = datetime.now().hour
    wait_pin_change(button)
    if not GPIO.input(button):
        if ((weekday == 5 or weekday == 6) or (weekday == 5 and hour >= 17) or (hour >= 20) or (hour == 6)):
            print(weekday)
            print(hour)
            GPIO.output(out, True)
    else:
        GPIO.output(out, False)
    

