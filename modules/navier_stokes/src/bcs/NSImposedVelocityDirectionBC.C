/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "NSImposedVelocityDirectionBC.h"

// Full specialization of the validParams function for this object
template<>
InputParameters validParams<NSImposedVelocityDirectionBC>()
{
  // Initialize the params object from the base class
  InputParameters params = validParams<NodalBC>();

  // Coupled variables
  params.addRequiredCoupledVar("rho", "");
  params.addRequiredCoupledVar("u", "");
  params.addRequiredCoupledVar("v", "");
  params.addCoupledVar("w", ""); // only required in 3D

  // Coupled parameters
  params.addRequiredParam<Real>("desired_unit_velocity_component", "");

  return params;
}




// Constructor, be sure to call the base class constructor first!
NSImposedVelocityDirectionBC::NSImposedVelocityDirectionBC(const InputParameters & parameters)
    : NodalBC(parameters),

      // Coupled variables
      _rho(coupledValue("rho")),
      _u_vel(coupledValue("u")),
      _v_vel(coupledValue("v")),
      _w_vel(_mesh.dimension() == 3 ? coupledValue("w") : _zero),

      _desired_unit_velocity_component(getParam<Real>("desired_unit_velocity_component"))
{}



// Specialization of the computeQpResidual function for this class
Real NSImposedVelocityDirectionBC::computeQpResidual()
{
  // The velocity vector
  RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

  // Specify desired velocity component
  return _u[_qp] - _rho[_qp] * _desired_unit_velocity_component * vel.size();
}


