select jmeno, prijmeni
from zamestnanec
join pobocka using(id_pobocka)
where id_pobocka = 1;
