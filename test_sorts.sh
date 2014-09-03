#!/bin/bash

RESULTS_FILE="sort_results.txt"

function sort_ints(){
	local SIZE=$1

	./int_sort "${SIZE}" >> "${RESULTS_FILE}"
}

function clear_file(){
	cat /dev/null > "${RESULTS_FILE}"
}

clear_file

sort_ints "1024"
sort_ints "2048"
sort_ints "4096"
sort_ints "8192"
sort_ints "16384"
sort_ints "32768"
sort_ints "65536"

exit 0
