/*
	Harris M. Snyder, 2023
	This is free and unencumbered software released into the public domain.

	Just a little CPUID program...

	Compiles with GCC, including on windows (e.g. mingw, https://github.com/skeeto/w64devkit)
*/

#include <stdio.h>

#define SYSV_ABI __attribute__((sysv_abi))
extern void SYSV_ABI issue_cpuid(unsigned registers[static 4], unsigned eax, unsigned ecx);
asm(
".intel_syntax noprefix       \n"
".global issue_cpuid          \n"
"issue_cpuid:                 \n"
"	push	rbx           \n"
"	mov	eax, esi      \n"
"	mov	ecx, edx      \n"
"	cpuid                 \n"
"	mov 	[rdi],    eax \n"
"	mov 	[rdi+4],  ebx \n"
"	mov 	[rdi+8],  ecx \n"
"	mov 	[rdi+12], edx \n"
"	pop	rbx           \n"
"       ret                   \n"
".att_syntax                  \n"
);

static int getbit(unsigned value, unsigned whichbit)
{
	return (value & (1<<whichbit)) != 0;
}

static void 
feature(char *name, unsigned reg, unsigned bit)
{
	printf("\t[%c]  %s\n", getbit(reg,bit) ? 'x' : ' ', name);
}

int main(void)
{
	unsigned registers[4] = {0};
	unsigned eax,ebx,ecx,edx;

	issue_cpuid(registers, 1, 0);
	eax = registers[0];
	ebx = registers[1];
	ecx = registers[2];
	edx = registers[3];
	printf("CPUID eax=1\n");
	printf("\teax = 0x%0.8x\n", eax);
	printf("\tebx = 0x%0.8x\n", ebx);
	printf("\tecx = 0x%0.8x\n", ecx);
	printf("\tedx = 0x%0.8x\n", edx);
	printf("\n");

	feature("x87 fpu", edx, 0);
	feature("mmx",     edx, 23);
	feature("sse",     edx, 25);
	feature("sse2",    edx, 26);

	feature("sse3",    ecx, 0);
	feature("fma",     ecx, 12);
	feature("sse4.1",  ecx, 19);
	feature("sse4.2",  ecx, 20);
	feature("popcnt",  ecx, 23);
	feature("avx",     ecx, 28);
	feature("f16c",    ecx, 29);
	feature("rdrand",  ecx, 30);

	printf("\n");

	issue_cpuid(registers, 7, 0);
	eax = registers[0];
	ebx = registers[1];
	ecx = registers[2];
	edx = registers[3];
	printf("CPUID eax=7, ecx=0\n");
	printf("\teax = 0x%0.8x\n", eax);
	printf("\tebx = 0x%0.8x\n", ebx);
	printf("\tecx = 0x%0.8x\n", ecx);
	printf("\tedx = 0x%0.8x\n", edx);
	printf("\n");

	feature("avx2",        ebx, 5);
	feature("avx512-f",    ebx, 16);
	feature("avx512-dq",   ebx, 17);
	feature("avx512-ifma", ebx, 21);
	feature("avx512-pf",   ebx, 26);
	feature("avx512-er",   ebx, 27);
	feature("avx512-cd",   ebx, 28);
	feature("avx512-bw",   ebx, 30);
	feature("avx512-vl",   ebx, 31);

	feature("prefetchwt1",      ecx, 0);
	feature("avx512-vbmi",      ecx, 1);
	feature("avx512-vbmi2",     ecx, 6);
	feature("avx512-vnni",      ecx, 11);
	feature("avx512-bitalg",    ecx, 12);
	feature("avx512-vpopcntdq", ecx, 14);

	feature("avx512-vnniw",        edx, 2);
	feature("avx512-4fmaps",       edx, 3);
	feature("avx512-vp2intersect", edx, 8);
	feature("amx-bf16",            edx, 22);
	feature("avx512-fp16",         edx, 23);
	feature("amx-tile",            edx, 24);
	feature("amx-int8",            edx, 25);

	printf("\n");

	issue_cpuid(registers, 7, 1);
	eax = registers[0];
	ebx = registers[1];
	ecx = registers[2];
	edx = registers[3];
	printf("CPUID eax=7, ecx=1\n");
	printf("\teax = 0x%0.8x\n", eax);
	printf("\tebx = 0x%0.8x\n", ebx);
	printf("\tecx = 0x%0.8x\n", ecx);
	printf("\tedx = 0x%0.8x\n", edx);
	printf("\n");

	feature("avx-vnni",    eax, 4);
	feature("avx512-bf16", eax, 5);
	feature("amx-fp16",    eax, 21);
	feature("avx-ifma",    eax, 23);

	feature("avx-vnni-int8",   edx, 4);
	feature("avx-ne-convert",  edx, 5);
	feature("amx-complex",     edx, 8);
	feature("avx-vnni-int16",  edx, 10);
	feature("prefetchi",       edx, 14);

}
