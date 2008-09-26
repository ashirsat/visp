/****************************************************************************
 *
 * $Id: vpServo.h,v 1.16 2008-09-26 15:20:58 fspindle Exp $
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
 * Visual servoing control law.
 *
 * Authors:
 * Eric Marchand
 * Nicolas Mansard
 *
 *****************************************************************************/


#ifndef vpServo_H
#define vpServo_H

/*!
  \file vpServo.h
  \brief  Class required to compute the visual servoing control law
*/

#include <visp/vpConfig.h>
#include <visp/vpMatrix.h>
#include <visp/vpTwistMatrix.h>
#include <visp/vpBasicFeature.h>
#include <visp/vpServoException.h>

#include <visp/vpList.h>
#include <visp/vpAdaptativeGain.h>


/*!
  \class vpServo

  \ingroup VsTask
  \brief Class required to compute the visual servoing control law.

  \warning To avoid potential memory leaks, it is mendatory to call
  explicitly the kill() function to destroy the task. Otherwise, the
  destructor ~vpServo() launch an exception
  vpServoException::notKilledProperly.

  \code
  vpServo task ;

  vpThetaUVector tuv;
  tuv[0] =0.1;
  tuv[1] =0.2;
  tuv[2] =0.3;

  vpFeatureThetaU tu;
  tu.buildFrom(tuv);
  ...
  task.addFeature(tu); // Add current ThetaU feature

  // A call to kill() is requested here to destroy properly the current
  // and desired feature lists.
  task.kill();
  \endcode


  \author Eric Marchand   (Eric.Marchand@irisa.fr) Irisa / Inria Rennes
*/

class VISP_EXPORT vpServo
{
private:
  //! basic initialization
  void init() ;
public:
  //! constructor
  vpServo() { init() ; }
  //! destructor
  virtual ~vpServo() ;
  //! destruction (memory deallocation if required)
  void kill() ;
public:
  //! Interaction matrix
  vpMatrix L ;
  //! error
  vpColVector error ;
  //! task Jacobian  J1 = L cVa aJe
  vpMatrix J1 ;
  //! pseudo inverse of the Jacobian
  vpMatrix J1p ;

  //! current state
  vpColVector s ;
  //! desired state
  vpColVector sStar ;

  //! primary task e1 = J1p(s-s*)
  vpColVector e1 ;
  //! task e = e1 + (I-J1p J1) e2
  vpColVector e ;


  //! articular velocity
  vpColVector q_dot ;
  //! camera velocity
  vpColVector v ;

  /*
    Choice of the visual servoing control law
  */
public:
  typedef enum
    {
      NONE,
      EYEINHAND_CAMERA,
      EYEINHAND_L_cVe_eJe,
      EYETOHAND_L_cVe_eJe,
      EYETOHAND_L_cVf_fVe_eJe,
      EYETOHAND_L_cVf_fJe
    } vpServoType;

  typedef enum
    {
      CURRENT,
      DESIRED,
      MEAN,
      USER_DEFINED
    } vpServoIteractionMatrixType;

  typedef enum
    {
      TRANSPOSE,
      PSEUDO_INVERSE
    } vpServoInversionType;

  typedef enum
    {
      ALL,
      MINIMUM
    } vpServoPrintType;

  //!  chosen visual servoing control law
  vpServoType servoType;
  //!  Choice of the visual servoing control law
  void setServo(vpServoType _servo_type) ;
  //! constructor with Choice of the visual servoing control law
  vpServo(vpServoType _servoType) ;
  /*
    Twist transformation matrix
  */
private:
  //! Twist transformation matrix between Re and Rc
  vpTwistMatrix cVe ;
  bool init_cVe ;
  //! Twist transformation matrix between Rf and Rc
  vpTwistMatrix cVf ;
  bool init_cVf ;
  //! Twist transformation matrix between Re and Rf
  vpTwistMatrix fVe ;
  bool init_fVe ;

public:
  void set_cVe(vpTwistMatrix &_cVe) { cVe = _cVe ; init_cVe = true ; }
  void set_cVf(vpTwistMatrix &_cVf) { cVf = _cVf ; init_cVf = true ; }
  void set_fVe(vpTwistMatrix &_fVe) { fVe = _fVe ; init_fVe = true ; }

  void set_cVe(vpHomogeneousMatrix &cMe) { cVe.buildFrom(cMe); init_cVe=true ;}
  void set_cVf(vpHomogeneousMatrix &cMf) { cVf.buildFrom(cMf); init_cVf=true ;}
  void set_fVe(vpHomogeneousMatrix &fMe) { fVe.buildFrom(fMe); init_fVe=true ;}

  /*
    Jacobians
  */
private:
  //! Jacobian expressed in the end-effector frame
  vpMatrix eJe ;
  bool init_eJe ;
  //! Jacobian expressed in the robot reference frame
  vpMatrix fJe ;
  bool init_fJe ;

public:
  void set_eJe(vpMatrix &_eJe) { eJe = _eJe ; init_eJe = true ; }
  void set_fJe(vpMatrix &_fJe) { fJe = _fJe ; init_fJe = true ; }

  /*
    Task building
  */
private:
  //! true if the error has been computed
  bool errorComputed ;
  //! true if the interaction matrix has been computed
  bool interactionMatrixComputed ;
  //! dimension of the task
  int dim_task ;
  bool taskWasKilled; // flag to indicate if the task was killed
public:
  //! rank of the task Jacobian
  int rankJ1 ;
public:
  //! list of visual features (produce s)
  vpList<vpBasicFeature *> featureList ;
  //! list of desired visual features (produce s*)
  vpList<vpBasicFeature *> desiredFeatureList ;
  //! list of selection among visual features
  //! used to selection a subset of each visual feature if required
  vpList<int> featureSelectionList ;
public:

  //! sign of the interaction +-1 (Eye-in-hand vs eye-to-hand)
  int signInteractionMatrix ;
  //! type of the interaction matrox (current, mean, desired, user)
  vpServoIteractionMatrixType interactionMatrixType ;
  vpServoInversionType inversionType ;
  //! set the type of the interaction matrox (current, mean, desired, user)
  void setInteractionMatrixType(const vpServoIteractionMatrixType &interactionMatrixType,
				const vpServoInversionType &interactionMatrixInversion=PSEUDO_INVERSE) ;

  //! create a new ste of  two visual features
  void addFeature(vpBasicFeature& s, vpBasicFeature& s_star,
	       const int select=vpBasicFeature::FEATURE_ALL) ;
  //! create a new ste of  two visual features
  void addFeature(vpBasicFeature& s,
	       const int select=vpBasicFeature::FEATURE_ALL) ;

  //! compute the interaction matrix related to the set of visual features
  vpMatrix computeInteractionMatrix() ;
  //! compute the error between the current set of visual features and
  //! the desired set of visual features
  vpColVector computeError() ;
  //! test if all the initialiazation are correct if true control law can
  //! be computed
  bool testInitialization() ;
  //! test if all the update are correct if true control law can
  //! be computed
  bool testUpdated() ;
  //! compute the desired control law
  vpColVector computeControlLaw() ;


private:
  //! projection operators WpW
  vpMatrix WpW ;
  //! projection operators I-WpW
  vpMatrix I_WpW ;

public:
  //! add a secondary task
  vpColVector secondaryTask(vpColVector &de2dt) ;
  //! add a secondary task
  vpColVector secondaryTask(vpColVector &e2, vpColVector &de2dt) ;

public:
  //! get the task dimension
  int getDimension() ;

  /*    gain  */
public:
  //! gain
  vpAdaptativeGain lambda ;
public:
  //! set the gain lambda
  void setLambda(double _lambda) { lambda .initFromConstant (_lambda) ; }
  void setLambda(const double at_zero,
		 const double at_infinity,
		 const double deriv_at_zero)
  { lambda .initStandard (at_zero, at_infinity, deriv_at_zero) ; }
  void setLambda(const vpAdaptativeGain& _l){lambda=_l;}

  void print(const vpServo::vpServoPrintType display_level=ALL,
	     std::ostream &os = std::cout) ;

} ;


#endif

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
