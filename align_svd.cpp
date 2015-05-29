#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;
using namespace std;

// Given two sets of 3D points, find the rotation + translation
// which best maps the first set to the second.
// Source: http://en.wikipedia.org/wiki/Kabsch_algorithm

// this cpp code is modified from https://github.com/oleg-alexandrov/projects/blob/master/eigen/Kabsch.cpp

// The input 3D points are stored as columns.
Affine3d Find3DAffineTransform(Matrix3Xd P, Matrix3Xd Q) {

  // Default output
  Affine3d A;
  A.linear() = Matrix3d::Identity(3, 3);
  A.translation() = Vector3d::Zero();

  if (P.cols() != Q.cols())
    throw "Find3DAffineTransform(): input data mis-match";

  // Center the data
  Vector3d p = P.rowwise().mean();
  Vector3d q = Q.rowwise().mean();

  Matrix3Xd X = P.colwise() - p;
  Matrix3Xd Y = Q.colwise() - q;

  // SVD
  MatrixXd Cov = X*Y.transpose();
  JacobiSVD<MatrixXd> svd(Cov, ComputeThinU | ComputeThinV);

  // Find the rotation, and prevent reflections
  Matrix3d I = Matrix3d::Identity(3, 3);
  double d = (svd.matrixV()*svd.matrixU().transpose()).determinant();
  (d > 0.0) ? d = 1.0 : d = -1.0;
  I(2, 2) = d;
  
  Matrix3d R = svd.matrixV()*I*svd.matrixU().transpose();

  // The final transform
  A.linear() = R;
  A.translation() = q - R*p;

  return A;
}


int main()
{
  // test the above alignment algorithm

  // choose four points in 3-space
  Vector3d oldpoints1(1.0,2.0,3.0);
  Vector3d oldpoints2(4.0,5.0,6.0);
  Vector3d oldpoints3(7.0,8.0,9.0);
  Vector3d oldpoints4(0.0,1.0,2.0);
  // make them a matrix
  Matrix<double,3,4> oldpoints,newpoints,remappedpoints;
  oldpoints.col(0)=oldpoints1;
  oldpoints.col(1)=oldpoints2;
  oldpoints.col(2)=oldpoints3;
  oldpoints.col(3)=oldpoints4;
  // print them out
  cout << "oldpoints =" << endl << oldpoints << endl;

  // create a rotation matrix from a normalized quaternion and a translation vector
  Quaternion<double> H(1.0, 2.0, 3.0, 4.0);
  H.normalize();
  Matrix3d intialrot = H.toRotationMatrix(); 
  Vector3d initialtrans(7.0,4.0,5.0);

  newpoints = (intialrot*oldpoints).colwise() + initialtrans;

  // print out the result of apping the translation to the rotated points
  cout << "newpoints =" << endl << newpoints << endl;
 
  // use svd alignment algorithm defined above
  Affine3d A = Find3DAffineTransform(newpoints, oldpoints);

  // print out the transformation that aligns the points
  cout << "A.linear =" << endl << A.linear() << endl;
  cout << "A.translation =" << endl << A.translation() << endl;

  // apply transformation to restore the points
  remappedpoints = (A.linear()*newpoints).colwise() + A.translation();

  // print out the remapped points
  cout << "remappedpoints =" << endl << remappedpoints << endl;
}

