## CommandExe Support

This is a protocol to send commands to the logger when it is connected via cell. 

> [!WARNING]
> Do not execute any commands unless you are confident of their function and use! Failure to do so could result in loss of data 

## Supported Commands 

| Version | Command | Returns |                                      Description                                     |
|:-------:|:-------:|:-------:|:------------------------------------------------------------------------------------:|
| `>v2.9.5` |   `102`   |    `1`    |                     Has device return a level 2 diagnostic packet                    |
| `>v2.9.5` |   `103`   |    `1`    |                     Has device return a level 3 diagnostic packet                    |
| `>v2.9.5` |   `104`   |    `1`    |                     Has device return a level 4 diagnostic packet                    |
| `>v2.9.5` |   `111`   |    `1`    |                            Has device return a data packet                           |
| `>v2.9.5` |   `120`   |    `1`    |                           Has device return an error packet                          |
| `>v2.9.5` |   `130`   |    `1`    |                          Has device return a metadata packet                         |
| `>v2.9.5` |   `300`   |    `1`    | Release WDT - this causes the device to power cycle next time the timer comes around |
| `>v2.9.5` |   `401`   |    `1`    |                                 Dump contents of FRAM                                |
| `>v2.9.5` |   `410`   |    `1`    |                                Erase contents of FRAM                                |
|   N/A   |  Other  |    `-1`   |                               Any unsupported command                                |