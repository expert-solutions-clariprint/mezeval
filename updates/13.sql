CREATE TABLE `t_evaluationeleveinfo` (
	evaluation int,
	eleve int ,
	description TEXT,
	created datetime,
	creator int,
	PRIMARY KEY (evaluation,eleve),
  CONSTRAINT FOREIGN KEY (`evaluation`) REFERENCES `t_evaluation` (`id`) ON DELETE CASCADE,
  CONSTRAINT FOREIGN KEY (`eleve`) REFERENCES `t_eleve` (`id`)  ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

