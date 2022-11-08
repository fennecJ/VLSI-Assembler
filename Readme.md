# An simple assembler for cpu used in course VLSI ckt design

### Usage:
Clone the repo and make.
* Build
```bash
make
```
* Run
```bash
./asm $(ASM_FILE_PATH) $(MACHINE_CODE_SAVE_PATH)
```
And the converted machine code will appeared in `$(MACHINE_CODE_SAVE_PATH)`.

* Add other instruction
Go to file ins.c, you'll see something like:
```c
char *op_table[] = {
    "NOP" , "BRA",
    "LD"  , "STR",
    "ADD" , "MUL",
    "CMP" , "SHF",
    "ROT" , "HLT",
    "EXT1", 
};
```
Then you can add any instruction on your favor depends on opCode of your instruction.  
For example, if you add an instruction `DIV` whose opCode is 9 (Insert between `ROT` and `HLT`),  
Then the modified op_table will be:  
```c
char *op_table[] = {
    "NOP" , "BRA",
    "LD"  , "STR",
    "ADD" , "MUL",
    "CMP" , "SHF",
    "ROT" , "DIV",
    "HLT", "EXT1", 
};
```

**Note that the golden file in tst/ will be incorrect after this step, because the opCode of `HLT` is no longer 9 but 10, so the new generated opCode would be 1010 for `HLT`. You also need to modified RTL/cpu.v for new instruct, otherwise it won't work properly.**


### Test with RTL code:
Copy the machineCode into dir RTL/ and rename machineCode into `sisc.prog` .
Use cmd
```bash
iverilog cpu.v -o cpu
./cpu
```
To see how RTL cpu run with machineCode `sisc.prog`.


### Known issues/bugs:
* Currently none