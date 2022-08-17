insert into menu_item (menu_name,module_id,menu_owner,order_by) values ('Feedback','15','4','120');
insert into menu_item (menu_name,module_id,menu_owner,order_by) values ('Feedback','15','1','100');
insert into menu_item (menu_name,module_id,menu_owner,order_by) values ('Feedback','15','88','98');
insert into department_module values (15,1),(15,2),(15,18);

ALTER TABLE `contact_actions_set` DROP INDEX `contact_id_2`;
ALTER TABLE `employer_company` DROP INDEX `emp_id_index`;
ALTER TABLE `employer_login` DROP INDEX `contact_id`;
ALTER TABLE `interview` DROP INDEX `job_id_index`;
ALTER TABLE `interview_interviewer` DROP INDEX `interviewer_id_index`;
ALTER TABLE `interview_time` DROP INDEX `time_id_index`;
ALTER TABLE `status` DROP INDEX `status_id_index`;
ALTER TABLE `student` DROP INDEX `student_index`;

LOCK TABLES `resume` WRITE;
ALTER TABLE `resume` CHANGE `resume_id` `resume_id` MEDIUMINT( 8 )  UNSIGNED NOT NULL;
ALTER TABLE `resume` DROP PRIMARY KEY;
ALTER TABLE `resume` DROP INDEX `resume_student`;
ALTER TABLE `resume` ADD PRIMARY KEY ( `resume_id` , `student_number` );
ALTER TABLE `resume` CHANGE `resume_id` `resume_id` MEDIUMINT( 8 )  UNSIGNED NOT NULL AUTO_INCREMENT;
UNLOCK TABLES;

DROP TABLE `reset_when`;
