graph TD
    MainLoop([Main Loop]) --> WakeSensors[Wake Up Logger and Sensors]
    
    WakeSensors --> WaitTimer[Wait Until Timer Period Done]
    WaitTimer --> StartTimer[Start Next Timer Period]
    
    StartTimer --> LogCheck{Which Logging Mode?}
    
    LogCheck -->|PERFORMANCE| LogPerformance[Log Data Only for Speed]
    LogCheck -->|STANDARD| CountCheck1{Log Count?}
    LogCheck -->|BALANCED| LogBalanced[Log Data & Errors]
    LogCheck -->|NO_LOCAL| CountCheck2{Log Count?}
    
    CountCheck1 -->|Count % 16 = 0| LogFull1[Full Log: Data, Diagnostics, Metadata]
    CountCheck1 -->|Count % 8 = 0| LogPartial1[Partial Log: Data & Diagnostics]
    CountCheck1 -->|Other| LogMinimal1[Minimal Log: Data Only]
    
    CountCheck2 -->|Count % 10 = 0| LogFull2[Full Log to Particle: Data, Diagnostics, Metadata]
    CountCheck2 -->|Count % 5 = 0| LogPartial2[Partial Log to Particle: Data & Diagnostics]
    CountCheck2 -->|Other| LogMinimal2[Minimal Log to Particle: Data Only]
    
    LogPerformance --> TimeDiagCheck{Time for Diagnostics?}
    LogBalanced --> TimeDiagCheck
    
    TimeDiagCheck -->|Yes| LogFullDiag[Log Full Diagnostics & Metadata]
    TimeDiagCheck -->|No| SkipDiag[Skip Extended Diagnostics]
    
    LogFull1 --> SleepSensors[Sleep Sensors]
    LogPartial1 --> SleepSensors
    LogMinimal1 --> SleepSensors
    LogFull2 --> SleepSensors
    LogPartial2 --> SleepSensors
    LogMinimal2 --> SleepSensors
    LogFullDiag --> SleepSensors
    SkipDiag --> SleepSensors
    
    SleepSensors --> BackhaulCheck{Time for Backhaul?}
    
    BackhaulCheck -->|Yes| PowerCheck{Low Power Mode?}
    BackhaulCheck -->|No| IncrementCount[Increment Log Count]
    
    PowerCheck -->|Yes| ConnectParticle[Connect to Particle]
    PowerCheck -->|No| SkipConnect[Skip Connect]
    
    ConnectParticle --> WaitConnect[Wait for Connection]
    WaitConnect --> SyncTime[Sync Time]
    SkipConnect --> SyncTime
    
    SyncTime --> DumpFRAM[Dump FRAM to Particle]
    DumpFRAM --> IncrementCount
    
    IncrementCount --> SleepFileSystem[Sleep File System]
    SleepFileSystem --> SleepLogger[Sleep Logger]
    SleepLogger --> Loop([Loop Back])
    
    Loop --> MainLoop