#ifndef CYGONCE_HAL_CACHE_H
#define CYGONCE_HAL_CACHE_H

//=============================================================================
//
//      hal_cache.h
//
//      HAL cache control API
//
//=============================================================================

#if defined(IMST_USE_CACHE)

#include <cyg/infra/cyg_type.h>
//#include <cyg/hal/hal_mmu.h>


//-----------------------------------------------------------------------------
// Cache dimensions

#if defined(CYGPKG_HAL_ARM_ARM9_ARM946ES)
# define HAL_ICACHE_SIZE                 0x4000
# define HAL_ICACHE_LINE_SIZE            16
# define HAL_ICACHE_WAYS                 2
# define HAL_ICACHE_SETS (HAL_ICACHE_SIZE/(HAL_ICACHE_LINE_SIZE*HAL_ICACHE_WAYS))

# define HAL_DCACHE_SIZE                 0x2000
# define HAL_DCACHE_LINE_SIZE            16
# define HAL_DCACHE_WAYS                 2
# define HAL_DCACHE_SETS (HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))

# define HAL_WRITE_BUFFER                64
# define CYGHWR_HAL_ARM_ARM9_CLEAN_DCACHE // has instruction to clean D-cache
#endif

// FIXME: much of the code below should make better use of
// the definitions from hal_mmu.h

//-----------------------------------------------------------------------------
// Global control of Instruction cache

#if HAL_ICACHE_SIZE != 0
// FIXME: disable/enable instruction streaming?

// Program MPU
#define HAL_MEM_REGION_PROGRAM(_region_, _base_, _size_, _protection_)              \
CYG_MACRO_START                                                         \
    asm volatile (                                                      \
        "mrc  p15,0,r1,c1,c0,0;"                                        \
        "orr  r1,r1,#0x1000;"                                           \
        "orr  r1,r1,#0x0003;" /* enable ICache (also ensures   */       \
                              /* that MMU and alignment faults */       \
                              /* are enabled)                  */       \
        "mcr  p15,0,r1,c1,c0,0"                                         \
        :                                                               \
        :                                                               \
        : "r1" /* Clobber list */                                       \
        );                                                              \
CYG_MACRO_END

// Enable the instruction cache
#define HAL_ICACHE_ENABLE()                                             \
CYG_MACRO_START                                                         \
    asm volatile (                                                      \
        "mrc  p15,0,r1,c1,c0,0;"                                        \
        "orr  r1,r1,#0x1000;"                                           \
        "orr  r1,r1,#0x0003;" /* enable ICache (also ensures   */       \
                              /* that MMU and alignment faults */       \
                              /* are enabled)                  */       \
        "mcr  p15,0,r1,c1,c0,0"                                         \
        :                                                               \
        :                                                               \
        : "r1" /* Clobber list */                                       \
        );                                                              \
CYG_MACRO_END

// Disable the instruction cache (and invalidate it, required semanitcs)
#define HAL_ICACHE_DISABLE()                                            \
CYG_MACRO_START                                                         \
    asm volatile (                                                      \
        "mrc    p15,0,r1,c1,c0,0;"                                      \
        "bic    r1,r1,#0x1000;" /* disable ICache (but not MMU, etc) */ \
        "mcr    p15,0,r1,c1,c0,0;"                                      \
        "mov    r1,#0;"                                                 \
        "mcr    p15,0,r1,c7,c5,0;"  /* flush ICache */                  \
        "nop;" /* next few instructions may be via cache    */          \
        "nop;"                                                          \
        "nop;"                                                          \
        "nop;"                                                          \
        "nop;"                                                          \
        "nop"                                                           \
        :                                                               \
        :                                                               \
        : "r1" /* Clobber list */                                       \
        );                                                              \
CYG_MACRO_END

// Query the state of the instruction cache
#define HAL_ICACHE_IS_ENABLED(_state_)                                   \
CYG_MACRO_START                                                          \
    register cyg_uint32 reg;                                             \
    asm volatile ("mrc  p15,0,%0,c1,c0,0"                                \
                  : "=r"(reg)                                            \
                  :                                                      \
        );                                                               \
    (_state_) = (0 != (0x1000 & reg)); /* Bit 12 is ICache enable */     \
CYG_MACRO_END

// Invalidate the entire cache
#define HAL_ICACHE_INVALIDATE_ALL()                                     \
CYG_MACRO_START                                                         \
    /* this macro can discard dirty cache lines (N/A for ICache) */     \
    asm volatile (                                                      \
        "mov    r1,#0;"                                                 \
        "mcr    p15,0,r1,c7,c5,0;"  /* flush ICache */                  \
        "mcr    p15,0,r1,c8,c5,0;"  /* flush ITLB only */               \
        "nop;" /* next few instructions may be via cache    */          \
        "nop;"                                                          \
        "nop;"                                                          \
        "nop;"                                                          \
        "nop;"                                                          \
        "nop;"                                                          \
        :                                                               \
        :                                                               \
        : "r1" /* Clobber list */                                       \
        );                                                              \
CYG_MACRO_END

// Synchronize the contents of the cache with memory.
// (which includes flushing out pending writes)
#define HAL_ICACHE_SYNC()                                       \
CYG_MACRO_START                                                 \
    HAL_DCACHE_SYNC(); /* ensure data gets to RAM */            \
    HAL_ICACHE_INVALIDATE_ALL(); /* forget all we know */       \
CYG_MACRO_END

#else

#define HAL_ICACHE_ENABLE()
#define HAL_ICACHE_DISABLE()
#define HAL_ICACHE_IS_ENABLED(_state_) ((_state_) = 0)
#define HAL_ICACHE_INVALIDATE_ALL()
#define HAL_ICACHE_SYNC()

#endif

// Set the instruction cache refill burst size
//#define HAL_ICACHE_BURST_SIZE(_size_)

// Load the contents of the given address range into the instruction cache
// and then lock the cache so that it stays there.
//#define HAL_ICACHE_LOCK(_base_, _size_)

// Undo a previous lock operation
//#define HAL_ICACHE_UNLOCK(_base_, _size_)

// Unlock entire cache
//#define HAL_ICACHE_UNLOCK_ALL()

//-----------------------------------------------------------------------------
// Instruction cache line control

// Invalidate cache lines in the given range without writing to memory.
//#define HAL_ICACHE_INVALIDATE( _base_ , _size_ )

//-----------------------------------------------------------------------------
// Global control of data cache

#if HAL_DCACHE_SIZE != 0

// Enable the data cache
#define HAL_DCACHE_ENABLE()                                             \
CYG_MACRO_START                                                         \
    asm volatile (                                                      \
        "mrc  p15,0,r1,c1,c0,0;"                                        \
        "orr  r1,r1,#0x000F;" /* enable DCache (also ensures    */      \
                              /* the MMU, alignment faults, and */      \
                              /* write buffer are enabled)      */      \
        "mcr  p15,0,r1,c1,c0,0"                                         \
        :                                                               \
        :                                                               \
        : "r1" /* Clobber list */                                       \
        );                                                              \
CYG_MACRO_END

// Disable the data cache (and invalidate it, required semanitcs)
#define HAL_DCACHE_DISABLE()                                            \
CYG_MACRO_START                                                         \
    asm volatile (                                                      \
        "mrc  p15,0,r1,c1,c0,0;"                                        \
        "bic  r1,r1,#0x000C;" /* disable DCache AND write buffer  */    \
                              /* but not MMU and alignment faults */    \
        "mcr  p15,0,r1,c1,c0,0;"                                        \
        "mov    r1,#0;"                                                 \
        "mcr  p15,0,r1,c7,c6,0" /* clear data cache */                  \
        :                                                               \
        :                                                               \
        : "r1" /* Clobber list */                                       \
        );                                                              \
CYG_MACRO_END

// Query the state of the data cache
#define HAL_DCACHE_IS_ENABLED(_state_)                                   \
CYG_MACRO_START                                                          \
    register int reg;                                                    \
    asm volatile ("mrc  p15,0,%0,c1,c0,0;"                               \
                  : "=r"(reg)                                            \
                  :                                                      \
        );                                                               \
    (_state_) = (0 != (4 & reg)); /* Bit 2 is DCache enable */           \
CYG_MACRO_END

// Flush the entire dcache (and then both TLBs, just in case)
#define HAL_DCACHE_INVALIDATE_ALL()                                     \
CYG_MACRO_START  /* this macro can discard dirty cache lines. */        \
    asm volatile (                                                      \
	"mov    r0,#0;"                                                 \
        "mcr    p15,0,r0,c7,c6,0;" /* flush d-cache */                  \
        "mcr    p15,0,r0,c8,c7,0;" /* flush i+d-TLBs */                 \
        :                                                               \
        :                                                               \
        : "r0","memory" /* clobber list */);                            \
CYG_MACRO_END

// Synchronize the contents of the cache with memory.
#ifdef CYGHWR_HAL_ARM_ARM9_CLEAN_DCACHE
#define HAL_DCACHE_SYNC()                                               \
CYG_MACRO_START                                                         \
    asm volatile (                                                      \
        "mov    r0, #0;"                                                \
        "mcr    p15,0,r0,c7,c10,0;"  /* clean DCache */                 \
        "1: mrc p15,0,r0,c15,c4,0;"  /* wait for dirty flag to clear */ \
        "ands   r0,r0,#0x80000000;"                                     \
        "bne    1b;"                                                    \
        "mov    r0,#0;"                                                 \
        "mcr    p15,0,r0,c7,c6,0;"  /* flush DCache */                  \
        "mcr    p15,0,r0,c7,c10,4;" /* and drain the write buffer */    \
        :                                                               \
        :                                                               \
        : "r0" /* Clobber list */                                       \
        );                                                              \
CYG_MACRO_END
#elif defined(CYGHWR_HAL_ARM_ARM9_CLEAN_DCACHE_INDEX)
#define HAL_DCACHE_SYNC()                                               \
CYG_MACRO_START                                                         \
    cyg_uint32 _tmp1, _tmp2;                                            \
    asm volatile (                                                      \
        "mov    %0, #0;"                                                \
        "1: "                                                           \
        "mov    %1, #0;"                                                \
        "2: "                                                           \
        "orr    r0,%0,%1;"                                              \
        "mcr    p15,0,r0,c7,c14,2;"  /* clean index in DCache */        \
        "add    %1,%1,%2;"                                              \
        "cmp    %1,%3;"                                                 \
        "bne    2b;"                                                    \
        "add    %0,%0,#0x04000000;"  /* get to next index */            \
        "cmp    %0,#0;"                                                 \
        "bne    1b;"                                                    \
        "mcr    p15,0,r0,c7,c10,4;" /* drain the write buffer */        \
        : "=r" (_tmp1), "=r" (_tmp2)                                    \
        : "I" (CYGHWR_HAL_ARM_ARM9_CLEAN_DCACHE_INDEX_STEP),            \
          "I" (CYGHWR_HAL_ARM_ARM9_CLEAN_DCACHE_INDEX_LIMIT)            \
        : "r0" /* Clobber list */                                       \
        );                                                              \
CYG_MACRO_END
#else
# error "Don't know how to sync Dcache"
#endif

#else

#define HAL_DCACHE_ENABLE()
#define HAL_DCACHE_DISABLE()
#define HAL_DCACHE_IS_ENABLED(_state_) ((_state_) = 0)
#define HAL_DCACHE_INVALIDATE_ALL()
#define HAL_DCACHE_SYNC()

#endif


// Set the data cache refill burst size
//#define HAL_DCACHE_BURST_SIZE(_size_)

// Set the data cache write mode
//#define HAL_DCACHE_WRITE_MODE( _mode_ )

#define HAL_DCACHE_WRITETHRU_MODE       0
#define HAL_DCACHE_WRITEBACK_MODE       1

// Get the current writeback mode - or only writeback mode if fixed
#define HAL_DCACHE_QUERY_WRITE_MODE( _mode_ ) CYG_MACRO_START           \
    _mode_ = HAL_DCACHE_WRITEBACK_MODE;                                 \
CYG_MACRO_END

// Load the contents of the given address range into the data cache
// and then lock the cache so that it stays there.
//#define HAL_DCACHE_LOCK(_base_, _size_)

// Undo a previous lock operation
//#define HAL_DCACHE_UNLOCK(_base_, _size_)

// Unlock entire cache
//#define HAL_DCACHE_UNLOCK_ALL()

//-----------------------------------------------------------------------------
// Data cache line control

// Allocate cache lines for the given address range without reading its
// contents from memory.
//#define HAL_DCACHE_ALLOCATE( _base_ , _size_ )

// Write dirty cache lines to memory and invalidate the cache entries
// for the given address range.
// ---- this seems not to work despite the documentation ---
//#define HAL_DCACHE_FLUSH( _base_ , _size_ )
//CYG_MACRO_START
//    HAL_DCACHE_STORE( _base_ , _size_ );
//    HAL_DCACHE_INVALIDATE( _base_ , _size_ );
//CYG_MACRO_END

// Invalidate cache lines in the given range without writing to memory.
// ---- this seems not to work despite the documentation ---
//#define HAL_DCACHE_INVALIDATE( _base_ , _size_ )
//CYG_MACRO_START
//    register int addr, enda;
//    for ( addr = (~(HAL_DCACHE_LINE_SIZE - 1)) & (int)(_base_),
//              enda = (int)(_base_) + (_size_);
//          addr < enda ;
//          addr += HAL_DCACHE_LINE_SIZE )
//    {
//        asm volatile (
//                      "mcr  p15,0,%0,c7,c6,1;" /* flush entry away */
//                      :
//                      : "r"(addr)
//                      : "memory"
//            );
//    }
//CYG_MACRO_END
                          
// Write dirty cache lines to memory for the given address range.
// ---- this seems not to work despite the documentation ---
//#define HAL_DCACHE_STORE( _base_ , _size_ )
//CYG_MACRO_START
//    register int addr, enda;
//    for ( addr = (~(HAL_DCACHE_LINE_SIZE - 1)) & (int)(_base_),
//              enda = (int)(_base_) + (_size_);
//          addr < enda ;
//          addr += HAL_DCACHE_LINE_SIZE )
//    {
//        asm volatile ("mcr  p15,0,%0,c7,c10,1;" /* push entry to RAM */
//                      :
//                      : "r"(addr)
//                      : "memory"
//            );
//    }
//CYG_MACRO_END

// Preread the given range into the cache with the intention of reading
// from it later.
//#define HAL_DCACHE_READ_HINT( _base_ , _size_ )

// Preread the given range into the cache with the intention of writing
// to it later.
//#define HAL_DCACHE_WRITE_HINT( _base_ , _size_ )

// Allocate and zero the cache lines associated with the given range.
//#define HAL_DCACHE_ZERO( _base_ , _size_ )

//-----------------------------------------------------------------------------
// Cache controls for safely disabling/reenabling caches around execution
// of relocated code.

#define HAL_FLASH_CACHES_OFF(_d_, _i_)          \
    CYG_MACRO_START                             \
    HAL_ICACHE_IS_ENABLED(_i_);                 \
    HAL_DCACHE_IS_ENABLED(_d_);                 \
    HAL_ICACHE_INVALIDATE_ALL();                \
    HAL_ICACHE_DISABLE();                       \
    HAL_DCACHE_SYNC();                          \
    HAL_DCACHE_INVALIDATE_ALL();                \
    HAL_DCACHE_DISABLE();                       \
    CYG_MACRO_END

#define HAL_FLASH_CACHES_ON(_d_, _i_)           \
    CYG_MACRO_START                             \
    if (_d_) HAL_DCACHE_ENABLE();               \
    if (_i_) HAL_ICACHE_ENABLE();               \
    CYG_MACRO_END

//-----------------------------------------------------------------------------
// Virtual<->Physical translations
#ifndef HAL_VIRT_TO_PHYS_ADDRESS
extern cyg_uint32 hal_virt_to_phys_address(cyg_uint32 phys);
#define HAL_VIRT_TO_PHYS_ADDRESS(_va, _pa) \
   _pa = hal_virt_to_phys_address(_va)
#endif
//-----------------------------------------------------------------------------


#else /* USE_CACHE */

#include <cyg/infra/cyg_type.h>

//-----------------------------------------------------------------------------
// Cache dimensions

// Data cache
#define HAL_DCACHE_SIZE                 0    // Size of data cache in bytes
#define HAL_DCACHE_LINE_SIZE            0    // Size of a data cache line
#define HAL_DCACHE_WAYS                 0    // Associativity of the cache

// Instruction cache
#define HAL_ICACHE_SIZE                 0    // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            0    // Size of a cache line
#define HAL_ICACHE_WAYS                 0    // Associativity of the cache

#define HAL_DCACHE_SETS (HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))
#define HAL_ICACHE_SETS (HAL_ICACHE_SIZE/(HAL_ICACHE_LINE_SIZE*HAL_ICACHE_WAYS))

//-----------------------------------------------------------------------------
// Global control of data cache

// Enable the data cache
#define HAL_DCACHE_ENABLE()

// Disable the data cache
#define HAL_DCACHE_DISABLE()

// Invalidate the entire cache
#define HAL_DCACHE_INVALIDATE_ALL()

// Synchronize the contents of the cache with memory.
#define HAL_DCACHE_SYNC()

// Query the state of the data cache
#define HAL_DCACHE_IS_ENABLED(_state_)	0

// Purge contents of data cache
#define HAL_DCACHE_PURGE_ALL()

// Set the data cache refill burst size
//#define HAL_DCACHE_BURST_SIZE(_size_)

// Set the data cache write mode
//#define HAL_DCACHE_WRITE_MODE( _mode_ )

//#define HAL_DCACHE_WRITETHRU_MODE       0
//#define HAL_DCACHE_WRITEBACK_MODE       1

// Load the contents of the given address range into the data cache
// and then lock the cache so that it stays there.
//#define HAL_DCACHE_LOCK(_base_, _size_)

// Undo a previous lock operation
//#define HAL_DCACHE_UNLOCK(_base_, _size_)

// Unlock entire cache
//#define HAL_DCACHE_UNLOCK_ALL()

//-----------------------------------------------------------------------------
// Data cache line control

// Allocate cache lines for the given address range without reading its
// contents from memory.
//#define HAL_DCACHE_ALLOCATE( _base_ , _size_ )

// Write dirty cache lines to memory and invalidate the cache entries
// for the given address range.
//#define HAL_DCACHE_FLUSH( _base_ , _size_ )

// Invalidate cache lines in the given range without writing to memory.
//#define HAL_DCACHE_INVALIDATE( _base_ , _size_ )

// Write dirty cache lines to memory for the given address range.
//#define HAL_DCACHE_STORE( _base_ , _size_ )

// Preread the given range into the cache with the intention of reading
// from it later.
//#define HAL_DCACHE_READ_HINT( _base_ , _size_ )

// Preread the given range into the cache with the intention of writing
// to it later.
//#define HAL_DCACHE_WRITE_HINT( _base_ , _size_ )

// Allocate and zero the cache lines associated with the given range.
//#define HAL_DCACHE_ZERO( _base_ , _size_ )

//-----------------------------------------------------------------------------
// Global control of Instruction cache

// Enable the instruction cache
#define HAL_ICACHE_ENABLE()

// Disable the instruction cache
#define HAL_ICACHE_DISABLE()

// Invalidate the entire cache
#define HAL_ICACHE_INVALIDATE_ALL()

// Synchronize the contents of the cache with memory.
#define HAL_ICACHE_SYNC()

// Set the instruction cache refill burst size
//#define HAL_ICACHE_BURST_SIZE(_size_)

// Load the contents of the given address range into the instruction cache
// and then lock the cache so that it stays there.
//#define HAL_ICACHE_LOCK(_base_, _size_)

// Undo a previous lock operation
//#define HAL_ICACHE_UNLOCK(_base_, _size_)

// Unlock entire cache
//#define HAL_ICACHE_UNLOCK_ALL()

//-----------------------------------------------------------------------------
// Instruction cache line control

// Invalidate cache lines in the given range without writing to memory.
//#define HAL_ICACHE_INVALIDATE( _base_ , _size_ )


#endif /* USE_CACHE */


#endif // ifndef CYGONCE_HAL_CACHE_H
// End of hal_cache.h
