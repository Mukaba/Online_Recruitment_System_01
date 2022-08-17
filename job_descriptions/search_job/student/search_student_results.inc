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
 | Filename: search_student_results.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This file forms the query, runs it against the database, and    |
 | displays the results of the search for the user.                             |
 +------------------------------------------------------------------------------+

*/

// These variables will help reduce the problem of too many companies on a screen

if ($per_page_max == "") { $per_page_max = 50; }
if ($per_page_max < 5) { $per_page_max = 5; }
if ($start_row == '') { $start_row = 0; }

/*
 We will only need to search for either area, country, province/state, or region, so determine which of
 the four it is we need to search for, and then use the corresponding search input.
*/

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='search_job' />");
echo("<input type='hidden' name='search_job' value='" . packObject($search_job) . "' />"); 
if ($search_job->search_area)
{
	$search_location = "area";
	$search_location_key = $search_job->search_area;
}
if ($search_job->search_country)
{
	$search_location = "country";
	$search_location_key = $search_job->search_country;
}
if ($search_job->search_provstate)
{
	$search_location = "provstate";
	$search_location_key = $search_job->search_provstate;
}
if ($search_job->search_region)
{
	$search_location = "region";
	$search_location_key = $search_job->search_region;
}

$nextTermYear = getNextTermYear();

$result = $GLOBALS['dbh']->Execute("
    SELECT DISTINCT *
    FROM job_info
    WHERE (status!=" . INT_ONLY . " AND status!=" . REJECTED . ")
    ");

if ($result->RecordCount() == 0)
{
    // if there are no job in the database, indicate this 

    echo("<p>There are currently no jobs registered in the system.</p>\n");
} 
else      
{
    $result->Close();

    // there are jobs in the system

    $query = ("
        SELECT DISTINCT a.job_id, a.position_title, b.company_name, a.min_academic, a.min_workterms,
        a.student_status, a.job_code, a.closing_date, a.term_id, a.year, a.resumes_pulled
        FROM term AS c, job_info AS a, country_list AS d
        LEFT JOIN employer_company AS b ON b.employer_id=a.employer_id
        ");
    if($search_job->folder_id != 0)
    {
        $query.="INNER JOIN student_job_folder_join AS sjfj ON sjfj.job_id=a.job_id AND sjfj.folder_id ='".$search_job->folder_id."' AND sjfj.student_number='$student_number'";
    }
    
    $query.=("  LEFT JOIN employer_department AS ed ON ed.department_id=a.employer_department_id
        WHERE a.job_id IS NOT NULL
        AND (a.status!=" . INT_ONLY . " AND a.status!=" . REJECTED . ")
        AND (a.status=" . POSTED . " OR a.status=" . CLOSED . " OR (a.status=" . CANCELLED . " AND a.student_status='7'))
        AND d.country_id=a.country_id");
    // Set up rest of query here.

    //Add search_show_jobs variable into search query.

    switch ($search_job->search_on_jobs)
    {
        case "preferences":
            //Do nothing here right now, later add preferences to query.
            break;

        case "flagged":

            // Get all of the jobs that this student has flagged.

            $sql = ("
                SELECT DISTINCT job_id
                FROM student_jobs_flagged
                WHERE student_number='" . $student_number . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                $job_id_list .= ("'" . $row["job_id"] . "',");
            }
            $job_id_list = substr($job_id_list, 0, -1);

            if (!$job_id_list)
            {
                $job_id_list = ("''");
            }

            $extra_sql = ("
                AND job_id IN (" . $job_id_list . ")
                ");
            break;

        case "applied":

            // Get all of the jobs that this student has applied for.

            $sql = ("
                SELECT DISTINCT job_id
                FROM applications
                WHERE student_number='" . $student_number . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                $job_id_list .= ("'" . $row["job_id"] . "',");
            }
            $job_id_list = substr($job_id_list, 0, -1);

            if (!$job_id_list)
            {
                $job_id_list = ("''");
            }

            $extra_sql = ("
                AND job_id IN (" . $job_id_list . ")
                ");
            break;

        case "uvic":
            // If they want to see all jobs registered at UVic, do nothing to the query.
            break;

        default:
            // If they got here, they're probably viewing jobs for a specific department.

            if ($search_job->search_on_jobs && preg_match("/\d+/", $search_job->search_on_jobs))
            {
                $sql = ("
                    SELECT DISTINCT job_id
                    FROM department_job_join
                    WHERE department_id='" . $search_job->search_on_jobs . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
                while ($row = $result->FetchRow())
                {
                    $job_id_list .= ("'" . $row["job_id"] . "',");
                }

                if ($job_id_list)
                {
                    $job_id_list = substr($job_id_list, 0, -1);
                }
                else
                {
                    $job_id_list = ("''");
                }

                $extra_sql = ("
                    AND a.job_id IN (" . $job_id_list . ")
                    ");
            }
            break;
    } //End of switch statement.

    $query .= $extra_sql;

    //Add whether or not to search on future terms to the query

    if ($search_job->search_future_jobs == 'no')
    {
        $query .= ("
            AND a.term_id='" . addslashes($search_job->search_show_term) . "'
            AND a.year='" . addslashes($search_job->search_show_year) . "' 
            ");
    }
    elseif ($search_job->search_future_jobs == 'yes')
    {
        $datesql = (" 
            SELECT DISTINCT start_date, end_date
            FROM term 
            WHERE term_id='" . $search_job->search_show_term . "'
            ");
        $result = $GLOBALS['dbh']->Execute($datesql);
        $row = $result->FetchRow();

        $start_end = $row;

        $query .= ("
			AND (
				(a.term_id='" . addslashes($search_job->search_show_term) . "' AND a.year='" . addslashes($search_job->search_show_year) . "')
				OR
				( (a.term_id=c.term_id AND c.start_date>'" . $start_end["end_date"] . "'
				AND a.year='" . trim($search_job->search_show_year) . "')
				OR ( a.year>'" . trim($search_job->search_show_year) . "' ) )
			)
			");
	}

	//Add status of job to search query.
	
	if ($search_job->search_status)
	{
		$search_inputs[] = 'Status';
		$query .= ("AND (");
		for ($i = 0; $i < (sizeof($search_job->search_status) - 1); $i++)
		{
			$query .= ("a.student_status='" . $search_job->search_status[$i] . "' OR ");
		}
		$query .= ("a.student_status='" . $search_job->search_status[$i] . "') ");
	}

    if ($search_job->apply_method)
    {
        $search_inputs[] = 'Status';
        $query .= ("AND (");
        for ($i = 0; $i < (sizeof($search_job->apply_method) - 1); $i++)
        {
            $query .= ("a.apply_method='" . $search_job->apply_method[$i] . "' OR ");
        }
        $query .= ("a.apply_method='" . $search_job->apply_method[$i] . "') ");
    }

	//Add location to search query

	if ($search_location && $search_location_key)
	{
		switch($search_location)
		{
		case "region":
			$query .= ("AND a.region_id='" . $search_location_key . "' ");
			break;
		case "provstate":
			$query .= ("AND a.provstate_id='" . $search_location_key . "' ");
			break;
		case "country":
			$query .= ("AND a.country_id='" . $search_location_key . "' ");
			break;
		case "area":
			$query .= ("AND (d.area_id='" . $search_location_key . "' AND a.country_id=d.country_id) ");
			break;
		}
	}

	//Add job codes to search query

	if ($search_job->search_job_codes)
	{
		$search_inputs[] = 'Job Code';
		$query .= ("AND a.job_code REGEXP '^" . addslashes(trim($search_job->search_job_codes)) . ".*' ");
	}
	if ($search_job->search_company_name)
	{
		$search_inputs[] = 'Company';
		$query .= ("AND b.company_name REGEXP '^" . addslashes(trim($search_job->search_company_name)) . ".*' ");
	}

	//Add closing dates to search query.

	if ($search_job->search_close_date)
	{
		$search_inputs[] = 'Closing Date';
		if ($search_job->search_close_date_as == 'on' || $search_job->search_close_date_as == '')
		{
			$query .= ("AND a.closing_date='" . addslashes(trim($search_job->search_close_date)) . "' ");
		}
		if ($search_job->search_close_date_as == 'before')
		{
			$query .= ("AND a.closing_date<'" . addslashes(trim($search_job->search_close_date)) . "' ");
		}
		if ($search_job->search_close_date_as == 'after')
		{
			$query .= ("AND a.closing_date>'" . addslashes(trim($search_job->search_close_date)) . "' ");
		}
	}

	//Add workterm start to search query.

	/*
	 This part of the code isn't used anymore, since students can only request that their search be
	 performed on future jobs.  
	*/

	if ($search_job->search_start_term)
	{
		$sql = ("
            SELECT DISTINCT start_date, end_date
            FROM term
            WHERE term_id='" . $search_job->search_start_term . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $start_end = $row;

		$search_inputs[] = 'Start of Workterm';
		if ($search_job->search_start_as == 'on' || $search_job->search_start_as == '')
		{
			$query .= ("AND a.year='" . addslashes(trim($search_job->search_start_year)) . "' ");
			$query .= ("AND a.term_id='" . addslashes(trim($search_job->search_start_term)) . "' ");
		}
		if ($search_job->search_start_as == 'before')
		{
			$query .= ("
				AND ( (a.term_id=c.term_id AND c.end_date<'" . $start_end["start_date"] . "'
				AND a.year='" . trim($search_job->search_start_year) . "')
				OR ( a.year<'" . trim($search_job->search_start_year) . "' ) )
				");
		}
		if ($search_job->search_start_as == 'after')
		{
			$query .= (" 
				AND ( (a.term_id=c.term_id AND c.start_date>'" . $start_end["end_date"] . "' 
				AND a.year='" . trim($search_job->search_start_year) . "')
				OR ( a.year>'" . trim($search_job->search_start_year) . "' ) ) 
				");
		}
	}

	//Add academic years to the search query.

	if ($search_job->search_min_academic)
	{
		$search_inputs[] = 'Min Academic Yrs';
		if ($search_job->search_academic_as == 'equal' || $search_job->search_academic_as == '')
		{
			$query .= ("AND a.min_academic='" . addslashes(trim($search_job->search_min_academic)) . "' ");
		}
		if ($search_job->search_academic_as == 'lesser')
		{
			$query .= ("AND a.min_academic<'" . addslashes(trim($search_job->search_min_academic)) . "' ");
		}
		if ($search_job->search_academic_as == 'greater')
		{
			$query .= ("AND a.min_academic>'" . addslashes(trim($search_job->search_min_academic)) . "' ");
		}
	}

	//Add workterms completed to search query.

	if ($search_job->search_min_workterms != '')
	{
		$search_inputs[] = 'Min Workterms';
		if ($search_job->search_workterms_as == 'equal' || $search_job->search_workterms_as == '')
		{
			$query .= ("AND a.min_workterms='" . addslashes(trim($search_job->search_min_workterms)) . "' ");
		}
		if ($search_job->search_workterms_as == 'lesser')
		{
			$query .= ("AND a.min_workterms<'" . addslashes(trim($search_job->search_min_workterms)) . "' ");
		}
		if ($search_job->search_workterms_as == 'greater')
		{
			$query .= ("AND a.min_workterms>'" . addslashes(trim($search_job->search_min_workterms)) . "' ");
		}
	}

	//Add industries to the search query.

    if ($search_job->search_yes_industries) 
    {               
        for ($i = 0; $i < sizeof($search_job->search_yes_industries); $i++)
        {
            $yes_industry_ids .= ("'" . $search_job->search_yes_industries[$i] . "',");
        }
        $yes_industry_ids = substr($yes_industry_ids, 0, -1);

        $query .= (" 
            AND (
                b.industry_id IN (" . $yes_industry_ids . ")
                OR ed.industry_id IN (" . $yes_industry_ids . ")
                )
            ");
    }       

    if ($search_job->search_no_industries)
    {
        for ($i = 0; $i < sizeof($search_job->search_no_industries); $i++)
        {
            $no_industry_ids .= ("'" . $search_job->search_no_industries[$i] . "',");
        }
        $no_industry_ids = substr($no_industry_ids, 0, -1);

        /*
           The second part of the below query addition should NEVER happen, because
           industries are forced on jobs, but it's there just in case.
         */

        $query .= ("
            AND (
                (b.industry_id NOT IN (" . $no_industry_ids . ") OR
                 b.industry_id IS NULL)
                AND
                (ed.industry_id NOT IN (" . $no_industry_ids . ") OR
                 ed.industry_id IS NULL)
                )
            ");
    }

    //Add disciplines to search query.

    if ($search_job->search_disciplines)
    {
        $search_inputs[] = 'Disciplines';

        $disciplines .= ("'" . ALL_DISCIPLINES . "',");
        foreach ($search_job->search_disciplines AS $sd)
        {
            $disciplines .= ("'" . $sd . "',");
        }
        if ($disciplines)
        {
            $disciplines = substr($disciplines, 0, -1);
        }
        else
        {
            $disciplines = ("''");
        }

        $mini_query = ("
            SELECT DISTINCT job_id
            FROM job_discipline_join
            WHERE discipline_id IN (" . $disciplines . ")
            ");
        $result = $GLOBALS['dbh']->Execute($mini_query);
        while ($row = $result->FetchRow())
        {
            $discipline_job_ids .= ("'" . $row["job_id"] . "',");
        }
        if ($discipline_job_ids)
        {
            $discipline_job_ids = substr($discipline_job_ids, 0, -1);
        }
        else
        {
            $discipline_job_ids = ("''");
        }	

        $query .= ("
            AND a.job_id IN (" . $discipline_job_ids . ")
            ");
    }

    //Add keywords to the search query.

    if ($search_job->search_keywords)
    {
        $query .= ("AND (");
        for ($i = 0; $i < (sizeof($search_job->search_keywords) - 1); $i++)
        {
            $query .= (" (");
            $query .= ("a.position_title REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
            $query .= ("a.other_postings REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
			$query .= ("a.job_description REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
			$query .= ("a.special_requirements REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
			$query .= ("a.skills_required REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
			$query .= ("b.company_name REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
			$query .= ("b.company_description REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' ");
			
			$query .= (") OR");
		}
		$query .= (" (");
		$query .= ("a.position_title REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
		$query .= ("a.other_postings REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
		$query .= ("a.job_description REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
		$query .= ("a.special_requirements REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
		$query .= ("a.skills_required REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
		$query .= ("b.company_name REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' OR ");
		$query .= ("b.company_description REGEXP '[[:<:]]" . addslashes($search_job->search_keywords[$i]) . "[[:>:]]' ");
		$query .= (") ");

		$query .= (") ");
	}

	//Add keywords to not have into the search query.

    if ($search_job->search_not_keywords)
    {
        $query .= ("AND NOT (");
        for ($i = 0; $i < (sizeof($search_job->search_not_keywords) - 1); $i++)
        {
            $query .= (" (");
            $query .= ("a.position_title REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
            $query .= ("a.other_postings REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
            $query .= ("a.job_description REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
            $query .= ("a.special_requirements REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
            $query .= ("a.skills_required REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
            $query .= ("b.company_name IS NOT NULL AND b.company_name REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
            $query .= ("b.company_description IS NOT NULL AND b.company_description REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' ");

            $query .= (") OR");
        }
        $query .= (" (");
        $query .= ("a.position_title REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
        $query .= ("a.other_postings REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
        $query .= ("a.job_description REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
        $query .= ("a.special_requirements REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
        $query .= ("a.skills_required REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
        $query .= ("b.company_name IS NOT NULL AND b.company_name REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' OR ");
        $query .= ("b.company_description IS NOT NULL AND b.company_description REGEXP '[[:<:]]" . addslashes($search_job->search_not_keywords[$i]) . "[[:>:]]' ");
        $query .= (") ");

        $query .= (") ");
    }

    $query .= (" ORDER BY a.job_code, a.position_title, b.company_name");



    $limit = 5 - sizeof($display_inputs);

    for ($i = 0; $i < $limit; $i++)
    {
        for ($j = 0; $j < sizeof($search_inputs); $j++)
        {
            if (!is_array($display_inputs) || !in_array($search_inputs[$j], $display_inputs))
            {
                $display_inputs[] = $search_inputs[$j];
                break;
            }
        }
    }

    $limit = 5 - sizeof($display_inputs);

    for ($i = 0; $i < $limit; $i++)
    {
        if (!is_array($display_inputs) || !in_array('Job Code', $display_inputs))
        {
            $display_inputs[] = 'Job Code';
        } elseif (!is_array($display_inputs) || !in_array('Company', $display_inputs))
        {
            $display_inputs[] = 'Company';
        } elseif (!is_array($display_inputs) || !in_array('Min Academic Yrs', $display_inputs))
        {
            $display_inputs[] = 'Min Academic Yrs';
        } elseif (!is_array($display_inputs) || !in_array('Disciplines', $display_inputs))
        {
            $display_inputs[] = 'Min Workterms';
        } elseif (!is_array($display_inputs) || !in_array('Status', $display_inputs))
        {
            $display_inputs[] = 'Status';
        }
    }

    $total_sql = ("
        SELECT DISTINCT a.job_id
        FROM job_info AS a, term AS c
        WHERE (a.status=" . POSTED . " OR a.status=" . CLOSED . " OR (a.status=" . CANCELLED . " AND a.student_status='".STUDENT_NOT_HIRING."'))
        ");

    if ($search_job->search_future_jobs == 'no')
    {
        $extra_sql .= ("
            AND a.term_id='" . addslashes($search_job->search_show_term) . "'
            AND a.year='" . addslashes($search_job->search_show_year) . "' 
            ");
    }
    elseif ($search_job->search_future_jobs == 'yes')
    {
        $extra_sql .= ("
            AND (
                (a.term_id='" . addslashes($search_job->search_show_term) . "' 
                AND a.year='" . addslashes($search_job->search_show_year) . "')
                OR
                ( (a.term_id=c.term_id AND c.start_date>'" . $start_end["end_date"] . "'
                AND a.year='" . trim($search_job->search_show_year) . "')
                OR ( a.year>'" . trim($search_job->search_show_year) . "' ) )
            )	
            ");
    }

    $total_sql .= $extra_sql;
    $total_result = $GLOBALS['dbh']->Execute($total_sql);
    $num_result = $GLOBALS['dbh']->Execute($query);
    $num_returned = $num_result->RecordCount();
    
    if (($per_page_max = (int)$per_page_max) < 5){
        $per_page_max = 5;
    }

    $limit_query .= ("
            LIMIT " . $start_row . ", " . $per_page_max
            );
	$query .= $limit_query;
    $result = $GLOBALS['dbh']->Execute($query);
    $num_jobs = $result->RecordCount();
	if ($num_jobs == 0)
	{
		notify("There are no jobs currently registered in the database that match your search criteria.");
	}
	else
	{
	
		$pages = ceil($num_returned / $per_page_max);
	
	        $first = $start_row + 1;
	        $end = $start_row + $per_page_max;
	        if ($end > $num_returned) { $end = $num_returned; }
	
		/*
		 Big change here.  Since the user is searching on criteria THEY specify.  It's not necessary to display
		 a whole bunch of information when showing the search results.  Instead, it's best to just display
		 the job code, job name, and company name, and then they can click on the job for more information.
		*/
			
		printf("<h4 align='left'>Results of search (performed on %d jobs):</h4>", ($total_result->RecordCount()));
        if($num_jobs >= 50 || $per_page_max!=50 )
        {
		echo("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

                echo("<tr>");
                echo("<td>");
                echo($num_returned . " job" . (($num_returned != 1) ? "s" : "") . " returned on " . $pages . " page" . (($pages > 1) ? "s" : "").";</td>");
                echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='" . $per_page_max . "' />");
                echo(" jobs per page.");
                echo("</td>");
                echo("</tr>");

                echo("<tr>");

                echo("<td>");
                echo("<select name='start_row'>");

                for ($i = 0; $i < $pages; $i++)
                {
                    $row_num_start = $i * $per_page_max;
                    $row_num_end = $row_num_start + $per_page_max - 1;
                    if ($row_num_end >= $num_returned) $row_num_end = $num_returned - 1;

                    $num_result->Move($row_num_start);
                    $num_result_row = $num_result->FetchRow();
                    $start = getJobCode($num_result_row[0]);

                    $num_result->Move($row_num_end);
                    $num_result_row = $num_result->FetchRow();
                    $end = getJobCode($num_result_row[0]);

                    if (strlen($start) > 15) { $start = substr($start, 0, 13) . "..."; }
                    if (strlen($end) > 15) { $end = substr($end, 0, 13) . "..."; }

                    echo("<option value='" . $row_num_start . "' ");
                    if ($start_row == $row_num_start)
                    {
                            echo("selected='selected'");
                    }
                    echo(">" . $start . " to " . $end . "</option>\n");
                }

                echo("</select>");
                echo("</td>");

                echo("<td align=right>");
                echo("<input type='submit' name='continue' value='Go' />");
                echo("</td>");

                echo("</tr>");
                echo("</table>");
           }	
	        echo("<table cellspacing='0' cellpadding='0' border='1'>");
		echo("<tr>");
		echo("<td>\n");
	        echo("<table border='0' cellpadding='2'>");
	        echo("<tr>");
			echo("<td class='rowgrey' align='center'><img src='misc/images/job_flagged.gif' alt='' /></td>");
            //echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b></td>");
			echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b></td>");
			echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Department</b></td>");
			echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b></td>");
			echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b></td>");
			echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Your Status</b></td>");
			echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Flag/Unflag Job</b>&nbsp;</td>");
		echo("</tr>");
	
		$rowclass = 0;
		while ($row = $result->FetchRow())
		{
			//$job_code = $row["job_code"];
            $company_name = $row["company_name"];
			$min_academic = $row["min_academic"];
			$min_workterms = $row["min_workterms"];
			$student_status= $row["student_status"];
			$resumes_pulled = $row["resumes_pulled"];
			$job_id = $row["job_id"];
			$closing_date = $row["closing_date"];
			$start_month = $row["term_id"];
			$start_year = $row["year"];
			$position_title = $row["position_title"];
		        
			$sql = ("
				SELECT DISTINCT d.department_code
				FROM department_job_join AS djj, department AS d
				WHERE djj.job_id='" . $job_id . "'
				AND d.department_id=djj.department_id
				ORDER BY d.department_id
				");
			$result2 = $GLOBALS['dbh']->Execute($sql);
			
			$apply_departments = '';
			if ($result2->RecordCount())
			{
				while ($row2 = $result2->FetchRow())
				{
					$apply_departments .= ($row2["department_code"] . ", ");
				}
				$apply_departments = substr($apply_departments, 0, -2);
			}
			else
			{
				$apply_departments = ("None");
			}

			$href = $PHP_SELF . "&amp;select=search_job&amp;job_id=" . urlencode($job_id);
		
		
			echo("<tr>");
				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
				echo((is_job_flagged($job_id, $student_number)) ? "<img src='misc/images/job_flagged.gif' alt='' />" : "&nbsp;");
				echo("</td>");

				/*
				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
				echo("&nbsp;");
				echo("<a class='blue' href='$href'>" . $job_code . "</a>&nbsp;");
				echo("</td>\n");
				*/

				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
				echo("&nbsp;");
				echo("<a class='blue' href='$href'>" . $company_name . "</a>&nbsp;");
				echo("</td>\n");	

				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo($apply_departments);
				echo("</td>");
		
				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				$disc_string = '';
				$disc_array = getDiscArray($job_id);
				foreach ($disc_array AS $da)
				{
					$disc_string .= ($da . ", ");
				}
				$disc_string = substr($disc_string, 0, -2);
				echo($disc_string);
				echo("</td>\n");

				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
		        echo(getStudentStatusGif($job_id));
                /*
                 * :EXPIRY: May 31 2005 - Patrick - We have the function above which handles this stuff, no need for the switch statement.
                switch($student_status)
				{
				case "1":
					echo("&nbsp;<img src='misc/images/doc10.gif' alt='' />&nbsp;");
					if ($resumes_pulled)
					{
						echo("&nbsp;<img src='misc/images/Warning.gif' alt='' />&nbsp;");
					}
					break;
				case "2":
					echo("&nbsp;<img src='misc/images/cancel.gif' alt='' />&nbsp;");
					break;
				case "3":
					echo("&nbsp;<img src='misc/images/job_shortlist.gif' alt='' />&nbsp;");
					break;
				case "4":
					echo("&nbsp;<img src='misc/images/clock.gif' alt='' />&nbsp;");
					break;
				case "5":
					echo("&nbsp;<img src='misc/images/smchk_gr.gif' alt='' />&nbsp;");
					break;
				case "6":
					echo("&nbsp;<img src='misc/images/smchk_yl.gif' alt='' />&nbsp;");
					break;
				case "7":
					echo("&nbsp;<img src='misc/images/X.gif' alt='' />&nbsp;");
					break;
				}
                */
				echo("</td>\n");
			
				echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				echo("&nbsp;");
                 // Determine how far this student has progressed in this job.
                echo(getStudentOwnStatusGif($job_id, $student_number));
               
                /*
                 * :EXPIRY: May 31 2005 - Patrick - We have the function above which handles this stuff, no need for the switch statement.
                if (hasStudentApplied($row["job_id"], $student_number))
				{
					echo("<img src='misc/images/you_applied.gif' alt='' />&nbsp;");
				}
				if (isStudentShortlisted($row["job_id"], $student_number) && isShowable($row["job_id"]) && hasStudentApplied($row["job_id"], $student_number))
				{
					echo("<img src='misc/images/you_have_been_shortlisted.gif' alt='' />&nbsp;");
				}
				if (isStudentSignedUp($row["job_id"], $student_number) && isShowable($row["job_id"]) && hasStudentApplied($row["job_id"], $student_number))
				{
					echo("<img src='misc/images/you_have_an_interview.gif' alt='' />&nbsp;");
				}
				if (hasStudentBeenPlacedInJob($row["job_id"], $student_number))
				{
					echo("<img src='misc/images/you_have_been_placed.gif' alt='' />&nbsp;");
				}
				if (!further_progress($job_id, $student_number))
				{
					echo("<img src='misc/images/no_more_progress.gif' alt='' />&nbsp;");
				}
                */
				echo("</td>");
			
				echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
				if (is_job_flagged($job_id, $student_number))
				{
					echo("<input type='checkbox' name='jobs_to_unflag[]' value='" . $job_id . "' />");
				}
				else
				{
					echo("<input type='checkbox' name='jobs_to_flag[]' value='" . $job_id . "' />");
				}
				echo("</td>");

			echo("</tr>\n");
			$rowclass++;
		}

		echo("<tr>");
			echo("<td align='center' colspan='8' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
			echo("<input type='submit' name='continue' value='Flag/Unflag Selected Jobs' />");
			echo("</td>");
		echo("</tr>");
	
		echo("</table>");
		echo("</td></tr></table>");
		
	
		?>
	
		<h4>Status Icon Key:</h4>
		
		<table cellspacing="0" cellpadding="0" border="1">
		<tr>
		<td>
		<table border="0" cellpadding="2">
		<?php
		echo("<tr>");
			echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>");
			echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/doc10.gif' alt='' />&nbsp;</td>");
			echo("<td class='row0d' align='center'>&nbsp;" . getStudentStatusName(STUDENT_ACCEPTING_APP) . "&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/cancel.gif' alt='' />&nbsp;</td>");
			echo("<td class='row1d' align='center'>&nbsp;" . getStudentStatusName(STUDENT_NOT_ACCEPTING_APP) . "&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/Warning.gif' alt='' />&nbsp;</td>");
			echo("<td class='row0d' align='center'>&nbsp;Resumes pulled&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/job_shortlist.gif' alt='' />&nbsp;</td>");
			echo("<td class='row1d' align='center'>&nbsp;" . getStudentStatusName(STUDENT_SHORTLIST) . "&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/clock.gif' alt='' />&nbsp;</td>");
			echo("<td class='row0d' align='center'>&nbsp;" . getStudentStatusName(STUDENT_WAIT_RANKING) . "&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/smchk_gr.gif' alt='' />&nbsp;</td>");
			echo("<td class='row1d' align='center'>&nbsp;" . getStudentStatusName(STUDENT_HIRED_UVIC) . "&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/smchk_yl.gif' alt='' />&nbsp;</td>");
			echo("<td class='row0d' align='center'>&nbsp;" . getStudentStatusName(STUDENT_HIRED_ELSEWHERE) . "&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/X.gif' alt='' />&nbsp;</td>");
			echo("<td class='row1d' align='center'>&nbsp;" . getStudentStatusName(STUDENT_NOT_HIRING) . "&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/you_applied.gif' alt='' />&nbsp;</td>");
			echo("<td class='row0d' align='center'>&nbsp;You have applied to this job&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/your_app_has_been_pulled.gif' alt='' />&nbsp;</td>");
			echo("<td class='row1d' align='center'>&nbsp;Your application was added by admin&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/you_have_been_shortlisted.gif' alt='' />&nbsp;</td>");
			echo("<td class='row0d' align='center'>&nbsp;You have been shortlisted&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/you_have_an_interview.gif' alt='' />&nbsp;</td>");
			echo("<td class='row1d' align='center'>&nbsp;You have an interview&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/you_have_been_placed.gif' alt='' />&nbsp;</td>");
			echo("<td class='row0d' align='center'>&nbsp;You have been placed&nbsp;</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/no_more_progress.gif' alt='' />&nbsp;</td>");
			echo("<td class='row1d' align='center'>&nbsp;You did not get this job&nbsp;</td>");
		echo("</tr>");
	
		echo("</table>");
		echo("</td>");
		echo("</tr>");
		echo("</table>");
		echo("</form>");
	}
}
?>
