/*
 * =====================================================================================
 *
 *  Filename:  quatinterface.cpp
 *
 *  Description: We created the following file to help with the interfacing
 *               between the different classes from pinocchio and the dual quaternion
 *               skinning library. Since they both used data classes defined by themselves,
 *               we had to convert between the different data classes. For example, while
 *               Pinocchio uses the Vector3 class, the DQS library uses a Vec3 class.
 *
 *  Version:  1.0
 *  Created:  03/14/2015 07:01:53 PM
 *  Revision:  none
 *  Compiler:  gcc
 *
 *  Author: Group F, CMPUT414 Winter 2015
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "quatinterface.h"
#include "vector.h"

using namespace Tbx;

namespace Pinocchio {

/* 
 * Given a transofrmation matrix, it converts it to a dual quaternion and returns that */
Dual_quat_cu getQuatFromMat(Transform<> matrix)
{
  Mat3 mat3;
  Vector<double, 3> trans;
  Vec3 transVec3;
  Quaternion<double> quat;
  Dual_quat_cu dualQuat;
  Transfo transfo;
  //int x = 0, y = 1, z = 2;

  // Get translation from matrix, convert it to Vec3
  trans = matrix.getTrans();
  transVec3 = Vec3(trans[0], trans[1], trans[2]);

  // Get rotation axis and angle from matrix
  quat = matrix.getRot();
  double angle = quat.getAngle();
  Vector<double, 3> axis = quat.getAxis();
  Vec3 axisVec3 = Vec3(axis[0], axis[1], axis[2]);

  // Create rotation quaternion from axis and angle
  Quat_cu rotquat = Quat_cu(axisVec3, (float) angle);

  // create the dual quaternion from rotation quaternions and translation vector
  dualQuat = Dual_quat_cu(rotquat, transVec3);
  return dualQuat;
}


/* 
 * Given a point (Vector3 object) and a dual quaternion, this functions applies
 * the transformation defined by the dual quaternion on the point.
 */
Vector3 transformPoint(Vector3 vpos, Dual_quat_cu &dquat_blend)
{
  Point3 restPos = Point3(vpos[0], vpos[1], vpos[2]);
  Point3 newPos = dquat_blend.transform(restPos);
  return Vector3(newPos.x, newPos.y, newPos.z);
}

} // namespace Pinocchio
