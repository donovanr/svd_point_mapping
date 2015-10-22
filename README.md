This is a Julia implementation of a solution to a point-mapping problem:
given a reference set of points, and another set of points, how do you rigidly
rotate and translate the second set of points back onto the first, as nearly as possible?
Scaling the second set is not considered here.

This is sometimes called the Procrustes problem, or the Kabsch problem (when no translation is present).

My code is heavily indebted to Olga Sorkine's very readable explaination of using a
singular value decomposition (SVD) to solve the problem: [svd_rot.pdf] (http://igl.ethz.ch/projects/ARAP/svd_rot.pdf)

The Euler angle conventions are (I believe) consistent with much of the physics literature,
but are taken directly from
[Eric Weisstein's "Euler Angles" From MathWorld] (http://mathworld.wolfram.com/EulerAngles.html)

I also implemeted the algorithm in C++ using the Eigen library (http://eigen.tuxfamily.org/).

The C++ code can be compiled using:
```g++ -I ../eigen/ align_svd.cpp -o align_svd```
but you may have to change the path to the Eigen libraries.

The C++ and Julia implementations give the same results for the test case I used (up to floating point error), but the actual affine transforms they use are different.
Not sure why this is the case.
