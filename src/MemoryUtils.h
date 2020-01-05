#pragma once

class MemoryUtils
{
private:
	static void* trampoline_memory_base;

public:
	static int DetourCall(void* hook_call_addr, const void* hook_function);
	static void DetourVFTCall(void** vft_entry_addr, void* hook_function, void** original_fn_ptr);
};

