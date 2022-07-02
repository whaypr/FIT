###
#
# Testy související s metodou `inverse`.
#

@testset "$(rpad("inverse: non-invertible matrices", TITLE_WIDTH))" begin
    A = [1 2 3; 4 5 6]
    @test_throws ErrorException inverse(A)

    A = [1 1; 0 0]
    @test_throws ErrorException inverse(A)

    A = Rational{Int64}[1 1; 1 -1; 1//3 0]
    @test_throws ErrorException inverse(A)
end

@testset "$(rpad("inverse: computation over rational field, small examples", TITLE_WIDTH))" begin
    import LinearAlgebra: diagm

    A = Rational{Int64}[2 0; 0 3]
    @test inverse(A) == [1//2 0; 0 1//3]

    A = Rational{Int64}[1 1; 1 -1]
    @test inverse(A) == Rational{Int64}[1//2 1//2; 1//2 -1//2]

    H = [ 1 // (1 + j + k) for j=0:6, k=0:6]
    @test inverse(H) * H == diagm(ones(7))
end

@testset "$(rpad("inverse: computation in floating point arithmetic", TITLE_WIDTH))" begin
    import LinearAlgebra: diagm

    # inversion of the 11x11 Hilbert matrix fails miserably in Float64
    H = Float64[ 1 // (1 + j + k) for j=0:10, k=0:10 ]
    @test !(inverse(H) * H ≈ diagm(ones(11)))

    # inversion of the 11x11 Hilbert matrix succeeds in BigFloat
    H = BigFloat[ 1 // (1 + j + k) for j=0:10, k=0:10 ]
    @test inverse(H) * H ≈ diagm(ones(11))
end
