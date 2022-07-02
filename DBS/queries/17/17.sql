select id_adresa, psc, mesto, ulice, cislo_popisne, count(*) pocet_zamestnancu
from zamestnanec
join adresa using(id_adresa)
group by id_adresa, psc, mesto, ulice, cislo_popisne
having count(*) > 1
order by pocet_zamestnancu desc;
