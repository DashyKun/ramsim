#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define RAM_SIZE     32768 // 32kb
#define NULL_TERMINATOR '\0'

uint8_t ram[RAM_SIZE];
uint16_t next_free_address = 0;

void iwrite(uint16_t address, uint16_t value)
{
    if (address < RAM_SIZE)
    {
        ram[address] = (uint8_t)value;
    }
}

void swrite(uint16_t address, const char* string)
{
    while (*string)
    {
        if (address >= RAM_SIZE) break;
        ram[address++] = (uint8_t)(*string++);
    }
    if (address < RAM_SIZE)
    {
        ram[address] = NULL_TERMINATOR;
    }
}

int read(uint16_t address)
{
    if (address < RAM_SIZE)
    {
        return ram[address];
    }
    return -1;
}

void sread(uint16_t address, char* buffer, uint16_t max_len)
{
    uint16_t i = 0;
    while (i < max_len - 1 && address < RAM_SIZE && ram[address] != NULL_TERMINATOR)
    {
        buffer[i++] = (char)ram[address++];
    }
    buffer[i] = NULL_TERMINATOR;
}



void printmenu()
{
    printf("\nRam simulator\n");
    printf("iw - write integer into memory\n");
    printf("ir - read integer from memory\n");
    printf("as - allocate string into memory\n");
    printf("vs - view string from memory\n");
    printf("vf - view free memory\n");
    printf("q  - quit\n");
    printf("Enter command: ");
}

int main(void)
{
    char command[10];
    char buffer[256];

    while (true) {
        printmenu();
        if (scanf("%9s", command) != 1) break;

        if (strcmp(command, "iw") == 0) {
            uint16_t addr;
            int val;
            printf("Enter address (hex): ");
            scanf("%hx", &addr);
            printf("Enter integer value: ");
            scanf("%d", &val);

            iwrite(addr, (uint16_t)val);
            printf("Wrote %d at 0x%04x\n", val, addr);

        } else if (strcmp(command, "ir") == 0) {
            uint16_t addr;
            printf("Enter address (hex): ");
            scanf("%hx", &addr);

            int val = read(addr);
            if (val >= 0)
                printf("Value at 0x%04x: %d\n", addr, val);
            else
                printf("Invalid address\n");

        } else if (strcmp(command, "as") == 0) {
            char input_str[128];
            printf("Enter string to allocate: ");
            scanf(" %[^\n]", input_str);

            size_t len = strlen(input_str) + 1;
            if (next_free_address + len > RAM_SIZE) {
                printf("Not enough free memory to allocate string\n");
            } else {
                swrite(next_free_address, input_str);
                printf("Allocated string at 0x%04x\n", next_free_address);
                next_free_address += len;
            }

        } else if (strcmp(command, "vs") == 0) {
            uint16_t addr;
            printf("Enter address of string to view (hex): ");
            scanf("%hx", &addr);

            sread(addr, buffer, sizeof(buffer));
            printf("String at 0x%04x: %s\n", addr, buffer);

        } else if (strcmp(command, "vf") == 0) {
            printf("Free memory: %u bytes\n", RAM_SIZE - next_free_address);

        } else if (strcmp(command, "q") == 0) {
            break;
        } else {
            printf("Unknown command\n");
        }
    }

    return 0;
}