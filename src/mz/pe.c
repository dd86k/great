#include <stdio.h>
#include <string.h>
#include "../utils.h"
#include "../settings.h"
#include "pe.h"

//peh.Characteristics & EXECUTABLE_IMAGE for OBJ/EXE diff?

void scan_pe() {
	/*
	 * Stick to the peoh64 structure, the 32-bit variant is translated
	 * to the 64-bit one (PE32->PE32+) for simplicity. The BaseOfData field
	 * is translated under this function.
	 */

	struct PE_HEADER peh;
	struct PE_OPTIONAL_HEADER peoh;
	struct PE_OPTIONAL_HEADER64 peoh64;
	struct IMAGE_DATA_DIRECTORY dirs;

	uint32_t _BaseOfData;
	uint16_t magic;

	_ddread(&peh, sizeof(peh));

	if (peh.SizeOfOptionalHeader) { // PE Optional Header
		_ddread(&magic, sizeof(magic)); // 2-byte
		switch (magic) {
		case HDR32:
			_ddread(&peoh, sizeof(peoh));
			// Got lazy, some easy optimization would be a delicate use of memcpy
			peoh64.MajorLinkerVersion = peoh.MajorLinkerVersion;
			peoh64.MinorLinkerVersion = peoh.MinorLinkerVersion;
			peoh64.SizeOfCode = peoh.SizeOfCode;
			peoh64.SizeOfInitializedData = peoh.SizeOfInitializedData;
			peoh64.SizeOfUninitializedData = peoh.SizeOfUninitializedData;
			peoh64.AddressOfEntryPoint = peoh.AddressOfEntryPoint;
			peoh64.BaseOfCode = peoh.BaseOfCode;
			_BaseOfData = peoh.BaseOfData;
			peoh64.ImageBase = peoh.ImageBase;
			peoh64.SectionAlignment = peoh.SectionAlignment;
			peoh64.FileAlignment = peoh.FileAlignment;
			peoh64.MajorOperatingSystemVersion = peoh.MajorOperatingSystemVersion;
			peoh64.MinorOperatingSystemVersion = peoh.MinorOperatingSystemVersion;
			peoh64.MajorImageVersion = peoh.MajorImageVersion;
			peoh64.MinorImageVersion = peoh.MinorImageVersion;
			peoh64.MajorSubsystemVersion = peoh.MajorSubsystemVersion;
			peoh64.MinorSubsystemVersion = peoh.MinorSubsystemVersion;
			peoh64.Win32VersionValue = peoh.Win32VersionValue;
			peoh64.SizeOfImage = peoh.SizeOfImage;
			peoh64.SizeOfHeaders = peoh.SizeOfHeaders;
			peoh64.CheckSum = peoh.CheckSum;
			peoh64.Subsystem = peoh.Subsystem;
			peoh64.DllCharacteristics = peoh.DllCharacteristics;
			peoh64.SizeOfStackReserve = peoh.SizeOfStackReserve;
			peoh64.SizeOfStackCommit = peoh.SizeOfStackCommit;
			peoh64.SizeOfHeapReserve = peoh.SizeOfHeapReserve;
			peoh64.SizeOfHeapCommit = peoh.SizeOfHeapCommit;
			peoh64.LoaderFlags = peoh.LoaderFlags;
			peoh64.NumberOfRvaAndSizes = peoh.NumberOfRvaAndSizes;
			printf("PE32 ");
			break;
		case HDR64:
			_ddread(&peoh64, sizeof(peoh64));
			printf("PE32+ ");
			break;
		case ROM: printf("PE32-ROM "); return; // Unsupported for now
		default:
			printf("PE32?");
			return;
		}
		// Should depend on NumberOfRvaAndSizes, but I'm lazy!
		_ddread(&dirs, sizeof(dirs));
	}

	switch (peoh64.Subsystem) {
	case NATIVE:
		printf("Windows Native");
		break;
	case WINDOWS_GUI:
		printf("Windows GUI");
		break;
	case WINDOWS_CUI:
		printf("Windows Console");
		break;
	case POSIX_CUI:
		printf("Posix Console");
		break;
	case WINDOWS_CE_GUI:
		printf("Windows CE GUI");
		break;
	case EFI_APPLICATION :
		printf("EFI");
		break;
	case EFI_BOOT_SERVICE_DRIVER :
		printf("EFI Boot Service driver");
		break;
	case EFI_RUNTIME_DRIVER:
		printf("EFI Runtime driver");
		break;
	case EFI_ROM:
		printf("EFI ROM");
		break;
	case XBOX:
		printf("XBOX");
		break;
	case WINDOWS_BOOT_APPLICATION:
		printf("Windows Boot Application");
		break;
	default:
		printf("?");
		break;
	}

	if (dirs.CLRHeaderSize)
		printf(" .NET");

	if (peh.Characteristics & DLL)
		printf(" Library for ");
	else if (peh.Characteristics & EXECUTABLE_IMAGE)
		printf(" Executable for ");
	else
		printf(" ? for ");

	switch (peh.Machine) {
	default: // UNKNOWN
		printf("?");
		break;
	case I386:
		printf("x86");
		break;
	case AMD64:
		printf("x86-64");
		break;
	case IA64:
		printf("IA64");
		break;
	case EBC:
		printf("EFI (Byte Code)");
		break;
	case CLR:
		printf("Common Language Runtime");
		break;
	case ARM:
		printf("ARM (Little Endian)");
		break;
	case ARMNT:
		printf("ARMv7+ (Thumb)");
		break;
	case ARM64:
		printf("ARMv8 (64-bit)");
		break;
	case M32R:
		printf("Mitsubishi M32R (Little endian)");
		break;
	case AM33:
		printf("Matsushita AM33");
		break;
	case MIPS16:
		printf("MIPS16");
		break;
	case MIPSFPU:
		printf("MIPS (w/FPU)");
		break;
	case MIPSFPU16:
		printf("MIPS16 (w/FPU)");
		break;
	case POWERPC:
		printf("PowerPC");
		break;
	case POWERPCFP:
		printf("PowerPC (w/FPU)");
		break;
	case R4000:
		printf("MIPS (Little endian)");
		break;
	case SH3:
		printf("Hitachi SH3");
		break;
	case SH3DSP:
		printf("Hitachi SH3 DSP");
		break;
	case SH4:
		printf("Hitachi SH4");
		break;
	case SH5:
		printf("Hitachi SH5");
		break;
	case THUMB:
		printf("ARM or Thumb (\"Interworking\")");
		break;
	case WCEMIPSV2:
		printf("MIPS WCE v2 (Little Endian)");
		break;
	}

	printf(" machines");

	if (peh.Characteristics) {
		if (peh.Characteristics & RELOCS_STRIPPED)
			printf(", RELOCS_STRIPPED");
		if (peh.Characteristics & LINE_NUMS_STRIPPED)
			printf(", LINE_NUMS_STRIPPED");
		if (peh.Characteristics & LOCAL_SYMS_STRIPPED)
			printf(", LOCAL_SYMS_STRIPPED");
		if (peh.Characteristics & LARGE_ADDRESS_AWARE)
			printf(", LARGE_ADDRESS_AWARE");
		if (peh.Characteristics & _16BIT_MACHINE)
			printf(", 16BIT_MACHINE");
		if (peh.Characteristics & _32BIT_MACHINE)
			printf(", 32BIT_MACHINE");
		if (peh.Characteristics & DEBUG_STRIPPED)
			printf(", DEBUG_STRIPPED");
		if (peh.Characteristics & REMOVABLE_RUN_FROM_SWAP)
			printf(", REMOVABLE_RUN_FROM_SWAP");
		if (peh.Characteristics & NET_RUN_FROM_SWAP)
			printf(", NET_RUN_FROM_SWAP");
		if (peh.Characteristics & SYSTEM)
			printf(", SYSTEM");
		if (peh.Characteristics & UP_SYSTEM_ONLY)
			printf(", UP_SYSTEM_ONLY");
	}

	if (peoh64.DllCharacteristics) {
		printf("\nDLL Characteristics:\n  <%08Xh", peoh64.DllCharacteristics);
		if (peoh64.DllCharacteristics & DLL_DYNAMIC_BASE)
			printf(", DYNAMIC_BASE");
		if (peoh64.DllCharacteristics & DLL_FORCE_INTEGRITY)
			printf(", FORCE_INTEGRITY");
		if (peoh64.DllCharacteristics & DLL_NX_COMPAT)
			printf(", NX_COMPAT");
		if (peoh64.DllCharacteristics & DLL_NO_ISOLATION)
			printf(", NO_ISOLATION");
		if (peoh64.DllCharacteristics & DLL_NO_SEH)
			printf(", NO_SEH");
		if (peoh64.DllCharacteristics & DLL_NO_BIND)
			printf(", NO_BIND");
		if (peoh64.DllCharacteristics & DLL_WDM_DRIVER)
			printf(", WDM_DRIVER");
		if (peoh64.DllCharacteristics & DLL_TERMINAL_SERVER_AWARE)
			printf(", TERMINAL_SERVER_AWARE");
		puts(">");
	}

	if (setting_header) printf(
		"\nHeader data\n"
		"\nTimestamp              : %Xh\n"
		"Number of sections     : %d\n"
		"Number of symbols      : %d\n"
		"Symbol Table Pointer   : %Xh\n"
		"Optional Header Size   : %d\n"
		// Optional
		"Linker Version         : %d.%d\n"
		"Size of Code           : %Xh\n"
		"Initialized Data Size  : %Xh\n"
		"Uninitialized Data Size: %Xh\n"
		"Entry Point Address    : %Xh\n"
		"Base of Code           : %Xh\n"
		"Base of Data           : %Xh\n"
		"Base of Image          : %llXh\n"
		"Section Alignment      : %d\n"
		"File Alignment         : %d\n"
		"OS Version             : %d.%d\n"
		"Image Version          : %d.%d\n"
		"Subsystem Version      : %d.%d\n"
		"Win32 Version          : %Xh\n"
		"Size of Image          : %Xh\n"
		"Size of Headers        : %Xh\n"
		"Checksum               : %Xh\n"
		"Stack Reserve Size     : %llXh\n"
		"Stack Commit Size      : %llXh\n"
		"Heap Reserve Size      : %llXh\n"
		"Heap Commit Size       : %llXh\n"
		"Loader Flags (Obsolete): %Xh\n"
		"Number of RVA and Sizes: %d\n",
		peh.TimeDateStamp,
		peh.NumberOfSections,
		peh.NumberOfSymbols,
		peh.PointerToSymbolTable,
		peh.SizeOfOptionalHeader,
		// Optional
		peoh64.MajorLinkerVersion, peoh64.MinorLinkerVersion,
		peoh64.SizeOfCode,
		peoh64.SizeOfInitializedData,
		peoh64.SizeOfUninitializedData,
		peoh64.AddressOfEntryPoint,
		peoh64.BaseOfCode,
		_BaseOfData,
		peoh64.ImageBase,
		peoh64.SectionAlignment,
		peoh64.FileAlignment,
		peoh64.MajorOperatingSystemVersion, peoh64.MinorOperatingSystemVersion,
		peoh64.MajorImageVersion, peoh64.MinorImageVersion,
		peoh64.MajorSubsystemVersion, peoh64.MinorSubsystemVersion,
		peoh64.Win32VersionValue,
		peoh64.SizeOfImage,
		peoh64.SizeOfHeaders,
		peoh64.CheckSum,
		peoh64.SizeOfStackReserve,
		peoh64.SizeOfStackCommit,
		peoh64.SizeOfHeapReserve,
		peoh64.SizeOfHeapCommit,
		peoh64.LoaderFlags,
		peoh64.NumberOfRvaAndSizes
	);

	if (setting_directories) printf( // Directories
		"\nDirectories\n\n"
		"Export Table          : %8Xh  %d B\n"
		"Import Table          : %8Xh  %d B\n"
		"Resource Table        : %8Xh  %d B\n"
		"Exception Table       : %8Xh  %d B\n"
		"Certificate Table     : %8Xh  %d B\n"
		"Base Relocation Table : %8Xh  %d B\n"
		"Debug Directory       : %8Xh  %d B\n"
		"Architecture Data     : %8Xh  %d B\n"
		"Global Pointer        : %8Xh  %d B\n"
		"TLS Table             : %8Xh  %d B\n"
		"Load Configuration    : %8Xh  %d B\n"
		"Bound Import          : %8Xh  %d B\n"
		"Import Address        : %8Xh  %d B\n"
		"Delay Import          : %8Xh  %d B\n"
		"CLR Header            : %8Xh  %d B\n"
		"Reserved              : %8Xh  %d B\n",
		dirs.ExportTableVA, dirs.ExportTableSize,
		dirs.ImportTableVA, dirs.ImportTableSize,
		dirs.ResourceTableVA, dirs.ResourceTableSize,
		dirs.ExceptionTableVA, dirs.ExceptionTableSize,
		dirs.CertificateTableVA, dirs.CertificateTableSize,
		dirs.BaseRelocationTableVA, dirs.BaseRelocationTableSize,
		dirs.DebugDirectoryVA, dirs.DebugDirectorySize,
		dirs.ArchitectureDataVA, dirs.ArchitectureDataSize,
		dirs.GlobalPtrVA, dirs.GlobalPtrSize,
		dirs.TLSTableVA, dirs.TLSTableSize,
		dirs.LoadConfigurationTableVA, dirs.LoadConfigurationTableSize,
		dirs.BoundImportTableVA, dirs.BoundImportTableSize,
		dirs.ImportAddressTableVA, dirs.ImportAddressTableSize,
		dirs.DelayImportVA, dirs.DelayImportSize,
		dirs.CLRHeaderVA, dirs.CLRHeaderSize,
		dirs.ReservedVA, dirs.ReservedSize
	);

	if (setting_sections) {
		if (peh.NumberOfSections) {
			int i = 0;
			puts("\nSections");
			struct PE32_SECTION s;
			char nbuf[9] = { 0 };
			do {
				_ddread(&s, sizeof(struct PE32_SECTION));
				memcpy(nbuf, s.Name, 8);
				printf(
					"\n%d  %s\n"
					"  Virtual Address : %8Xh  %d B\n"
					"  Raw Data        : %8Xh  %d B\n"
					"  Relocations     : %8Xh  %d B\n"
					"  Line Numbers    : %8Xh  %d B\n"
					"  <%08X",
					++i, nbuf,
					s.VirtualAddress, s.VirtualSize,
					s.PointerToRawData, s.SizeOfRawData,
					s.PointerToRelocations, s.NumberOfRelocations,
					s.PointerToLinenumbers, s.NumberOfLinenumbers,
					s.Characteristics
				);
				if (s.Characteristics & SEC_NO_PAD)
					printf(", NO_PAD");
				if (s.Characteristics & SEC_CNT_CODE)
					printf(", CNT_CODE");
				if (s.Characteristics & SEC_INITIALIZED_DATA)
					printf(", INITIALIZED_DATA");
				if (s.Characteristics & SEC_UNINITIALIZED_DATA)
					printf(", UNINITIALIZED_DATA");
				if (s.Characteristics & SEC_LNK_INFO)
					printf(", LNK_INFO");
				if (s.Characteristics & SEC_LNK_REMOVE)
					printf(", LNK_REMOVE");
				if (s.Characteristics & SEC_LNK_COMDAT)
					printf(", LNK_COMDAT");
				if (s.Characteristics & SEC_GPREL)
					printf(", GPREL");
				if (s.Characteristics & SEC_MEM_PURGEABLE)
					printf(", MEM_PURGEABLE");
				if (s.Characteristics & SEC_MEM_LOCKED)
					printf(", MEM_LOCKED");
				if (s.Characteristics & SEC_MEM_PRELOAD)
					printf(", MEM_PRELOAD");
				if (s.Characteristics & ALIGN_1BYTES)
					printf(", ALIGN_1BYTES");
				if (s.Characteristics & ALIGN_2BYTES)
					printf(", ALIGN_2BYTES");
				if (s.Characteristics & ALIGN_4BYTES)
					printf(", ALIGN_4BYTES");
				if (s.Characteristics & ALIGN_8BYTES)
					printf(", ALIGN_8BYTES");
				if (s.Characteristics & ALIGN_16BYTES)
					printf(", ALIGN_16BYTES");
				if (s.Characteristics & ALIGN_32BYTES)
					printf(", ALIGN_32BYTES");
				if (s.Characteristics & ALIGN_64BYTES)
					printf(", ALIGN_64BYTES");
				if (s.Characteristics & ALIGN_128BYTES)
					printf(", ALIGN_128BYTES");
				if (s.Characteristics & ALIGN_256BYTES)
					printf(", ALIGN_256BYTES");
				if (s.Characteristics & ALIGN_512BYTES)
					printf(", ALIGN_512BYTES");
				if (s.Characteristics & ALIGN_1024BYTES)
					printf(", ALIGN_1024BYTES");
				if (s.Characteristics & ALIGN_2048BYTES)
					printf(", ALIGN_2048BYTES");
				if (s.Characteristics & ALIGN_4096BYTES)
					printf(", ALIGN_4096BYTES");
				if (s.Characteristics & ALIGN_8192BYTES)
					printf(", ALIGN_8192BYTES");
				if (s.Characteristics & SEC_LNK_NRELOC_OVFL)
					printf(", LNK_NRELOC_OVFL");
				if (s.Characteristics & SEC_MEM_DISCARDABLE)
					printf(", MEM_DISCARDABLE");
				if (s.Characteristics & SEC_MEM_NOT_CACHED)
					printf(", MEM_NOT_CACHED");
				if (s.Characteristics & SEC_MEM_NOT_PAGED)
					printf(", MEM_NOT_PAGED");
				if (s.Characteristics & SEC_MEM_SHARED)
					printf(", MEM_SHARED");
				if (s.Characteristics & SEC_MEM_EXECUTE)
					printf(", MEM_EXECUTE");
				if (s.Characteristics & SEC_MEM_READ)
					printf(", MEM_READ");
				if (s.Characteristics & SEC_MEM_WRITE)
					printf(", MEM_WRITE");
				puts(">");
			} while (--peh.NumberOfSections);
		} else puts("\nNo sections");
	}

	if (setting_symbols) {
		if (peh.NumberOfSymbols && peh.PointerToSymbolTable) {
			struct PE32_SYMBOL symbol;
			int i = 0;
			_ddseek(peh.PointerToSymbolTable, SEEK_SET);
			do {
				*(uint32_t*)symbol.Name = 0; *((uint32_t*)symbol.Name + 1) = 0;
				_ddread(&symbol, sizeof(struct PE32_SYMBOL));
				printf(
					"\n%-4d (%d)  %s\n"
					"  <%08X>  Type:%d  Class:%d  Aux:%d",
					i, symbol.SectionNumber, symbol.Name,
					symbol.Value, symbol.Type, symbol.StorageClass, symbol.NumberOfAuxSymbols
				);
				++i;
			} while (--peh.NumberOfSymbols);
		} else puts("\nNo symbols");
	}
}