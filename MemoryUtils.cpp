#include "stdafx.h"
#include "MemoryUtils.h"

void* MemoryUtils::trampoline_memory_base = nullptr;

//TODO: Replace with a proper jit assembler + detour class solution if more complex hooks or removable hooks are ever required.
int MemoryUtils::DetourCall(void* hook_call_addr, const void* hook_function) {
	if(trampoline_memory_base == nullptr)
		trampoline_memory_base = VirtualAlloc((LPVOID)0x17F000000, 0x200, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	const unsigned int TRAMPONLINE_SIZE = 17;
	void* trampoline_addr = trampoline_memory_base;
	while (*reinterpret_cast<char*>(trampoline_addr) == 0x48)
		trampoline_addr = (void*)((long long)trampoline_addr + TRAMPONLINE_SIZE);

	//build hook jmp
	int jmp_operand = (int)((long long)trampoline_addr - ((long long)hook_call_addr + 5));

	unsigned char hook_bytes[5]{
		0xE9, 0x00, 0x00, 0x00, 0x00 // jmp rip+XXXXXXXX
	};
	*(int*)(hook_bytes + 1) = jmp_operand;

	DWORD oldProtection;
	VirtualProtect(hook_call_addr, 0x05, PAGE_EXECUTE_READWRITE, &oldProtection);
	memcpy_s(hook_call_addr, sizeof(hook_bytes), hook_bytes, sizeof(hook_bytes));
	VirtualProtect(hook_call_addr, 0x05, oldProtection, &oldProtection);

	//build trampoline call
	unsigned char trampoline_bytes[TRAMPONLINE_SIZE]{
		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov rax, XXXXXXXXXXXXXXXX
		0xFF, 0xD0,													//call rax
		0xE9, 0x00, 0x00, 0x00, 0x00								//jmp rip+XXXXXXXX
	};

	*(long long*)(trampoline_bytes + 2) = (long long)hook_function;
	int return_jmp_operand = ((long long)hook_call_addr + 5) - ((long long)trampoline_addr + 17);
	*(int*)(trampoline_bytes + 13) = return_jmp_operand;
	memcpy_s(trampoline_addr, sizeof(trampoline_bytes), trampoline_bytes, sizeof(trampoline_bytes));

	return 0;
}

void MemoryUtils::DetourVFTCall(void** vft_entry_addr, void* hook_function, void** original_fn_ptr) {
	*original_fn_ptr = *vft_entry_addr;

	DWORD oldProtection;
	VirtualProtect(vft_entry_addr, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtection);
	*vft_entry_addr = hook_function;
	VirtualProtect(vft_entry_addr, sizeof(void*), oldProtection, &oldProtection);
}
