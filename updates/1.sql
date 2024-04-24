# sql
ALTER TABLE t_classe ADD COLUMN a_color VARCHAR(7) default null;
ALTER TABLE t_classe ADD COLUMN b_color VARCHAR(7) default null;
ALTER TABLE t_classe ADD COLUMN c_color VARCHAR(7) default null;
ALTER TABLE t_classe ADD COLUMN z_color VARCHAR(7) default null;
ALTER TABLE t_classe ADD COLUMN socle VARCHAR(100) default null;

ALTER TABLE t_competence ADD COLUMN code_socle VARCHAR(100) default null;

RENAME TABLE t_competence TO t_progression;

UPDATE t_ticket SET tmodule = 'progressions' WHERE tmodule = 'competences';
UPDATE t_ticket SET tmodule = 'progression' WHERE tmodule = 'competence';


CREATE TABLE `t_elements` (
	id INT NOT NULL AUTO_INCREMENT,
	order_index int,
	evaluation int,
	code_socle VARCHAR(20),
	progression int,
	mode varchar(200),
	valeur_max int,
	description TEXT,
	created datetime,
	creator int,
	PRIMARY KEY (`id`));

INSERT INTO t_element (
	id,
	order_index,
	evaluation,
	progression,
	mode,
	valeur_max,
	description,
	created,
	creator)
SELECT
	id,
	order_index,
	evaluation,
	competence,
	mode,
	valeur_max,
	description,
	created,
	creator FROM t_evaluationcompetence ;

RENAME TABLE t_evaluationcompetence TO t_evaluationcompetence_dep;

