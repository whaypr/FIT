##
#
# Pomocné funkce pro Covid experimenty.
#

using DataStructures, Multisets


"""

    find_peaks(data; offset=13)

Najde indexy "peaků" v číselném vektoru `data`, kde "peak" je definován jako
prvek `data` s indexem `j`, který je větší než všechny hodnoty s indexy
`j-offset` až `j-1` a `j+1` až `j+offset`. Tento rozsah na krajích vektorů
případně omezíme (tj. v extrémním případě třeba prvního prvku je levé "okolí" prázdné a díváme se pouze o `offset` doprava).

"""
function find_peaks(data::Vector{T}; offset=13) where { T <: Number }
    peaks = [] # result vector
    LRmax = BinaryMaxHeap{T}(data[2:offset+1]) # maximums for both left and right surrounding
    to_be_deleted = Multiset{T}() # helper multiset (basically a dict) of items which will be eventually removed from the heap

    len = length(data)

    data[1] > first(LRmax) && append!(peaks, 1)

    for curr in 2:len
        # delete
        left  = curr - offset
        left > 1 && push!(to_be_deleted, data[left - 1])

        push!(to_be_deleted, data[curr])

        # truly delete deleted values if on top
        while first(LRmax) in to_be_deleted
            to_be_deleted[ first(LRmax) ] -= 1
            pop!(LRmax)
        end

        # add
        push!(LRmax, data[curr - 1])

        right = curr + offset
        right <= len && push!(LRmax, data[right])

        ## add new peak if it is really a peak
        data[curr] > first(LRmax) && append!(peaks, curr)
    end

    peaks
end


"""

    average(data; offset=3)

Vyhladí číselný vektor `data` jednoduchým průměrováním. Tj. hodnota s indexem
`j` je nahrazena průměrnou hodnotou prvků s indexy `j-offset` až `j+offset`.
Na krajích vektoru tento rozsah případně omezíme prvním a posledním prvkem (průměruje se pak tedy menší počet prvků).

"""
function average(data::Vector{T}; offset=3) where { T <: Number }
    len = length(data)
    avgs = zeros(Number, len) # result vector

    asum = sum(data[1:offset+1])

    avgs[1] = asum / (offset + 1)

    for curr in 2:len
        # update (inclusive) bounds
        left  = max(1, curr - offset)
        right = min(curr + offset, len + 1)

        # update number of items
        alen = right - left
        alen += (right < len + 1) ? 1 : 0

        # update sum
        left   > 1   ? asum -= data[left-1] : nothing
        right <= len ? asum += data[right]  : nothing

        avgs[curr] = asum / alen
    end

    avgs
end
