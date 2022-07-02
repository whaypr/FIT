begin;

delete from zamestnanec where id_zamestnanec in (
    select id_zamestnanec from zamestnanec
    where id_zamestnanec not in (
        select id_zamestnanec from vazba_hra_zamestnanec
    )
);

-- KONTROLA
select 'Nikdo, kdo nedělal žádnou hru, už u nás nepracuje'  as kontrola
where not exists (
    select id_zamestnanec from zamestnanec
    where id_zamestnanec not in ( select id_zamestnanec from vazba_hra_zamestnanec )
);
