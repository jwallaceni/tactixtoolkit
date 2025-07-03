# Converts shellcode from hex C style shellcode to C# style shellcode

import re

# Paste your hex-escaped shellcode string here (use raw string or double backslashes)
shellcode = r"\xff\xff\xca\xb8\x00\x00\x00\x00\xc9\xc3"

def hex_escaped_to_csharp_array(shellcode_str):
    bytes_list = re.findall(r'\\x([0-9a-fA-F]{2})', shellcode_str)
    if not bytes_list:
        print("No hex bytes found in input.")
        return

    formatted_bytes = ','.join(f'0x{b.lower()}' for b in bytes_list)
    csharp_array = f'byte[] shellcode = new byte[] {{ {formatted_bytes} }};'
    return csharp_array

if __name__ == "__main__":
    result = hex_escaped_to_csharp_array(shellcode)
    if result:
        print(result)
