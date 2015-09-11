/*******************************************************************
 Copyright 2014 - 2019, Huawei Tech. Co., Ltd.
 ALL RIGHTS RESERVED

 Filename      : kvf.h
 Author        : KVF group
 Creation time : 2015.01.05
 Description   : this file define the error codes in kvf.

 Version       : 0.9.0
 ********************************************************************/

#ifndef __KVF_ERR_CODE_H__
#define __KVF_ERR_CODE_H__

/*
 * error codes
 */
enum kv_err
{
	RET_OK = 0,
	INVALID_PARAM   = -1,
	MEM_NOT_ENOUGH  = -2,
	KVF_MM_MISMATCH = -3,
	KVF_ALREADY_EXISTED = -4,
	KVF_NOT_EXISTED  	= -5,
};

#endif	//__KVF_ERR_CODE_H__
