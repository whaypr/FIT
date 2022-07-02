using MyGraph
using Documenter

DocMeta.setdocmeta!(MyGraph, :DocTestSetup, :(using MyGraph); recursive=true)

makedocs(;
    modules=[MyGraph],
    authors="Patrik Drbal <drbalpat@fit.cvut.cz> and contributors",
    repo="https://gitlab.fit.cvut.cz/drbalpat/MyGraph.jl/blob/{commit}{path}#{line}",
    sitename="MyGraph.jl",
    format=Documenter.HTML(;
        prettyurls=get(ENV, "CI", "false") == "true",
        canonical="https://drbalpat.gitlab.io/MyGraph.jl",
        assets=String[],
    ),
    pages=[
        "Home" => "index.md",
    ],
)
