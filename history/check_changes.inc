<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: check_changes.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: Check the changes that have been made to the record             |
 +------------------------------------------------------------------------------+

*/
	if ($errorlevel<>1)
    {
		if ($row['student_work_phone']<>$student_work_phone && $row['student_work_phone']<>NULL){
			$change=$change."Student Work Phone,";
		}
		if ($row['job_title']<>stripslashes($job_title) && $row['job_title']<>NULL){
			$change=$change."Student Job Title,";
		}
		if ($row['site_visit_by']<>$site_visit_by && $row['site_visit_by']<>NULL){
			$change=$change."Site Visit By,";
		}
		if ($row['report_title']<>$report_title && $row['report_title']<>NULL){
			$change=$change."Report Title,";
		}
		if ($row['report_code']<>$report_code && $row['report_code']<>NULL){
			$change=$change."Report Code,";
		}
		if ($row['term_id']<>$term && $row['term_id']<>NULL){
			$change=$change."Term,";
		}
		if ($row['year']<>$year && $row['year']<>NULL){
			$change=$change."Year,";
		}
		if ($row['work_email']<>$work_email && $row['work_email']<>NULL){
			$change=$change."Work Email,";
		}
        if ($row['salary']<>$salary && $row['salary']<>NULL)
        {
            $change=$change."Salary,";
        }
        if ($row['salary_period_id']<>$salary_period_id && $row['salary_period_id'] <> NULL) {
            $change=$change."Salary Period,";
        }
        if ($row['work_term_hours_per_week']<>$work_term_hours_per_week && $row['work_term_hours_per_week'] <> NULL) {
            $change=$change."Hours Per Week,";
        }
        if ($row['start_date']<>$start_date && $row['start_date'] <> NULL) {
            $change=$change."Start Date,";
        }
        if ($row['end_date']<>$end_date && $row['end_date'] <> NULL) {
            $change=$change."End Date,";
        }
        if ($row['work_term_length']<>$work_term_length && $row['work_term_length'] <> NULL) {
            $change=$change."Work Term Length,";
        }
		if ($row['company_city']<>stripslashes($company_city) && $row['company_city']<>NULL){
			$change=$change."Company City,";
		}
		if ($row['report_subject']<>stripslashes($report_subject) && $row['report_subject']<>NULL){
			$change=$change."Report Subject,";
		}
		if ($row['company_name']<>stripslashes($company_name) && $row['company_name']<>NULL){
			$change=$change."Company Name,";
		}
		if ($row['department_name']<>stripslashes($department_name) && $row['department_name']<>NULL){
			$change=$change."Department Name,";
		}
		if ($row['company_street_address_1']<>stripslashes($company_street_address_1) && $row['company_street_address_1']<>NULL){
			$change=$change."Company Street Address 1,";
		}
		if ($row['company_street_address_2']<>stripslashes($company_street_address_2) && $row['company_street_address_2']<>NULL){
			$change=$change."Company Street Address 2,";
		}
		if ($row['company_postal_code']<>$company_postal_code && $row['company_postal_code']<>NULL){
			$change=$change."Company Postal Code,";
		}
		if ($row['company_position']<>stripslashes($company_position) && $row['company_position']<>NULL){
			$change=$change."Company Position,";
		}
		if ($row['supervisor_phone_number']<>$supervisor_phone_number && $row['supervisor_phone_number']<>NULL){
			$change=$change."Supervisor Phone Number,";
		}
        if ($row['supervisor_cellphone_number']<>$supervisor_cellphone_number && $row['supervisor_cellphone_number']<>NULL){
			$change=$change."Supervisor Cellphone Number,";
		}
		if ($row['supervisor_name']<>stripslashes($supervisor_name) && $row['supervisor_name']<>NULL){
			$change=$change."Supervisor Name,";
		}
		if ($row['supervisor_fax_number']<>$supervisor_fax_number && $row['supervisor_fax_number']<>NULL){
			$change=$change."Supervisor Fax Number,";
		}
		if ($row['supervisor_email']<>$supervisor_email && $row['supervisor_email']<>NULL){
			$change=$change."Supervisor E-mail,";
		}
		if ($row['work_term_street_address_1']<>stripslashes($work_term_street_address_1) && $row['work_term_street_address_1']<>NULL){
			$change=$change."Work Term Street Address 1,";
		}
		if ($row['work_term_street_address_2']<>stripslashes($work_term_street_address_2) && $row['work_term_street_address_2']<>NULL){
			$change=$change."Work Term Street Address 2,";
		}
		if ($row['work_term_city']<>stripslashes($work_term_city) && $row['work_term_city']<>NULL){
			$change=$change."Work Term City,";
		}
		if ($row['work_term_postal_code']<>$work_term_postal_code && $row['work_term_postal_code']<>NULL){
			$change=$change."Work Term Postal Code,";
		}
		if ($row['work_term_province']<>$work_term_province && $row['work_term_province']<>NULL){
			$change=$change."Work Term Province,";
		}
		if ($row['work_term_country']<>$work_term_country && $row['work_term_country']<>NULL){
			$change=$change."Work Term Country,";
		}
		if ($row['wt_status']<>$wt_status && $row['wt_status']<>NULL){
			$change=$change."Work Term Status,";
		}
		if ($row['options']<>$options && $row['options']<>NULL){
			$change=$change."Options,";
		}
		if ($row['site_visit_date_supervisor']<>$site_visit_date_supervisor && $row['site_visit_date_supervisor']<>NULL){
			$change=$change."Site Visit Date Supervisor,";
		}
		if ($row['company_province']<>$company_province && $row['company_province']<>NULL){
			$change=$change."Company Province,";
		}
		if ($row['work_term_home_phone']<>$work_term_home_phone && $row['work_term_home_phone']<>NULL){
			$change=$change."Work Term Home Phone,";
		}
		if ($row['company_country']<>$company_country && $row['company_country']<>NULL){
			$change=$change."Company country,";
		}
		if ($row['company_region_id']<>$company_region && $row['company_region_id']<>NULL){
			$change=$change."Company Region,";
		}
		if ($row['site_visit_type_id_supervisor']<>$site_visit_type_id_supervisor && $row['site_visit_type_id_supervisor']<>NULL){
			$change=$change."Supervisor Site Visit Type,";
		}
		if ($row['site_visit_date']<>$site_visit_date && $row['site_visit_date']<>NULL){
			$change=$change."Student Site Visit Date,";
		}
		if ($row['site_visit_type_id']<>$site_visit_type_id && $row['site_visit_type_id']<>NULL){
			$change=$change."Site Visit Type,";
		}
		if ($row['report_marker']<>stripslashes($report_marker) && $row['report_marker']<>NULL){
			$change=$change."Report Marker,";
		}
		if ($row['site_visit_by_supervisor']<>$site_visit_by_supervisor && $row['site_visit_by_supervisor']<>NULL){
			$change=$change."Supervisor Site Visit By,";
		}
		if ($row['supervisor_position']<>stripslashes($supervisor_position) && $row['supervisor_position']<>NULL){
			$change=$change."Supervisor Position,";
		}
		if ($change<>NULL){
			$change=substr($change,0,strlen($change)-1);
			$contact_sql = "select contact_id from contact_internal where login_id='".$auth->login."' or netlink_id='".$auth->login."'";
			$contact_results=$GLOBALS['dbh']->Execute($contact_sql);
			$contact_row=$contact_results->FetchRow();
			$contact=$contact_row['contact_id'];

			$changes_sql = ("
                SELECT changes_recorded_1,changes_recorded_2,changes_recorded_3,change_by_1,change_by_2,change_by_3,change_date_1,change_date_2,change_date_3 
                FROM history 
                WHERE history_id=".$history_id."
                ");
			$changes_results = $GLOBALS['dbh']->Execute($changes_sql);
			$changes_row = $changes_results->FetchRow(); 

			$update_sql = "update history set changes_recorded_3='".$changes_row['changes_recorded_2']."',change_by_3='".$changes_row['change_by_2']."',change_date_3='".$changes_row['change_date_2']."' where history_id=".$history_id;	
			$GLOBALS['dbh']->Execute($update_sql); 

			$update_sql = "update history set changes_recorded_2='".$changes_row['changes_recorded_1']."',change_by_2='".$changes_row['change_by_1']."',change_date_2='".$changes_row['change_date_1']."' where history_id=".$history_id;	
			$GLOBALS['dbh']->Execute($update_sql); 

			$update_sql = "update history set changes_recorded_1='".$change."', change_by_1='".$contact."',change_date_1='".date("Y-m-d")."' where history_id=".$history_id;
			$GLOBALS['dbh']->Execute($update_sql);
		}
	}
?>
