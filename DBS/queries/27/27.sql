begin;

update zamestnanec
set id_pozice = ( select id_pozice from pozice where nazev = 'programátor' )
where id_zamestnanec in (
    select id_zamestnanec from zamestnanec
    where id_pozice in (
        select id_pozice from pozice where nazev = 'podavač kafe'
    )
    order by random()
    fetch first 5 rows only
);

-- KONTROLA (původní počty v dotazu výše)
select nazev, count(*) as pocet from zamestnanec
join pozice using(id_pozice)
where nazev in ('podavač kafe', 'programátor')
group by id_pozice, nazev;

rollback;
