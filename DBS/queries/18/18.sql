select d.jmeno
from distributor d
join vydavatel using(id_distributor)
join hra using(id_vydavatel)
group by id_distributor, d.jmeno
having count(*) = 4
