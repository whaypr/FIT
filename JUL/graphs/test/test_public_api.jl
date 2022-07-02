
@testset "$(rpad("Graph constructor", TITLE_WIDTH))" begin
  @test isa(Graph([]), Graph)
  @test isa(Graph([(1, 2)]), Graph)
  @test isa(Graph([(1, "a")]), Graph)
  @test isa(Graph([("a", "b")]), Graph)

  @test_throws ErrorException Graph([(1, 1)])
  @test_throws ErrorException Graph([(1, 2), (2, 1)])
  @test_throws ErrorException Graph([(1, 2), (1, 2)])

  # not continuous
  @test_throws ErrorException Graph([(1, 2), (3, 4)])
end

@testset "$(rpad("edges and count_edges methods", TITLE_WIDTH))" begin
  G = Graph([(3, 2), (2, 1)])

  @test sort(edges(G)) == [(2, 1), (3, 2)]
  @test count_edges(G) == 2

  G = Graph([("a", "b"), ("b", "c")])

  @test sort(edges(G)) == [("a", "b"), ("b", "c")]
  @test count_edges(G) == 2

  G = Graph([("a", 2), ("b", 2)])

  @test sort(edges(G)) == [("a", 2), ("b", 2)]
  @test count_edges(G) == 2
end

@testset "$(rpad("vertices and count_vertices methods", TITLE_WIDTH))" begin
  G = Graph([(3, 2), (2, 1)])

  @test sort(vertices(G)) == [1, 2, 3]
  @test count_vertices(G) == 3

  G = Graph([("c", "d"), ("a", "d")])

  @test sort(vertices(G)) == ["a", "c", "d"]
  @test count_vertices(G) == 3

  G = Graph([(3, "a"), ("a", 1)])

  # This is hackish, but apparently `isless(::String, ::Int64)` is
  # not defined. Why does it work with tuples above?
  @test sort(string.(vertices(G))) == ["1", "3", "a"]
  @test count_vertices(G) == 3
end

@testset "$(rpad("neighborhood method", TITLE_WIDTH))" begin
  G = Graph([(3, 2), (2, 1)])

  @test sort(neighborhood(G, 1)) == [2]
  @test sort(neighborhood(G, 2)) == [1, 3]

  G = Graph([(1, 2), (2, 3), (3, 1)])

  @test sort(neighborhood(G, 1)) == [2, 3]

  G = Graph([("a", "b"), ("b", "d")])

  @test sort(neighborhood(G, "a")) == ["b"]
  @test sort(neighborhood(G, "b")) == ["a", "d"]

  G = Graph([(1, 2), (2, "b"), (3, "b")])

  # see note on lines 32-33.
  @test sort(string.(neighborhood(G, 2))) == ["1", "b"]
end

@testset "$(rpad("add_edge! method", TITLE_WIDTH))" begin
  # old vertices
  G = Graph([(3, 2), (2, 1)])

  add_edge!(G, (3, 1))

  @test count_edges(G) == 3 
  @test (3, 1) in edges(G)
  @test sort(neighborhood(G, 3)) == [1, 2]
  @test sort(neighborhood(G, 1)) == [2, 3]

  # new vertex
  G = Graph([(1, 2), (2, 3)])

  add_edge!(G, (3, 4))

  @test count_edges(G) == 3
  @test count_vertices(G) == 4
  @test 4 in vertices(G)
  @test (3, 4) in edges(G)
  @test sort(neighborhood(G, 3)) == [2, 4]
  @test sort(neighborhood(G, 4)) == [3]

  # prohibited actions
  G = Graph([])

  @test_throws ErrorException add_edge!(G, (1, 1))

  add_edge!(G, (1, 2))
  @test_throws ErrorException add_edge!(G, (1, 2))
  @test_throws ErrorException add_edge!(G, (2, 1))

  # not continuous
  G = Graph([ (2,3), (2,6)] )

  @test_throws ErrorException add_edge!(G, (4, 1))
end

@testset "$(rpad("remove_edge! method", TITLE_WIDTH))" begin
  G = Graph([(1, 2)])

  remove_edge!(G, (1, 2))

  @test count_edges(G) == 0
  @test count_vertices(G) == 0

  G = Graph([(1, 2), (2, 3)])

  remove_edge!(G, (1, 2))

  @test count_edges(G) == 1
  @test edges(G) == [(2, 3)]
  @test count_vertices(G) == 2
  @test sort(vertices(G)) == [2, 3]

  G = Graph([(1, 2), (2, 3), (3, 1)])

  remove_edge!(G, (1, 2))

  @test count_edges(G) == 2
  @test sort(edges(G)) == [(2, 3), (3, 1)]
  @test count_vertices(G) == 3
  @test sort(vertices(G)) == [1, 2, 3]

  G = Graph([(1, 2), (2, 3), (3, 1)])

  remove_edge!(G, (2, 1))

  @test count_edges(G) == 2
  @test sort(edges(G)) == [(2, 3), (3, 1)]
  @test count_vertices(G) == 3
  @test sort(vertices(G)) == [1, 2, 3]

  # not continuous
  G = Graph( [(1,2), (2,3), (3,4), (4,5)] )

  @test_throws ErrorException remove_edge!(G, (2, 3))
end

@testset "$(rpad("remove_vertex!", TITLE_WIDTH))" begin
  G = Graph([(1, 2), (2, 3), (3, 1)])

  remove_vertex!(G, 1)

  @test count_edges(G) == 1
  @test edges(G) == [(2, 3)]
  @test count_vertices(G) == 2
  @test sort(vertices(G)) == [2, 3]

  # not continuous
  G = Graph( [("a","b"), ("b","c"), ("c","d"), ("d","e")] )

  @test_throws ErrorException remove_vertex!(G, "c")
end

@testset "$(rpad("adjacency_matrix", TITLE_WIDTH))" begin
  G = Graph([(1, 2), (2, 3), (3, 4)])

  @test adjacency_matrix(G) == [
    0 1 0 0
    1 0 1 0
    0 1 0 1
    0 0 1 0
  ]
end
