# An simple assembler for cpu used in course VLSI ckt design

### Usage:
Clone the repo and make.
* Build
```bash
cd assembler
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
Then you can add any instruction on your favor depends on opCode of your instruction. Op's Opcode is set according to their index in the op_table.  
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
**Note that the golden file in tst/ will be incorrect after this step, because the index of `HLT` become 10 (op_table[10] == `HLT`) and opCode of `HLT` is no longer 9 but 10, so the new generated opCode would be 1010 for `HLT`. You also need to modified RTL/cpu.v for new instruct, otherwise it won't work properly.**

* Clean built targets
```bash
make clean
```

### Test with RTL code:
Set $(MACHINE_CODE_SAVE_PATH) to be `../RTL/sisc.prog` and run the assembler.
Use cmd
```bash
cd ASM/
./asm $(ASM_FILE_PATH) ../RTL/sisc.prog
cd ../RTL/
iverilog cpu.v -o cpu
./cpu
```
To see how RTL cpu run with machineCode `sisc.prog`.


### Known issues/bugs:
* Only support one line comment token "//"
    Multi line comment token "/**/" is illegal and won't be implemented currently.
* This assembler parser has no syntax checking function, user should make sure the input has no syntax error by their own.


### assembler default rule and example:
![](https://i.imgur.com/LEm5Pbf.png)
![](https://i.imgur.com/tmiaJ35.png)
![](https://i.imgur.com/u2PhSsx.png)
cc: cond code.
The detail definition of ins can be found in `ins.c`.
