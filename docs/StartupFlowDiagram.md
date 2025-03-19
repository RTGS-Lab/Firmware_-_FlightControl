graph TD
    Start([Start]) --> ConfigPower[Configure Power Save Mode]
    ConfigPower --> EnableFeatures[Enable System Features]
    EnableFeatures --> CheckReset{Reset Reason?}
    
    CheckReset -->|Power Down| NormalStart[Continue Normal Startup]
    CheckReset -->|Other| ParticleConnect[Connect to Particle Cloud]
    ParticleConnect --> NormalStart
    
    NormalStart --> InitLogger[Initialize Kestrel Logger]
    InitLogger --> InitFileSystem[Initialize File System]
    InitFileSystem --> CheckErrors{Critical Errors?}
    
    CheckErrors -->|Yes| SetErrorLED[Set Error LED]
    CheckErrors -->|No| SetPassLED[Set Pass LED]
    
    SetErrorLED --> InitBattery[Initialize Battery System]
    SetPassLED --> InitBattery
    
    InitBattery --> InitIO[Initialize IO Expanders]
    InitIO --> EnableLEDs[Enable LED Indicators]
    
    EnableLEDs --> SerialCheck{Serial Connected?}
    SerialCheck -->|Yes| CommandMode[Enter Command Mode]
    SerialCheck -->|No| ContinueStartup[Continue Startup]
    CommandMode --> ContinueStartup
    
    ContinueStartup --> ConnectParticle[Connect to Particle Cloud]
    ConnectParticle --> DetectTalons[Detect Connected Talons]
    DetectTalons --> DetectSensors[Detect Connected Sensors]
    
    DetectSensors --> UpdateGPS[Update GPS Location]
    UpdateGPS --> InitSensors[Initialize All Sensors]
    InitSensors --> LogInitData[Log Initialization Data]
    
    LogInitData --> RapidCheck{Rapid Start?}
    RapidCheck -->|Yes| SkipWait[Skip Connection Wait]
    RapidCheck -->|No| WaitForConnect[Wait for Connections]
    
    WaitForConnect --> ConnCheck{Cell Connected?}
    ConnCheck -->|Yes| SetCellPass[Set Cell LED to Pass]
    ConnCheck -->|No| SetCellError[Set Cell LED to Error]
    
    SetCellPass --> GPSCheck{GPS Fix?}
    SetCellError --> GPSCheck
    
    GPSCheck -->|Yes| SetGPSPass[Set GPS LED to Pass]
    GPSCheck -->|No| SetGPSError[Set GPS LED to Error]
    
    SetGPSPass --> Backhaul[Try Backhaul Unsent Logs]
    SetGPSError --> Backhaul
    
    SkipWait --> Backhaul
    
    Backhaul --> SetTimer[Start Log Timer]
    SetTimer --> EnterMainLoop([Enter Main Loop])