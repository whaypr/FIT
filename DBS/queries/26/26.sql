select nazev, count(*) as pocet from zamestnanec
join pozice using(id_pozice)
group by nazev
order by pocet desc;
