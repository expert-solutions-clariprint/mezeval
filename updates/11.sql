
CREATE TABLE `t_archives` (
	id int(11) NOT NULL auto_increment,
	description TEXT,
	type varchar(10) not null,
	userid int NOT NULL,
	data longblob,
	date_creation datetime,
	PRIMARY KEY (id),
	CONSTRAINT  FOREIGN KEY (userid) REFERENCES `t_users` (`id`) ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `t_user_option` (
	userid int(11) NOT NULL,
	name varchar(20) NOT NULL,
	domain varchar(50) NOT NULL,
	value text,
	PRIMARY KEY (userid,name,domain),
	CONSTRAINT  FOREIGN KEY (userid) REFERENCES `t_users` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
