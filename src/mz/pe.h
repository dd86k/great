struct PE_HEADER {
	uint8_t	Signature[2]; // Padding
	uint16_t	Machine;
	uint16_t	NumberOfSections;
	uint32_t	TimeDateStamp;
	uint32_t	PointerToSymbolTable;
	uint32_t	NumberOfSymbols;
	uint16_t	SizeOfOptionalHeader;
	uint16_t	Characteristics;
};

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms680339(v=vs.85).aspx
struct PE_OPTIONAL_HEADER { // PE32 header
	// Magic removed
	uint8_t	MajorLinkerVersion;
	uint8_t	MinorLinkerVersion;
	uint32_t	SizeOfCode;
	uint32_t	SizeOfInitializedData;
	uint32_t	SizeOfUninitializedData;
	uint32_t	AddressOfEntryPoint;
	uint32_t	BaseOfCode;
	uint32_t	BaseOfData;
	uint32_t	ImageBase;
	uint32_t	SectionAlignment;
	uint32_t	FileAlignment;
	uint16_t	MajorOperatingSystemVersion;
	uint16_t	MinorOperatingSystemVersion;
	uint16_t	MajorImageVersion;
	uint16_t	MinorImageVersion;
	uint16_t	MajorSubsystemVersion;
	uint16_t	MinorSubsystemVersion;
	uint32_t	Win32VersionValue;
	uint32_t	SizeOfImage;
	uint32_t	SizeOfHeaders;
	uint32_t	CheckSum;
	uint16_t	Subsystem;
	uint16_t	DllCharacteristics;
	uint32_t	SizeOfStackReserve;
	uint32_t	SizeOfStackCommit;
	uint32_t	SizeOfHeapReserve;
	uint32_t	SizeOfHeapCommit;
	uint32_t	LoaderFlags; // Obsolete
	uint32_t	NumberOfRvaAndSizes;
};
struct PE_OPTIONAL_HEADER64 { // PE32+ header
	// Magic removed
	uint8_t	MajorLinkerVersion;
	uint8_t	MinorLinkerVersion;
	uint32_t	SizeOfCode;
	uint32_t	SizeOfInitializedData;
	uint32_t	SizeOfUninitializedData;
	uint32_t	AddressOfEntryPoint;
	uint32_t	BaseOfCode;
	uint64_t	ImageBase;
	uint32_t	SectionAlignment;
	uint32_t	FileAlignment;
	uint16_t	MajorOperatingSystemVersion;
	uint16_t	MinorOperatingSystemVersion;
	uint16_t	MajorImageVersion;
	uint16_t	MinorImageVersion;
	uint16_t	MajorSubsystemVersion;
	uint16_t	MinorSubsystemVersion;
	uint32_t	Win32VersionValue;
	uint32_t	SizeOfImage;
	uint32_t	SizeOfHeaders;
	uint32_t	CheckSum;
	uint16_t	Subsystem;
	uint16_t	DllCharacteristics;
	uint64_t	SizeOfStackReserve;
	uint64_t	SizeOfStackCommit;
	uint64_t	SizeOfHeapReserve;
	uint64_t	SizeOfHeapCommit;
	uint32_t	LoaderFlags;
	uint32_t	NumberOfRvaAndSizes;
};

// IMAGE_NUMBEROF_DIRECTORY_ENTRIES = 16
struct IMAGE_DATA_DIRECTORY {
	/*
	 * Usually, these are defined with IMAGE_DATA_DIRECTORY structs, but I
	 * dislike working with nested structs. This struct still has the
	 * Virtual Address and Size for each directory.
	 */
	uint32_t ExportTableVA;
	uint32_t ExportTableSize;
	uint32_t ImportTableVA;
	uint32_t ImportTableSize;
	uint32_t ResourceTableVA;
	uint32_t ResourceTableSize;
	uint32_t ExceptionTableVA;
	uint32_t ExceptionTableSize;
	uint32_t CertificateTableVA;
	uint32_t CertificateTableSize;
	uint32_t BaseRelocationTableVA;
	uint32_t BaseRelocationTableSize;
	uint32_t DebugDirectoryVA;
	uint32_t DebugDirectorySize;
	uint32_t ArchitectureDataVA;
	uint32_t ArchitectureDataSize;
	uint32_t GlobalPtrVA;
	uint32_t GlobalPtrSize;
	uint32_t TLSTableVA;
	uint32_t TLSTableSize;
	uint32_t LoadConfigurationTableVA;
	uint32_t LoadConfigurationTableSize;
	uint32_t BoundImportTableVA;
	uint32_t BoundImportTableSize;
	uint32_t ImportAddressTableVA;
	uint32_t ImportAddressTableSize;
	uint32_t DelayImportVA;
	uint32_t DelayImportSize;
	uint32_t CLRHeaderVA;
	uint32_t CLRHeaderSize;
	uint32_t ReservedVA;
	uint32_t ReservedSize;
};

struct PE32_SECTION {
	char Name[8];
	uint32_t VirtualSize;
	uint32_t VirtualAddress;
	uint32_t SizeOfRawData;
	uint32_t PointerToRawData;
	uint32_t PointerToRelocations;
	uint32_t PointerToLinenumbers;
	uint16_t NumberOfRelocations;
	uint16_t NumberOfLinenumbers;
	uint32_t Characteristics;
};

enum { // PE_MACHINE
	UNKNOWN = 0,
	AM33 = 0x1d3,
	AMD64 = 0x8664,
	ARM = 0x1c0,
	ARMNT = 0x1c4,
	ARM64 = 0xaa64,
	EBC = 0xebc,
	I386 = 0x14c,
	IA64 = 0x200,
	M32R = 0x9041, // LE
	MIPS16 = 0x266,
	MIPSFPU = 0x366,
	MIPSFPU16 = 0x466,
	POWERPC = 0x1f0,
	POWERPCFP = 0x1f1,
	R4000 = 0x166,
	SH3 = 0x1a2,
	SH3DSP = 0x1a3,
	SH4 = 0x1a6,
	SH5 = 0x1a8,
	THUMB = 0x1c2,
	WCEMIPSV2 = 0x169,
	CLR = 0xC0EE,
};

enum { // PE_CHARACTERISTIC
	RELOCS_STRIPPED = 0x0001,
	EXECUTABLE_IMAGE = 0x0002,
	LINE_NUMS_STRIPPED = 0x0004,
	LOCAL_SYMS_STRIPPED = 0x0008,
	AGGRESSIVE_WS_TRIM = 0x0010, // obsolete
	LARGE_ADDRESS_AWARE = 0x0020,
	_16BIT_MACHINE = 0x0040,
	BYTES_REVERSED_LO = 0x0080, // obsolete
	_32BIT_MACHINE = 0x0100,
	DEBUG_STRIPPED = 0x0200,
	REMOVABLE_RUN_FROM_SWAP = 0x0400,
	NET_RUN_FROM_SWAP = 0x0800,
	SYSTEM = 0x1000,
	DLL = 0x2000,
	UP_SYSTEM_ONLY = 0x4000,
	BYTES_REVERSED_HI = 0x8000 // obsolete
};

enum { // DllCharacteristics
	DLL_DYNAMIC_BASE = 0x0040,
	DLL_FORCE_INTEGRITY = 0x0080,
	DLL_NX_COMPAT = 0x0100,
	DLL_NO_ISOLATION = 0x0200,
	DLL_NO_SEH = 0x0400,
	DLL_NO_BIND = 0x0800,
	DLL_WDM_DRIVER = 0x2000,
	DLL_TERMINAL_SERVER_AWARE = 0x8000
};

enum { // Section flags
	SEC_NO_PAD = 0x00000008,
	SEC_CNT_CODE = 0x00000020,
	SEC_INITIALIZED_DATA = 0x00000040,
	SEC_UNINITIALIZED_DATA = 0x00000080,
	SEC_LNK_INFO = 0x00000200,
	SEC_LNK_REMOVE = 0x00000800,
	SEC_LNK_COMDAT = 0x00001000,
	SEC_GPREL = 0x00008000,
	SEC_MEM_PURGEABLE = 0x00020000,
	SEC_MEM_LOCKED = 0x00040000,
	SEC_MEM_PRELOAD = 0x00080000,
	ALIGN_1BYTES = 0x00100000,
	ALIGN_2BYTES = 0x00200000,
	ALIGN_4BYTES = 0x00300000,
	ALIGN_8BYTES = 0x00400000,
	ALIGN_16BYTES = 0x00500000,
	ALIGN_32BYTES = 0x00600000,
	ALIGN_64BYTES = 0x00700000,
	ALIGN_128BYTES = 0x00800000,
	ALIGN_256BYTES = 0x00900000,
	ALIGN_512BYTES = 0x00A00000,
	ALIGN_1024BYTES = 0x00B00000,
	ALIGN_2048BYTES = 0x00C00000,
	ALIGN_4096BYTES = 0x00D00000,
	ALIGN_8192BYTES = 0x00E00000,
	SEC_LNK_NRELOC_OVFL = 0x01000000,
	SEC_MEM_DISCARDABLE = 0x02000000,
	SEC_MEM_NOT_CACHED = 0x04000000,
	SEC_MEM_NOT_PAGED = 0x08000000,
	SEC_MEM_SHARED = 0x10000000,
	SEC_MEM_EXECUTE = 0x20000000,
	SEC_MEM_READ = 0x40000000,
	SEC_MEM_WRITE = 0x80000000
};

#define ROM   0x0107
#define HDR32 0x010B
#define HDR64 0x020B

// PE_SUBSYSTEM
#define NATIVE 1
#define WINDOWS_GUI 2
#define WINDOWS_CUI 3
#define OS2_CUI 5
#define POSIX_CUI 7
#define WINDOWS_CE_GUI 9
#define EFI_APPLICATION 10
#define EFI_BOOT_SERVICE_DRIVER 11
#define EFI_RUNTIME_DRIVER 12
#define EFI_ROM 13
#define XBOX 14
#define WINDOWS_BOOT_APPLICATION 16

void scan_pe(void);