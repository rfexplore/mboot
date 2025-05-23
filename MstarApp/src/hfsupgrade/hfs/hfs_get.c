//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

#include <lwip/init.h>
#include <lwip/timers.h>
#include <lwip/sys.h>
#include <lwip/tcp.h>
#include <lwip/netif.h>
#include <netif/etharp.h>
#include <netif/ethernetif.h>

#include <stdio.h>
#include <linux/string.h>

#include <MsOS.h>
#include <common.h>
#include <net.h>

#include "hfs_get.h"


#define ESTART_DELAY                2000 // millisecond
#define RETRY_DELAY                 1000 // millisecond

#define HFS_GET_TIMEOUT             15000 // millisecond
#define HFS_GET_RETRY               50
#define HFS_PORT                    80

#define HTTP_HEADER_GET_START       "GET /"
#define HTTP_HEADER_GET_BOTTOM      " HTTP/1.1\r\n\r\n"
#define CONTENT_LENGTH_STRING       "Content-Length: "
#define CONTENT_LENGTH_PATTERN      "Content-Length: %d"
#define HTTP_HEADER_NEWLINE         "\r\n"
#define HTTP_HEADER_BOTTOM_STRING   "\r\n\r\n"
#define HTTP_HEADER_BOTTOM_LENGTH   4
#define HTTP_RESPONSE_CODE_OK       "200 OK"
#define HTTP_HEADER_BUFFER_SIZE     1024

#define ENV_SERVERIP                "serverip"
#define ENV_IPADDR                  "ipaddr"
#define ENV_NETMASK                 "netmask"
#define ENV_GATEWAY                 "gatewayip"

#define HFS_ERR_MSG_EMPTY           ""
#define HFS_ERR_ENV_MSG             "IP configuration invalid"
#define HFS_ERR_TIMEOUT_MSG         "TCP connection timeout"
#define HFS_ERR_HTTP_ERROR_MSG      "HTTP response code is not 200 OK"
#define HFS_ERR_READY_TIME_MSG      "Failed before download"
#define HFS_ERR_DOWNLOAD_TIME_MSG   "Failed during download"
#define HFS_ERR_TCP_PCB_MSG         "TCP_PCB cannot be created"
#define HFS_ERR_TCP_CREATE_MSG      "TCP connection cannot be created\n"
#define HFS_ERR_TCP_ABORT_MSG       "TCP Connection abort"


typedef enum
{
    HFS_NETIF_ON,
    HFS_NETIF_OFF
} HFS_NETIF_STATE;

typedef enum
{
    HFS_TCP_ON,
    HFS_TCP_OFF
} HFS_TCP_STATE;

typedef enum
{
    HFS_STATE_READY,
    HFS_STATE_DOWNLOAD,
    HFS_STATE_COMPLETE
} HFS_STATE;

typedef enum
{
    HFS_ERR_OK,
    HFS_ERR_ENV,
    HFS_ERR_CTRL_C_ABORT,
    HFS_ERR_TIMEOUT,
    HFS_ERR_HTTP_ERROR,
    HFS_ERR_READY_TIME,
    HFS_ERR_DOWNLOAD_TIME,
    HFS_ERR_TCP_PCB,
    HFS_ERR_TCP_CREATE,
    HFS_ERR_TCP_ABORT,
} HFS_ERR;

static char *HFS_ERR_MSG[] =
{
    HFS_ERR_MSG_EMPTY,
    HFS_ERR_ENV_MSG,
    HFS_ERR_MSG_EMPTY,
    HFS_ERR_TIMEOUT_MSG,
    HFS_ERR_HTTP_ERROR_MSG,
    HFS_ERR_READY_TIME_MSG,
    HFS_ERR_DOWNLOAD_TIME_MSG,
    HFS_ERR_TCP_PCB_MSG,
    HFS_ERR_TCP_CREATE_MSG,
    HFS_ERR_TCP_ABORT_MSG
};


// write related
static void *m_addr;
static char *m_boot_file;

// hfs init related
static struct netif m_netif;
static HFS_NETIF_STATE m_netif_state = HFS_NETIF_OFF;

// hfs state releated
static HFS_TCP_STATE m_tcp_state;
static HFS_STATE m_state;
static HFS_ERR m_err;

// hfs_get_response related
static u64 file_size;
static u64 file_size_recv;
static int loading_percentage;
static u32_t last_active_time;
static char header[HTTP_HEADER_BUFFER_SIZE];


static HFS_ERR hfs_get_ip(ip_addr_t *ip, char *env_name)
{
    char *env_ip;
    if ((env_ip = getenv(env_name)) == NULL)
    {
        printf("*** ERROR: '");
        printf(env_name);
        printf("' not set\n");
        return HFS_ERR_ENV;
    }

    int a, b, c, d;
	if (sscanf(env_ip, "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
    {
        printf("*** ERROR: '");
        printf(env_name);
        printf("' invalid\n");
        return HFS_ERR_ENV;
    }
	IP4_ADDR(ip, a, b, c, d);

    return HFS_ERR_OK;
}

static HFS_ERR hfs_get_init(void)
{
    // get ifconfig
    ip_addr_t ipaddr, netmask, gateway;
    if ((m_err = hfs_get_ip(&ipaddr, ENV_IPADDR)) != HFS_ERR_OK ||
        (m_err = hfs_get_ip(&netmask, ENV_NETMASK)) != HFS_ERR_OK ||
        (m_err = hfs_get_ip(&gateway, ENV_GATEWAY)) != HFS_ERR_OK)
    {
        return m_err;
    }

    // lwip initialization and add net_interface
    lwip_init();
    netif_add(&m_netif, &ipaddr, &netmask, &gateway, NULL, ethernetif_init, ethernet_input);
    netif_set_default(&m_netif);
    netif_set_up(&m_netif);

    m_netif_state = HFS_NETIF_ON;

    return HFS_ERR_OK;
}

static err_t hfs_get_request(void * arg, struct tcp_pcb * tpcb, err_t err)
{
    // concatenate http request header
    char data[128];
    strcpy(data, HTTP_HEADER_GET_START);
    strcat(data, m_boot_file);
    strcat(data, HTTP_HEADER_GET_BOTTOM);

    // send request
    tcp_write(tpcb, data, strlen(data), TCP_WRITE_FLAG_COPY);
	tcp_output(tpcb);

    return err;
}

static void hfs_get_response_process_header(void *data, int len)
{
    // concatenate http response header
    memcpy(header + file_size_recv, data, len);
    file_size_recv += len;

    // check http response code = 200
    char *find = NULL;
    if ((find = strstr(header, HTTP_HEADER_NEWLINE)) != NULL && strstr(header, HTTP_RESPONSE_CODE_OK) == NULL)
    {
        char code[4] = {0};
        sscanf(header, "HTTP/%*s %s", &code);
        printf("\n*** Warning: HTTP Response Code:%s", code);

        char message[64] = {0};
        char *p1 = strstr(header, code);
        if (p1 != NULL)
        {
            int start = (int) p1 + 3 + 1; // lef of errCode:3, space:1
            int offset = (int) find - start;
            strncpy(message, (void *) start, offset);
            printf(", Message:%s", message);
        }

        m_err = HFS_ERR_HTTP_ERROR;
        return;
    }

    // get file size
    if (file_size == 0 && (find = strstr(header, CONTENT_LENGTH_STRING)) != NULL)
    {
        sscanf(find, CONTENT_LENGTH_PATTERN, &file_size);
    }

    // check end of http header, and store first data segment
    if ((find = strstr(header, HTTP_HEADER_BOTTOM_STRING)) != NULL)
    {
        int offset = (int) find + HTTP_HEADER_BOTTOM_LENGTH;
        int first_data_len = file_size_recv - (offset - (int) header);
        memcpy(m_addr, (void *) offset, first_data_len);

        file_size_recv = first_data_len;
        m_state = HFS_STATE_DOWNLOAD;
    }
}

static void hfs_get_response_process_file(void *data, int len)
{
    // write file
    memcpy(m_addr + file_size_recv, data, len);
    file_size_recv += len;

    // show loading progress
    int tmp_percentage = file_size_recv * 100 / file_size;
    if (tmp_percentage > loading_percentage)
    {
        loading_percentage = tmp_percentage;
        printf("\b\b\b\b%3d%%", loading_percentage);
    }

    // check file download complete
    if (file_size_recv == file_size)
    {
        m_state = HFS_STATE_COMPLETE;
    }
}

static err_t hfs_get_response(void * arg, struct tcp_pcb * tpcb, struct pbuf * p, err_t err)
{
    last_active_time = sys_now();

    if (p != NULL)
    {
        tcp_recved(tpcb, p->tot_len);
        struct pbuf *q;
        for (q = p; q != NULL; q = q->next)
        {
            if (m_state == HFS_STATE_DOWNLOAD)
            {
                hfs_get_response_process_file(q->payload, q->len);
                if (m_state == HFS_STATE_COMPLETE)
                {
                    tcp_close(tpcb);
                }
            }
            else
            {
                hfs_get_response_process_header(q->payload, q->len);
                if (m_err == HFS_ERR_HTTP_ERROR)
                {
                    tcp_close(tpcb);
                }
            }
        }
        pbuf_free(p);
    }
    else if (err == ERR_OK)
    {
        m_tcp_state = HFS_TCP_OFF;

        if (m_state != HFS_STATE_COMPLETE && m_err == HFS_ERR_OK)
        {
            m_err = m_state == HFS_STATE_READY ? HFS_ERR_READY_TIME : HFS_ERR_DOWNLOAD_TIME;
        }
        return tcp_close(tpcb);
    }

    return ERR_OK;
}

static void hfs_get_err(void * arg, err_t err)
{
    printf("\n*** Warning: TCP Connection Abort (err:%d)", err);

    m_tcp_state = HFS_TCP_OFF;

    if (m_err == HFS_ERR_OK)
    {
        m_err = HFS_ERR_TCP_ABORT;
    }
}

HFS_ERR hfs_get_process(void)
{
    // variable initialization
    m_tcp_state = HFS_TCP_ON;
    m_state = HFS_STATE_READY;
    m_err = HFS_ERR_OK;

    file_size = 0;
    file_size_recv = 0;
    loading_percentage = 0;
    last_active_time = sys_now();
    memset(header, 0, HTTP_HEADER_BUFFER_SIZE);

    // get server ip
    ip_addr_t serverip;
    if ((m_err = hfs_get_ip(&serverip, ENV_SERVERIP)) != HFS_ERR_OK)
    {
        return m_err;
    }

    // create tcp control block
    struct tcp_pcb *hfs_tcp_pcb = tcp_new();
    if (hfs_tcp_pcb == NULL)
    {
        return HFS_ERR_TCP_PCB;
    }

    // create tcp connection
    if (tcp_connect(hfs_tcp_pcb, &serverip, HFS_PORT, hfs_get_request) != ERR_OK)
    {
        return HFS_ERR_TCP_CREATE;
    }

    // set response processing callback & timeout
	tcp_recv(hfs_tcp_pcb, hfs_get_response);
	tcp_err(hfs_tcp_pcb, hfs_get_err);

    // show download info
#if defined(CONFIG_NET_MULTI)
    printf("Using %s device\n", eth_get_name());
#endif
	printf("HFS from server %pI4; our IP address is %pI4\n", (void *) serverip.addr, (void *) m_netif.ip_addr.addr);
	printf("Filename '%s'.\n", m_boot_file);
	printf("Load address: 0x%lx\n", (ulong) m_addr);
    printf("Loading: ....");

    // tcp main loop: 1.start rx, 2.check tcp timeout, 3.check tcp disconnect, 4.check user abort, 5.timeout mechanism
	while (1)
	{
		ethernetif_recv(low_level_input);

		sys_check_timeouts();

        if (m_tcp_state == HFS_TCP_OFF)
        {
            return m_err;
        }
		else if (ctrlc())
        {
            tcp_abort(hfs_tcp_pcb);
			return HFS_ERR_CTRL_C_ABORT;
		}
        else if (sys_now() - last_active_time > HFS_GET_TIMEOUT)
        {
            tcp_abort(hfs_tcp_pcb);
            return HFS_ERR_TIMEOUT;
        }
	}
}

int hfs_get(ulong addr, char *file)
{
    // network interface initialization
	if (m_netif_state == HFS_NETIF_OFF)
	{
        ethernetif_open();
	    MsOS_DelayTask(ESTART_DELAY); // delay for waiting estart complete
	    if (hfs_get_init() != HFS_ERR_OK)
        {
            ethernetif_close();
			return -1;
        }
    }

    // start hfs download with retry
    m_addr = (void *) addr;
    m_boot_file = file;

    int retry = 0;
    HFS_ERR result;

    for (retry = 0; retry < HFS_GET_RETRY; retry++)
    {
        ethernetif_close();
        ethernetif_open();

        result = hfs_get_process();
        if (result == HFS_ERR_OK)
        {
            // show download result
            printf("\ndone\n");
            printf("Bytes transferred = %ld (%lx hex)\n", (ulong) file_size_recv, (ulong) file_size_recv);

            char buf[20];
            sprintf(buf, "%lX", (ulong) file_size_recv);
            setenv("filesize", buf);

            sprintf(buf, "%lX", (unsigned long) m_addr);
            setenv("fileaddr", buf);
            break;
        }
        else if (result == HFS_ERR_CTRL_C_ABORT)
        {
            printf("\nAbort\n");
            break;
        }
        else
        {
            printf("\nHFS error: '%s'\n", HFS_ERR_MSG[result]);

            // not retry case
            if (result == HFS_ERR_ENV || result == HFS_ERR_HTTP_ERROR)
            {
                printf("Not retrying...\n");
                break;
            }
            else if (retry != HFS_GET_RETRY - 1)
            {
                printf("\nStarting again");
                MsOS_DelayTask(RETRY_DELAY); // retry delay
                printf("\n");
            }
        }
    }

    ethernetif_close();

    return result == HFS_ERR_OK ? file_size_recv : -1;
}
