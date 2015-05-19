module rots

# return 2d rotation matrix implied by single angle
function rot_2D(θ)
    [cos(θ) -sin(θ);
     sin(θ)  cos(θ)]
end

# return 3d rotation matrix implied by the three euler angles
# uses z-x-z' convention
function rot_3D(θ,ϕ,ψ)
    R11 =  cos(ψ)*cos(ϕ) - cos(θ)*sin(ϕ)*sin(ψ)
    R12 =  cos(ψ)*sin(ϕ) + cos(θ)*cos(ϕ)*sin(ψ)
    R13 =  sin(ψ)*sin(θ)
    R21 = -sin(ψ)*cos(ϕ) - cos(θ)*sin(ϕ)*cos(ψ)
    R22 = -sin(ψ)*sin(ϕ) + cos(θ)*cos(ϕ)*cos(ψ)
    R23 =  cos(ψ)*sin(θ)
    R31 =  sin(θ)*sin(ϕ)
    R32 = -sin(θ)*cos(ϕ)
    R33 =  cos(θ)
    R = [R11 R12 R13;
         R21 R22 R23;
         R31 R32 R33]
end

end
