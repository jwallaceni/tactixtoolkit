/*
  Extract opcodes from the .text section of a disassembled binary and format into \xHH (Hex/C style) shellcode
  
  Examples:
  
    objdump -D mimikatz.exe | ./extract_bytes
    
    objdump -D mimikatz.exe > mimikatz.txt
    ./extract_bytes mimikatz.txt

    Given this objdump output:

    08048400 <.text>:
    8048400: 55                    push   %ebp
    8048401: 89 e5                 mov    %esp,%ebp
    8048403: 83 ec 08              sub    $0x8,%esp

    Outputs:

    \x55\x89\xe5\x83\xec\x08

    Compile with: gcc extract_bytes.c -o extract_bytes
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_MAX 1024

int main(int argc, char *argv[]) {
    FILE *fp = stdin;
    char line[LINE_MAX];
    int in_text_section = 0;

    if (argc > 1) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            perror("Failed to open file");
            return 1;
        }
    }

    while (fgets(line, sizeof(line), fp)) {
        // Debug: print lines to see what we get
        // printf("Line: %s", line);

        // Detect start of .text section more flexibly
        if (!in_text_section) {
            if (strstr(line, "<.text>") != NULL || strstr(line, "Disassembly of section .text:") != NULL) {
                in_text_section = 1;
                continue; // Skip the header line itself
            }
            continue;
        }

        // If we reach another section header, stop processing
        if (strstr(line, "Disassembly of section") != NULL && strstr(line, ".text") == NULL) {
            break;
        }

        char *colon = strchr(line, ':');
        if (!colon) continue;

        char *p = colon + 1;
        while (*p && isspace((unsigned char)*p)) p++;

        while (*p) {
            if (isxdigit((unsigned char)*p)) {
                char byte_str[3] = {0};
                byte_str[0] = tolower((unsigned char)*p++);
                if (isxdigit((unsigned char)*p)) {
                    byte_str[1] = tolower((unsigned char)*p++);
                } else {
                    break;
                }
                printf("\\x%s", byte_str);
            } else if (isspace((unsigned char)*p)) {
                p++;
            } else {
                break;
            }
        }
    }

    if (fp != stdin) fclose(fp);
    return 0;
}
