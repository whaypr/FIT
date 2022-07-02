
@testset "$(rpad("find_peaks method", TITLE_WIDTH))" begin
  data = vcat([ j for j=1:50 ], [ j for j=49:-1:1 ])
  @test find_peaks(data, offset=13) == [50]

  data = [ abs(j/2) for j=-20:20 ]
  @test find_peaks(data, offset=13) == [1, 41]

  data = [ -abs(j/2) for j=-20:20 ]
  @test find_peaks(data, offset=13) == [21]
end

@testset "$(rpad("average method", TITLE_WIDTH))" begin
  data = fill(2.0, 10)
  @test average(data, offset=3) == data

  data = [2.5 * j for j in 1:5]
  @test average(data, offset=1) == [3.75, 5, 7.5, 10, 11.25]
end
