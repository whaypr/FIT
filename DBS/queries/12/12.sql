select z.*, a.mesto from zamestnanec z
join pobocka p on z.id_pobocka = p.id_pobocka
join adresa a on p.id_adresa = a.id_adresa
order by a.mesto asc, z.prijmeni desc;
