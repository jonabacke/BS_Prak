#!/bin/bash

# Jonathan Backes
# 06.04.2020
usage() {
    cat <<EOF
$0 [OPTIONS]
Erstellt Prozesse und beendet sie wieder
OPTIONS:
-h
 --help
 Display this help
EOF
}

main
seed_value_list="2806 225 353";
page_size_list="8 16 32 64";
page_rep_algo_list="FIFO CLOCK AGING";
search_algo_list="quicksort bubblesort";

mkdir -p result;
rm -r result/**;
# -lt less then
if [ $# -lt 1 ]; then
    for i in $page_size_list; do
        gcc mockup.c -o p1 -D VMEM_PAGESIZE=$i -D STOP_BEFORE_END=0;
        gcc mockup.c -o p2 -D VMEM_PAGESIZE=$i;
        for seed in $seed_value_list; do
            for repo in $page_rep_algo_list; do
                for search in $search_algo_list; do
                    cat <<EOF
####################
seed_value_list = $seed
page_rep_algo_list = $repo
search_algo_list = $search
####################
EOF
                    ./p1 -$repo >> info & 
                    sleep 1;
                    ps;
                    ./p2 -seed=$seed -$search >> result/output_${seed}_${search}_${repo} && pkill p1;
                    ps;

                done
            done
        done
    done
else
    # at least 1 arg, let’s check it
    case $1 in
    "-h" | "--help")
        # the only valid arg
        usage
        ;;
    *)
        # anything else is not valid
        echo "Invalid option"
        ;;
    esac
fi

exit 0
