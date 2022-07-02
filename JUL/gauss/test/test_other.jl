###
#
# Testy pomocných metod
#

@testset "$(rpad("isZero: Exact", TITLE_WIDTH))" begin
    @test isZero(0) == true
    @test isZero(3) == false

    @test isZero(3//6) == false
    @test isZero(0//42) == true

    g1 = GaloisField(5)[9]
    @test isZero(g1[1]) == false

    g1 = GaloisField(5)[10]
    @test isZero(g1[1]) == true
end

@testset "$(rpad("isZero: Floating point", TITLE_WIDTH))" begin
    @test isZero(0.) == true
    @test isZero(3.) == false
    @test isZero( eps(Float64) / 2 ) == true

    @test isZero(4. + 3im) == false
    @test isZero(4. + 0im) == false
    @test isZero(0. + 3im) == false
    @test isZero(0. + 0im) == true
end

#------------------------------------------

@testset "$(rpad("pickPivot: Exact", TITLE_WIDTH))" begin
    @test pickPivot( [2, 5, 0] ) == 1
    @test pickPivot( [0, 0, 10] ) == 3

    @test pickPivot( [0//3, 5//9, 4//1] ) == 2
    @test pickPivot( [3, 8, 4//4] ) == 1
end

@testset "$(rpad("pickPivot: Floating point", TITLE_WIDTH))" begin
    @test pickPivot( [2., 5, 0] ) == 2
    @test pickPivot( [2, -5, 0.] ) == 2
    @test pickPivot( [55, 9., 55] ) == 1

    @test pickPivot( [0, 1. + 4im, 10 + 6im] ) == 3
    @test pickPivot( [0., 1 + 4im, -(10 + 6im)] ) == 3
end

#------------------------------------------

@testset "$(rpad("swapRows", TITLE_WIDTH))" begin
    A = [
        1 2 3
        4 5 6
        7 8 9
    ]

    swapRows(A, 1, 3)
    @test A == [ 7 8 9 ; 4 5 6 ; 1 2 3 ]

    swapRows(A, 3, 1)
    @test A == [ 1 2 3 ; 4 5 6 ; 7 8 9 ]

    swapRows(A, 2, 2)
    @test A == [ 1 2 3 ; 4 5 6 ; 7 8 9 ]
end
