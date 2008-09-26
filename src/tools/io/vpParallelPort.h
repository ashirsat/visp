/****************************************************************************
 *
 * $Id: vpParallelPort.h,v 1.4 2008-09-26 15:20:58 fspindle Exp $
 *
 * Copyright (C) 1998-2006 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * Parallel port management.
 *
 * Authors:
 * Fabien Spindler
 *
 *****************************************************************************/


#ifndef vpParallelPort_h
#define vpParallelPort_h

/*!
  \file vpParallelPort.h
  \brief Parallel port management under unix.
*/

#include <iostream>

#include <visp/vpConfig.h>

#ifdef VISP_HAVE_PARPORT

#  include <errno.h>
#  include <linux/parport.h>
#  include <linux/ppdev.h>

#  include <visp/vpParallelPortException.h>

/*!

  \class vpParallelPort
  \ingroup ParallelPort
  \brief Parallel port management under unix.

  \code
  vpParallelPort parport; // open /dev/parport0 to access to the parallel port.

  parport.sendData(127); // D7 is set to 0 and [D6-D0] are set to logical level 1

  \endcode

*/
class VISP_EXPORT vpParallelPort
{

public:
  vpParallelPort();
  ~vpParallelPort();

  void sendData(unsigned char &data);
  unsigned char getData();

private:
  void open();
  void close();

private:
  int fd; // parallel port descriptor
  char device[FILENAME_MAX];
} ;

#endif

#endif
