ALTER TABLE t_classe ADD COLUMN niveau varchar(10);
UPDATE t_classe SET niveau = intitule;

ALTER TABLE t_eleve ADD COLUMN fin_scolarite date;
