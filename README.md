This is a Julia implementation of a solution to a point-mapping problem:
given a reference set of points, and another set of points, how do you rigidly
rotate and translate the second set of points back onto the first, as nearly as possible?
Scaling the second set is not considered here.

This is sometimes called the Procrustes problem, or the Kabsch problem (when no translation is present).

My code is heavily indebted to Olga Sorkine's very readable explaination of using a
singular value decomposition (SVD) to solve the problem:
`http://igl.ethz.ch/projects/ARAP/svd_rot.pdf`

The Euler angle conventions are (I believe) consistent with much of the physics literature,
but are taken directly from:
`http://mathworld.wolfram.com/EulerAngles.html`

