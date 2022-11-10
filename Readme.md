# A simple assembler for cpu used in course VLSI system design

### Usage:
* Clone the repo.
```
git clone https://github.com/fennecJ/VLSI-Assembler
```
* Build
```bash
cd VLSI-Assembler/assembler
make
```
* Run
```bash
./asm $(ASM_FILE_PATH) $(MACHINE_CODE_SAVE_PATH)
```
And the converted machine code will appeared in `$(MACHINE_CODE_SAVE_PATH)`.  

* Add other instruction  
Go to file `assembler/ins.c`, you'll see something like:  
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
* Disclaimer: I do not own the right of `cpu.v`, if the owner hope me to remove the src code of cpu.v, please contact me. 
**Please note that the license is only valid for file under dir assembler/**

### Known issues/bugs:
* Only support one line comment token "//"  
    Multi line comment token "/**/" is illegal and won't be implemented currently.  

* Comment must be seperated from code by at least 1 space  
    e.g.
    ```
    (X) BRA END, CCZ// If cond is zero then goto end
    (O) BRA END, CCZ // If cond is zero then goto end
                    ^ There must be at least 1 space
    ```

* opCode = 0 is reserved for NOP  
    If you want to change opCode of NOP to other number, goto `assembler/assembler.c`, find funct set_all_field, and change  
    ```c
    if(i == 0)     //NOP
    return ins;
    ```
    to   
    ```c
    if(i == NUM_OF_NOP)     //NOP
    return ins;
    ```

* Currently you can only add new instruct with format {inst dest, src}  
    The instuct added to file `assembler/ins.c` must be with format {inst dest, src}, **inst without dest and/or src is not allowed**  
    e.g.
    ```
    (X)JEQ R0        (Invalid, missing dest and/or src) 
    (O)DIV R0, R1
    ```
    If you want to add instruct other than format {inst dest, src}, you need to modify funct set_all_field in `assembler/assembler.c` by yourself.  

* This assembler parser has no syntax checking function, user should make sure the input has no syntax error by their own.  

* The hex format is only acceptable way to assign value using label currently, and the leading zero cannot be omitted. **The length of hex val must be exactly 8.**  
    e.g.  If you want to assign 7:    
    ```  
    (X) Num1:7          (invalid, leading zero cannot be omitted)  
    (O) Num1:00000007   (correct, leading zero is kept and length of hex val is exactly 8)  
    ```

### assembler default rule and example:
![](https://i.imgur.com/LEm5Pbf.png)
![](https://i.imgur.com/tmiaJ35.png)
![](https://i.imgur.com/u2PhSsx.png)
cc: cond code.  
SHF and ROT: Right:positive/Left:negative.  
The detail definition and implementataion of ins can be found in `assembler/ins.c` and `RTL/cpu.v`.