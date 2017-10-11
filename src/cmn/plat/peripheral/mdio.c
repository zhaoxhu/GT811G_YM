/* Copyright: (c) 2007 Cortina Systems Inc. All Rights Reserved. */


#include "iros_config.h"
#include "plat_common.h"
#include "cs_types.h"
#include "mdio.h"

static cs_uint32 access_mdio;

void mdio_init(cs_uint32 mdio_Mhz)
{
    cs_uint32 mdio_cfg_val=0;
    
    cs_mutex_init((cs_uint32 *)&access_mdio,"mdio mutex",0);
    mdio_cfg_val=(32/mdio_Mhz)-1;
    CS_REG_WRITE(ARM_MDIO_CFG_REG, ((mdio_cfg_val<<16)| MDIO_MODE_MANUAL));
}

cs_status mdio_clock_set(cs_uint32 clk_Mhz)
{
    cs_uint32 mdio_cfg_val=0;
    
    if(0 == clk_Mhz)
      return CS_E_PARAM;
    
    cs_mutex_lock(access_mdio);
    mdio_cfg_val=(32/clk_Mhz)-1;
    CS_REG_WRITE(ARM_MDIO_CFG_REG, ((mdio_cfg_val<<16)| MDIO_MODE_MANUAL));
    cs_mutex_unlock(access_mdio);
    
    return CS_OK;
}

cs_status mdio_clock_get(cs_uint32 *clk_Mhz)
{
    cs_uint32  cfg;

    if(NULL == clk_Mhz)
      return CS_E_PARAM;
    
    CS_REG_READ(ARM_MDIO_CFG_REG, cfg);
    *clk_Mhz = 32/((cfg>>16) +1);

    return CS_OK;
}

int mdio_read(cs_uint8 device,cs_uint8 reg, cs_uint16 *data)
{
    cs_uint32 val32=0,i=0;
    
    cs_mutex_lock(access_mdio);
    CS_REG_WRITE(ARM_MDIO_ADDR_REG, ( MDIO_ADDR_RD | device | (reg<<8)));

    CS_REG_WRITE(ARM_MDIO_CTRL_REG, MDIO_CTRL_START);

    for(i=0; i<MDIO_TIMEOUT; i++) {
        CS_REG_READ(ARM_MDIO_CTRL_REG, val32);
        if (BITON(val32, 0))
            {
                CS_REG_READ(ARM_MDIO_RDDATA_REG, val32);
                CS_REG_WRITE(ARM_MDIO_CTRL_REG, MDIO_CTRL_DONE);
                break;
            }
    }

    cs_mutex_unlock(access_mdio);

    if(i==MDIO_TIMEOUT){
        return CS_E_TIMEOUT;
    }else{
        val32 &=0x0000ffff;   // Lower 16-bits are data
        *data = val32;
        return CS_OK;
    }

}

int mdio_write(cs_uint8 device,cs_uint8 reg,cs_uint16 data)
{
    cs_uint32 val32=0,i=0;

    cs_mutex_lock(access_mdio);
    CS_REG_WRITE(ARM_MDIO_ADDR_REG, ( MDIO_ADDR_WR | device | (reg<<8)));
    CS_REG_WRITE(ARM_MDIO_WRDATA_REG, (0x0000ffff & data));

    CS_REG_WRITE(ARM_MDIO_CTRL_REG, MDIO_CTRL_START);

    for(i=0; i<MDIO_TIMEOUT; i++) {
        CS_REG_READ(ARM_MDIO_CTRL_REG, val32);
        if (BITON(val32, 0)){
            CS_REG_WRITE(ARM_MDIO_CTRL_REG, MDIO_CTRL_DONE);
            break;
        }
    }

    cs_mutex_unlock(access_mdio);

    if(i==MDIO_TIMEOUT){
        return CS_E_TIMEOUT;
    }else{
        return CS_OK;
    }
}


