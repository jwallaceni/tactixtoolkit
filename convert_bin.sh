cat shellcode.txt | sed 's/\\x//g' | xxd -r -p > shellcode.bin
