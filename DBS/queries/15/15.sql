-- ZPŮSOB 1
select z.jmeno, z.prijmeni, min(d.datum_vydani) as datum_vydani
from zamestnanec z
join vazba_hra_zamestnanec v using(id_zamestnanec)
join dokoncena d using(id_hra)
group by id_zamestnanec; --, z.jmeno, z.prijmeni

-- ZPŮSOB 2
select * from ( -- pro odstranění null hodnot
    select  z.jmeno,
            z.prijmeni,
            (
                select min(datum_vydani) from dokoncena
                join vazba_hra_zamestnanec v using(id_hra)
                where z.id_zamestnanec = v.id_zamestnanec
            ) as datum_vydani
    from zamestnanec z
) as tmp
where datum_vydani is not null
