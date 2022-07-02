select h.nazev, count(distinct id_zamestnanec) as pocet_zamestnancu
from hra h
join vazba_hra_zamestnanec using(id_hra)
join oceneni using(id_hra)
group by id_hra, h.nazev
having count(distinct id_zamestnanec) < 5
order by pocet_zamestnancu
