
/*=======================================================================*/
/* HEADER FILES                                                          */
/*=======================================================================*/

#include <config.h>
#include <common.h>
#include <version.h>
#include <stdarg.h>
#include <linux/types.h>
//#include <devices.h>
#include <lcd.h>
#include <video_fb.h>

//#include <asm/arch/mt6573.h>
#include <asm/arch/mt65xx_typedefs.h>
//#include <asm/arch/mt6573_pmu_hw.h>
//#include <asm/arch/mt6573_pdn_sw.h>
//#include <asm/arch/mt6573_gpio.h>
#include <asm/arch/mt65xx_disp_drv.h>
#include <asm/arch/disp_drv.h>
#include <asm/arch/lcd_drv.h>
//#include <asm/arch/mt65xx_dpi_drv.h>
#include <asm/arch/mt65xx_logo.h>

#include "lcm_drv.h"

#include <cust_display.h>
#include <u-boot/zlib.h>
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static const UINT32 VOLTAGE_SCALE = 4;

typedef struct {
    UINT32 left, top, right, bottom;
} RECT;

static RECT bar_rect = {BAR_LEFT, BAR_TOP, BAR_RIGHT, BAR_BOTTOM};

static UINT32 bar_occupied_color = BAR_OCCUPIED_COLOR;
static UINT32 bar_empty_color    = BAR_EMPTY_COLOR;
static UINT32 bar_bg_color       = BAR_BG_COLOR;

static LOGO_CUST_IF *logo_cust_if = NULL;


bool mt65xx_logo_decompress(void *in, void *out, int inlen, int outlen)
{
    int ret;
    unsigned have;
    z_stream strm;

    memset(&strm, 0, sizeof(z_stream));
    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = inlen; 
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = outlen;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = outlen - strm.avail_out;
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void mt65xx_logo_get_custom_if(void)
{
    if(logo_cust_if == NULL)
    {
        logo_cust_if = LOGO_GetCustomIF();
    }
}

#define ALIGN_TO(x, n)  \
	(((x) + ((n) - 1)) & ~((n) - 1))

static void show_logo(UINT32 index)
{
	UINT32 logonum;
    UINT32 logolen;
	UINT32 inaddr;
    void  *fb_addr = mt65xx_get_fb_addr();
    UINT32 fb_size = mt65xx_get_fb_size();
    void  *db_addr = mt65xx_get_logo_db_addr();

	unsigned int *pinfo = (unsigned int*)db_addr;
    logonum = pinfo[0];
	
	ASSERT(index < logonum);

	if(index < logonum)
		logolen = pinfo[3+index] - pinfo[2+index];
	else
		logolen = pinfo[1] - pinfo[2+index];

	inaddr = (unsigned int)db_addr+pinfo[2+index];
    printf("show_logo, in_addr=0x%08x, fb_addr=0x%08x, logolen=%d, ticks=%d\n", 
                inaddr, fb_addr, logolen, get_ticks());
//    mt65xx_logo_decompress((void*)inaddr, (void*)fb_addr + 2 * fb_size, logolen, fb_size); 
#if 1
	{
		unsigned short *d;
		int j,k;
		if(0 == strncmp(MTK_LCM_PHYSICAL_ROTATION, "270", 3))
		{
			unsigned int l;
			unsigned short *s;
			unsigned int width = CFG_DISPLAY_WIDTH;
			unsigned int height = CFG_DISPLAY_HEIGHT;
			mt65xx_logo_decompress((void*)inaddr, (void*)fb_addr + 2 * fb_size, logolen, fb_size); 
			s = fb_addr + 2 * fb_size;
			for (j=0; j<width; j++){
		  		for (k=0, l=height-1; k<height; k++, l--)
		    	{
					d = fb_addr + ((ALIGN_TO(width, 32) * l + j) << 1);
					*d = *s++;
		    	}
			}
		}
		else if(0 == strncmp(MTK_LCM_PHYSICAL_ROTATION, "90", 2))
		{
			unsigned int l;
			unsigned short *s;
			unsigned int width = CFG_DISPLAY_WIDTH;
			unsigned int height = CFG_DISPLAY_HEIGHT;
			mt65xx_logo_decompress((void*)inaddr, (void*)fb_addr + 2 * fb_size, logolen, fb_size); 
			s = fb_addr + 2 * fb_size;
			for (j=width - 1; j>=0; j--){
		  		for (k=0, l=0; k<height; k++, l++)
		    	{
					d = fb_addr + ((ALIGN_TO(width, 32) * l + j) << 1);
					*d = *s++;
		    	}
			}
		}
		else
#endif		
		{
			if(0 != CFG_DISPLAY_WIDTH % 32){
				unsigned short *s;
				unsigned short *d;
				unsigned int width = CFG_DISPLAY_WIDTH;
				unsigned int height = CFG_DISPLAY_HEIGHT;
				mt65xx_logo_decompress((void*)inaddr, (void*)fb_addr + 2 * fb_size, logolen, fb_size); 
				s = fb_addr + 2 * fb_size;
				d = fb_addr;
				for (j=0;j < height; j++){
		    		{
						memcpy(d, s, width * 2);
						d += ALIGN_TO(width, 32);
						s += width;
		    		}
				}
			}
			else{
				mt65xx_logo_decompress((void*)inaddr, (void*)fb_addr, logolen, fb_size); 
			}
		}
	}
    printf("ticks=%d\n", get_ticks());
}


void cust_show_battery_capacity(UINT32 capacity)
{
#if MTK_QVGA_LANDSCAPE_SUPPORT
//	DISP_PROG_BAR_DIRECT direct = DISP_HORIZONTAL_PROG_BAR;
#else
//	DISP_PROG_BAR_DIRECT direct = DISP_VERTICAL_PROG_BAR;
#endif
	DISP_PROG_BAR_DIRECT direct = DISP_VERTICAL_PROG_BAR;
    UINT32 capacity_grids = 0;
 
    if (capacity > 100) capacity = 100;

    capacity_grids = (capacity * VOLTAGE_SCALE) / 100;

    show_logo(1);

    // Fill Occupied Color
    
    mt65xx_disp_draw_prog_bar(direct,
                              bar_rect.left + 1, bar_rect.top + 1,
                              bar_rect.right, bar_rect.bottom,
                              bar_occupied_color, bar_bg_color,
                              0, VOLTAGE_SCALE, capacity_grids);
    
    // Fill Empty Color
    
    mt65xx_disp_draw_prog_bar(direct,
                              bar_rect.left + 1, bar_rect.top + 1,
                              bar_rect.right, bar_rect.bottom,
                              bar_empty_color, bar_bg_color,
                              capacity_grids, VOLTAGE_SCALE,
                              VOLTAGE_SCALE - capacity_grids);

    mt65xx_disp_update(0, 0, CFG_DISPLAY_WIDTH, CFG_DISPLAY_HEIGHT);
}


void mt65xx_disp_show_boot_logo(void)
{
#ifndef CFG_MT6577_FPGA
    mt65xx_logo_get_custom_if();

    if(logo_cust_if->show_boot_logo)
    {
    	logo_cust_if->show_boot_logo();
    }
    else
    {
        show_logo(0);
        mt65xx_disp_update(0, 0, CFG_DISPLAY_WIDTH, CFG_DISPLAY_HEIGHT);
    }

    return;
#endif
}

void mt65xx_disp_enter_charging_state(void)
{
    mt65xx_logo_get_custom_if();

    if(logo_cust_if->enter_charging_state)
    {
    	logo_cust_if->enter_charging_state();
    }
    else
    {

    }

    return;
}

void mt65xx_disp_show_battery_full(void)
{
    mt65xx_logo_get_custom_if();

    if(logo_cust_if->show_battery_full)
    {
    	logo_cust_if->show_battery_full();
    }
    else
    {
        cust_show_battery_capacity(100);
    }

    return;
}

void mt65xx_disp_show_battery_capacity(UINT32 capacity)
{
    mt65xx_logo_get_custom_if();

    if(logo_cust_if->show_battery_capacity)
    {
    	logo_cust_if->show_battery_capacity(capacity);
    }
    else
    {
        cust_show_battery_capacity(capacity);
    }

    return;
}

void mt65xx_disp_show_charger_ov_logo(void)
{
    mt65xx_logo_get_custom_if();

    if(logo_cust_if->show_boot_logo)
    {
    	logo_cust_if->show_boot_logo();
    }
    else
    {
        show_logo(3);
        mt65xx_disp_update(0, 0, CFG_DISPLAY_WIDTH, CFG_DISPLAY_HEIGHT);
    }

    return;
}

void mt65xx_disp_show_low_battery(void)
{
    mt65xx_logo_get_custom_if();

    if(logo_cust_if->show_boot_logo)
    {
    	logo_cust_if->show_boot_logo();
    }
    else
    {
        show_logo(2);
        mt65xx_disp_update(0, 0, CFG_DISPLAY_WIDTH, CFG_DISPLAY_HEIGHT);
    }

    return;
}

void mt65xx_disp_fill_rect(UINT32 left, UINT32 top,
                           UINT32 right, UINT32 bottom,
                           UINT32 color)
{
    void * fb_addr = mt65xx_get_fb_addr();
    const UINT32 WIDTH = ALIGN_TO(CFG_DISPLAY_WIDTH, 32);
	const UINT32 HEIGHT = CFG_DISPLAY_HEIGHT;
    const UINT16 COLOR = (UINT16)color;

    UINT16 *pLine;
    INT32 x, y;
	pLine = (UINT16 *)fb_addr + top * WIDTH + left;
#if 1
	if(0 == strncmp(MTK_LCM_PHYSICAL_ROTATION, "270", 3))
	{
		unsigned int l;
        UINT16 *d = fb_addr;
		
		for (x=top; x<bottom; x++){
	  		for (y=left, l= HEIGHT - left; y<right; y++, l--)
	    	{
				d = fb_addr + ((WIDTH * l + x) << 1);
				*d = COLOR;
	    	}
		}
	}
	else if(0 == strncmp(MTK_LCM_PHYSICAL_ROTATION, "90", 2))
	{
		unsigned int l;
        UINT16 *d = fb_addr;
		for (x=WIDTH - top + 1; x > WIDTH - bottom; x--){
			for (y=left, l=left; y<right; y++, l++)
		   	{
				d = fb_addr + ((WIDTH * l + x) << 1);
				*d = COLOR;
		   	}
		}
	}
	else
#endif
	{
    	for (y = top; y < bottom; ++ y) {
        	UINT16 *pPixel = pLine;
        	for (x = left; x < right; ++ x) {
            	*pPixel++ = COLOR;
        	}
        	pLine += WIDTH;
    	}
	}
}

void mt65xx_disp_draw_prog_bar(DISP_PROG_BAR_DIRECT direct,
                               UINT32 left, UINT32 top,
                               UINT32 right, UINT32 bottom,
                               UINT32 fgColor, UINT32 bgColor,
                               UINT32 start_div, UINT32 total_div,
                               UINT32 occupied_div)
{
    const UINT32 PADDING = 3;
    UINT32 div_size  = (bottom - top) / total_div;
    UINT32 draw_size = div_size - PADDING;
    
    UINT32 i;

    if (DISP_HORIZONTAL_PROG_BAR == direct) 
	{
		div_size = (right - left) / total_div;
		draw_size = div_size - PADDING;
    	for (i = start_div; i < start_div + occupied_div; ++ i)
    	{
			UINT32 draw_left = left + div_size * i + PADDING;
			UINT32 draw_right = draw_left + draw_size;

        	// fill one division of the progress bar
        	mt65xx_disp_fill_rect(draw_left, top, draw_right, bottom, fgColor);
		}
    }
	else if(DISP_VERTICAL_PROG_BAR == direct)
	{
		div_size  = (bottom - top) / total_div;
    	draw_size = div_size - PADDING;
 
    	for (i = start_div; i < start_div + occupied_div; ++ i)
    	{
        	UINT32 draw_bottom = bottom - div_size * i - PADDING;
        	UINT32 draw_top    = draw_bottom - draw_size;

        	// fill one division of the progress bar
        	mt65xx_disp_fill_rect(left, draw_top, right, draw_bottom, fgColor);
    	}
	}
	else
	{
		NOT_IMPLEMENTED();
	}
}
