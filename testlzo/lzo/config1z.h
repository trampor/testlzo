/* config1z.h -- configuration for the LZO1Z algorithm

   This file is part of the LZO real-time data compression library.

   Copyright (C) 1996-2017 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
   http://www.oberhumer.com/opensource/lzo/
 */


/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the library and is subject
   to change.
 */


#ifndef __LZO_CONFIG1Z_H
#define __LZO_CONFIG1Z_H 1

#if !defined(LZO1Z)
#  define LZO1Z 1
#endif

#include "lzo_conf.h"
#include "lzo1z.h"

#define M2_MAX_OFFSET   0x0700
#include "config1x.h"

#endif /* already included */


/* vim:set ts=4 sw=4 et: */
