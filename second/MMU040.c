/*
 *
 * (c) 2004,2005 Laurent Vivier <Laurent@Vivier.EU>
 *
 * a lot of parts from penguin booter
 */

#include <stdio.h>
#include <sys/types.h>

#include "console.h"

#include "MMU040.h"

#define GET_TC_ENABLE(TC)	(TC & 0x8000)
#define IS_8K_PAGE(TC)		(TC & 0x4000)

#define GET_TC_PAGE_SIZE(TC)	(IS_8K_PAGE(TC) ? 8192 : 4096)


#define GET_PDT(PDT)		(PDT&3)
#define UDT_IS_INVALID(PDT)	(GET_PDT(PDT) == 0)
#define UDT_IS_RESIDENT(PDT)	((GET_PDT(PDT) == 1) || (GET_PDT(PDT) == 3))
#define UDT_IS_INDIRECT(PDT)	(GET_PDT(PDT) == 2)

#define GET_RP_UDT(RP)		(RP & 0x00000003)
#define GET_RP_W(RP)		(RP & 0x00000004)
#define GET_RP_U(RP)		(RP & 0x00000008)
#define GET_RP_ADDR(RP)		(RP & 0xFFFFFE00)

#define GET_TD_4K_ADDR(TD)	(TD & 0xFFFFFF00)
#define GET_TD_8K_ADDR(TD)	(TD & 0xFFFFFF80)

#define GET_TT_ENABLE(TT)	(TT & 0x8000)
#define GET_TT_BASE(TT)		( (TT >> 24) & 0xFF )
#define GET_TT_MASK(TT)		( (TT >> 16) & 0xFF )

#ifdef TRACE_MMU
#define TRACE(format, args...)  if (MMU_trace) printf(format, ##args)
static int MMU_trace = 0;
void MMU040_set_trace(int enable)
{
	MMU_trace = enable;
}
#else
#define TRACE(format, args...)
#endif


static int isTTSegment(unsigned long addr)
{
	unsigned long DTT0;
	unsigned long DTT1;
	unsigned long base;
	unsigned long mask;
	unsigned long size;

	addr >>= 24;

	MMU040_get_DTT0(&DTT0);

	if (GET_TT_ENABLE(DTT0))
	{
		mask = GET_TT_MASK(DTT0);
		base = GET_TT_BASE(DTT0);

		base &= ~mask;
		addr &= ~mask;
		size = mask & 0x000000FF;

		if ( (base <= addr) && (addr <= base + size) )
			return 1;
	}

	MMU040_get_DTT1(&DTT1);

	if (GET_TT_ENABLE(DTT1))
	{
		mask = GET_TT_MASK(DTT1);
		base = GET_TT_BASE(DTT1);

		base &= ~mask;
		addr &= ~mask;
		size = mask & 0x000000FF;

		if ( (base <= addr) && (addr <= base + size) )
			return 1;
	}

	/* if come here : no Transparent Translation */

	return 0;
}

int MMU040_logical2physicalAttr(unsigned long logicalAddr, unsigned long *physicalAddr, unsigned long *attr)
{
	unsigned long TC;
	int rootIndex, ptrIndex, pageIndex;
	unsigned long pageOffst;
	unsigned long rootTable, ptrTable, pageTable;
	unsigned long rootEntry, tableEntry, pageEntry;

	TRACE("logical: %08lx ", logicalAddr);

	MMU040_get_TC(&TC);
	
	TRACE("TC: %08lx\n", TC);

	if ( !GET_TC_ENABLE(TC) || isTTSegment(logicalAddr) )
	{
		*physicalAddr = logicalAddr;
		return 0;
	}

	MMU040_get_SRP(&rootTable);
	TRACE("SRP: %ld\n", rootTable);

	rootIndex = (logicalAddr & 0xFE000000) >> 25;
	rootEntry = MMU040_read_phys(rootTable + 4 * rootIndex);
	TRACE("Root Entry: %08lx\n", rootEntry);

	if ( UDT_IS_INVALID(rootEntry) || UDT_IS_RESIDENT(rootEntry) )
	{
		*physicalAddr = logicalAddr;
		return 0;
	}

	ptrTable = GET_RP_ADDR(rootEntry);
	ptrIndex  = (logicalAddr & 0x01FC0000) >> 18;
	tableEntry = MMU040_read_phys(ptrTable + 4 * ptrIndex);
	TRACE("table Entry: %08lx\n", tableEntry);

	if ( UDT_IS_INVALID(tableEntry) || UDT_IS_RESIDENT(tableEntry) )
	{
		*physicalAddr = logicalAddr;
		return 0;
	}

	if (IS_8K_PAGE(TC))
	{
		pageTable = GET_TD_8K_ADDR(tableEntry);
		pageIndex = (logicalAddr & 0x0003E000) >> 13;

		pageEntry = MMU040_read_phys(pageTable + 4 * pageIndex);
		pageOffst    = logicalAddr & 0x00001FFF;

		*physicalAddr = (pageEntry & 0xFFFFE000) + pageOffst;
	}
	else
	{
		pageTable = GET_TD_4K_ADDR(tableEntry);
		pageIndex = (logicalAddr & 0x0003F000) >> 12;

		pageEntry = MMU040_read_phys(pageTable + 4 * pageIndex);
		pageOffst    = logicalAddr & 0x00000FFF;

		*physicalAddr = (pageEntry & 0xFFFFF000) + pageOffst;
	}

	*attr = pageEntry & 0x000004FF;

	TRACE("physical: %08lx\n", *physicalAddr);

	return 0;
}

int MMU040_logical2physical(unsigned long logicalAddr, unsigned long *physicalAddr)
{
	unsigned long attr;

	return MMU040_logical2physicalAttr(logicalAddr, physicalAddr, &attr);
}

unsigned long MMU040_get_page_size(void)
{
	unsigned long TC;

	MMU040_get_TC(&TC);

	TRACE("Page Size: %d\n", GET_TC_PAGE_SIZE(TC));

	return GET_TC_PAGE_SIZE(TC);
}
