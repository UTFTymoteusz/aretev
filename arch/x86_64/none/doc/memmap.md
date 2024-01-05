# art/2 x86_64 Memory Map

 | Start Address      | End Address        | Size    | Description                  | 
 | -------------------|------------------- | ------- | ---------------------------- | 
 | 0x0000000000000000 | 0x7FFFFFFFFFFFFFFF |         | Userspace                    | 
 | 0xFFFF800000000000 | 0xFFFF807FFFFFFFFF | 512 GiB | Kernelspace data             | 
 | 0xFFFFFA0000000000 | 0xFFFFFA7FFFFFFFFF | 512 GiB | Recursive page table mapping | 
 | 0xFFFFFFFF80100000 | 0xFFFFFFFFFFFFEFFF | ~2 GiB  | Kernelspace exec             | 