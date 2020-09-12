/**
 * \file tuya_ble_mutli_tsf_protocol.c
 *
 * \brief 
 */
/*
 *  Copyright (C) 2014-2019, Tuya Inc., All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of tuya ble sdk 
 */
 
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "tuya_ble_type.h"
#include "tuya_ble_mem.h"
#include "tuya_ble_mutli_tsf_protocol.h"

#define __MUTLI_TSF_PROTOCOL_GLOBALS

/***********************************************************
*************************micro define***********************
***********************************************************/
#define MTP_DEBUG 1

#if MTP_DEBUG
#define MTP_PR_DEBUG(_fmt_, ...) \
	printf("[mtp:dbg]%s:%d "_fmt_"\n\r", __FILE__, __LINE__, ##__VA_ARGS__)
#define MTP_PR_DEBUG_RAW(_fmt_, ...) \
	printf(_fmt_, ##__VA_ARGS__)
#else
#define MTP_PR_DEBUG(...)
#define MTP_PR_DEBUG_RAW(_fmt_, ...)
#endif

#define MTP_PR_NOTICE(_fmt_, ...) \
	printf("[mtp:notice]%s:%d "_fmt_"\n\r", __FILE__, __LINE__, ##__VA_ARGS__)
#define MTP_PR_ERR(_fmt_, ...) \
	printf("[mtp:err]%s:%d "_fmt_"\n\r", __FILE__, __LINE__, ##__VA_ARGS__)

#undef  NULL
#define NULL (void *)0
/***********************************************************
*************************variable define********************
***********************************************************/
static frame_seq_t frame_seq = 0;

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: create_trsmitr_init
*  description: create a transmitter and initialize
*  Input: none
*  Output:
*  Return: transmitter handle
***********************************************************/
frm_trsmitr_proc_s *create_trsmitr_init(void)
{
    frm_trsmitr_proc_s *frm_trsmitr = NULL;

    frm_trsmitr = (frm_trsmitr_proc_s *)tuya_ble_malloc(sizeof(frm_trsmitr_proc_s));
    if ((void *)0 == frm_trsmitr) {
        return (void *)0;
    }
    memset(frm_trsmitr, 0, sizeof(frm_trsmitr_proc_s));

    return frm_trsmitr;
}

/***********************************************************
*  Function: trsmitr_init
*  description: init a transmitter
*  Input: transmitter handle
*  Output:
*  Return:
***********************************************************/
void trsmitr_init(frm_trsmitr_proc_s *frm_trsmitr)
{
    memset(frm_trsmitr, 0, sizeof(frm_trsmitr_proc_s));
}

/***********************************************************
*  Function: delete_trsmitr
*  description: delete transmitter
*  Input: transmitter handle
*  Output:
*  Return:
***********************************************************/
void delete_trsmitr(frm_trsmitr_proc_s *frm_trsmitr)
{
    //free(frm_trsmitr);
    tuya_ble_free((uint8_t *)frm_trsmitr);
}

/***********************************************************
*  Function: get_trsmitr_frame_total_len
*  description: get a transmitter total data len
*  Input: transmitter handle
*  Output:
*  Return: frame_total_t
***********************************************************/
frame_total_t get_trsmitr_frame_total_len(frm_trsmitr_proc_s *frm_trsmitr)
{
    return frm_trsmitr->total;
}

/***********************************************************
*  Function: get_trsmitr_frame_type
*  description:
*  Input: transmitter handle
*  Output:
*  Return: frame_type_t
***********************************************************/
//frame_type_t get_trsmitr_frame_type(frm_trsmitr_proc_s *frm_trsmitr)
//{
//    return frm_trsmitr->type;
//}


uint8_t get_trsmitr_frame_version(frm_trsmitr_proc_s *frm_trsmitr)
{
    return frm_trsmitr->version;
}

/***********************************************************
*  Function: get_trsmitr_frame_seq
*  description:
*  Input: transmitter handle
*  Output:
*  Return: frame_seq_t
***********************************************************/
frame_seq_t get_trsmitr_frame_seq(frm_trsmitr_proc_s *frm_trsmitr)
{
    return frm_trsmitr->seq;
}

/***********************************************************
*  Function: get_trsmitr_subpkg_len
*  description:
*  Input: transmitter handle
*  Output:
*  Return: frame_subpkg_len_t
***********************************************************/
frame_subpkg_len_t get_trsmitr_subpkg_len(frm_trsmitr_proc_s *frm_trsmitr)
{
    return frm_trsmitr->subpkg_len;
}

/***********************************************************
*  Function: get_trsmitr_subpkg
*  description:
*  Input: transmitter handle
*  Output:
*  Return: subpackage buf
***********************************************************/
unsigned char *get_trsmitr_subpkg(frm_trsmitr_proc_s *frm_trsmitr)
{
    return frm_trsmitr->subpkg;
}

static frame_seq_t get_frame_seq(void) //由于暂时没有使用该类型数据，所以暂时可用于多线程
{
    return (frame_seq >= FRAME_SEQ_LMT) ? 0 : frame_seq++;
}

/***********************************************************
*  Function: trsmitr_send_pkg_encode
*  description: frm_trsmitr->transmitter handle
*               type->frame type
*               buf->data buf
*               len->data len
*  Input:
*  Output:
*  Return: MTP_OK->buf send up
*          MTP_TRSMITR_CONTINUE->need call again to be continue
*          other->error
*  Note: could get from encode data len and encode data by calling method
         get_trsmitr_subpkg_len() and get_trsmitr_subpkg()
***********************************************************/
mtp_ret trsmitr_send_pkg_encode(frm_trsmitr_proc_s *frm_trsmitr, unsigned char version, unsigned char *buf, unsigned int len)
{
    if (((void *)0) == frm_trsmitr) {
        return MTP_INVALID_PARAM;
    }

    if (FRM_PKG_INIT == frm_trsmitr->pkg_desc) {
        frm_trsmitr->total = len;
        frm_trsmitr->version = version;
        frm_trsmitr->seq = get_frame_seq();
        frm_trsmitr->subpkg_num = 0;
        frm_trsmitr->pkg_trsmitr_cnt = 0;
    }

    if (frm_trsmitr->subpkg_num >= 0x10000000 || len >= 0x10000000) {
        return MTP_COM_ERROR;
    }

    unsigned char sunpkg_offset = 0;

    // package code
    // subpackage num encode
    int i;
    unsigned int tmp = 0;
    tmp = frm_trsmitr->subpkg_num;
    for (i = 0; i < 4; i++)
    {
        frm_trsmitr->subpkg[sunpkg_offset] = tmp % 0x80;
        if ((tmp / 0x80))
        {
            frm_trsmitr->subpkg[sunpkg_offset] |= 0x80;
        }
        sunpkg_offset++;
        tmp /= 0x80;
        if(0 == tmp)
        {
            break;
        }
    }

    // the first package include the frame total len
    if (0 == frm_trsmitr->subpkg_num) {
        // frame len encode
        tmp = len;
        for (i = 0; i < 4; i++) {
            frm_trsmitr->subpkg[sunpkg_offset] = tmp % 0x80;
            if ((tmp / 0x80)) {
                frm_trsmitr->subpkg[sunpkg_offset] |= 0x80;
            }
            sunpkg_offset++;
            tmp /= 0x80;
            if(0 == tmp) {
                break;
            }
        }

        // frame type and frame seq
        frm_trsmitr->subpkg[sunpkg_offset++] = (frm_trsmitr->version << 0x04) | (frm_trsmitr->seq & 0x0f);
    }

    // frame data transfer
    unsigned char send_data = (SNGL_PKG_TRSFR_LMT - sunpkg_offset);
    if ((len - frm_trsmitr->pkg_trsmitr_cnt) < send_data) {
        send_data = len - frm_trsmitr->pkg_trsmitr_cnt;
    }

    memcpy(&(frm_trsmitr->subpkg[sunpkg_offset]), buf + frm_trsmitr->pkg_trsmitr_cnt, send_data);
    frm_trsmitr->subpkg_len = sunpkg_offset+send_data;

    frm_trsmitr->pkg_trsmitr_cnt += send_data;
    if (0 == frm_trsmitr->subpkg_num) {
        frm_trsmitr->pkg_desc = FRM_PKG_FIRST;
    } else {
        frm_trsmitr->pkg_desc = FRM_PKG_MIDDLE;
    }

    if (frm_trsmitr->pkg_trsmitr_cnt < frm_trsmitr->total) {
        frm_trsmitr->subpkg_num++;
        return MTP_TRSMITR_CONTINUE;
    }

    frm_trsmitr->pkg_desc = FRM_PKG_END;
    return MTP_OK;
}



mtp_ret trsmitr_recv_pkg_decode(frm_trsmitr_proc_s *frm_trsmitr, unsigned char *raw_data, unsigned char raw_data_len)
{
    if (NULL == raw_data || \
            (raw_data_len > SNGL_PKG_TRSFR_LMT) || \
            NULL == frm_trsmitr) {
        return MTP_INVALID_PARAM;
    }

    if (FRM_PKG_INIT == frm_trsmitr->pkg_desc) {
        frm_trsmitr->total = 0;
        frm_trsmitr->version = 0;
        frm_trsmitr->seq = 0;
        frm_trsmitr->pkg_trsmitr_cnt = 0;
    }

    unsigned char sunpkg_offset = 0;
    // package code
    // subpackage num decode
    int i;
    unsigned int multiplier = 1;
    unsigned char digit;
    frame_subpkg_num_t subpkg_num = 0;
//Package number
    for (i = 0; i < 4; i++) {
        digit = raw_data[sunpkg_offset++];
        subpkg_num += (digit & 0x7f) * multiplier;
        multiplier *= 0x80;

        if (0 == (digit & 0x80)) {
            break;
        }
    }

    if(0 == subpkg_num)
    {
        frm_trsmitr->total = 0;
        frm_trsmitr->version = 0;
        frm_trsmitr->seq = 0;
        frm_trsmitr->pkg_trsmitr_cnt = 0;
        frm_trsmitr->pkg_desc = FRM_PKG_FIRST;
    }
    else
    {
        frm_trsmitr->pkg_desc = FRM_PKG_MIDDLE;
    }

    if (frm_trsmitr->subpkg_num >= 0x10000000)
    {
        return MTP_COM_ERROR;
    }
    // is receive the subpackage num valid?
    if (frm_trsmitr->pkg_desc != FRM_PKG_FIRST)
    {
        if (subpkg_num < frm_trsmitr->subpkg_num)
        {
            return MTP_TRSMITR_ERROR;
        }
        else if (subpkg_num == frm_trsmitr->subpkg_num)
        {
            return MTP_TRSMITR_CONTINUE;
        }

        if (subpkg_num - frm_trsmitr->subpkg_num > 1) {
            return MTP_TRSMITR_ERROR;
        }
    }
    frm_trsmitr->subpkg_num = subpkg_num;

    if (0 == frm_trsmitr->subpkg_num)
    {
        // frame len decode
        multiplier = 1;
        for (i = 0; i < 4; i++)
        {
            digit = raw_data[sunpkg_offset++];
            frm_trsmitr->total += (digit & 0x7f) * multiplier;
            multiplier *= 0x80;

            if (0 == (digit & 0x80))
            {
                break;
            }
        }

        if (frm_trsmitr->total >= 0x10000000)
        {
            return MTP_COM_ERROR;
        }

        // frame type and frame seq decode
        frm_trsmitr->version = (raw_data[sunpkg_offset] & FRM_VERSION_OFFSET) >> 4;
        frm_trsmitr->seq = raw_data[sunpkg_offset++] & FRM_SEQ_OFFSET;
    }

    unsigned char recv_data = raw_data_len - sunpkg_offset;
    if((frm_trsmitr->total - frm_trsmitr->pkg_trsmitr_cnt) < recv_data)
    {
        recv_data = frm_trsmitr->total - frm_trsmitr->pkg_trsmitr_cnt;
    }

    // decode data cp to transmitter subpackage buf
    memcpy(frm_trsmitr->subpkg, &raw_data[sunpkg_offset], recv_data);
    frm_trsmitr->subpkg_len = recv_data;
    frm_trsmitr->pkg_trsmitr_cnt += recv_data;

    if (frm_trsmitr->pkg_trsmitr_cnt < frm_trsmitr->total)
    {
        return MTP_TRSMITR_CONTINUE;
    }
    frm_trsmitr->pkg_desc = FRM_PKG_END;
    return MTP_OK;
}
/***********************************************************
*  Function: free_klv_list
*  description:
*  Input: list
*  Output:
*  Return:
***********************************************************/
void free_klv_list(klv_node_s *list)
{
    if (NULL == list)
    {
        return;
    }

    klv_node_s *node = list;
    klv_node_s *next_node = NULL;

    do
    {
        next_node = node->next;
        //free(node);
        tuya_ble_free((uint8_t*)(node->data));

        tuya_ble_free((uint8_t*)node);
        node = next_node;
    } while (node);
}

/***********************************************************
*  Function: make_klv_list
*  description:
*  Input:
*  Output:
*  Return:
***********************************************************/
klv_node_s *make_klv_list(klv_node_s *list,unsigned char id,dp_type type,\
                          void *data,unsigned char len)
{
    klv_node_s *node;
    
    if(NULL == data || type >= DT_LMT) {
        return NULL;
    }

    if(DT_VALUE == type && DT_VALUE_LEN != len) {
        goto err_ret;
    } else if(DT_BITMAP == type && len > DT_BITMAP_MAX) {
        goto err_ret;
    } else if(DT_BOOL == type && DT_BOOL_LEN != len) {
        goto err_ret;
    } else if(DT_ENUM == type && DT_ENUM_LEN != len) {
        goto err_ret;
    }

    //klv_node_s *node = (klv_node_s *)malloc(sizeof(klv_node_s)+len);
    node = (klv_node_s *)tuya_ble_malloc(sizeof(klv_node_s));

    if(NULL == node)
    {
        goto err_ret;
    }
    memset(node, 0, sizeof(klv_node_s));
    node->data =tuya_ble_malloc(len);
    if ((NULL == node)||(node->data==NULL))
    {
        tuya_ble_free(node->data);
        tuya_ble_free((uint8_t*)node);
        goto err_ret;
    }
    node->id = id;
    node->len = len;
    node->type = type;

    if(DT_VALUE == type || \
            DT_BITMAP == type)
    {   // change to big-end
        unsigned int tmp = *(unsigned int *)data;
       // unsigned char shift = 0;
        node->data[0] = (tmp >> 24) & 0xff;
        node->data[1] = (tmp >> 16) & 0xff;
        node->data[2] = (tmp >> 8) & 0xff;
        node->data[3] = (tmp >> 0) & 0xff;
    } 
    else
    {
        //memcpy((void *restrict)node->data,(unsigned char*)data,len);
        memcpy((void *)node->data,(unsigned char*)data,len);
    }
    node->next = list;
    return node;

err_ret:
    free_klv_list(list);
    return NULL;
}

/***********************************************************
*  Function: klvlist_2_data
*  description:
*  Input: type:0-data中len占1个字节，1-data中len占两个字节
*  Output:
*  Return:
***********************************************************/
mtp_ret klvlist_2_data(klv_node_s *list,unsigned char **data,unsigned int *len,unsigned char type)
{
    if(NULL == list || NULL == data || NULL == len)
    {
        return MTP_INVALID_PARAM;
    }

    klv_node_s *node = list;

    // count data len
    unsigned int mk_data_len = 0;
    while(node)
    {
        mk_data_len += sizeof(klv_node_s)+node->len-sizeof(struct s_klv_node *);
        node = node->next;
    }

    unsigned char *mk_data = (unsigned char *)tuya_ble_malloc(mk_data_len);
    if(NULL == mk_data)
    {
        return MTP_MALLOC_ERR;
    }

    // fill data
    unsigned int offset = 0;
    node = list;
    while(node)
    {
        mk_data[offset++] = node->id;
        mk_data[offset++] = node->type;
        if(1 == type)
        {
            mk_data[offset++] = 0;
            mk_data[offset++] = node->len;
        }
        else
        {
            mk_data[offset++] = node->len;
        }
        memcpy(&mk_data[offset],node->data,node->len);
        offset += node->len;
        node = node->next;
    }
    *len = offset;
    *data = mk_data;

    return MTP_OK;
}

/***********************************************************
*  Function: data_2_klvlist
*  description:
*  Input:   type:0-data中len占1个字节，1-data中len占两个字节
*  Output:
*  Return:
***********************************************************/
mtp_ret data_2_klvlist(unsigned char *data,unsigned int len,klv_node_s **list,unsigned char type)
{   //数据拆分成dp点列表  dpid+dp_tp+len+data
    if(NULL == data || NULL == list)
    {
        return MTP_INVALID_PARAM;
    }

    unsigned int offset = 0;
//    unsigned char *alloc_dpdata;
    klv_node_s *klv_list = NULL;
    klv_node_s *node = NULL;
    do
    {
        // not full klv
        //if((len-offset) < (sizeof(klv_node_s)-sizeof(struct s_klv_node *)))
        if(type == 1)
        {
            if((len-offset) < 4)
            {
                free_klv_list(klv_list);
                return MTP_COM_ERROR;
            }
        }
        else
        {
            if((len-offset) < 3)
            {
                free_klv_list(klv_list);
                return MTP_COM_ERROR;
            }
        }

        //node = (klv_node_s *)tuya_malloc(sizeof(klv_node_s)+data[(2+offset)]);
        node = (klv_node_s *)tuya_ble_malloc(sizeof(klv_node_s));
        if(NULL == node)
        {
            free_klv_list(klv_list);
            return MTP_MALLOC_ERR;
        }
        memset(node,0,sizeof(sizeof(klv_node_s)));
        if(1 == type)
        {
            node->data=tuya_ble_malloc(data[3+offset]);  //这里虽然len用两个字节表示，但是只用了低字节
        }
        else
        {
            node->data=tuya_ble_malloc(data[2+offset]);
        }

        if(NULL == node||node->data==NULL)
        {
            tuya_ble_free(node->data);
            tuya_ble_free((uint8_t*)node);
            free_klv_list(klv_list);
            return MTP_MALLOC_ERR;
        }


        node->id = data[offset++];
        node->type = data[offset++];
        if(1 == type)
        {
            //offset++;
            node->len = data[offset++];
            node->len = (node->len<<8) + data[offset++];
        }
        else
        {
            node->len = data[offset++];
        }
        if((len-offset) < node->len)
        {   // is remain data len enougn?
            tuya_ble_free(node->data);
            tuya_ble_free((uint8_t*)node);
            free_klv_list(klv_list);
            return MTP_COM_ERROR;
        }

        memcpy(node->data,&data[offset],node->len);
        offset += node->len;

        node->next = klv_list;
        klv_list = node;
    } while(offset < len);

    if(NULL == klv_list)
    {
        return MTP_COM_ERROR;
    }
    *list = klv_list;
    return MTP_OK;
}

