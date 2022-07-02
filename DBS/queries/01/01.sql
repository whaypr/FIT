elect p.nazev
from hra h
join vazba_hra_platforma v on h.id_hra = v.id_hra
join platforma p on v.id_platforma = p.id_platforma
where h.nazev = 'Quake Live';
