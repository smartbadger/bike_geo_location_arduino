#include "secrets.h"
#include "location.h"
#include "sensordata.h"
//#include "nfc_reader.h"
//#include "sensor.h"
#include "coordinate.h"
#include "Async_Operations.h"

long long dtt = {250};
Async_Operations delayedd(&dtt, 1, -1);
class Bike
{
    Async_Operations delayed = delayedd;
    SensorData _state;
    Location _location;
    double _battery;
    bool _controls;
    enum State
    {
        LOCKED,
        UNLOCKED,
        ALARM,
    };

    State _currentState = LOCKED;
    State _targetState = LOCKED;

public:
    Bike()
    {
        _controls = false;
    }

    bool isLocked()
    {
        return _currentState == LOCKED || _currentState == ALARM;
    }
    void setLock(bool status)
    {
        if (status)
        {
            _currentState = LOCKED;
            _controls = false;
        }
        else
        {
            _currentState = UNLOCKED;
            _controls = true;
        }
    }
    void setup()
    {
        delayed.start();
    }
    void readNFC()
    {
    }
    SensorData getSensorData()
    {
        return _state;
    }
    void setSensorData(SensorData s)
    {
        _state = s;
    }

private:
    bool aboveThresholdValue(float original, float current, float threshold)
    {
        return abs((original - current) / original) >= threshold;
    }

    bool checkForMotion(SensorData current, SensorData previous)
    {
        float t = 0.5;
        float cr = current.rotation.calculateAverage();
        float pr = previous.rotation.calculateAverage();
        float ca = current.acceleration.calculateAverage();
        float pa = previous.acceleration.calculateAverage();
        return aboveThresholdValue(cr, pr, t) || aboveThresholdValue(ca, pa, t);
    }
};