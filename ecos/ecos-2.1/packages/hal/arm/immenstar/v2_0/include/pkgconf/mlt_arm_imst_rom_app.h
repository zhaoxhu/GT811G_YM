#include <cyg/infra/cyg_type.h>
#include <stddef.h>
#include <pkgconf/system.h>


#define CYGMEM_REGION_ram0      IMST_RAM_ORIGIN
#define CYGMEM_REGION_ram0_SIZE IMST_RAM_LEN
#define CYGMEM_REGION_ram0_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)

// ram1 is rom/flash
#define CYGMEM_REGION_ram1      IMST_RAM_APP_START
#define CYGMEM_REGION_ram1_SIZE IMST_RAM_APP_LEN
#define CYGMEM_REGION_ram1_ATTR (CYGMEM_REGION_ATTR_R)

// ram2 is external ram memory
#define CYGMEM_REGION_ram2      IMST_RAM_EXT_START
#define CYGMEM_REGION_ram2_SIZE IMST_RAM_EXT_LEN
#define CYGMEM_REGION_ram2_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)



