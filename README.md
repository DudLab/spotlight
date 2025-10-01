# spotlight


teensy code to control multiple spotlights 

see following examples to control the lights  via serial port 

| command | action|
|---------|-------|
|`00`   | turn off ALL spotlights|
| `10`  | turn off light 1       |
| `31`  | turn on light 3|

- you can only turn on one light at a time
- do not turn on more than two lights simultaneously

note: the current version parses strings over serial port and there is a noticeable latency between sending the above commands and lights toggling. While this is not an issue now, a more efficient/faster way to parse and process the serial command would be really nice.