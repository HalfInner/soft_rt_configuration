# soft_rt_configuration
Univerisity Project. Configure one of the common linux distrubution to work in defined time scope. 


## Drafts:

# Isolated cpu processes - restover of configuration
Persisent process - even with isolcpu 
```txt
PSR     PID COMMAND
  3      26 [cpuhp/3]
  3      27 [migration/3]
  3      28 [ksoftirqd/3]
  3      29 [kworker/3:0-mm_percpu_wq]
  3      30 [kworker/3:0H]
  3      95 [kworker/3:1]
```

1. cpuhp     - obsrrving life
1. migration - migration
1. ksoftirqd - K-soft-irq-d

# IRQ
Configuring irq affinity on Raspberry PI OS is not possible. Raspberry pi has no configurable irq, but a script is providede.