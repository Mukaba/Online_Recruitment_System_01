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
 | Filename: save_job_description.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is called to move all of the job inputs from the      |
 | $newEmployer variable into the database.                                     |
 +------------------------------------------------------------------------------+

*/

assert(isset($newEmployer));

global $REMOTE_ADDR;

$error = 0;

if ($error == 0)
{
	// Insert the new values into the employer_company database.

	// Turn the workterm lengths and applicable departments from arrays into strings, for storage.

	$newEmployer->job_workterm_lengths = implode(',', $newEmployer->job_workterm_lengths);
	$newEmployer->job_apply_departments = implode(',', $newEmployer->job_apply_departments);
	$newEmployer->job_disciplines = implode(',', $newEmployer->job_disciplines);

	$sql = ("
		INSERT INTO pending_job_description 
        (
		employer_id, contact_id, term_id, year, position_title, employer_code, num_positions, 
		apply_method, receive_address, country_id, provstate_id, region_id, city, salary_amount1, salary_amount2, 
		salary_period, workterm_hours, workterm_lengths, other_postings, apply_departments, host_department, 
		job_description, min_academic, min_workterms, special_requirements, skills_required, comments, date_added, 
		industry, ip_added_with, disciplines, start_date, end_date, displayname_on_coverltr
		)
		VALUES
        (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?, ?, ?, ?, ?, ?)
        ");
    $stmt = $GLOBALS['dbh']->Prepare($sql);
    $result = $GLOBALS['dbh']->Execute($stmt,
            array($newEmployer->employer_id,
                $newEmployer->contact_id,
                $newEmployer->job_start_term,
                $newEmployer->job_start_year,
                $newEmployer->job_position_title,
                $newEmployer->job_employer_code,
                $newEmployer->job_num_positions,
                $newEmployer->job_apply_method,
                $newEmployer->job_receive_address,
                $newEmployer->job_country_id,
                $newEmployer->job_provstate_id,
                $newEmployer->job_region_id,
                $newEmployer->job_city,
                $newEmployer->job_salary_amount1,
                $newEmployer->job_salary_amount2,
                $newEmployer->job_salary_period,
                $newEmployer->job_workterm_hours,
                $newEmployer->job_workterm_lengths,
                $newEmployer->job_other_postings,
                $newEmployer->job_apply_departments,
                $newEmployer->job_host_department,
                $newEmployer->job_description,
                $newEmployer->job_min_academic,
                $newEmployer->job_min_workterms,
                $newEmployer->job_special_requirements,
                $newEmployer->job_skills_required,
                $newEmployer->job_comments,
                $newEmployer->job_industry_id,
                $REMOTE_ADDR,
                $newEmployer->job_disciplines,
                ($newEmployer->job_start_date
                 ? $newEmployer->job_start_date
                 : null),
                ($newEmployer->job_end_date
                 ? $newEmployer->job_end_date
                 : null),
                ($newEmployer->job_displayname_on_coverltr=='1'
                 ? $newEmployer->job_displayname_on_coverltr
                 : "0")));
	
echo("<h3>Final Confirmation Page</h3>");
echo("<br />Your job posting has been successfully submitted and your contact information has been saved.<br /><br />
    You will be sent a ".BRAND_NAME." UserID and password to the e-mail address you provided on the Contact Information 
    Form (".$newEmployer->contact_email."). Once you receive your ".BRAND_NAME." login information, you can freely 
    access the system to post and edit job descriptions, view student applications, and communicate with students 
    and staff.<br /><br />
    <b>Thank you</b> for choosing to post a job with the University of Victoria's Co-operative Education Programs.  
    By participating in our program you become more than an employer; you become a co-educator.<br /><br />
	We look forward to building our partnership with you.");
}

?>
