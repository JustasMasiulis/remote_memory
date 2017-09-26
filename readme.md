# remote_memory [![Build Status](https://travis-ci.com/JustasMasiulis/remote_memory.svg?token=pq8nGGFpC3YcRv1Qh2jz&branch=master)](https://travis-ci.com/JustasMasiulis/remote_memory) [![Build status](https://ci.appveyor.com/api/projects/status/ygc8vwnowox1krjb?svg=true)](https://ci.appveyor.com/project/JustasMasiulis/remote-memory)
A modern, easy to used library to read or write a remote processes memory that can be customized to your liking. Currently there is support for windows, linux and OSX operating systems.

## usage
By default the library uses jm::process_handle so passing it a process id or native handle is valid.

All of the functions also accept std::error_code as their last parameter.

```cpp
remote::memory mem = ...;
auto i = mem.read<int>(address); // address can be anything that has size of 4 or 8 bytes
mem.read(another_address, i); // overwrites i
mem.read(another_address, &i, sizeof(i));

mem.write(address, i);
mem.write(address, &i, sizeof(i));

// same as *(*(*address + 0x16) + 0x14) if all of these were byte pointers
mem.traverse_pointers_chain(address, 0x16, 0x4);

// free functions are also present
remote::read_memory(handle, address, &buffer, size);
remote::write_memory(handle, address, &buffer
```
