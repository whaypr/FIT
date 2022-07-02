select * from zamestnanec
where id_pozice = ( select id_pozice from pozice where nazev = 'programátor' )

except

select PROGRAMATOR.* from
(
    select h.*
    from vazba_hra_platforma v1
    join ( select * from platforma p where p.nazev = 'PlayStation 4' or p.nazev = 'PlayStation 5' ) as tmp using(id_platforma) -- nebo where p.nazev in ('PlayStation 4', 'PlayStation 5')
    join hra h on v1.id_hra = h.id_hra
) as HRA_NA_PS
join vazba_hra_zamestnanec v2 on HRA_NA_PS.id_hra = v2.id_hra
join (
    select * from zamestnanec
    where id_pozice in ( select id_pozice from pozice where nazev = 'programátor' )
) PROGRAMATOR on v2.id_zamestnanec = PROGRAMATOR.id_zamestnanec;
