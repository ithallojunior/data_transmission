# -*- coding: utf-8 -*-
"""
Modules file for the plotters, which will be used for multiple codes.
Author: Ithallo Junior Alves Guimarães
March 2019
"""

import serial
from sys import exit

import settings


def convert_input(a, b, raw=False):
    """Converts the two bytes into a single number."""

    if (a == '') or (b == ''):
        return 0.  # if it was hanging defaults to 0

    value = ord(a[0]) * 256 + ord(b[0])

    if raw:
        return value

    return (settings.voltage_range * (value/1023.)) - settings.offset


def serial_port():
    """Encapsulates the opening of the serial port."""

    try:
        p = serial.Serial(
            port=settings.device, baudrate=settings.baud_rate,
            bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE,
            timeout=settings.timeout
        )
    except serial.SerialException:
        print("Device <%s> not found. Check your circuits." % settings.device)
        exit()

    return p
