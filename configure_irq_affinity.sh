#! /bin/sh 
# For more info: https://www.kernel.org/doc/html/latest/core-api/irq/irq-affinity.html

function usage() {
    echo "Usage:"
    echo "      $0 <test|configure>"
}

option=$1
if [[ "$option" == "test" ]]; then
    for irq in /proc/irq/* ; do
        [ -L "${d%/}" ] && continue
        echo "IRQ Affinities:"
        echo "IRQ${d}: $(cat "$d/smp_affinity_list)"
    done
elif [[ "$option" == "configure" ]]; then
    echo "NotSupported"
else 
    usage
fi
