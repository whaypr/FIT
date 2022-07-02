select sum(cena_za_ucast) as celkem
from veletrh
join vazba_hra_veletrh using(id_veletrh)
