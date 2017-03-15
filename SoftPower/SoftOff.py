#!/usr/bin/env python2.7
# date: 30/05/16
# author: Popcorn <abandonedemails@gmail.com> - Add "Sudomod" in the subject or your message will not be received
# Moded: A few moded by kelroy, just deleted the LowBattery section because on my project isnt needed.
# version: 1.0a
# name: GBZ-Power-Monitor - a Power Management utility for the Gameboy Zero project
# description: a GPIO monitor that detects low battery and power switch status which provides a graceful shutdown facility
# original-source: https://github.com/Camble/GBZ-Power-Monitor_PB

import RPi.GPIO as GPIO
import os
import sys
import time


powerGPIO      = 27  # GPIO 27/pin 2
sampleRate     = 0.1 # tenth of a second
powerTimeout   = 1   # 1 second

GPIO.setmode(GPIO.BCM)

#GPIO que da los 3,3v para mantener vivo el booster mientras se apaga.
GPIO.setup(powerGPIO, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)


def powerSwitch(channel):
  #Checking for LED bounce for the duration of the Power Timeout
  for bounceSample in range(1, int(round(powerTimeout / sampleRate))):
    time.sleep(sampleRate)

    if GPIO.input(powerGPIO) is 1:
       break

  if bounceSample is int(round(powerTimeout / sampleRate)) - 1:
      #When the Power Switch is placed in the off position with no bounce for the duration of the Power Timeout, we immediately shutdown
      os.system("sudo shutdown -h now")
      try:
         sys.stdout.close()
      except:
         pass
      try:
         sys.stderr.close()
      except:
         pass

      sys.exit(0)

def main():

  #if the Power Switch is active when the program launches, handle it
  if GPIO.input(powerGPIO) is 0:
    powerSwitch(powerGPIO)

  #Add threaded event listeners for the Power Switch
  try:

    GPIO.remove_event_detect(powerGPIO)
    GPIO.add_event_detect(powerGPIO, GPIO.FALLING, callback=powerSwitch, bouncetime=300)
  except KeyboardInterrupt:
    GPIO.cleanup()

main()

#We make an endless loop so the threads running the GPIO events will always be listening, in the future we can add Battery Level monitoring here
while True:
  time.sleep(1)

GPIO.cleanup()
