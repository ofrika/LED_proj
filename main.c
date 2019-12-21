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

	start_application();


	// ################################################## XILINX CODE END ##################################################




	// ############################################## Ofri & Samah CODE START ##############################################


	// Initial setup (See appendix in book for explanation)
	int leave_loop;
//
	leave_loop = parseMessage("Init(default)");

	leave_loop = parseMessage("Add_sub_board(1,1,1,250,60)");
	leave_loop = parseMessage("Add_text_area(1,1,32,15,192,32,250,0,0,1)");
	leave_loop = parseMessage("Insert_text(1,1,{72,37,51,33,57,53,37,96,66,57,96,36,36,91})");

	// left side
	leave_loop = parseMessage("Add_sub_board(2,5,64,90,63)");
	leave_loop = parseMessage("Add_text_area(2,1,5,64,80,16,255,255,0,0)");
	leave_loop = parseMessage("Insert_text(2,1,{81,17,6,10,16,8})");
	// floor A
	leave_loop = parseMessage("Add_text_area(2,2,15,90,32,16,0,20,220,0)");
	leave_loop = parseMessage("Add_picture_area(2,3,52,90,16,16,1,255,255,255)");
	leave_loop = parseMessage("Add_text_area(2,4,68,90,16,16,255,255,255,0)");
	leave_loop = parseMessage("Insert_text(2,2,{82,81},2)");
	leave_loop = parseMessage("Insert_picture(2,3,0)");
	leave_loop = parseMessage("Insert_text(2,4,{1},1)");
	// floor B
	leave_loop = parseMessage("Add_text_area(2,5,15,106,32,16,0,20,220,0)");
	leave_loop = parseMessage("Add_picture_area(2,6,52,106,16,16,1,255,255,255)");
	leave_loop = parseMessage("Add_text_area(2,7,68,106,16,16,255,255,255,0)");
	leave_loop = parseMessage("Insert_text(2,5,{85,86},2)");
	leave_loop = parseMessage("Insert_picture(2,6,2)");
	leave_loop = parseMessage("Insert_text(2,7,{2},1)");

	// right side
	leave_loop = parseMessage("Add_sub_board(3,160,64,82,63)");
	leave_loop = parseMessage("Add_text_area(3,1,160,64,80,16,255,255,0,0)");
	leave_loop = parseMessage("Insert_text(3,1,{80,17,6,10,16,8})");
	/*
	// floor A
	leave_loop = parseMessage("Add_text_area(3,2,170,90,32,16,0,20,220,0)");
	leave_loop = parseMessage("Add_picture_area(3,3,210,90,16,16,2,255,255,255)");
	leave_loop = parseMessage("Insert_text(3,2,{80,86},2)");
	leave_loop = parseMessage("Insert_picture(3,3,3)");
	*/

	// floor A
	leave_loop = parseMessage("Add_text_area(3,2,180,90,32,16,0,20,220,0)");
	leave_loop = parseMessage("Add_picture_area(3,3,197,90,16,16,1,255,255,255)");
	leave_loop = parseMessage("Add_text_area(3,4,213,90,16,16,255,255,255,0)");
	leave_loop = parseMessage("Insert_text(3,2,{82,81},2)");
	leave_loop = parseMessage("Insert_picture(3,3,0)");
	leave_loop = parseMessage("Insert_text(3,4,{1},1)");
	// floor B
	leave_loop = parseMessage("Add_text_area(3,5,180,106,32,16,0,20,220,0)");
	leave_loop = parseMessage("Add_picture_area(3,6,197,106,16,16,1,255,255,255)");
	leave_loop = parseMessage("Add_text_area(3,7,213,106,16,16,255,255,255,0)");
	leave_loop = parseMessage("Insert_text(3,5,{85,86},2)");
	leave_loop = parseMessage("Insert_picture(3,6,3)");
	leave_loop = parseMessage("Insert_text(3,7,{2},1)");


	leave_loop = parseMessage("Draw_frame(255,255,255)");



	/*
	XTime start;
	XTime end;
	double cpu_time_used;
	XTime_GetTime(&start);


	int i = 1;
	while (1) {
		sleep(0.6);

		XTime_GetTime(&end);
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

		if (cpu_time_used >= 500)
		{
			scroll_whole_board(i);
			XTime_GetTime(&start);
		}

		scroll_whole_board(i);

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

		if (getTerminatorOut() == 1)
			break;
		i++;
	}
	*/

	xil_printf("\n\nProgram stopped.\r\n");
	xil_printf("\r\n ---------- All done! ~Ofri & Samah ---------- \r\n");
	xil_printf(" ------------------------------------------------ \r\n");
	cleanup_platform();

	return 0;
}

// ################################################## XILINX CODE END ##################################################
