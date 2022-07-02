-- odebrání funkce na odebrání tabulek a sekvencí (pokud existuje)
DROP FUNCTION IF EXISTS remove_all();

-- vytvoření funkce na odebrání tabulek a sekvencí
CREATE or replace FUNCTION remove_all() RETURNS void AS $$
DECLARE
    rec RECORD;
    cmd text;
BEGIN
    cmd := '';

    FOR rec IN SELECT
            'DROP SEQUENCE ' || quote_ident(n.nspname) || '.'
                || quote_ident(c.relname) || ' CASCADE;' AS name
        FROM
            pg_catalog.pg_class AS c
        LEFT JOIN
            pg_catalog.pg_namespace AS n
        ON
            n.oid = c.relnamespace
        WHERE
            relkind = 'S' AND
            n.nspname NOT IN ('pg_catalog', 'pg_toast') AND
            pg_catalog.pg_table_is_visible(c.oid)
    LOOP
        cmd := cmd || rec.name;
    END LOOP;

    FOR rec IN SELECT
            'DROP TABLE ' || quote_ident(n.nspname) || '.'
                || quote_ident(c.relname) || ' CASCADE;' AS name
        FROM
            pg_catalog.pg_class AS c
        LEFT JOIN
            pg_catalog.pg_namespace AS n
        ON
            n.oid = c.relnamespace WHERE relkind = 'r' AND
            n.nspname NOT IN ('pg_catalog', 'pg_toast') AND
            pg_catalog.pg_table_is_visible(c.oid)
    LOOP
        cmd := cmd || rec.name;
    END LOOP;

    EXECUTE cmd;
    RETURN;
END;
$$ LANGUAGE plpgsql;

-- zavolání funkce (mohli bychom dropnout celé schéma a znovu jej vytvořit, použíjeme však PLSQL)
select remove_all();


-- vytvoření tabulek
CREATE TABLE adresa (
    id_adresa SERIAL NOT NULL,
    psc VARCHAR(10),
    mesto VARCHAR(50) NOT NULL,
    ulice VARCHAR(20),
    cislo_popisne INTEGER
);
ALTER TABLE adresa ADD CONSTRAINT pk_adresa PRIMARY KEY (id_adresa);

CREATE TABLE distributor (
    id_distributor SERIAL NOT NULL,
    jmeno VARCHAR(50) NOT NULL
);
ALTER TABLE distributor ADD CONSTRAINT pk_distributor PRIMARY KEY (id_distributor);

CREATE TABLE dokoncena (
    id_hra INTEGER NOT NULL,
    datum_vydani DATE NOT NULL,
    naklady DECIMAL(10,2),
    vydelek DECIMAL(17,2)
);
ALTER TABLE dokoncena ADD CONSTRAINT pk_dokoncena PRIMARY KEY (id_hra);

CREATE TABLE hra (
    id_hra SERIAL NOT NULL,
    id_hra_stav SMALLINT NOT NULL,
    id_zanr SMALLINT NOT NULL,
    id_vydavatel SMALLINT,
    nazev VARCHAR(100) NOT NULL,
    rozpocet DECIMAL(10,2)
);
ALTER TABLE hra ADD CONSTRAINT pk_hra PRIMARY KEY (id_hra);

CREATE TABLE hra_stav (
    id_hra_stav SERIAL NOT NULL,
    popis VARCHAR(30) NOT NULL
);
ALTER TABLE hra_stav ADD CONSTRAINT pk_hra_stav PRIMARY KEY (id_hra_stav);

CREATE TABLE oceneni (
    id_oceneni SERIAL NOT NULL,
    id_hra INTEGER NOT NULL,
    nazev VARCHAR(100) NOT NULL,
    rok SMALLINT NOT NULL CHECK ( rok > '1500-01-01' )
);
ALTER TABLE oceneni ADD CONSTRAINT pk_oceneni PRIMARY KEY (id_oceneni);

CREATE TABLE platforma (
    id_platforma SERIAL NOT NULL,
    id_platforma_typ SMALLINT NOT NULL,
    nazev VARCHAR(50) NOT NULL
);
ALTER TABLE platforma ADD CONSTRAINT pk_platforma PRIMARY KEY (id_platforma);

CREATE TABLE platforma_typ (
    id_platforma_typ SERIAL NOT NULL,
    nazev VARCHAR(50) NOT NULL
);
ALTER TABLE platforma_typ ADD CONSTRAINT pk_platforma_typ PRIMARY KEY (id_platforma_typ);

CREATE TABLE pobocka (
    id_pobocka SERIAL NOT NULL,
    id_adresa INTEGER NOT NULL,
    kapacita SMALLINT NOT NULL
);
ALTER TABLE pobocka ADD CONSTRAINT pk_pobocka PRIMARY KEY (id_pobocka);

CREATE TABLE pozice (
    id_pozice SERIAL NOT NULL,
    nazev VARCHAR(50) NOT NULL,
    popis_prace VARCHAR(200) NOT NULL
);
ALTER TABLE pozice ADD CONSTRAINT pk_pozice PRIMARY KEY (id_pozice);

CREATE TABLE vazba_hra_platforma (
    id_hra INTEGER NOT NULL,
    id_platforma SMALLINT NOT NULL
);
ALTER TABLE vazba_hra_platforma ADD CONSTRAINT pk_vazba_hra_platforma PRIMARY KEY (id_hra, id_platforma);

CREATE TABLE vazba_hra_veletrh (
    id_vazba_hra_veletrh SERIAL NOT NULL,
    id_hra INTEGER NOT NULL,
    id_veletrh INTEGER NOT NULL,
    rok_navstiveni SMALLINT NOT NULL
);
ALTER TABLE vazba_hra_veletrh ADD CONSTRAINT pk_vazba_hra_veletrh PRIMARY KEY (id_vazba_hra_veletrh);

CREATE TABLE vazba_hra_zamestnanec (
    id_hra INTEGER NOT NULL,
    id_zamestnanec INTEGER NOT NULL
);
ALTER TABLE vazba_hra_zamestnanec ADD CONSTRAINT pk_vazba_hra_zamestnanec PRIMARY KEY (id_hra, id_zamestnanec);

CREATE TABLE veletrh (
    id_veletrh SERIAL NOT NULL,
    id_adresa INTEGER NOT NULL,
    nazev VARCHAR(50) NOT NULL,
    cena_za_ucast DECIMAL(8,2) NOT NULL
);
ALTER TABLE veletrh ADD CONSTRAINT pk_veletrh PRIMARY KEY (id_veletrh);
ALTER TABLE veletrh ADD CONSTRAINT u_fk_veletrh_adresa UNIQUE (id_adresa);

CREATE TABLE ve_vyvoji (
    id_hra INTEGER NOT NULL,
    zacatek_vyvoje DATE NOT NULL,
    testovaci_faze SMALLINT NOT NULL
);
ALTER TABLE ve_vyvoji ADD CONSTRAINT pk_ve_vyvoji PRIMARY KEY (id_hra);

CREATE TABLE vydavatel (
    id_vydavatel SERIAL NOT NULL,
    id_distributor INTEGER,
    jmeno VARCHAR(50) NOT NULL
);
ALTER TABLE vydavatel ADD CONSTRAINT pk_vydavatel PRIMARY KEY (id_vydavatel);

CREATE TABLE zamestnanec (
    id_zamestnanec SERIAL NOT NULL,
    id_adresa INTEGER NOT NULL,
    id_pobocka SMALLINT,
    id_pozice SMALLINT,
    jmeno VARCHAR(50) NOT NULL,
    prijmeni VARCHAR(50) NOT NULL,
    rodne_cislo VARCHAR(25) NOT NULL,
    homeoffice_povoleni SMALLINT NOT NULL,
    homeoffice SMALLINT NOT NULL
);
ALTER TABLE zamestnanec ADD CONSTRAINT pk_zamestnanec PRIMARY KEY (id_zamestnanec);
ALTER TABLE zamestnanec ADD CONSTRAINT uc_zamestnanec_rodne_cislo UNIQUE (rodne_cislo);

CREATE TABLE zanr (
    id_zanr SERIAL NOT NULL,
    nazev VARCHAR(50) NOT NULL
);
ALTER TABLE zanr ADD CONSTRAINT pk_zanr PRIMARY KEY (id_zanr);


ALTER TABLE dokoncena ADD CONSTRAINT fk_dokoncena_hra FOREIGN KEY (id_hra) REFERENCES hra (id_hra) ON DELETE CASCADE;

ALTER TABLE hra ADD CONSTRAINT fk_hra_hra_stav FOREIGN KEY (id_hra_stav) REFERENCES hra_stav (id_hra_stav) ON DELETE CASCADE;
ALTER TABLE hra ADD CONSTRAINT fk_hra_zanr FOREIGN KEY (id_zanr) REFERENCES zanr (id_zanr) ON DELETE CASCADE;
ALTER TABLE hra ADD CONSTRAINT fk_hra_vydavatel FOREIGN KEY (id_vydavatel) REFERENCES vydavatel (id_vydavatel) ON DELETE CASCADE;

ALTER TABLE oceneni ADD CONSTRAINT fk_oceneni_dokoncena FOREIGN KEY (id_hra) REFERENCES dokoncena (id_hra) ON DELETE CASCADE;

ALTER TABLE platforma ADD CONSTRAINT fk_platforma_platforma_typ FOREIGN KEY (id_platforma_typ) REFERENCES platforma_typ (id_platforma_typ) ON DELETE CASCADE;

ALTER TABLE pobocka ADD CONSTRAINT fk_pobocka_adresa FOREIGN KEY (id_adresa) REFERENCES adresa (id_adresa) ON DELETE CASCADE;

ALTER TABLE ve_vyvoji ADD CONSTRAINT fk_ve_vyvoji_hra FOREIGN KEY (id_hra) REFERENCES hra (id_hra) ON DELETE CASCADE;

ALTER TABLE veletrh ADD CONSTRAINT fk_veletrh_adresa FOREIGN KEY (id_adresa) REFERENCES adresa (id_adresa) ON DELETE CASCADE;

ALTER TABLE vydavatel ADD CONSTRAINT fk_vydavatel_distributor FOREIGN KEY (id_distributor) REFERENCES distributor (id_distributor) ON DELETE CASCADE;

ALTER TABLE zamestnanec ADD CONSTRAINT fk_zamestnanec_adresa FOREIGN KEY (id_adresa) REFERENCES adresa (id_adresa) ON DELETE CASCADE;
ALTER TABLE zamestnanec ADD CONSTRAINT fk_zamestnanec_pobocka FOREIGN KEY (id_pobocka) REFERENCES pobocka (id_pobocka) ON DELETE CASCADE;
ALTER TABLE zamestnanec ADD CONSTRAINT fk_zamestnanec_pozice FOREIGN KEY (id_pozice) REFERENCES pozice (id_pozice) ON DELETE CASCADE;

ALTER TABLE vazba_hra_zamestnanec ADD CONSTRAINT fk_hra_zamestnanec_hra FOREIGN KEY (id_hra) REFERENCES hra (id_hra) ON DELETE CASCADE;
ALTER TABLE vazba_hra_zamestnanec ADD CONSTRAINT fk_hra_zamestnanec_zamestnanec FOREIGN KEY (id_zamestnanec) REFERENCES zamestnanec (id_zamestnanec) ON DELETE CASCADE;

ALTER TABLE vazba_hra_platforma ADD CONSTRAINT fk_hra_platforma_hra FOREIGN KEY (id_hra) REFERENCES hra (id_hra) ON DELETE CASCADE;
ALTER TABLE vazba_hra_platforma ADD CONSTRAINT fk_hra_platforma_platforma FOREIGN KEY (id_platforma) REFERENCES platforma (id_platforma) ON DELETE CASCADE;

ALTER TABLE vazba_hra_veletrh ADD CONSTRAINT fk_hra_veletrh_hra FOREIGN KEY (id_hra) REFERENCES hra (id_hra) ON DELETE CASCADE;
ALTER TABLE vazba_hra_veletrh ADD CONSTRAINT fk_hra_veletrh_veletrh FOREIGN KEY (id_veletrh) REFERENCES veletrh (id_veletrh) ON DELETE CASCADE;


-- dodatečný check
ALTER TABLE zamestnanec ADD CONSTRAINT check_homeoffice_povoleni CHECK (
    zamestnanec.homeoffice_povoleni == 1 or zamestnanec.homeoffice_povoleni == 0
);
