
ALTER TABLE t_classe ADD COLUMN socle_langue varchar(255);
ALTER TABLE t_classe ADD COLUMN socle_b2i varchar(255);

ALTER TABLE t_bilan_socle ADD COLUMN type varchar(10) NOT NULL DEFAULT 'socle';

ALTER TABLE t_bilan_socle ADD INDEX (eleve);
ALTER TABLE t_bilan_socle DROP PRIMARY KEY;
ALTER TABLE t_bilan_socle ADD PRIMARY KEY (eleve,type);

ALTER TABLE t_bilan_socle_elements ADD COLUMN type varchar(10) NOT NULL DEFAULT 'socle';
ALTER TABLE t_bilan_socle_elements ADD INDEX (eleve);
ALTER TABLE t_bilan_socle_elements DROP PRIMARY KEY;
ALTER TABLE t_bilan_socle_elements ADD PRIMARY KEY (eleve,socle_ref,type);
