-- MOŽNOST 2 (první v dotazu D3)

select z.* from zamestnanec z
join vazba_hra_zamestnanec v on z.id_zamestnanec = v.id_zamestnanec
join hra h on v.id_hra = h.id_hra
where h.nazev = 'Dark Souls III'
    except
select z.* from zamestnanec z
join vazba_hra_zamestnanec v on z.id_zamestnanec = v.id_zamestnanec
join hra h on v.id_hra = h.id_hra
where h.nazev != 'Dark Souls III';

-- další možnosti jsou:
--     nevztaženě pomocí NOT IN
--     vztaženě pomocí NOT EXISTS
