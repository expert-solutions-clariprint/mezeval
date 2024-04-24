


ALTER TABLE t_progression_periode ENGINE = InnoDB;
delete from t_progression_periode where not periode in (select id from t_periode);
ALTER TABLE t_progression_periode ADD FOREIGN KEY (periode) REFERENCES t_periode (id) ON DELETE RESTRICT;
ALTER TABLE t_progression_periode ADD FOREIGN KEY (progression) REFERENCES t_progression (id) ON DELETE RESTRICT;

ALTER TABLE t_bilan_progression ENGINE = InnoDB;
delete from t_bilan_progression where not eleve in (select id from t_eleve);
ALTER TABLE t_bilan_progression ADD FOREIGN KEY (eleve) REFERENCES t_eleve (id) ON DELETE RESTRICT;
delete from t_bilan_progression where not periode in (select id from t_periode);
ALTER TABLE t_bilan_progression ADD FOREIGN KEY (periode) REFERENCES t_periode (id) ON DELETE RESTRICT;



ALTER TABLE t_bilan_progression_elements ENGINE = InnoDB;
ALTER TABLE t_bilan_progression_elements ADD FOREIGN KEY (eleve) REFERENCES t_eleve (id) ON DELETE RESTRICT;
delete from t_bilan_progression_elements where not periode in (select id from t_periode);
ALTER TABLE t_bilan_progression_elements ADD FOREIGN KEY (periode) REFERENCES t_periode (id) ON DELETE RESTRICT;
delete from t_bilan_progression_elements where not progression in (select id from t_progression);
ALTER TABLE t_bilan_progression_elements ADD FOREIGN KEY (progression) REFERENCES t_progression (id) ON DELETE RESTRICT;



update t_progression set parentcp = null where parentcp = 0;
create table t_progression_ids select * from t_progression;
alter table t_progression_ids add index (id);
update t_progression SET  parentcp = null WHERE not parentcp IN (SELECT id from t_progression_ids);
alter table t_progression add foreign key (parentcp) references t_progression (id) ON DELETE RESTRICT;
DROP TABLE t_progression_ids;

update t_progression SET classe = -1 WHERE classe = 0 and parentcp is null;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id AND t2.classe = -1;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id AND t2.classe = -1;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id AND t2.classe = -1;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id AND t2.classe = -1;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id AND t2.classe = -1;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id AND t2.classe = -1;

update t_progression SET parentcp =null where classe=-1;
DELETE  FROM t_progression WHERE classe = -1;

update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id;
update t_progression as t1, t_progression as t2 SET t1.classe=t2.classe WHERE t1.parentcp = t2.id;

update t_progression set parentcp = null where classe not in (select id from t_classe);
DELETE FROM t_progression WHERE parentcp is null AND classe not in (select id from t_classe);

alter table t_progression add foreign key (classe) references t_classe (id) ON DELETE RESTRICT;

