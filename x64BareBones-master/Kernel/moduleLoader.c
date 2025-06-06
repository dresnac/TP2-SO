#include <stdint.h>
#include <shared_libc.h>
#include <moduleLoader.h>

static void load_module ( uint8_t ** module, void * target_module_address );
static uint32_t read_uint32 ( uint8_t ** address );

void loadModules ( void * payload_start, void ** target_module_address )
{
	int i;
	uint8_t * current_module = ( uint8_t * ) payload_start;
	uint32_t module_count = read_uint32 ( &current_module );

	for ( i = 0; i < module_count; i++ )
		load_module ( &current_module, target_module_address[i] );
}

static void load_module ( uint8_t ** module, void * target_module_address )
{
	uint32_t module_size = read_uint32 ( module );
	sharedLibcMemcpy ( target_module_address, *module, module_size );
	*module += module_size;

}

static uint32_t read_uint32 ( uint8_t ** address )
{
	uint32_t result = * ( uint32_t * ) ( *address );
	*address += sizeof ( uint32_t );
	return result;
}














// #include <stdint.h>
// #include <lib.h>
// #include <moduleLoader.h>
// #include <naiveConsole.h>

// static void loadModule(uint8_t ** module, void * targetModuleAddress);
// static uint32_t readUint32(uint8_t ** address);

// void loadModules(void * payloadStart, void ** targetModuleAddress)
// {
// 	int i;
// 	uint8_t * currentModule = (uint8_t*)payloadStart;
// 	uint32_t moduleCount = readUint32(&currentModule);

// 	for (i = 0; i < moduleCount; i++)
// 		loadModule(&currentModule, targetModuleAddress[i]);
// }

// static void loadModule(uint8_t ** module, void * targetModuleAddress)
// {
// 	uint32_t moduleSize = readUint32(module);

// 	ncPrint("  Will copy module at 0x");
// 	ncPrintHex((uint64_t)*module);
// 	ncPrint(" to 0x");
// 	ncPrintHex((uint64_t)targetModuleAddress);
// 	ncPrint(" (");
// 	ncPrintDec(moduleSize);
// 	ncPrint(" bytes)");

// 	memcpy(targetModuleAddress, *module, moduleSize);
// 	*module += moduleSize;

// 	ncPrint(" [Done]");
// 	ncNewline();
// }

// static uint32_t readUint32(uint8_t ** address)
// {
// 	uint32_t result = *(uint32_t*)(*address);
// 	*address += sizeof(uint32_t);
// 	return result;
// }
