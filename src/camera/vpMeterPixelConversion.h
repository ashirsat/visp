/****************************************************************************
 *
 * $Id: vpMeterPixelConversion.h,v 1.7 2008-01-31 17:35:51 fspindle Exp $
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
 * Meter to pixel conversion.
 *
 * Authors:
 * Eric Marchand
 * Anthony Saunier
 *
 *****************************************************************************/


#ifndef vpMeterPixelConversion_H
#define vpMeterPixelConversion_H


/*!
  \file vpMeterPixelConversion.h
  \brief meter to pixel  conversion

*/

#include <visp/vpConfig.h>
#include <visp/vpCameraParameters.h>
#include<visp/vpException.h>
#include<visp/vpMath.h>

/*!
  \class vpMeterPixelConversion

  \brief Conversion from normalized coordinates \f$(x,y)\f$ in meter
  to pixel coordinates \f$(u,v)\f$.

  This class relates to vpCameraParameters.

*/
class VISP_EXPORT vpMeterPixelConversion
{
public:

/*!

  \brief Point coordinates conversion from normalized coordinates
  \f$(x,y)\f$ in meter to pixel coordinates \f$(u,v)\f$.

  The used formula depends on the projection model of the camera. To
  know the currently used projection model use
  vpCameraParameter::get_projModel()

  \param cam : camera parameters.
  \param x,y : input coordinates in meter.
  \param u,v : output coordinates in pixels.
  
  \f$ u = x*p_x + u_0 \f$ and  \f$ v = y*p_y + v_0 \f$ in the case of
  perspective projection without distortion.

  \f$ u = x*p_x*(1+k_{ud}*r^2)+u_0 \f$ and  \f$ v = y*p_y*(1+k_{ud}*r^2)+v_0 \f$
  with \f$ r^2 = x^2+y^2 \f$ in the  case of perspective projection with
  distortion.
*/

  inline static void
  convertPoint( const vpCameraParameters &cam,
                const double &x, const double &y,
                double &u, double &v)

  {
    switch(cam.projModel){
      case vpCameraParameters::perspectiveProjWithoutDistortion :   
        convertPointWithoutDistortion(cam,x,y,u,v);
        break;
      case vpCameraParameters::perspectiveProjWithDistortion :
        convertPointWithDistortion(cam,x,y,u,v);
      break;
    }       
  }

  /*!

    \brief Point coordinates conversion without distortion from
    normalized coordinates \f$(x,y)\f$ in meter to pixel coordinates
    \f$(u,v)\f$.

    \f$ u = x*p_x+u_0 \f$ and  \f$ v = y*p_y+v_0  \f$
  */

  inline static void
  convertPointWithoutDistortion(const vpCameraParameters &cam,
              const double &x, const double &y,
              double &u, double &v)

  {
      u = x * cam.px + cam.u0 ;
      v = y * cam.py + cam.v0 ;
  }
  
  /*!

    \brief Point coordinates conversion with distortion from
    normalized coordinates \f$(x,y)\f$ in meter to pixel coordinates
    \f$(u,v)\f$.

    \param cam : camera parameters.
    \param x,y : input coordinates in meter.
    \param u,v : output coordinates in pixels.

    \f$ u = x*p_x*(1+k_{ud}*r^2)+u_0 \f$ and
    \f$ v = y*p_y*(1+k_{ud}*r^2)+v_0 \f$
    with \f$ r^2 = x^2+y^2 \f$
  */
  inline static void
  convertPointWithDistortion(const vpCameraParameters &cam,
              const double &x, const double &y,
              double &u, double &v)

  {
    double r2 = 1.+cam.kud*(x*x+y*y);
    u = cam.u0 + cam.px*x*r2;
    v = cam.v0 + cam.py*y*r2;
  }
  //! line coordinates conversion (rho,theta)
  static void convertLine(const vpCameraParameters &cam,
			  const double &rho_m, const double &theta_m,
			  double &rho_p, double &theta_p) ;
} ;

#endif

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */

