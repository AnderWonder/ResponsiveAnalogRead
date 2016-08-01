/*
 * ResponsiveAnalogRead.h
 * Arduino library for eliminating noise in analogRead inputs without decreasing responsiveness
 *
 * Copyright (c) 2016 Damien Clarke
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.  
 */
 
#ifndef RESPONSIVE_ANALOG_READ_H
#define RESPONSIVE_ANALOG_READ_H

#include <Arduino.h>

class ResponsiveAnalogRead
{
  public:

    // pin - the pin to read
    // sleepEnable - enabling sleep will cause values to take less time to stop changing and potentially stop changing more abruptly,
    //   where as disabling sleep will cause values to ease into their correct position smoothly
    // snapMultiplier - a value from 0 to 1 that controls the amount of easing
    //   increase this to lessen the amount of easing (such as 0.1) and make the responsive values more responsive
    //   but doing so may cause more noise to seep through if sleep is not enabled
    
    ResponsiveAnalogRead(int pin, bool sleepEnable, float snapMultiplier = 0.01);

    ResponsiveAnalogRead(bool sleepEnable, float snapMultiplier = 0.01);

    inline int getValue() { return responsiveValue; } // get the responsive value from last update
    inline int getRawValue() { return rawValue; } // get the raw analogRead() value from last update
    inline bool hasChanged() { return responsiveValueHasChanged; } // returns true if the responsive value has changed during the last update
    void update(); // updates the value by performing an analogRead() and calculating a responsive value based off it
    void update(int rawValue);// updates the value by rawValue and calculating a responsive value based off it
    void setSnapMultiplier(float newMultiplier);
    inline void enableSleep() { sleepEnable = true; }
    inline void disableSleep() { sleepEnable = false; }
    inline void enableEdgeSnap() { edgeSnapEnable = true; }
    // edge snap ensures that values at the edges of the spectrum (0 and 1023) can be easily reached when sleep is enabled
    inline void disableEdgeSnap() { edgeSnapEnable = false; }
    inline void setSleepDelay(unsigned int ms) { sleepDelayMS = ms; } // sets the amount of time before sleeping
    inline void setSleepActivityThreshold(unsigned int newThreshold) { sleepActivityThreshold = newThreshold; }
    // the amount of movement that must take place while asleep for it to register as activity and start moving the output value. Defaults to 20
    inline void setAwakeActivityThreshold(unsigned int newThreshold) { awakeActivityThreshold = newThreshold; }
    // the amount of movement that must take place while awake for it to register as activity, and reset the timer before sleep occurs. Defaults to 5.
    inline void setAnalogResolution(unsigned int resolution) { analogResolution = resolution; }
    // if your ADC is something other than 10bit (1024), set that here

  private:
    int pin;
    unsigned int analogResolution = 1024;
    float snapMultiplier;
    bool sleepEnable;
    unsigned int sleepDelayMS;
    unsigned int sleepActivityThreshold = 20;
    unsigned int awakeActivityThreshold = 5;
    bool edgeSnapEnable = true;

    float smoothValue;
    unsigned long lastActivityMS;
    bool sleeping = false;

    int rawValue;
    int responsiveValue;
    int prevResponsiveValue;
    bool responsiveValueHasChanged;

    int getResponsiveValue(int newValue);
    float snapCurve(float x);
};

#endif
