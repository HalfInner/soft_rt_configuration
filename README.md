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

bash execute_process_on_seperate_code.sh configure tasks/task_server.a
bash -x execute_process_on_seperate_code.sh configure tasks/task_client_a.a
bash -x execute_process_on_seperate_code.sh configure tasks/task_client_b.a   

pi@raspberrypi:~/soft_rt_configuration $ sudo  bash  configure_process_scheduler.sh test
pid 1232's current scheduling policy: SCHED_FIFO pid 1232's current scheduling priority: 99
pid 1234's current scheduling policy: SCHED_FIFO pid 1234's current scheduling priority: 99
pid 1236's current scheduling policy: SCHED_FIFO pid 1236's current scheduling priority: 99

DISPLAY=127.0.0.1:0.0 ssh -v -Y pi@raspberrypi
kernelshark

sudo trace-cmd record -p function -P 11881 -P 11883 -P 11885

# Step by step
1. bash configure_irq_affinity.sh configure
1. bash configure_isolating_cpu.sh configure
1. bash tasks/build.sh
1. bash tasks/run.sh out
1. bash execute_process_on_seperate_code.sh out
1. bash configure_process_scheduler.sh configure
1. bash configure_process_affinity.sh test


scp -r pi@raspberrypi:~/soft_rt_configuration/soft_rt_configuration_logs .
python plot_tracer.py soft_rt_configuration_logs/17_05_22__17_23_18/trace_report_cpu3.txt

### To work on remotely
/etc/systemd/system/ngrok_ssh_tunneling.service
```shki
[Unit]
Description=My script that requires network
After=network.target

[Service]
Restart=always
RestartSec=5
User=pi
#Type=oneshot
ExecStart=/usr/sbin/expose_ssh_over_tcp_ngrok.sh

[Install]
WantedBy=multi-user.target
```
 /usr/sbin/expose_ssh_over_tcp_ngrok.sh
```sh
#! /usr/bin/bash
ngrok --log=stdout tcp 22 > /tmp/ngrok.log

exit 0
```
```sh
sudo systemctl daemon-reload
sudo systemctl enable ngrok_ssh_tunneling
```

To Check logs `journalctl -r`