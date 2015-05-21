#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;
using namespace std;

// Given two sets of 3D points, find the rotation + translation
// which best maps the first set to the second.
// Source: http://en.wikipedia.org/wiki/Kabsch_algorithm

// this cpp code is from https://github.com/oleg-alexandrov/projects/blob/master/eigen/Kabsch.cpp

// The input 3D points are stored as columns.
Eigen::Affine3d Find3DAffineTransform(Eigen::Matrix3Xd in, Eigen::Matrix3Xd out) {

  // Default output
  Eigen::Affine3d A;
  A.linear() = Eigen::Matrix3d::Identity(3, 3);
  A.translation() = Eigen::Vector3d::Zero();

  if (in.cols() != out.cols())
    throw "Find3DAffineTransform(): input data mis-match";

  // Find the centroids then shift to the origin
  Eigen::Vector3d in_ctr = Eigen::Vector3d::Zero();
  Eigen::Vector3d out_ctr = Eigen::Vector3d::Zero();
  for (int col = 0; col < in.cols(); col++) {
    in_ctr  += in.col(col);
    out_ctr += out.col(col);
  }
  in_ctr /= in.cols();
  out_ctr /= out.cols();
  for (int col = 0; col < in.cols(); col++) {
    in.col(col)  -= in_ctr;
    out.col(col) -= out_ctr;
  }

  // SVD
  Eigen::MatrixXd Cov = in * out.transpose();
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(Cov, Eigen::ComputeThinU | Eigen::ComputeThinV);

  // Find the rotation
  double d = (svd.matrixV() * svd.matrixU().transpose()).determinant();
  if (d > 0)
    d = 1.0;
  else
    d = -1.0;
  Eigen::Matrix3d I = Eigen::Matrix3d::Identity(3, 3);
  I(2, 2) = d;
  Eigen::Matrix3d R = svd.matrixV() * I * svd.matrixU().transpose();

  // The final transform
  A.linear() = R;
  A.translation() = out_ctr - R*in_ctr;

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
 
  // create a rotation matrix from a normalized quaternion
  Eigen::Quaternion<double> Q(1.0, 2.0, 3.0, 4.0);
  Q.normalize();
  // print it out
  Eigen::Matrix3d intialrot = Q.toRotationMatrix();
  cout << "intialrot =" << endl << intialrot << endl;

  // rotate the intial points
  newpoints = intialrot*oldpoints;

  // and translate them
  Vector3d initialtrans(7.0,4.0,5.0);
  // print out the translation
  cout << "intialtrans =" << endl << initialtrans << endl;
  // apply the translation to the rotated points
  for (int col = 0; col < newpoints.cols(); col++) {
    newpoints.col(col) += initialtrans;
  }

  // print out the result of apping the translation to the rotated points
  cout << "newpoints =" << endl << newpoints << endl;
 
  // use svd alignment algorithm defined above
  Eigen::Affine3d A = Find3DAffineTransform(newpoints, oldpoints);

  // print out the transformation that aligns the points
  cout << "A.linear =" << endl << A.linear() << endl;
  cout << "A.translation =" << endl << A.translation() << endl;

  // apply transformation to restore the points
  remappedpoints = A.linear()*newpoints;
  for (int col = 0; col < remappedpoints.cols(); col++) {
    remappedpoints.col(col) += A.translation();
  }
  
  // print out the remapped points
  cout << "remappedpoints =" << endl << remappedpoints << endl;
}

