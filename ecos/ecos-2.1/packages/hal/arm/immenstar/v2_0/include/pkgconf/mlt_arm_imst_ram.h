#include <cyg/infra/cyg_type.h>
#include <stddef.h>
#include <pkgconf/system.h>


#define CYGMEM_REGION_ram0      IMST_RAM_ORIGIN
#define CYGMEM_REGION_ram0_SIZE IMST_RAM_LEN
#define CYGMEM_REGION_ram0_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)


#define CYGMEM_REGION_ram1      IMST_RAM_APP_START
#define CYGMEM_REGION_ram1_SIZE IMST_RAM_APP_LEN
#define CYGMEM_REGION_ram1_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)




