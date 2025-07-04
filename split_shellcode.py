def split_shellcode(shellcode, bytes_per_line=16):
    # Remove whitespace and quotes
    shellcode = shellcode.replace('"', '').replace("'", '').replace('\n', '').replace(' ', '')
    # Split into list of \x.. bytes
    bytes_list = [shellcode[i:i+4] for i in range(0, len(shellcode), 4) if shellcode[i:i+4].startswith('\\x')]
    # Group into lines
    lines = [''.join(bytes_list[i:i+bytes_per_line]) for i in range(0, len(bytes_list), bytes_per_line)]
    # Surround each line with double quotes
    quoted_lines = ['"' + line + '"' for line in lines]
    return quoted_lines

def main():
    input_file = 'shellcode.txt'  # Change this to your filename
    with open(input_file, 'r') as f:
        shellcode = f.read()
    for line in split_shellcode(shellcode):
        print(line)

if __name__ == "__main__":
    main()
