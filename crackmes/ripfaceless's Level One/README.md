#crackmes-one
This is a x86 PE files (32 bits), the challenge description said it's x86-64 (?).

I simply opened Ghidra in Kali Linux.
![[ripfaceless-level-one (4).png]]

First time analysis in Ghidra, you can see the information about this binary.

Then, you see the entry point for this C program.

![[ripfaceless-level-one (3).png]]

The left-hand side is disassembly, right-hand side is decompiled C code.
It shows how to execute this binary: 
```powershell
./crackme666.exe <password>
```

## Main Function Analysis

The logic is straightforward:

- The program checks if you provided exactly one argument (`_Argc == 2`)
- Your input (`_Argv[1]`) gets passed to `validate_key()` function
- `validate_key()` returns a boolean stored in `bVar1`:
    - `true` = correct password → prints success messages
    - `false` = wrong password → prints failure messages
- `iVar2` doesn't really matter for the challenge - it's just a loop control variable set to 0 after the validation check

## **Assembly confirmation:**

```asm
0x00401620: CMP dword ptr [EBP + _Argc], 0x2
0x00401624: JZ  LAB_00401642
```

This confirms the `argc` check before validation.

>**Noted:** The main function is just a wrapper. All the interesting logic is inside `validate_key()` - that's where we need to focus our analysis to find what password it expects.

# Go to function `validate_key`

This function is using calling convention `cdecl`, it means it's wrapped with c calling method to interact with OS.
![[ripfaceless-level-one (2).png]]
You see the hardcoded password in plaintext in the decompiled panel.

Another way is to go to `.rdata` section to check the value (see below).

![[ripfaceless-level-one (1).png]]
