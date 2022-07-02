"""

This package provides very basic methods for manipulation of simple
continuous undirected graphs and ability to draw planar representations
of graphs using a simple variant of force-directed algorithm.

"""
module MyGraph

using Bijections
using DataStructures

export Graph,
        edges, count_edges, vertices, count_vertices, neighborhood,
        add_edge!, remove_vertex!, remove_edge!, adjacency_matrix


#---------#
#  GRAPH  #
#---------#

"""

    Graph(es::Vector)

Mutable structure representing continuous undirected graph withhout
multiple edges and loops.
Constructor throws an error if any of the above is violated.

Vertices can be integers or strings (or both).
When there are both types, integers are always sorted before strings.

Data is held in a vector of tuples data structure representing graph edges.

```jldoctest
julia> Graph([])
Vertices [0]:

Edges [0]:
Matrix{Bool}(undef, 0, 0)

```

```jldoctest
julia> Graph( [(1,2), (2,3)] )
Vertices [3]:
1 2 3 
Edges [2]:
Bool[0 1 0; 1 0 1; 0 1 0]

```

"""
mutable struct Graph
    edges::Vector{Tuple}

    function Graph(es::Vector)
        edges::Vector{Tuple} = []

        sort!(es)
        prev = (0,0)
        for (i,j) in es
            if i == j || prev == (i,j) || prev == (j,i)
                error("Loops and multiple edges are prohibited!")
            end

            push!(edges, (i,j))
            prev = (i,j)
        end

        G = new(edges)

        if length(es) == 0 || length(es) == 1
            return G
        end
        continuous(G) || error("Graph is not continuous!")

        G
    end
end


#--------#
# BASICS #
#--------#

"""

    edges(G::Graph)

Returns vector of edges of graph `G`.

"""
edges(G::Graph) = G.edges


"""

    count_edges(G::Graph)

Returns number of edges of graph `G.

"""
count_edges(G::Graph) = length(G.edges)


"""

    vertices(G::Graph)

Returns vector of vertices of graph `G`.

"""
vertices(G::Graph) = unique( collect( Iterators.flatten(G.edges) ) )


"""

    count_vertices(G::Graph)

Returns number of vertices of graph `G.

"""
count_vertices(G::Graph) = length( unique( collect( Iterators.flatten(G.edges) ) ) )


"""

    neighborhood(G::Graph, vertex)

Returns neighbors of vertex `vertex` in graph `G`.

"""
function neighborhood(G::Graph, vertex)
    res = []
    for (i,j) in G.edges
        i == vertex && push!(res, j)
        j == vertex && push!(res,i)
    end
    res
end


"""

    add_edge!(G::Graph, edge)

Adds edge `edge` to graph `G`.

Checks for loops, multiple edges and continuity.
If violated, throws an error.

Returns graph `G`.

"""
function add_edge!(G::Graph, edge)
    i,j = edge
    errmsg = "Loops and multiple edges are prohibited!"
    connected = false

    i == j && error(errmsg)
    for e in G.edges
        for x in 1:2
            if i == e[3-x]
                connected = true
                j == e[x] && error(errmsg)
            end
        end
    end

    if ! connected && length(G.edges) > 0
        error("Adding this edge would make the graph discontinuous!")
    end

    push!(G.edges, edge)
    G
end


"""

    remove_vertex!(G::Graph, vertex)

Removes all edges from graph `G` where vertex `vertex` is present.

If vertex is not present, nothing happens.

Returns graph `G`.

"""
function remove_vertex!(G::Graph, vertex)
    backup = deepcopy(G.edges)

    filter!( x -> x[1] != vertex && x[2] != vertex, G.edges )

    if ! continuous(G)
        G.edges = backup
        error("Removing this vertex would make the graph discontinuous!")
    end

    G
end


"""

    remove_edge!(G::Graph, edge)

Removes edge `edge` from graph `G`.

If edge is not present, nothing happens.

Error is thrown and operation is not done if resulting graph is discontinuous.

Returns graph `G`.

"""
function remove_edge!(G::Graph, edge)
    backup = deepcopy(G.edges)

    i,j = edge
    filter!( x -> x != (i,j) && x != (j,i), G.edges )

    if ! continuous(G)
        G.edges = backup
        error("Removing this edge would make the graph discontinuous!")
    end

    G
end


"""

    adjacency_matrix(G::Graph)

Returns adjacency matrix of graph `G` where vertices are in increasing order.

Integers are always sorted before strings.

"""
function adjacency_matrix(G::Graph)
    vmap = Dict() # vertex to matrix index mapping
    for v in sort(vertices(G))
        vmap[v] = length(vmap) + 1
    end

    res = zeros( Bool, count_vertices(G), count_vertices(G) )
    for (i,j) in G.edges
        res[vmap[i],vmap[j]] = res[vmap[j],vmap[i]] = true
    end

    res
end


#---------#
# DRAWING #
#---------#

"""

    coordinates(G::Graph; alpha = 2, beta = 1​)

Assignes plane coordinates to all vertices in graph `G`.

Returns a pair of vectors of x- and y-coordinates, respectively.

"""
function coordinates(G::Graph; alpha = 2, beta = 1)
    xs = []
    ys = []

    (xs, ys)
end


"""

    draw(G::Graph; filename = nothing, labels = true)

Draws graphical representation of graph `G`.

Edges are drawn as lines, vertices are drawn as dots.

If `filename` is specified, saves image to file instead.
If `labels` argument is set to `true`, display vertices labels.

"""
function draw(G::Graph; filename = nothing, labels = true)
end


#---------#
# HELPERS #
#---------#

"""

    continuous(G::Graph)

Continuity check using DFS algorithm.

Returns `true` if graph `G` is continuous or `false` if it is discontinuous.

"""
function continuous(G::Graph)
    length(G.edges) == 0 && return true

    mat = adjacency_matrix(G)

    vmap = Bijection() # vertex to matrix index mapping
    vfound = Dict()
    for v in sort(vertices(G))
        vmap[v] = length(vmap) + 1
        vfound[v] = false
    end

    s = Stack{Union{Integer, AbstractString}}()
    push!( s, first(vmap)[1] )
    while length(s) > 0
        v = pop!(s)
        vfound[v] = true

        for i in 1:size(mat, 2)
            if mat[vmap[v], i] && ! vfound[vmap(i)]
                push!(s, vmap(i))
            end
        end
    end

    sum(x -> !x[2], vfound) == 0
end


# THIS IS TYPE PIRACY!
Base.isless(s::AbstractString, i::Integer) = false
Base.isless(i::Integer, s::AbstractString) = true


function Base.show(io::IO, G::Graph)
    println(io, "Vertices [", count_vertices(G), "]:")
    for v in vertices(G)
        print(io, v, " ")
    end
    println(io, "")

    println(io, "Edges [", count_edges(G), "]:")
    println(io, adjacency_matrix(G))
end

end # module
