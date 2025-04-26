# ACS-85-0710

Button Select - Single value state

## Overview

Controller sketch that allows you to select pins with two buttons.  One for up and one for down.

The output is a single pin going high. See state diagrams.

There is an internal software switch that allows the states to wrap. This is declared at compile time and is not configurable in real time.

up    | down   | State |  notes
--- | --- | --- | ---
 0   |  0  | 1 0 0  |  init
 1  |  0  |  0 1 0  |  press up button
 0  |  0  |  0 1 0  |  release
 1  |  0  |  0 0 1  |  press up button
 0  |  0  |  0 0 1  |  release
 1  |  0  |  0 0 1  |  press up button  it limits
 0  |  0  |  0 0 1  |  release
 0  |  1  |  0 1 0  |  press down
 0  |  0  |  0 1 0  |  release
 0  |  1  |  1 0 0  |  press down

Wrap can be configured with the following line of code
 
```
    boolean doWrap = LOW;
```

![nowrap](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0710/images/states1.png)

![wrap](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0710/images/states2.png)

## State Diagram - mermaid

for funsies  

```mermaid
stateDiagram-v2
    [*] --> State1
    State1: "State 1 0 0"
    State2: "State 0 1 0"
    State3: "State 0 0 1"
    
    State1 --> State2: Up
    State2 --> State3: Up
    State3 --> State3: Up (limits)
    
    State3 --> State2: Down
    State2 --> State1: Down
    
    note right of State1: Initial State
    note right of State3: Terminal State (no wrap)
```

When wrap mode is enabled (`doWrap = HIGH`), the states form a circular pattern:

```mermaid
stateDiagram-v2
    State1: "State 1 0 0"
    State2: "State 0 1 0"
    State3: "State 0 0 1"
    
    State1 --> State2: Up
    State2 --> State3: Up
    State3 --> State1: Up (wrap)
    
    State1 --> State3: Down
    State3 --> State2: Down
    State2 --> State1: Down
```

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0710/images/acs-85-0710.png)

## Examples
