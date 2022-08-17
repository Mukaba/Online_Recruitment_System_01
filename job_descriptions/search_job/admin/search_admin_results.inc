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
 | Filename: search_admin_results.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file forms the query, runs it against the database, and    |
 | displays the results of the search for the user.                             |
 +------------------------------------------------------------------------------+

*/
// These variables will help reduce the problem of too many job on a screen
if ($per_page_max == "") { $per_page_max = 50; }
if ($per_page_max < 5) { $per_page_max = 5; }
if (!isInteger($per_page_max)) { $per_page_max = 50;}
if ($start_row == '') { $start_row = 0; }
$export_string='';

// Paging form
echo("<form method='post' action='" . $PHP_SELF . "'>");

if( isset($quick_type) )
{
    switch( $quick_type )
    {
        case( "contact_quickview" ):
            echo("<input type='hidden' name='contact_id' value='$contact_id' />");
            break;
        
        case( "division_quickview" ):
            echo("<input type='hidden' name='department_id' value='$department_id' />");
            break;
        
        case( "company_quickview" ):
            echo("<input type='hidden' name='employer_id' value='$employer_id' />");
            break;
    
        default:
            assert(false);  //we should never be here
    }
}
//print("search_job value is...".$search_job);
//print("<br />");
  //      print("<pre>");
    //    print_r(get_object_vars($search_job));
      //  print("</pre>");
 
echo("<input type='hidden' name='select' value='search_job' />");
echo("<input type='hidden' name='search_job' value='" . packObject($search_job). "' />");
//echo("============");
//print("<pre>");
  //      print_r(get_object_vars($search_job));
    //    print("</pre>");

 
//variables used to indicate whether to show the quickmenu and which one (either company/division/contact)
if( isset($quick_type) )
{
    echo("<input type='hidden' name='quick_type' value='$quick_type' />");
}
echo("<input type='hidden' name='show_quick' value='$show_quick' />");

//indicates if we are coming from company/division/contact quickmenu
echo("<input type='hidden' name='quickmenu_search' value='$quickmenu_search' />");

//indicates what columns are to be included in search results (when coming from company/division/contact quickmenu)
if( $quickmenu_search )
{
    if( is_string( $display_inputs ) )
    {
        echo("<input type='hidden' name='display_inputs' value='".($display_inputs)."' />");
        $display_inputs = unpackObject(($display_inputs));
    }
    else
    {
        echo("<input type='hidden' name='display_inputs' value='".(packObject(($display_inputs)))."' />");
    }
} 


/*
 We will only need to search for either area, country, province/state, or region, so determine which of
 the four it is we need to search for, and then use the corresponding search input.
*/
if ($search_job->search_region)
{
	$search_location = "region";
	$search_location_key = $search_job->search_region;
}
elseif ($search_job->search_provstate)
{
	$search_location = "provstate";
	$search_location_key = $search_job->search_provstate;
}
elseif ($search_job->search_country)
{
	$search_location = "country";
	$search_location_key = $search_job->search_country;
}
elseif ($search_job->search_area)
{
	$search_location = "area";
	$search_location_key = $search_job->search_area;
}

$nextTermYear = getNextTermYear();
// figure out if there's jobs in the system or not
$count_jobs = $GLOBALS['dbh']->GetOne("
        SELECT COUNT(job_id)
		FROM job_info
		WHERE status <> ".INT_ONLY." 
        AND status <> ".REJECTED."
        ");

//echo("<pre>"); 
  // print_r($search_job->search_order); //print "a"
//echo("</pre>");
 
//print("<pre>");
 //  print_r("job conting".$count_jobs);
//print("</pre>");

if (!$count_jobs)
{
	echo("<p>There are currently no jobs registered in the system.</p>\n");
}
else      
{
	// there are jobs in the system, build a query to search through them with the returned form parameters
    $query_start = "SELECT DISTINCT ";

    // set the first selected element to be determined by what the user chose as the first ordering parameter.  We will use this as what we display
    // in the form used for switching between pages
    $order_parameters = array(
                            'a.job_code' => 'a.job_code'
                            , 'b.company_name' => 'b.company_name'
                            , 'a.closing_date' => 'a.closing_date'
                            , 'a.date_entered' => 'a.date_entered'
                            , 'a.status' => 'a.status'
                            , 'a.year' => 'a.year'
                            , 'a.min_workterms' => 'a.min_workterms'
                            );
  // echo("<pre>"); 
  // print_r($search_job->search_order); //print "a"
// echo("</pre>");
// include('job_descriptions/edit_job/EditJob.class');
  //$search_job = restore_job($search_job);
 //echo("<pre>");
 //  print_r($search_job->search_order);
 //  foreach($search_job->search_order as $i){
 //    echo "Value: $i<br>\n";
 //  }    
 // echo("</pre>");
   if ($search_job->search_order[0])
    {
        $query_start .= $search_job->search_order[0].', ';

        // remove them from order_parameters so we know what to add to the main query's select statement
        unset($order_parameters[$search_job->search_order[0]]);
    }
       // append the missing columns to the select statement
    $query_start .= implode(',', $order_parameters).', ';
// print($query_start);
	$query_start .= ("
		            a.job_id
                    , a.position_title
                    , a.min_academic
		            , a.admin_status
                    , a.term_id
                    , a.contact_id 
                    , a.region_id
                    , a.city
                    , a.country_id
                    , a.provstate_id
                    , a.year
                    , ed.industry_id
                    , b.company_type_id
        ");

	$query = ("
		FROM job_info AS a
		LEFT JOIN employer_company AS b ON b.employer_id=a.employer_id
		LEFT JOIN employer_department AS ed ON ed.department_id=a.employer_department_id
        LEFT JOIN country_list AS d ON d.country_id=a.country_id
		WHERE a.job_id IS NOT NULL
		AND (a.status!=" . INT_ONLY . " AND a.status!=" . REJECTED . ")
        ");

    // Set up rest of query here.
    $job_id_list = '';
    
    if($search_job->search_on_jobs == 'group')
    {
        /*
           If they want to see jobs for their group, first check if they're department is part of a group,
           If they're not, then just continue on and display as if they had selected to view jobs for their
           department.
         */
        $group_sql = ("
            SELECT DISTINCT group_id
            FROM department_group
            WHERE department_id = '".addslashes($auth->department)."'
            ");
        
        $group_result = $GLOBALS['dbh']->Execute($group_sql);

        if ($group_result->RecordCount())
        {
            /*
               Now pull out all departments associated with this group, and put them into a comma delimited
               string.
             */
            $group_row = $group_result->FetchRow();
            $group = $group_row["group_id"];

            $sql = ("
                SELECT DISTINCT department_id
                FROM department_group
                WHERE group_id='" . $group . "'
                ");
            
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                $department_ids .= ("'" . $row["department_id"] . "',");
            }
            $department_ids = substr($department_ids, 0, -1);

            /*
               Now pull out all of the job_ids that are listed in the department_job_join table
               that have one of these departments listed.  This means that we will be returned
               all of the job_ids in the system that have one of the departments in this group
               listed as applicable.
             */
            $sql = ("
                SELECT DISTINCT job_id
                FROM department_job_join
                WHERE department_id IN (".$department_ids.")
                ");
            
            $result = $GLOBALS['dbh']->Execute($sql);

            // build a comma delimited list
            while ($row = $result->FetchRow())
            {
                $job_id_list .= ("'" . $row["job_id"] . "',");
            }
            $job_id_list = substr($job_id_list, 0 , -1);

            if (!$job_id_list)
            {
                $job_id_list = ("''");
            }

            $extra_sql = ("
                AND a.job_id IN (" . $job_id_list . ")
                ");
        }
    }
    elseif($search_job->search_on_jobs == 'department')
    {
        // If they weren't part of a group, they'll fall through here into the department case.  This is desirable.
        // Get all of the jobs that have this department listed as applicable.
        $sql = ("
            SELECT DISTINCT job_id
            FROM department_job_join
            WHERE department_id='" . $auth->department . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        // comma delimited list
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
            AND a.job_id IN (" . $job_id_list . ")
            ");
    }
    elseif(preg_match("/\d.*/", $search_job->search_on_jobs, $matches))
    {
        // in this case, our login is traffic director or director's office and $search_job->search_on_job is a department_id (a digit)
        // DEBUG echo("Regular expression matches: ". $matches[0]."<br />");
        $sql = ("
            SELECT DISTINCT job_id
            FROM department_job_join
            WHERE department_id =" .$search_job->search_on_jobs. "
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
       
        // comma delimited list
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
            AND a.job_id IN (" . $job_id_list . ")
        ");
    }
    elseif($search_job->search_on_jobs == 'uvic')
    {
        //do nothing to the query
        $extra_sql= "";
    }
    else
    {
         // default case, theres is no value for $search_job->search_on_jobs
        assert(false);
    } 

    // append the job_id condition to the main query
    $query .= $extra_sql;

    // Add status of job to search query.
    if ($search_job->search_status)
    {
        $pre_query = ("
                SELECT DISTINCT job_id
                FROM applications
                WHERE app_status IN ('".APP_ACTIVATED_ONLINE."', '".APP_PAPER_SENT."')
                ");
        $pre_result = $GLOBALS['dbh']->Execute($pre_query);
        while ($pre_row = $pre_result->FetchRow())
        {
            $apps_sent_ids .= ("'" . $pre_row["job_id"] . "',");
        }
        if ($apps_sent_ids)
        {
            $apps_sent_ids = substr($apps_sent_ids, 0, -1);
        }
        else
        {
            $apps_sent_ids = ("''");
        }

        $search_inputs[] = 'Status';
        $query .= ("AND (");
        for ($i = 0; $i < (sizeof($search_job->search_status) - 1); $i++)
        {
            if ($search_job->search_status[$i] == '12a')
            {
                $query .= ("(a.admin_status='".ADMIN_STATUS_CLOSED."' AND a.job_id NOT IN (" . $apps_sent_ids . ")) OR ");
            }
            elseif ($search_job->search_status[$i] == '12b')
            {
                $query .= ("(a.admin_status='".ADMIN_STATUS_CLOSED."' AND a.job_id IN (" . $apps_sent_ids . ")) OR ");
            }
            else
            {
                $query .= ("a.admin_status='" . $search_job->search_status[$i] . "' OR ");
            }
        }
        if ($search_job->search_status[$i] == '12a')
        {
            $query .= ("(a.admin_status='".ADMIN_STATUS_CLOSED."' AND a.job_id NOT IN (" . $apps_sent_ids . "))) ");
        }
        elseif ($search_job->search_status[$i] == '12b')
        {
            $query .= ("(a.admin_status='".ADMIN_STATUS_CLOSED."' AND a.job_id IN (" . $apps_sent_ids . "))) ");
        }
        else
        {
            $query .= ("a.admin_status='" . $search_job->search_status[$i] . "') ");
        }
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


    // Add location to search query
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
                $query .= ("AND d.area_id='" . $search_location_key . "' ");
            break;
        }
    }

    // Add job codes to search query
    if( $search_job->search_job_codes )
    {
        $search_inputs[] = 'Job Code';
        $query .= ("AND a.job_code LIKE '%" . addslashes(trim($search_job->search_job_codes)) . "%' ");
    }
    
    if( $search_job->search_contact_id  )
    {
        $query .= ("AND a.contact_id = '" . addslashes(trim($search_job->search_contact_id)) . "' ");
    }
    elseif( $search_job->search_division_id  )
    {
        $query .= ("AND a.employer_department_id = '".addslashes(trim($search_job->search_division_id)) . "' ");
    }
    elseif( $search_job->search_company_name )
    {
        $search_inputs[] = 'Company';
        $query .= ("AND b.company_name LIKE '%" . addslashes(trim($search_job->search_company_name)) . "%' ");
    }

    // Add closing dates to search query.
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
    if ($search_job->search_entry_date)
    {
        $search_inputs[] = 'Entry Date';
        
        if ($search_job->search_entry_date_as == 'on' || $search_job->search_entry_date_as == '')
        {
            $query .= ("AND a.date_entered='" . addslashes(trim($search_job->search_entry_date)) . "' ");
        }
        if ($search_job->search_entry_date_as == 'before')
        {
            $query .= ("AND a.date_entered<'" . addslashes(trim($search_job->search_entry_date)) . "' ");
        }
        if ($search_job->search_entry_date_as == 'after')
        {
            $query .= ("AND a.date_entered>'" . addslashes(trim($search_job->search_entry_date)) . "' ");
        }
    }
    // Add workterm start to search query.
    if ($search_job->search_start_term != 'ALL')
    {
        $query .= ("AND a.term_id='" . addslashes(trim($search_job->search_start_term)) . "' ");
    }
    if ($search_job->search_start_year != 'ALL')
    {
        $query .= ("AND a.year='" . addslashes(trim($search_job->search_start_year)) . "' ");
    }

    // Add academic years to the search query.
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

    // Add workterms completed to search query.
    if (trim($search_job->search_min_workterms) != '')
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

    // Add industries to the search query.
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
//print("<pre>");
//print_r($search_job->search_disciplines);
// print("</pre>");   // Add disciplines to search query.
    if ($search_job->search_disciplines)
    {
        $search_inputs[] = 'Disciplines';
        $disciplines .= ("'" . ALL_DISCIPLINES . "',");
        foreach ($search_job->search_disciplines as $sd) //error
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

    // Add keywords to the search query.
    if ($search_job->search_keywords)
    {
        $query .= ("AND (");
        for ($i = 0; $i < (sizeof($search_job->search_keywords) - 1); $i++)
        {
            $query .= (" (");
            $query .= ("a.position_title REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.other_postings REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.job_description REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.special_requirements REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.skills_required REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("b.company_name REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("b.company_description REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.comments REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.admin_comments REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' ");

            $query .= (") OR");
        }
        $query .= (" (");
        $query .= ("a.position_title REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.other_postings REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.job_description REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.special_requirements REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.skills_required REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("b.company_name REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("b.company_description REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.comments REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.admin_comments REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' ");
        $query .= (") ");

        $query .= (") ");
    }

    // Add keywords to not have into the search query.
    if ($search_job->search_not_keywords)
    {
        $query .= ("AND NOT (");
        for ($i = 0; $i < (sizeof($search_job->search_not_keywords) - 1); $i++)
        {
            $query .= (" (");
            $query .= ("a.position_title REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.other_postings REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.job_description REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.special_requirements REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.skills_required REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.comments REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("a.admin_comments REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_keywords[$i])) . "[[:>:]]' OR ");
            $query .= ("(b.company_name IS NOT NULL AND b.company_name REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]') OR ");
            $query .= ("(b.company_description IS NOT NULL AND b.company_description REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]') ");

            $query .= (") OR");
        }
        $query .= (" (");
        $query .= ("a.position_title REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.other_postings REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.job_description REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.special_requirements REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.skills_required REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.comments REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("a.admin_comments REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]' OR ");
        $query .= ("(b.company_name IS NOT NULL AND b.company_name REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]') OR ");
        $query .= ("(b.company_description IS NOT NULL AND b.company_description REGEXP '[[:<:]]" . addslashes(removeSlashes($search_job->search_not_keywords[$i])) . "[[:>:]]') ");
        $query .= (") ");

        $query .= (") ");
    }

    $query .= ("ORDER BY ");

    if ($search_job->search_order)
    {
        for ($i = 0; $i < (sizeof($search_job->search_order) - 1); $i++)
        {
            $query .= ($search_job->search_order[$i] . ", ");
        }
        $query .= ($search_job->search_order[$i]);
    } 
    else
    {
        $query .= ("a.job_code, a.status, b.company_name");
    }
/*
    // Now set the display inputs, so we know what to display on screen.
    for ($i = 0; $i < sizeof($search_job->search_order); $i++)
    {
        switch($search_job->search_order[$i])
        {
            case 'a.closing_date':
                if (!is_array($display_inputs) || !in_array('Closing Date', $display_inputs))
                {
                    $display_inputs[] = 'Closing Date';
                }
                break;
            case 'b.company_name':
                if (!is_array($display_inputs) || !in_array('Company', $display_inputs))
                {
                    $display_inputs[] = 'Company';
                }
                break;
            case 'a.job_code':
                if (!is_array($display_inputs) || !in_array('Job Code', $display_inputs))
                {
                    $display_inputs[] = 'Job Code';
                }
                break;
            case 'a.status':
                if (!is_array($display_inputs) || !in_array('Status', $display_inputs))
                {
                    $display_inputs[] = 'Status';
                }
                break;
            case 'a.min_academic':
                if (!is_array($display_inputs) || !in_array('Min Academic Yrs', $display_inputs))
                {
                    $display_inputs[] = 'Min Academic Yrs';
                }
                break;
            case 'a.min_workterms':
                if (!is_array($display_inputs) || !in_array('Min Workterms', $display_inputs))
                {
                    $display_inputs[] = 'Min Workterms';
                }
                break;
        }
    }
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
            $display_inputs[] = 'Disciplines';
        } elseif (!is_array($display_inputs) || !in_array('Status', $display_inputs))
        {
            $display_inputs[] = 'Status';
        }
    }
*/
    // Pull out only the job_ids so we know how many we have without a limit imposed.
    /*
    $sql_copy = ("
            SELECT a.job_id
            ");
    $sql_copy .= $query;
    */
    $total_sql = ("
        SELECT DISTINCT job_id
        FROM job_info
        WHERE (status!=" . INT_ONLY . " AND status!=" . REJECTED . ")
        ");

    if ($search_job->search_start_term != 'ALL')
    {
        $total_sql .= ("
            AND term_id='" . $search_job->search_start_term . "'
            ");
    }
    if ($search_job->search_start_year != 'ALL')
    {
        $total_sql .= ("
            AND year='" . $search_job->search_start_year . "'
            ");
    }

    $total_result = $GLOBALS['dbh']->Execute($total_sql);
//print("<pre>");
//print_r($total_sql);
//print("</pre>");

    // queries for paging
    $num_result = $GLOBALS['dbh']->Execute($query_start . $query);
    $num_returned = $num_result->RecordCount();
    // build a job_id_list
    $job_id_list = '';
    while ($row = $num_result->FetchRow())
    {
        $job_id_list .= ("'" . $row["job_id"] . "',");
    }
    $job_id_list = substr($job_id_list, 0, -1);

    if (!$job_id_list)
    {
        $job_id_list = ("''");
    }
    
    // Set query for export before limits applied
    $export_query = $query_start . $query;

    $limit_query = ("
        LIMIT " . $start_row . ", " . $per_page_max
        );

    $query .= $limit_query;

    // main query results
    $sql = $query_start . $query;
//print($sql);
$result = $GLOBALS['dbh']->Execute($sql);

    $num_results = $result->RecordCount();
    //Display the results if there are any
    if ($num_results != 0)
    {
        // set integer variables used for paging
        $pages = ceil($num_returned / $per_page_max);

        $first = $start_row + 1;
        $end = $start_row + $per_page_max;
        if ($end > $num_returned) 
        { 
            $end = $num_returned; 
        }
        
        //printf("<h4 align='left'>Results of search (%d out of %d jobs):</h4>", ($result->RecordCount()), ($total_result->RecordCount()));
        printf("<h4 align='left'>Results of search (%d out of %d jobs):</h4>", $num_returned, ($total_result->RecordCount()));
   
        // form for switching between pages
        echo("<table border='0' width='55%' class=row1 cellspacing=0 cellpadding=4>");
        echo("<tr>\n");
            echo("<td>");
            echo($num_returned . " job" . (($num_returned != 1) ? "s" : "") . " returned on " . $pages . " page" . (($pages > 1) ? "s" : "").";</td>");
            echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='" . $per_page_max . "' />");
            echo(" jobs per page.");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>\n");
            echo("<td>");
            echo("<select name='start_row'>");

            for ($i = 0; $i < $pages; $i++)
            {
                $row_num_start = $i * $per_page_max;
                $row_num_end = $row_num_start + $per_page_max - 1;
                if ($row_num_end >= $num_returned) $row_num_end = $num_returned - 1;

                $num_result->Move($row_num_start);
                $num_result_row = $num_result->FetchRow();
                $start = $num_result_row[0];

                $num_result->Move($row_num_end);
                $num_result_row = $num_result->FetchRow();
                $end = $num_result_row[0];

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
            for ($i = 0; $i < sizeof($display_inputs); $i++)
            {
                if($display_inputs[$i] != "")
                {
                    echo("<input type='hidden' name='display_inputs[]' value='$display_inputs[$i]' />");
                }
            }
                    
            echo("<input type='submit' name='submit' value='Go' />");
            echo("</td>");
            echo("</tr>\n");
        echo("</table>\n");
    }
    
    if( $quickmenu_search )
    { 
        //form for sorting the output - only used when job descriptions search reached via company/division/contact quickmenu  
        echo("<table border='0' width='55%' class=row1 cellspacing=0 cellpadding=4>");
        echo("<tr>\n");
            echo("<td colspan='2'>");
                if( $num_results != 0 ) //if there are any results:
                { 
                    echo("<hr />");       //we need the HR because the paging form is present
                }
                echo("Sort By:<br />");
                echo("<center>");
                echo("<table>");
                echo("<tr>\n");
                    echo("<td>");
                    echo("Jobs that:");
                    echo("</td>");
                    echo("<td>");
                    
                    //Select which group you want to choose jobs from (institution, co-op group, or department) 
                    echo("&nbsp;&nbsp;&nbsp;<select name='search_on_jobs'>");
                    echo("<option value='department' ");
                    if ($search_job->search_on_jobs == 'department')
                    {
                        echo("selected='selected'");
                    }

                    echo(">are in your department</option>");
                    echo("<option value='group' ");
                    if ($search_job->search_on_jobs == 'group')
                    {
                        echo("selected='selected'");
                    }
                    echo(">are in your Co-op group</option>");
                    echo("<option value='uvic' ");

                    if ($search_job->search_on_jobs == 'uvic')
                    {
                        echo("selected='selected'");
                    }
                    echo(">are registered at ".ORGANIZATION_CODE."</option>");
                    echo("</select>"); 
                    
                    echo("</td>");
                echo("</tr>\n");
                
                //Select which term you want to choose jobs from
                echo("<tr>\n");
                    echo("<td>");
                    echo("During:");
                    echo("</td>");
                    echo("<td>");
                    echo("<center><select name='search_start_term'>");

                    echo("<option value='ALL' ");
                    if ($search_start_term == 'ALL')
                    {
                        echo("selected='selected'");
                    }
                    echo(">ALL</option>");

                    $sql = ("
                        SELECT DISTINCT term_name, term_id
                        FROM term
                        ");

                    $result_sort_term = $GLOBALS['dbh']->Execute($sql);
                    while($row = $result_sort_term->FetchRow())
                    {
                        echo("<option value='" . $row["term_id"] . "'");
                        if ($search_job->search_start_term == $row["term_id"] || (!isset($search_job->search_start_term) 
                            && $row["term_id"] == $nextTermYear["term"]))
                        {
                            echo(" selected='selected'");
                        }
                        echo(">" . $row["term_name"] . "</option>");
                    }
                    echo("</select>&nbsp;");
                    
                    echo("<select name='search_start_year'>");
                    echo("<option value='ALL' ");
                    if ($search_job->search_start_year == 'ALL')
                    {
                        echo("selected='selected'");
                    }
                    echo(">ALL</option>");
                    $tempdate = explode("-", $current_date);
                    $limit = $tempdate[0] + 2;

                    //We need to find the earliest year for a job, and let that be the starting year for which the user can search jobs on.
                    $sql = ("
                        SELECT DISTINCT MIN(year)
                        FROM job_info
                        WHERE (status!=" . INT_ONLY . " AND status!=" . REJECTED . ")
                        AND year>1960
                        ");
                    $result_sort_year = $GLOBALS['dbh']->Execute($sql);
                    $row = $result_sort_year->FetchRow();
                    
                    for ($i = $row["MIN(year)"]; $i < $limit; $i++)
                    {
                        echo("<option value='" . $i . "'");
                        if ($search_job->search_start_year == $i || (!isset($search_job->search_start_year) && $i == $nextTermYear["year"]))
                        {
                            echo(" selected='selected'");
                        }
                        echo(">".$i."</option>");
                    }
                    echo("</select></center>");
                    echo("</td>");
                    
                    echo("<td>");
                    echo("&nbsp;&nbsp;&nbsp;<input type='submit' name='submit' value='Sort' />");
                    echo("</td>");
                echo("</tr>\n"); 
                echo("</table>\n");
                echo("</center>");
            echo("</td>");
        echo("</tr>\n");
        
        echo("</table>\n");
        echo("<br />");
    }
         
    if ($num_results == 0)
    {
        notify("There are no jobs currently registered in the database that match your search criteria.");
    }
    else
    {
?>

        <table cellspacing="0" cellpadding="0" border="1"><tr><td>
        <table border="0" cellpadding="2">
        <tr>

<?php 
        //Create the titles for columns of the search results     
        for ($i = 0; $i < sizeof($display_inputs); $i++)
        {
            if($display_inputs[$i] != "")
            {
                echo("<td class='rowgrey' align='center' valign='middle'>&nbsp;<b class='white'>");
                echo($display_inputs[$i]);
                echo("</b></td>");
                $export_string .= $display_inputs[$i]."\t";
            }
        }
        echo("</tr>");
        $export_string .= "\n";

        //Loop through export_query results for Export to File
        $result_export = $GLOBALS['dbh']->Execute($export_query);
        //Output the search results here:
        while ($row_export = $result_export->FetchRow())
        {
            $job_code = $row_export["job_code"];
            $company_name = $row_export["company_name"];
            $min_academic = $row_export["min_academic"];
            $min_workterms = $row_export["min_workterms"];
            $admin_status= $row_export["admin_status"];
            $job_id = $row_export["job_id"];
            $closing_date = $row_export["closing_date"];
            $entry_date = $row_export["date_entered"];
            $start_month = $row_export["term_id"];
            $start_year = $row_export["year"];
            $contact_id = $row_export["contact_id"];
            $region_id = $row_export["region_id"];
            $city = $row_export["city"];
            $country_id = $row_export["country_id"];
            $provstate_id = $row_export["provstate_id"];
            $term_id = $row_export["term_id"];
            $year = $row_export["year"];
            $company_type_id = $row_export["company_type_id"];
            $industry_id = $row_export["industry_id"];

            for ($i = 0; $i < sizeof($display_inputs); $i++)
            {
                switch($display_inputs[$i])
                {
                    case 'Job Code':
                        if ($job_code)
                        {
                            $export_string .= $job_code."\t";
                        }
                        else
                        {
                            $export_string .= "Not yet set\t";
                        }
                        break;
                    case 'Company':
                        $export_string .= $company_name."\t";
                        break;
                    case 'Min Academic Yrs':
                        $export_string .= $min_academic."\t";
                        break;
                    case 'Min Workterms':
                        $export_string .= $min_workterms."\t";
                        break;
                    case 'Disciplines':
                        $disc_string = '';
                        $disc_array = getDiscArray($job_id);
                        foreach ($disc_array as $da)
                        {
                            $disc_string .= ($da . ", ");
                        }
                        $disc_string = substr($disc_string, 0, -2);
                        $export_string .= $disc_string."\t";
                        break;

                    case 'Status':
                        $admin_status_name = getAdminStatusNameFromJob($job_id);
                        if ($admin_status_name == '&nbsp;')
                        {
                        $export_string .= "\t";
                        } else {
                        $export_string .= $admin_status_name."\t";
                        }
                        break;
                    case 'Closing Date':
                        if (validDate($closing_date) != -1)
                        {
                            $export_string .= $closing_date."\t";
                        }
                        else
                        {
                            $export_string .= "Not yet set\t";
                        }
                        break;
                    case 'Entry Date':
                        if (validDate($entry_date) != -1)
                        {
                            $export_string .= $entry_date."\t";
                        } else
                        {
                            $export_string .= "Never entered\t";
                        }
                        break;
                    case 'Start of Workterm':
                        $print_month = getMonth(getTermMonth($start_month));
                        $wt_start = $print_month.", ".$start_year;
                        $export_string .= $wt_start."\t";
                        break;
                    case 'Contact':
                        $contact_name=getContactName($contact_id);
                        $full_name = $contact_name["first_name"]." ".$contact_name["last_name"];
                        $export_string .= $full_name."\t";
                        break;
                    case 'Region':
                        $region_name = getRegionName($region_id);
                        if (!$region_name)
                        {
                        $export_string .= "\t";
                        } else {
                        $export_string .= $region_name."\t";
                        }
                        break;
                    case 'Area':
                        $area_name = getAreaNameFromCountry($country_id);
                        if (!$area_name)
                        {
                        $export_string .= "\t";
                        } else {
                        $export_string .= $area_name."\t";
                        }
                        break;
                    case 'City':
                        $export_string .= $city."\t";
                        break;
                    case 'Country':
                        $country_name = getCountryName($country_id);
                        if (!$country_name)
                        {
                        $export_string .= "\t";
                        } else {
                        $export_string .= $country_name."\t";
                        }
                        break;
                    case 'Province/State':
                        $provstate_name = getProvstateName($provstate_id);
                        if (!$provstate_name)
                        {
                        $export_string .= "\t";
                        } else {
                        $export_string .= $provstate_name."\t";
                        }
                        break;
                    case 'Term':
                        $term_name = getTermName($term_id);
                        if (!$term_name)
                        {
                        $export_string .= "\t";
                        } else {
                        $export_string .= $term_name."\t";
                        }
                        break;
                    case 'Year':
                        $export_string .= $year."\t";
                        break;
                    case 'Company Type':
                        $company_type_name = getCompanyTypeName($company_type_id);
                        if (!$company_type_name)
                        {
                        $export_string .= "\t";
                        } else {
                        $export_string .= $company_type_name."\t";
                        }
                        break;
                    case 'Industry':
                        $industry_name = getIndustryName($industry_id);
                        if (!$industry_name)
                        {
                        $export_string .= "\t";
                        } else {
                        $export_string .= $industry_name."\t";
                        }
                        break;
                    case '':
                        break;
                }
              //  echo("</td>\n");
            }
            unset($discp_arr);
            unset($exp_arr);
            $export_string .= "\n";
        }



        //Output the search results here:
        $rowclass = 0;
        while ($row = $result->FetchRow())
        {
            $job_code = $row["job_code"];
            $company_name = $row["company_name"];
            $min_academic = $row["min_academic"];
            $min_workterms = $row["min_workterms"];
            $admin_status= $row["admin_status"];
            $job_id = $row["job_id"];
            $closing_date = $row["closing_date"];
            $entry_date = $row["date_entered"];
            $start_month = $row["term_id"];
            $start_year = $row["year"];
            $contact_id = $row["contact_id"];
            $region_id = $row["region_id"];
            $city = $row["city"];
            $country_id = $row["country_id"];
            $provstate_id = $row["provstate_id"];
            $term_id = $row["term_id"];
            $year = $row["year"];
            $company_type_id = $row["company_type_id"];
            $industry_id = $row["industry_id"];

            $href = $PHP_SELF . "&amp;select=search_job&amp;show_quick=true&amp;job_id=" . urlencode($job_id);

            echo("<tr>\n");
            for ($i = 0; $i < sizeof($display_inputs); $i++)
            {
                if($display_inputs[$i] != "")
                {
                    echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
                }
                switch($display_inputs[$i])
                {
                    case 'Job Code':
                        echo("&nbsp;<a class='blue' href='$href'>");
                        if ($job_code)
                        {
                            echo($job_code);
                        } 
                        else
                        {
                            echo("Not yet set");
                        }
                        echo("</a>&nbsp;");
                        break;
                    case 'Company':
                        echo("&nbsp;<a class='blue' href='$href'>");
                        echo($company_name);
                        echo("</a>&nbsp;");
                        break;
                    case 'Min Academic Yrs':
                        echo("&nbsp;");
                        echo($min_academic);
                        echo("&nbsp;");
                        break;
                    case 'Min Workterms':
                        echo("&nbsp;");
                        echo($min_workterms);
                        echo("&nbsp;");
                        break;
                    case 'Disciplines':
                        $disc_string = '';
                        $disc_array = getDiscArray($job_id);
                        foreach ($disc_array as $da)
                        {
                            $disc_string .= ($da . ", ");
                        }
                        $disc_string = substr($disc_string, 0, -2);
                        echo("&nbsp;");
                        echo($disc_string);
                        echo("&nbsp;");
                        break;

                    case 'Status':
                        $admin_status_gif = getAdminStatusGif($job_id);
                        echo("&nbsp;");
                        echo($admin_status_gif);
                        echo("&nbsp;");
                        break;
                    case 'Closing Date':
                        echo("&nbsp;");
                        if (validDate($closing_date) != -1)
                        {
                            echo($closing_date);
                        } 
                        else
                        {
                            echo("Not yet set");
                        }
                        echo("&nbsp;");
                        break;
                    case 'Entry Date':
                        echo("&nbsp;");
                        if (validDate($entry_date) != -1)
                        {
                            echo($entry_date);
                        } else
                        {
                            echo("Never Entered");
                        }
                        echo("&nbsp;");
                        break;
                    case 'Start of Workterm':
                        $print_month = getMonth(getTermMonth($start_month));
                        $wt_start = $print_month.", ".$start_year;
                        echo("&nbsp;");
                        echo($wt_start);
                        echo("&nbsp;");
                        break;
                    case 'Contact':
                        $contacthref = $PHP_SELF . "&amp;select=view_contact&amp;level1=contact&amp;continue=view_specific_contact&amp;show_quick=true&amp;no_buttons=true&amp;contact_id=" . urlencode($contact_id);
                        echo("&nbsp;");
                        echo("<a class='blue' href='$contacthref'>");
                        $contact_name=getContactName($contact_id);
                        $full_name = $contact_name["first_name"]." ".$contact_name["last_name"];
                        echo($full_name);
                        echo("</a>&nbsp;");
                        break;
                    case 'Region':
                        $region_name = getRegionName($region_id);
                        echo("&nbsp;");
                        if (!$region_name)
                        {
                        echo("&nbsp;");
                        } else {
                        echo($region_name);
                        }
                        echo("&nbsp;");
                        break;
                    case 'Area':
                        $area_name = getAreaNameFromCountry($country_id);
                        if (!$area_name)
                        {
                        echo("&nbsp;");
                        } else {
                        echo("&nbsp;");
                        echo($area_name);
                        echo("&nbsp;");
                        }
                        break;
                    case 'City':
                        echo("&nbsp;");
                        echo($city);
                        echo("&nbsp;");
                        break;
                    case 'Country':
                        $country_name = getCountryName($country_id);
                        if (!$country_name)
                        {
                        echo("&nbsp;");
                        } else {
                        echo("&nbsp;");
                        echo($country_name);
                        echo("&nbsp;");
                        }
                        break;
                    case 'Province/State':
                        $provstate_name = getProvstateName($provstate_id);
                        if (!$provstate_name)
                        {
                        echo("&nbsp;");
                        } else {
                        echo("&nbsp;");
                        echo($provstate_name);
                        echo("&nbsp;");
                        }
                        break;
                    case 'Term':
                        $term_name = getTermName($term_id);
                        if (!$term_name)
                        {
                        echo("&nbsp;");
                        } else {
                        echo("&nbsp;");
                        echo($term_name);
                        echo("&nbsp;");
                        }
                        break;
                    case 'Year':
                        echo("&nbsp;");
                        echo($year);
                        echo("&nbsp;");
                        break;
                    case 'Company Type':
                        $company_type_name = getCompanyTypeName($company_type_id);
                        if (!$company_type_name)
                        {
                        echo("&nbsp;");
                        } else {
                        echo("&nbsp;");
                        echo($company_type_name);
                        echo("&nbsp;");
                        }
                        break;
                    case 'Industry':
                        $industry_name = getIndustryName($industry_id);
                        if (!$industry_name)
                        {
                        echo("&nbsp;");
                        } else {
                        echo("&nbsp;");
                        echo($industry_name);
                        echo("&nbsp;");
                        }
                        break;
                    case '':
                        break;
                }	
                echo("</td>\n");
            }
            unset($discp_arr);
            unset($exp_arr);
            echo("</tr>\n");
            $rowclass++;
        }

        echo("</table>");
        echo("</td></tr></table>"); 		
        echo("<input type='submit' name='export' value=' Export to File ' />");
        echo("<input type='hidden' name='export_string' value='". packObject($export_string) ."' />");
    }
}
echo('</form>');
echo('<br />');

if (strlen($job_id_list)) {
    // plugins
    // if you want to do anything else with the results, just use the following hook

    $arr_params = array();
    $arr_params['job_id_list'] = $job_id_list;
    $arr_params['PHP_SELF'] = $PHP_SELF;
    $arr_params['search_job'] = $search_job;

    $arr_buttons = get_hooks('mamook.job_descriptions_search_results.button', $arr_params);
    if (is_array($arr_buttons) && sizeof($arr_buttons)) {
        for ($i = 0; $i < sizeof($arr_buttons); $i++) {
            echo($arr_buttons[$i]['button']).'&nbsp;';
        }
    }

    // display the forms of the buttons from the plugins need
    if (is_array($arr_buttons) && sizeof($arr_buttons)) {
        for ($i = 0; $i < sizeof($arr_buttons); $i++) {
            echo($arr_buttons[$i]['form']);
        }
    }
}

// Status icon key

echo("<h4>Status Icon Key</h4>");

echo("<table cellspacing='0' cellpadding='0' border='1'>\n");
echo("<tr>\n");
echo("<td>");
echo("<table border='0' cellpadding='2'>\n");

echo("<tr>\n");
echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>");
echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");

echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>");
echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");
echo("</tr>\n");

$admin_status_array = getAdminStatusGifArray();
$newrow = 0;
$rowclass = 0;
foreach ($admin_status_array as $asa)
{
    if (!($newrow % 2))
    {
        echo("<tr>\n");
    }
    echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
    echo($asa["gif"]);
    echo("</td>\n");

    echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
    echo($asa["name"]);
    echo("</td>\n");

    if ($newrow % 2)
    {
        echo("</tr>");
        $rowclass++;
    }

    $newrow++;
}

if ($newrow % 2)
{
    // Fill out any extra columns
    echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;</td>");
    echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;</td>");
    echo("</tr>\n");
}

echo("</table>\n");
echo("</td>");
echo("</tr>\n");
echo("</table>\n");

?>
