
ALTER TABLE t_classe ADD COLUMN manager int;

ALTER TABLE t_element ADD COLUMN seuil_a int default 0;
ALTER TABLE t_element ADD COLUMN seuil_b int default 0;


ALTER TABLE t_users ADD COLUMN rattachement VARCHAR(200);

ALTER TABLE t_contrat ADD COLUMN items LONGTEXT;
