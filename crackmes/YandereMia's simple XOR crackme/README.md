


![[xor (1).png]]

![[xor (2).png]]

---
# Disassembly
This is the **function signature and stack frame layout** for the `main` function
![[xor (16).png]]
### `undefined8 main() `
- Returns `undefined8` (8 bytes) = typically `int` on `x86-64`
- Entry point at address `0x0010109a`

## Stack Frame Layout

The disassembly shows **local variable allocations on the stack**:
```
Stack[-0x10]:8  local_10    // 8 bytes at RSP-0x10
Stack[-0x18]:1  local_18    // 1 byte at RSP-0x18
Stack[-0x28]... local_28    // Array at RSP-0x28 (16 bytes)
Stack[-0x38]:1  local_38    // 1 byte at RSP-0x38
Stack[-0x48]... local_48    // Array at RSP-0x48 (16 bytes)
Stack[-0x58]:1  local_58    // 1 byte at RSP-0x58
Stack[-0x68]... local_68    // Array at RSP-0x68 (16 bytes)
Stack[-0x78]:1  local_78    // 1 byte at RSP-0x78
Stack[-0x88]... local_88    // Array at RSP-0x88 (16 bytes)
```
There are totally 4 arrays in this program.
- `local_88` = First XOR array (16 bytes)
- `local_68` = Second XOR array (16 bytes)
- `local_48` = Third XOR array (16 bytes)
- `local_28` = Fourth XOR array (16 bytes)

> The single-byte variables (`local_18`, `local_38`, etc.) are likely **padding** or temporary variables.

## Cross-References (XREF)

Shows where different parts of the code are referenced:

- `XREF[2]: 001010d0(W), 001011d0(*)` - Written at 0x1010d0, referenced at 0x1011d0
- These help track data flow through the program


![[xor (15).png]]
There are several loops.  `LAB_00101140`is XOR Validation Loop (Main Logic) we should focus on.
![[xor (10).png]]


# Decompiled Pseudo C Code
It's always easier to compare with pseudo C code (if the program is written in c/c++).
![[xor (11).png]]

![[xor (12).png]]

![[xor (13).png]]

![[xor (14).png]]



---

Let's code the logic in python. They have decimals in the array (full of hexadecimals)
```python
# Extracted from Ghidra decompilation

# pbVar2
local_88 = [ 0x5e, 0x36, 0x32, 0x28, 0x41, 0x79, 0x26, 0x33, 0x60, 0x72, 0x37, 0x6a, 0x7c, 0x51, 0x7d, 0x3e]

# pbVar5
local_68 = [0x36, 0x69, 0x75, 0x37, 0x28, 0x69, 0x55, 0x42, 0x70, 0x44, 0x24, 0x39, 0x4b, 0x6c, 0x49, 0x43]

# pbVar4
local_48 = [0x3a, 0x76, 0x54, 0x33, 0x3f, 0x5b, 0x5a, 0x7d, 99, 0x56, 0x27, 0x6f, 0x66, 0x38, 0x3f, 0x43] # 99 is decimal

# pbVar3
local_28 = [0x33, 0x4b, 0x70, 0x2a, 0x33, 0x2b, 0x4e, 100, 0x6a, 0x78, 0x5f, 0x29, 0x40, 0x6b, 100,  0x4e] # 100 is decimal

# pbVar1
key = ""
```

# Key (`pbVar1`)
![[xor (3).png]]
At line 23, it's extracting the **user's input from command-line arguments**:
```c
pbVar1 = *(byte **)(param_2 + 8);
```
>**What it means:**
>- `param_2` = `argv` (second parameter to `main`, which is `char **argv`)
>- `param_2 + 8` = `&argv[1]` (pointer to the first command-line argument)
>- `*(byte **)(param_2 + 8)` = `argv[1]` (the actual string the user typed)

**In simple terms:**
```c
pbVar1 = argv[1];  // User's input key
```

## Assembly Explanation
```assembly
MOV RCX, qword ptr [RAX + 0x8]
```

On x86-64:
- Each pointer is 8 bytes
- `argv[0]` is at offset 0 (program name)
- `argv[1]` is at offset 8 (first argument - the key)

## In the Validation Loop

Later in the code (line 93):
```c
if ((*pbVar1 != (byte)(*pbVar2 ^ *pbVar3 ^ *pbVar5 ^ *pbVar4 ^ 0x20))) {
    puts("Nope.");
}
```

`pbVar1` is dereferenced to get each character of the user's input and compare it against the XORed result.

**So `pbVar1` is simply a pointer to the user-provided key string** (what required key typed after `./crackme`).

## LAB_00101140 - XOR Validation Loop (Main Logic)
![[xor (33).png]]
```assembly
MOVZX  EAX, byte ptr [RDX]=>local_88    ; Load byte from array 1
XOR    AL, byte ptr [RSI]=>local_28      ; XOR with array 2
XOR    AL, byte ptr [R8]=>local_68       ; XOR with array 3
XOR    AL, byte ptr [RDI]=>local_48      ; XOR with array 4
XOR    EAX, 0x20                         ; XOR with 0x20
CMP    byte ptr [RCX], AL                ; Compare with user input
JNZ    LAB_00101190                      ; If not equal → "Nope"
```
![[xor (34).png]]
**Decompiled (Line 93):**
```c
if ((*pbVar1 != (byte)(*pbVar2 ^ *pbVar3 ^ *pbVar5 ^ *pbVar4 ^ 0x20))) {
    puts("Nope.");
    goto LAB_00101176;
	}
	pbVar2 = pbVar2 + 1;     // Increment pointer to array 1
	pbVar5 = pbVar5 + 1;     // Increment pointer to array 2
	pbVar4 = pbVar4 + 1;     // Increment pointer to array 3
	pbVar3 = pbVar3 + 1;     // Increment pointer to array 4
	pbVar1 = pbVar1 + 1;     // Increment user input pointer
	} while (pbVar2 != local_88 + 0x10);  // Loop until 16 bytes checked
puts("Pass valid!");
}
```
### What Each Part Means: 
 - **`local_88`** = Base address of the first XOR array (start of the array) 
 - **`0x10`** = Hexadecimal for **16 decimal** (the array length)
 - **`local_88 + 0x10`** = Address that is **16 bytes after** the start = **end of the array** **`pbVar2`** = Pointer that **starts at `local_88[0]`** and increments each iteration

In python code, it looks like:
```python
key = ""

for i in range(16):
    byte = local_88[i] ^ local_28[i] ^ local_68[i] ^ local_48[i] ^ 0x20
    key += chr(byte)

print(f"\nThe password is {key}")
print(f"\nThe hex is:")
print(' '.join(f'{ord(c):02x}' for c in key))
print(f"\nASCII code is:")
print(' '.join(f"{ord(c):3d}" for c in key))
```

![[xor (2) 1.png]]

> The password is `ABC&E@GH98K51NOP`.