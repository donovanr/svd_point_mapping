module align_svd

# This function finds the optimal rigid rotation R and translation t to map points P onto points Q.
# P and Q should be d x n matrices, where D is the dimension of space and N is the number of points,
# i.e., they should be short and long matrices, not tall and skinny.

function transformation(P,Q)
    size(P) == size(Q) || error("The two sets of points must be the same size.")
    
    d,n = size(P)
    d < n || error("You should specify more points than dimensions. Are your matrices transposed?")
    
    p = mean(P,2)
    q = mean(Q,2)

    X = P .- p
    Y = Q .- q

    U,S,V = svd(X*Y') # julia svd returns V, not V'

    S = eye(d)
    S[d,d] = sign(det(V*U')) # prohibit reflections

    R = V*S*U'
    t = q - R*p

    return (R,t)
end

# This function maps points P back onto reference points Q, returning the best fit remapped points M.
# P and Q should be d x n matrices, where D is the dimension of space and N is the number of points,
# i.e., they should be short and long matrices, not tall and skinny.

function map_points(P,Q)
    R,t = transformation(P,Q)
    M = R*P .+ t
end

end
