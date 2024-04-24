

CREATE TABLE `t_bilan_socle` (
	eleve int,
	commentaires TEXT,
	date_bilan datetime,
	created datetime,
	creator int,
	PRIMARY KEY (`eleve`),
	CONSTRAINT  FOREIGN KEY (eleve) REFERENCES `t_eleve` (`id`) ON DELETE RESTRICT
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `t_bilan_socle_elements` (
	eleve int ,
	socle_ref VARCHAR(20),
	socle_intitule TEXT,
	affichage int,
	commentaire TEXT,
	evaluation varchar(5),
	created datetime,
	creator int,
	PRIMARY KEY (eleve,socle_ref),
	CONSTRAINT FOREIGN KEY (eleve) REFERENCES t_eleve (id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
