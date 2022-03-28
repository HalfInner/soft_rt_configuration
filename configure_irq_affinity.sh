#! /bin/sh 
# For more info: https://www.kernel.org/doc/html/latest/core-api/irq/irq-affinity.html

function usage() {
    echo "Usage:"
    echo "      $0 <test|configure>"
}

option=$1
if [[ "$option" == "test" ]]; then
    echo "IRQ Affinities:"
    for irq in "/proc/irq/*" ; do
        #[ -f "${irq}/smp_affinity_list" ] && continue
        irq_affinity_list=$(cat $irq/smp_affinity_list)
        echo "IRQ${irq}: $irq_affinity_list"
    done
elif [[ "$option" == "configure" ]]; then
    echo "NotSupported"
else 
    usage
fi
