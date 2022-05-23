#! /usr/bin/bash 
# For more info: https://www.kernel.org/doc/html/latest/core-api/irq/irq-affinity.html and https://cs.uwaterloo.ca/~brecht/servers/apic/SMP-affinity.txt

function usage() {
    echo "Usage:"
    echo "      $0 <test|configure>"
}

option=$1
if [[ "$option" == "test" ]]; then
    echo "IRQ Affinities:"
    for irq in /proc/irq/*/smp_affinity_list ; do
        irq_affinity_list=$(cat $irq)
        echo "IRQ:${irq}: $irq_affinity_list"
    done
elif [[ "$option" == "configure" ]]; then
    for irq in /proc/irq/*/smp_affinity_list ; do
        irq_affinity_list=$(cat $irq)
        cores_to_use="0-2"
        echo "$cores_to_use" > $irq
    done
else 
    usage
fi
