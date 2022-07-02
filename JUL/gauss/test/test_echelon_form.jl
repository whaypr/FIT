###
#
# Testy související s `echelon_form!`.
#

@testset "$(rpad("echelon_form!: integer and trivial examples", TITLE_WIDTH))" begin
    #
    # Note: Matrices with integer-like entries are not modified.
    #       Instead, `echelon_form!` returns a new matrix.
    #
    A = [0 0; 0 0]
    @test echelon_form!(A) == [0 0; 0 0]

    A = [0 0 1; 0 0 0]
    @test echelon_form!(A) == [0 0 1; 0 0 0]

    A = [1 1 1; 0 0 0]
    @test echelon_form!(A) == [1 1 1; 0 0 0]

    A = ones(Int64, 3, 1)
    B = ones(Int64, 3, 1); B[2] = 0; B[3] = 0
    @test echelon_form!(A) == B

    A = [1 1 1; 1 1 1]
    @test echelon_form!(A) == [1 1 1; 0 0 0]

    A = [1 0 0; 1 0 0; 1 0 0]
    @test echelon_form!(A) == [1 0 0; 0 0 0; 0 0 0]

    A = [0 0 1; 0 1 0; 1 0 0]
    @test echelon_form!(A) == [1 0 0; 0 1 0; 0 0 1]

    A = [1 1 1; 1 2 3; 2 3 4]
    @test echelon_form!(A) == [1 0 -1; 0 1 2; 0 0 0]
end

@testset "$(rpad("echelon_form!: over rational numbers", TITLE_WIDTH))" begin
    A = Rational{Int64}[2 0 0; 0 3 0; 0 0 4]
    echelon_form!(A)
    @test A == [1 0 0; 0 1 0; 0 0 1]

    A = Rational{Int64}[2 0 0; 0 3 4]
    echelon_form!(A)
    @test A == [1 0 0; 0 1 4//3]

    A = Rational{Int64}[1 1 1; 2 0 0]
    echelon_form!(A)
    @test A == [1 0 0; 0 1 1]

    A = Rational{Int64}[1 1; 2 1; 1 2; -3 3]
    echelon_form!(A)
    @test A == [1 0; 0 1; 0 0; 0 0]
end

@testset "$(rpad("echelon_form!: GaloisField, GF(5)", TITLE_WIDTH))" begin
    using GaloisFields

    Z5 = GaloisField(5)

    A = Z5[2 0 0; 0 3 0; 0 0 4]
    echelon_form!(A)
    @test A == Z5[1 0 0; 0 1 0; 0 0 1]

    A = Z5[1 2 3; 4 3 2]
    echelon_form!(A)
    @test A == Z5[1 2 3; 0 0 0]

    Z13 = GaloisField(13)
    A = Z13[1 2 3 4; 5 6 7 8; 9 10 11 12]
    echelon_form!(A)
    @test A == Z13[1 0 12 11; 0 1 2 3; 0 0 0 0]

    Z2411 = GaloisField(2411)
    A = Z2411[1111 2222 1212 12; 3 78 42 666; 777 1717 2000 1]
    echelon_form!(A)
    @test A == Z2411[1 0 0 1710; 0 1 0 833; 0 0 1 1791]
end

@testset "$(rpad("echelon_form!: Float64", TITLE_WIDTH))" begin
    eps64 = eps(Float64)

    A = [1. 2.; eps64/2 eps64/2]
    echelon_form!(A)
    @test A ≈ [1. 2.; eps64/2 eps64/2]

    A = [1. -1. 2. -2.; 1. 0. 1. 0; 2. -1. 3. -2.]
    echelon_form!(A)
    @test A ≈ [1. 0. 1. 0.; 0. 1. -1. 2.; 0. 0. 0. 0.]

    A = [1 1/2 1/3; 1/2 1/3 1/4; 1/3 1/4 1/5]
    echelon_form!(A)
    @test A ≈ [1 0 0; 0 1 0; 0 0 1]
end

@testset "$(rpad("echelon_form!: BigFloat", TITLE_WIDTH))" begin
    epsBF = eps(BigFloat)

    A = [1. 2.; epsBF/2 epsBF/2]
    echelon_form!(A)
    @test A ≈ [1. 2.; epsBF/2 epsBF/2]

    A = BigFloat[1 -1 2 -2; 1 0 1 0; 2 -1 3 -2]
    echelon_form!(A)
    @test A ≈ BigFloat[1 0 1 0; 0 1 -1 2; 0 0 0 0]

    A = BigFloat[1 1/2 1/3; 1/2 1/3 1/4; 1/3 1/4 1/5]
    echelon_form!(A)
    @test A ≈ BigFloat[1 0 0; 0 1 0; 0 0 1]
end
