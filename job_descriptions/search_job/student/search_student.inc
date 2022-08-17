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
 | Filename: search_student.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This is the main search page for students.  It decides          |
 | whether or not to load the search form the search results, and it validates  |
 | any search inputs gathered.                                                  |
 +------------------------------------------------------------------------------+

*/

$result = $GLOBALS['dbh']->Execute("select CURRENT_DATE");
$row = $result->FetchRow();
$current_date = $row["CURRENT_DATE"];

$error = 0;

if( $continue == '')
{
	//no form has been submited yet, so show first screen
	include('job_descriptions/search_job/student/search_student_form1.inc');
}
elseif( $continue == 'View search results' )
{

	$error = 0;

	if (trim($search_job_codes_m) != '')
	{
		$search_job_codes = $search_job_codes_m;
	}

	if (trim($search_company_name_m) != '')
	{
		$search_company_name = $search_company_name_m;
	}

	if (trim($search_close_date == ''))
	{
		$search_close_date_as = '';
	} 
	elseif (validDate($search_close_date) != -1)
	{
		$search_close_date = validDate($search_close_date);
	}

	if ((trim($search_start_term) == '') && (trim($search_start_year) == ''))
	{	
		$search_start_as = '';
	}

	if ($search_min_academic == '')
	{
		$search_academic_as = '';
	}

	if ($search_min_workterms == '')
	{
		$search_workterms_as = '';
	}

	if ($search_keywords != '')
	{
		$search_keywords = explode(",", $search_keywords);
		for ($i = 0; $i < sizeof($search_keywords); $i++)
		{
			$search_keywords[$i] = trim($search_keywords[$i]);
		}
	}

	if ($search_not_keywords != '')
	{
		$search_not_keywords = explode(",", $search_not_keywords);
		for ($i = 0; $i < sizeof($search_not_keywords); $i++)
		{
			$search_not_keywords[$i] = trim($search_not_keywords[$i]);
		}
	}
	
	/*
	 Trim all of the empty elements out of search_status, and search_order, etc.
	*/
	
	$search_status = trim_array($search_status);
	$apply_method = trim_array($apply_method);
	$search_order = trim_array($search_order);
	$yes_industries = trim_array($yes_industries);
	$no_industries = trim_array($no_industries);
	$search_disciplines = trim_array($search_disciplines);

	$search_job = new SearchJob('');
	$search_job->set_student_search_values($search_show_term, $search_show_year, $search_future_jobs, $search_on_jobs, $folder_id,
        $search_status, $apply_method, $search_area, $search_country, $search_provstate, $search_region, $search_job_codes,
        $search_job_codes_m, $search_company_name, $search_company_name_m, $search_close_date_as, $search_close_date,
        $search_academic_as, $search_min_academic, $search_workterms_as, $search_min_workterms, $yes_industries,
        $no_industries, $search_keywords, $search_not_keywords, $search_disciplines, $search_order);

	if (($search_close_date) && (validDate($search_close_date) == -1))
	{
		$error = 1;
		error("If you wish to search by closing date, you must provide a valid closing date in the form YYYY-MM-DD.");
		$search_close_date = '';
		include('job_descriptions/search_job/student/search_student_form1.inc');
	}
	if (($search_start_term && !(trim($search_start_year))) || (!(trim($search_start_term)) && $search_start_year))
	{
		$error = 1;
		error("If you wish to search by the start of the workterm, you must provide both a starting term and a starting year.");
		include('job_descriptions/search_job/student/search_student_form1.inc');
	}

	if ($error == 0)
	{
		include('job_descriptions/search_job/student/search_student_results.inc');
	}
}
elseif ($continue == 'Go')
{
	/* 
	 They just want to change the limit on the jobs returned.  Restore search_job
	 and reload the page.
	*/

	if ($search_job && is_string($search_job))
	{
		$search_job = unpackObject($search_job);
	}
	
	include('job_descriptions/search_job/student/search_student_results.inc');
}
elseif ($continue == 'Flag/Unflag Selected Jobs')
{
	// Trim arrays, then flag and unflag the chosen jobs.

	$jobs_to_flag = trim_array($jobs_to_flag);
	$jobs_to_unflag = trim_array($jobs_to_unflag);

	if ($jobs_to_flag)
	{
		foreach ($jobs_to_flag AS $jtf)
		{
			$sql = ("
				INSERT INTO student_jobs_flagged
				(job_id, student_number)
				VALUES (
				'" . $jtf . "',
				'" . $student_number . "'
				)
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}

	if ($jobs_to_unflag)
	{
		foreach ($jobs_to_unflag AS $jtu)
		{
			$sql = ("
				DELETE FROM student_jobs_flagged
				WHERE job_id='" . $jtu . "'
				AND student_number='" . $student_number . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}

	if ($search_job && is_string($search_job))
        {
            $search_job = unpackObject($search_job);
        }

        include('job_descriptions/search_job/student/search_student_results.inc');
}

?>
