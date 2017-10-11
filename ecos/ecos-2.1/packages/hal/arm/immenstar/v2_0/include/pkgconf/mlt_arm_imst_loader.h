// this is only for onu loader

#include <cyg/infra/cyg_type.h>
#include <stddef.h>
#include <pkgconf/system.h>

#define CYGMEM_REGION_ram       IMST_RAM_ORIGIN

#ifdef IROSBOOT
#define CYGMEM_REGION_ram_SIZE  IMST_MIN_RAM_SIZE
#else
#define CYGMEM_REGION_ram_SIZE  IMST_RAM_LEN
#endif

#define CYGMEM_REGION_ram_ATTR  (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)

#define CYGMEM_REGION_rom       IMST_ROM_ORIGIN
#define CYGMEM_REGION_rom_SIZE  IMST_ROM_LEN
#define CYGMEM_REGION_rom_ATTR  (CYGMEM_REGION_ATTR_R)






