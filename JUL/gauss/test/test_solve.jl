###
#
# Testy související se `solve`.
#

@testset "$(rpad("solve: non-solvable examples", TITLE_WIDTH))" begin
    A = [0.0 1.0; 0.0 0.0]
    b = [1.0, 1.0]
    @test_throws ErrorException solve(A, b)

    A = [1.0 1.0; 2.0 2.0]
    b = [1.0, 1.0]
    @test_throws ErrorException solve(A, b)
end

@testset "$(rpad("solve: simple solvable examples", TITLE_WIDTH))" begin
    A = [0 0; 0 0]
    b = [0, 0]
    x = solve(A, b)
    @test A * x == b

    A = [1 0; 0 2]
    b = [3, 4]
    x = solve(A, b)
    @test x == [3, 2]

    A = [1 2 3; 0 0 0; 0 0 0]
    b = [1, 0, 0]
    x = solve(A, b)
    @test A * x == b

    A = [0 0 1; 0 1 0; 1 0 0]
    b = [1, 2, 3]
    x = solve(A, b)
    @test x == [3, 2, 1]
end

@testset "$(rpad("solve: a badly conditioned system", TITLE_WIDTH))" begin
    delta = 1e-6
    A  = [1. 1.0-delta; 1. 1.0+delta]
    x  = [2., 3.]
    b1 = A * x
    b2 = b1 + 1e-6 * rand(2)

    z = solve(A, b2)
    @test A * z ≈ b2
end

@testset "$(rpad("solve: randomized tests", TITLE_WIDTH))" begin
    # Galois fields
    using GaloisFields

    Z11 = GaloisField(11)
    A   = rand(Z11, 10, 10)
    x   = rand(Z11, 10)
    b   = A * x

    z = solve(A, b)
    @test A * z == b

    # Float64
    A = rand(3, 3)
    x = rand(3)
    b = A * x

    z = solve(A, b)
    @test A * z ≈ b

    A = rand(6, 3)
    x = rand(3)
    b = A * x

    z = solve(A, b)
    @test A * z ≈ b
end
