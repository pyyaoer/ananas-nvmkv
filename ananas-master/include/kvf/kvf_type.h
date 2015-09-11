/*******************************************************************
 Copyright 2014 - 2019, Huawei Tech. Co., Ltd.
 ALL RIGHTS RESERVED

 Filename      : kvf_type.h
 Author        : KVF group
 Creation time : 2015.01.05
 Description   : this file define the basic types in kvf.

 Version       : 0.9.0
 ********************************************************************/
#ifndef __KVF_TYPE_H__
#define __KVF_TYPE_H__

/*
 * basic types
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short	s16;
typedef unsigned short	u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long	s64;
typedef unsigned long long	u64;

/*
 * string type
 */
typedef  struct string
{
	u32 len;
	s8* data;
}string_t;

#endif
