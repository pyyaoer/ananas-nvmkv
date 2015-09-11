# Ananas 
[![Build Status](https://travis-ci.org/huaweistorage/ananas.png)](https://travis-ci.org/huaweistorage/ananas)

#### Note on Gerrithub.io

We have integrated our github repo with Gerrithub.io, so if you want to contribute, it would be great if you could perform git ops to our gerrithub repo first, for example :
git clone https://review.gerrithub.io/huaweistorage/ananas

In this way we could utilize Gerrit code review for the project :)

#### Googlegroup

https://groups.google.com/d/forum/project-ananas You're welcomed to join and start a discussion right away !

# Object Drive Key Value Framework

## Overview
Key Value Framework defined a programming frame work according to generic key-value storage data model, 
storage provides pools, and pool provides objects. Actually there are lots of key-value storage open 
source project among different domains, from HDD/Flash to NOSQL, and almost everyone has its own API 
definitions, applications need write lots of adaptive layer to use these key value storage even they 
has the same semantics. Key Value Framework provides the APIs semantics definition based on the generic 
key value storage data model, including the function name and parameters. Then applications will use 
unified API to access different key value storage, just like applications access different file system 
by virtual file system. As a result, it makes the key value based application programming simple.


## Prerequisites
### Linux Requirements
  * GNU-compatible Make or gmake
  * libtools

## How To Install
### Linux Install
For linux user, just simply run:

  * autoreconf -i
 
  * ./configure
 
  * make & make install
 
  _`if not works, try autoreconf -i -f`_

## Limitations
 1, Currently, `KVF` can only run on linux, for other platforms could be supported later.
 
 2, More features will be implemented soon. 
 
## Guide to source files
### Includes
include/kvf/kvf_api.h
    Main interface to kvf.

include/kvf/kvf.h
    The data structures defined in kvf.

include/kvf/kvf_err_code.h
    Common error codes are all put here.

include/kvf/kvf_type.h
    Basic typedefs.

include/kvf_list.h
	A simple list implementation.
	
### Source
src/kvf.c
	The kvf's basic APIs.
	
src/pool.c
	This file implements the basic operations of pool.

src/crud.c
	This file implements the basic operations of key-value objects.
	
## Notes
See [doc][1] for more detail.

[1]: https://github.com/huaweistorage/ananas/tree/master/doc
