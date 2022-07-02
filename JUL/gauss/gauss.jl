###
#
# BI-JUL Zadání č. 1: Párty s Gaussem
#
# B211
#

import GaloisFields: AbstractGaloisField
using LinearAlgebra

@doc """

    echelon_form!(A::Matrix{T}; augmented = false) where {T <: Number}

Převede čtvercovou matici `A` do redukovaného horního stupňovitého tvaru.
Vrací tento horní stupňovitý tvar.

Pokud typ prvků matice `A` podporuje dělení (bez změny typu), pak `echelon_form!(A)`
modifikuje přímo matici `A`. Jinak matici přetypuje na vhodný typ (viz zadání) a vrátí
výsledek výpočtu, vstupní matici nechá tak jak je.
"""
function echelon_form!(A::Matrix{T}; augmented = false) where {T <: Number}
    matrix = T <: Integer ? convert(Matrix{Rational{T}}, A) : A

    # make almost zero elems actually zero
    for j = 1:size(matrix, 2)
        for i = 1:size(matrix, 1)
            isZero(matrix[i, j]) ? matrix[i, j] = 0 : nothing
        end
    end

    # GEM
    rowID = 1
    for colID in 1:size(matrix, 2) # iterate column IDs
        subcol = matrix[rowID:end, colID]

        if subcol == zeros( eltype(subcol), length(subcol) ) # skip if column is just zeros
            continue
        end

        rowID_pivot = pickPivot(subcol) + rowID - 1 # find row with pivot, id is global
        swapRows(matrix, rowID_pivot, rowID) # get pivot row on local top

        pivot = matrix[rowID, colID]
        matrix[rowID, :] /= pivot # make pivot one

        last = augmented && colID == size(matrix, 2) ? true : false

        lower =  last ? rowID + 1 : 1
        for rid in lower:1:size(matrix, 1) # make elems in pivot (leading one) column zero
            if rid == rowID # skip row with pivot
                continue
            end
            matrix[rid, :] -= matrix[rowID, :] * matrix[rid, colID]
        end

        if last
            matrix = matrix[ 1:size(matrix, 2)-1, : ]
            break
        end

        rowID += 1
    end

    matrix
end


@doc """

    inverse(A)

Pro zadanou čtvercovou matici `A` spočte jeji inverzi, je-li to možné.

Vyvolá výjimku `ErrorException` pokud nejde o čtvercovou matici, nebo tato čtvercová matice není regulární.
"""
function inverse(A)
    n = size(A, 1)

    if n != size(A, 2)
        error("Matrix is not rectangular!")
    end

    matrix = echelon_form!( [A I] )
    possible_I = matrix[:, 1:n]

    if possible_I != I
        error("Matrix is not regular!")
    end

    matrix[:, n+1:end]
end


@doc """

    solve(A, b)

Vrátí nějaké řešení soustavy lineárních rovnic s maticí `A` a pravou stranou `b`, je-li to možné.

Vyvolá výjimku `ErrorException` pokud soustava nemá řešení.
"""
function solve(A, b)
    if size(A, 1) != length(b)
        error("Sizes do not match!")
    end

    reduced = echelon_form!( [A b], augmented=true )
    left = reduced[:, 1:end-1]
    right = reduced[:, end]

    # handle zero rows
    solution_zeros::Vector = []
    while left[end, :] == zeros( eltype(left), size(left, 2) )
        if right[end] != 0
            error("No solution!")
        end

        pushfirst!(solution_zeros, 0)

        left = left[1:end-1, :]
        pop!(right)

        if length(right) == 0
            break
        end
    end

    ## calculate the rest of the solution
    solution::Vector = []
    num = 0
    for row in eachrow( left[end:-1:1, end:-1:1] ) # from bottom to top row, from last to first column
        total = 0
        for i in 1:num
            total += row[i] * solution[i]
        end

        num += 1

        val = (right[length(right)-num+1] - total) / row[num]
        pushfirst!(solution, val)
    end

    append!(solution_zeros, solution)
end


#
# Další pomocné metody
#

@doc """

    isZero(x::T) where {S <: Integer, T <: Union{S, Rational{S}, AbstractGaloisField}}

Rozhodne, jestli `x` je rovno nule, či nikoliv.
"""
isZero(x::T) where {S <: Integer, T <: Union{S, Rational{S}, AbstractGaloisField}} =
    x == 0


@doc """

    isZero(x::T) where {S <: AbstractFloat, T <: Union{S, Complex{S}}}

Rozhodne, jestli `x` je menší než odchylka `2eps(S)`, či nikoliv.
"""
isZero(x::T) where {S <: AbstractFloat, T <: Union{S, Complex{S}}} =
    abs(x) <= 2eps(S)

#------------------------------------------

@doc """

    pickPivot(col::Vector{T}) where {S <: Integer, T <: Union{S, Rational{S}, AbstractGaloisField}}

Vybere pivota sloupce (nebo části) 'col' nějaké matice. Vrátí jeho index (relativně k zadané části).

Pivot je roven zeshora prvnímu nenulovému prvku.
"""
pickPivot(col::Vector{T}) where {S <: Integer, T <: Union{S, Rational{S}, AbstractGaloisField}} =
    findfirst(!isZero, col)


@doc """

    pickPivot(col::Vector{T}) where {S <: AbstractFloat, T <: Union{S, Complex{S}}} =

Vybere pivota sloupce (nebo jeho části) `col` nějaké matice. Vrátí jeho index (relativně k zadané části).

Pivot je roven prvku s největší možnou absolutní hodnotou.
"""
pickPivot(col::Vector{T}) where {S <: AbstractFloat, T <: Union{S, Complex{S}}} =
    argmax( abs.(col) )

#------------------------------------------

@doc """

    swapRows(matrix::S, i::T, j::T) where {S <: Matrix, T <: Integer}

Prohodí `i`-tý a `j`-tý řádek matice `matrix`.
"""
function swapRows(matrix::S, i::T, j::T) where {S <: Matrix, T <: Integer}
    r_i = matrix[i,:]
    matrix[i,:] = matrix[j,:]
    matrix[j,:] = r_i
end
