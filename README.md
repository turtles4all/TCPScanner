# TCPScanner
Simple and and quickly written TCP port scanner

compile with:
gcc -std=c11 -Wall -o tscan tscan.c

Bash command for IP range:
for x in {1..10}; do ./tscan 192.168.0.$x 1234 1230; done
