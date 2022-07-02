
@testset "coordinates method" begin
  G = Graph([(1, 2), (2, 3), (3, 4)])

  data = MyGraph.coordinates(G)

  @test isa(data, Tuple{Vector{Float64}, Vector{Float64}})
  @test length(data[1]) == 4
  @test length(data[2]) == 4
end
