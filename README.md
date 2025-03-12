# spotlight


teensy code to control multiple spotlights 

see following examples to control the lights  via serial port 

| command | action|
|---------|-------|
|`ONA` / `OFFA`   | turn on/off ALL spotlights|
|`TA`   | toggle the state of ALL spotlights|  
| `1 3 ON` | turn on lights 1 and 3 |
| `2 OFF` | turn off light 2|
|` 2 5 8 T ` | toggle the state of lights 2,5,and 8 |


note: the current version parses strings over serial port and there is a noticeable latency between sending the above commands and lights toggling. While this is not an issue now, a more efficient/faster way to parse and process the serial command would be really nice.