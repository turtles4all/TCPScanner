# TCPScanner
Simple and and quickly written TCP port scanner

compile with:
gcc -std=c11 -Wall -o tscan tscan.c

Bash command for IP range:
for x in {1..10}; do ./tscan -r 192.168.0.$x -p 1234 -e 1230; done
