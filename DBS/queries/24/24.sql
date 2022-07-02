create or replace view talentovani_zamestnanci as
select * from zamestnanec z
where id_zamestnanec in (
    select id_zamestnanec from vazba_hra_zamestnanec
    where id_hra in (
        select id_hra from dokoncena
        where vydelek - naklady > 100000
    )
    group by id_zamestnanec
    having count(*) > 4
);

select * from talentovani_zamestnanci;

-- NA KTERÝCH HRÁCH PRACOVALI
select tz.jmeno, tz.prijmeni, h.nazev from talentovani_zamestnanci tz
join vazba_hra_zamestnanec using(id_zamestnanec)
join hra h using(id_hra)
order by tz.prijmeni;

update talentovani_zamestnanci set homeoffice_povoleni = 1;

-- KONTROLA
select * from talentovani_zamestnanci;

rollback;
