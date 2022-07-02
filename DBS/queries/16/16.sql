select z.nazev, count(distinct id_hra) as pocet_her
from dokoncena d
join hra h using(id_hra)
join zanr z using(id_zanr)
where vydelek - naklady > 1000000
group by z.nazev
having count(distinct id_hra) >= 3
order by pocet_her desc;
