/****************************************************************************
 *
 * $Id: vpImageFilter.cpp,v 1.5 2007-02-26 16:39:17 fspindle Exp $
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
 * This file is part of the ViSP toolkit
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
 * Various image tools, convolution, ...
 *
 * Authors:
 * Eric Marchand
 *
 *****************************************************************************/

#include <visp/vpImageFilter.h>

/*!
  Apply a filter to an image.

  \param I : Image to filter
  \param If : Filtered image.
  \param M : Filter coefficients.

*/
void
vpImageFilter::filter(const vpImage<unsigned char> &I,
		      vpImage<double>& If,
		      const vpMatrix& M)
{

  unsigned size = M.getRows() ;
  unsigned half_size = size/2 ;

  If.resize(I.getHeight(),I.getWidth()) ;

  If = 0 ;

  for (unsigned i=half_size ; i < I.getHeight()-half_size ; i++)
  {
    for (unsigned j=half_size ; j < I.getWidth()-half_size ; j++)
    {
      double   conv_x = 0 ;

      for(unsigned a = 0 ; a < size ; a++ )
        for(unsigned b = 0 ; b < size ; b++ )
	{
	  double val =  I[i-half_size+a][j-half_size+b] ;
	  conv_x += M[a][b] * val ;
	}
      If[i][j] = conv_x ;
    }
  }

}

/*!
  Apply a filter to an image.

  \param I : Image to filter
  \param Iu : Filtered image along the horizontal axis (u = columns).
  \param Iv : Filtered image along the vertical axis (v = rows).
  \param M : Separate filter coefficients

*/
void
vpImageFilter::filter(const vpImage<double> &I,
		      vpImage<double>& Iu,
		      vpImage<double>& Iv,
		      const vpMatrix& M)
{

  unsigned size = M.getRows() ;
  unsigned half_size = size/2 ;

  Iu.resize(I.getHeight(),I.getWidth()) ;
  Iv.resize(I.getHeight(),I.getWidth()) ;

  Iu = 0 ;
  Iv = 0 ;
  for (unsigned v=half_size ; v < I.getHeight()-half_size ; v++)
  {
    for (unsigned u=half_size ; u < I.getWidth()-half_size ; u++)
    {
      double   conv_u = 0 ;
      double   conv_v = 0 ;

      for(unsigned a = 0 ; a < size ; a++ )
        for(unsigned b = 0 ; b < size ; b++ )
	{
	  double val =  I[v-half_size+a][u-half_size+b] ;
	  conv_u += M[a][b] * val ;
	  conv_v += M[b][a] * val  ;
	}
      Iu[v][u] = conv_u ;
      Iv[v][u] = conv_v ;
    }
  }

}

