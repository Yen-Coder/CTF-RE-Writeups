/*
 * XOR Crackme Solver - C Implementation
 * Based on Ghidra decompilation analysis
 * Formula: key[i] = local_88[i] ^ local_28[i] ^ local_68[i] ^ local_48[i] ^ 0x20
 */

#include <stdio.h>
#include <stdint.h>

#define KEY_LENGTH 16

int main(void)
{
    // Extracted from Ghidra decompilation
    uint8_t local_88[KEY_LENGTH] = {
        0x5e, 0x36, 0x32, 0x28, 0x41, 0x79, 0x26, 0x33,
        0x60, 0x72, 0x37, 0x6a, 0x7c, 0x51, 0x7d, 0x3e};

    uint8_t local_68[KEY_LENGTH] = {
        0x36, 0x69, 0x75, 0x37, 0x28, 0x69, 0x55, 0x42,
        0x70, 0x44, 0x24, 0x39, 0x4b, 0x6c, 0x49, 0x43};

    uint8_t local_48[KEY_LENGTH] = {
        0x3a, 0x76, 0x54, 0x33, 0x3f, 0x5b, 0x5a, 0x7d,
        99, 0x56, 0x27, 0x6f, 0x66, 0x38, 0x3f, 0x43};

    uint8_t local_28[KEY_LENGTH] = {
        0x33, 0x4b, 0x70, 0x2a, 0x33, 0x2b, 0x4e, 100,
        0x6a, 0x78, 0x5f, 0x29, 0x40, 0x6b, 100, 0x4e};

    char key[KEY_LENGTH + 1]; // +1 for null terminator

    // XOR all arrays together with 0x20
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        uint8_t byte = local_88[i] ^ local_28[i] ^ local_68[i] ^ local_48[i] ^ 0x20;
        key[i] = (char)byte;
    }
    key[KEY_LENGTH] = '\0'; // Null terminate the string

    // Print results
    printf("============================================================\n");
    printf("XOR CRACKME SOLUTION\n");
    printf("============================================================\n\n");

    printf("The correct key is: %s\n\n", key);
    printf("Key length: %d bytes\n\n", KEY_LENGTH);

    printf("Hex representation:\n");
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        printf("%02x ", (unsigned char)key[i]);
    }
    printf("\n\n");

    printf("ASCII codes:\n");
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        printf("%3d ", (unsigned char)key[i]);
    }
    printf("\n");

    printf("============================================================\n");

    return 0;
}