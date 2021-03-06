/*
 * Copyright 2011 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */

/*
#if defined(__tilepro__)
#include "opcode_tilepro.h"
#elif defined(__tilegx__)
#include "opcode_tilegx.h"
#else
#error Unexpected Tilera chip type
#endif*/

#ifdef __tilepro__
#include "opcode_tilepro.h"
#else
#include "opcode_tilegx.h"
#endif
