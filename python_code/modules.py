# -*- coding: utf-8 -*-
"""
Modules file for the plotters, which will be used for multiple codes.
Author: Ithallo Junior Alves Guimarães
March 2019
"""

import settings

#converts the two bytes into a single number
def convert_input(a, b):
    if (a=="" or b==""):
        return None # because it is non-blocking if it hangs

    value = ord(a[0]) *256 + ord(b[0]) 

    if (settings.remove_mean):
        return settings.voltage_range * (value/1023.)
    
    else: 
        if (settings.voltage_range is not None):
            return  (settings.voltage_range * (value/1023.)) - settings.offset
        else:
            return value 
        
