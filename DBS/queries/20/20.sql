select h.nazev as hra, v.jmeno as vydavatel, o.nazev as oceneni
from hra h
left join vydavatel v using(id_vydavatel)
left join oceneni o using(id_hra)
