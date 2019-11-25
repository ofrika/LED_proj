/*
 * Copyright (C) 2018 Xilinx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

// ################################################## XILINX CODE START ##################################################

#include <stdio.h>
#include "xparameters.h"
#include "netif/xadapter.h"
#include "platform.h"
#include "platform_config.h"
#include "lwipopts.h"
#include "xil_printf.h"
#include "sleep.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/init.h"
#include "lwip/inet.h"
#include "Display.h"
#include "logoR0.h"
#include "logoB0.h"
#include "logoG0.h"
#include "xtime_l.h"

#define LWIP_IPV6 0
#if LWIP_IPV6==1
#include "lwip/ip6_addr.h"
#include "lwip/ip6.h"
#else

#if LWIP_DHCP==1
#include "lwip/dhcp.h"
extern volatile int dhcp_timoutcntr;
#endif

#define DEFAULT_IP_ADDRESS	"132.68.61.6"
#define DEFAULT_IP_MASK		"255.255.255.128"
#define DEFAULT_GW_ADDRESS	"132.68.61.126"
#endif /* LWIP_IPV6 */

extern volatile int TcpFastTmrFlag;
extern volatile int TcpSlowTmrFlag;

void platform_enable_interrupts(void);
void start_application(void);
void transfer_data(void);
void print_app_header(void);

#if defined (__arm__) && !defined (ARMR5)
#if XPAR_GIGE_PCS_PMA_SGMII_CORE_PRESENT == 1 || \
		 XPAR_GIGE_PCS_PMA_1000BASEX_CORE_PRESENT == 1
int ProgramSi5324(void);
int ProgramSfpPhy(void);
#endif
#endif

#ifdef XPS_BOARD_ZCU102
#ifdef XPAR_XIICPS_0_DEVICE_ID
int IicPhyReset(void);
#endif
#endif

struct netif server_netif;

#if LWIP_IPV6==1
static void print_ipv6(char *msg, ip_addr_t *ip)
{
	print(msg);
	xil_printf(" %s\n\r", inet6_ntoa(*ip));
}
#else

static void print_ip(char *msg, ip_addr_t *ip)
{
	print(msg);
	xil_printf("%d.%d.%d.%d\r\n", ip4_addr1(ip), ip4_addr2(ip),
			ip4_addr3(ip), ip4_addr4(ip));
}

static void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{
	print_ip("Board IP:       ", ip);
	print_ip("Netmask :       ", mask);
	print_ip("Gateway :       ", gw);
}

static void assign_default_ip(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{
	int err;

	xil_printf("Configuring default IP %s \r\n", DEFAULT_IP_ADDRESS);

	err = inet_aton(DEFAULT_IP_ADDRESS, ip);
	if (!err)
		xil_printf("Invalid default IP address: %d\r\n", err);

	err = inet_aton(DEFAULT_IP_MASK, mask);
	if (!err)
		xil_printf("Invalid default IP MASK: %d\r\n", err);

	err = inet_aton(DEFAULT_GW_ADDRESS, gw);
	if (!err)
		xil_printf("Invalid default gateway address: %d\r\n", err);
}
#endif /* LWIP_IPV6 */

// ################################################## XILINX CODE END ##################################################




// ############################################## OFRI & SAMAH CODE START ##############################################

unsigned char* port44 = (unsigned char *) 0x46000000;

#define N 32


void One_Mat_RGB(unsigned char* port,int matrix_shift)
{
	unsigned char pixel[4];
	int i,j;
	for (j = 0; j<32; j++)
	{
		for (i = 0; i<32 ; i++)
		{
			pixel[3] = 0;	// r
			pixel[2] = 150;	// g
			pixel[1] = 150;	// b

			if(i==0 && j==0 && matrix_shift ==7){
				pixel[3] = 255;	// r
				pixel[2] = 0;	// g
				pixel[1] = 0;	// b
			}

			Xil_Out32((u32) (port + j*1024 + (i+N*matrix_shift)*4) ,*((int*)pixel));
		}
	}
	xil_printf("matrix shift %d end\r\n",matrix_shift);
}
/*
void Test_Running_Pixel()
{
	Draw_Picture();
	unsigned char running_pixel[4];
	running_pixel[3] = 255;	// r
	running_pixel[2] = 0;	// g
	running_pixel[1] = 0;	// b
	unsigned char bg_pixel[4];
	bg_pixel[3] = 0;	// r
	bg_pixel[2] = 150;	// g
	bg_pixel[1] = 150;	// b
	int i,j, prevI=31, prevJ=31;
	for (j = 31; j>=0; j--)
	{
		if (j !=31 )
			prevJ = j+1;
		for (i = 31; i>=0 ; i--)
		{
			if (i !=31 )
				prevI = i+1;
			Xil_Out32((u32) (port44 + j*1024 + (i+N*6)*4) ,*((int*)running_pixel));
			Xil_Out32((u32) (port44 + prevJ*1024 + (prevI+N*6)*4) ,*((int*)bg_pixel));
			swapBuffer();
			sleep(1);
		}
		prevI = 31;
	}
}
*/

void Draw_Picture()
{
	int i;
	xil_printf("Draw picture start\r\n");
	/*for (i = 7; i>=0 ; i--){one_matrgb(port1,i);}
	 	xil_printf(" painting port 1 done\r\n");
		for (i = 7; i>=0 ; i--){one_matrgb(port2,i);}
		 xil_printf(" painting port 2 done\r\n");
		 for (i = 7; i>=0;  i--){one_matrgb(port3,i);}
		 xil_printf(" painting port 3 done\r\n");
*/
	for (i = 7; i>=6 ; i--)
	{
		One_Mat_RGB(port44,i);
	}

//	for (i = 0; i<=100; i++)
//	{
//		xil_printf(" painting port 4 done\r\n");
//		Xil_Out32((u32) (port4 + (8191-i)*4) ,*((int*)pixel));
//		swapBuffer();
//		sleep(1);
//	}

	swapBuffer();
	xil_printf(" buffer swap triggered\r\n");
}

// ############################################### OFRI & SAMAH CODE END ###############################################





// ################################################## XILINX CODE START ##################################################

int main(void)
{
	struct ip4_addr ipaddr, netmask, gw;
	struct netif *netif;

	// The MAC address of the board. this should be unique per board
	unsigned char mac_ethernet_address[] = {
		0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	netif = &server_netif;
	#if defined (__arm__) && !defined (ARMR5)
		#if XPAR_GIGE_PCS_PMA_SGMII_CORE_PRESENT == 1 || \
			XPAR_GIGE_PCS_PMA_1000BASEX_CORE_PRESENT == 1
			ProgramSi5324();
			ProgramSfpPhy();
		#endif
	#endif

	// Define this board specific macro in order perform PHY reset on ZCU102
	#ifdef XPS_BOARD_ZCU102
			IicPhyReset();
	#endif

	init_platform();
	IP4_ADDR(&ipaddr,  132, 68,   61,   6);
	IP4_ADDR(&netmask, 255, 255, 255, 128);
	IP4_ADDR(&gw,      132, 68,   61, 126);

	xil_printf("\r\n\r\n ------------------------------------------------------ \r\n");
	xil_printf(" ------ lwIP RAW Mode TCP Client Application ------ \r\n");

	lwip_init();

	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add(netif, NULL, NULL, NULL, mac_ethernet_address,
				PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\r\n");
		return -1;
	}

	netif_set_default(netif);
	platform_enable_interrupts();
	netif_set_up(netif);

	#if (LWIP_IPV6==0)
	#if (LWIP_DHCP==1)
		/* Create a new DHCP client for this interface.
		 * Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
		 * the predefined regular intervals after starting the client. */
		dhcp_start(netif);
		dhcp_timoutcntr = 24;
		while (((netif->ip_addr.addr) == 0) && (dhcp_timoutcntr > 0))
			xemacif_input(netif);
		if (dhcp_timoutcntr <= 0) {
			if ((netif->ip_addr.addr) == 0) {
				xil_printf("ERROR: DHCP request timed out\r\n");
				assign_default_ip(&(netif->ip_addr),
						&(netif->netmask), &(netif->gw));
			}
		}

	#else
		assign_default_ip(&(netif->ip_addr), &(netif->netmask), &(netif->gw));
	#endif
		print_ip_settings(&(netif->ip_addr), &(netif->netmask), &(netif->gw));
	#endif // LWIP_IPV6
	print_app_header();

	// ################################################## XILINX CODE END ##################################################




	// ############################################## Ofri & Samah CODE START ##############################################

	start_application();


/*
	xil_printf("\r\n ------- Starting ~Ofri & Samah ------- \r\n");

	xil_printf("Entering loop to read input from user\r\n");
	char buff[1024];
	int isDone=0;
	xil_printf("Please insert input:\r\n");
	do
	{
		if (scanf("%s", buff) != -1)
		{
			if (strcmp(buff, "Done") == 0)
				isDone = 1;
			if (strcmp(buff, "DrawPic") == 0)
				Draw_Picture();
			if (strcmp(buff, "TestRunningPixel") == 0)
				Test_Running_Pixel();
			xil_printf("I just received: %s\r\n", buff);
		}
		sleep(1);
	} while(isDone == 0);
	xil_printf("Receiving input from user loop has ended.");
*/


    int arr[] = {8,8,8,8};
    // direction is in the format: "Row,Column,D" , while D={L/U/D/R}, and directions are seperated by semicolon; indexes from 0
    // for example: "3,6,L;3,7,D;"
    LedSignResult res1 = initBoard(4,arr,"");
    if(res1 != LED_SIGN_SUCCESS){
        xil_printf("res1 ERROR!!\n");
    	destroyBoard();
        return 0;
    }

    LedSignResult res2 = addSubBoard(5,0,0,256,128);
    if(res2 != LED_SIGN_SUCCESS){
    	xil_printf("res2 ERROR!!\n");
    	destroyBoard();
        return 0;
    }

    /*

    byte imageR[16] = {255,0,255,0,255,0,255,0,255,0,255,0,255,0,255,0};
    byte imageG[16] = {0,255,0,255,0,255,0,255,0,255,0,255,0,255,0,255};
    byte imageB[16] = {255,0,255,0,255,0,255,0,255,0,255,0,255,0,255,0};

	LedSignResult res3 = addImageToStock(9,4,4,imageR, imageG, imageB);
	if(res3 != LED_SIGN_SUCCESS){
		xil_printf("res3 ERROR!!\n");
		destroyBoard();
		return 0;
	} else {
		xil_printf("Adding Image To Stock Succeed \n \n");
	}
*/

    int text1[4] = {8,1,14,18};


    LedSignResult res6 = createTextArea(5,4,190,100,25,8,0,0,255,true);
    if(res6 != LED_SIGN_SUCCESS){
    	xil_printf("res6 ERROR %d !!\n",res6);
    	destroyBoard();
        return 0;
    } else {
		xil_printf("Creating Text Succeed\n \n");
	}

    LedSignResult res9 = createTextArea(5,8,190,110,15,8,0,0,255,true);
    if(res9 != LED_SIGN_SUCCESS){
    	xil_printf("res9 ERROR %d !!\n",res9);
    	destroyBoard();
        return 0;
    } else {
		xil_printf("Creating Text Succeed\n \n");
	}

    LedSignResult res7 = updateText(5,4, text1, 4,false);
    if(res7 != LED_SIGN_SUCCESS){
    	xil_printf("res7 ERROR %d !!\n",res7);
    	destroyBoard();
        return 0;
    } else {
		xil_printf("updating Text Succeed\n \n");
	}

    LedSignResult res10 = updateText(5,8, text1, 4,false);
    if(res10 != LED_SIGN_SUCCESS){
    	xil_printf("res10 ERROR %d !!\n",res10);
    	destroyBoard();
        return 0;
    } else {
		xil_printf("updating Text Succeed\n \n");
	}

//	LedSignResult res8 = DrawBoard();
//	if(res8 != LED_SIGN_SUCCESS){
//		xil_printf("Drawing Board ERROR!!\n");
//    	destroyBoard();
//		return 0;
//    } else {
//		xil_printf("Draw Board Succeed\n \n");
//	}



	XTime start, end;
	double cpu_time_used;
	XTime_GetTime(&start);
	int i = 1;
	while (i<100)
	{
		// If no new msg
		scroll_whole_board(i);
		// else - do scrolling
		XTime_GetTime(&end);
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		xil_printf("cpu time = %lf\n", cpu_time_used);
		if (cpu_time_used > 0.5)
		{
			xil_printf("It's time to scroll!!\n");
			// Call scrolling function
			XTime_GetTime(&start);

		}
		i++;
	}

	destroyBoard();

	xil_printf("\r\n ---------- All done! ~Ofri & Samah ---------- \r\n");
	xil_printf(" ------------------------------------------------ \r\n");

	// ############################################### Ofri & Samah CODE END ###############################################




	// ################################################## XILINX CODE START ##################################################



	while (1) {
		if (TcpFastTmrFlag) {
			tcp_fasttmr();
			TcpFastTmrFlag = 0;
		}
		if (TcpSlowTmrFlag) {
			tcp_slowtmr();
			TcpSlowTmrFlag = 0;
		}

		xemacif_input(netif);
		transfer_data();

	}

	xil_printf("Exit\r\n"); // Never reached
	cleanup_platform();

	return 0;
}

// ################################################## XILINX CODE END ##################################################

