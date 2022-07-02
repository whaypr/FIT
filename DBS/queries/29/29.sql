select id_pobocka, p.kapacita, count(*) as pocet, p.id_adresa
from zamestnanec
join pobocka p using(id_pobocka)
group by id_pobocka, p.kapacita, p.id_adresa
order by pocet desc
fetch first row only;
