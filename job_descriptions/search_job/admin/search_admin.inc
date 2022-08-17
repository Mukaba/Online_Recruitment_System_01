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
 | Filename: search_admin.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This is the main admin search page.  It descides whether or     |
 | not to load the search form, the search results, and checks the inputs from  |
 | the search form.                                                             |
 +------------------------------------------------------------------------------+

*/

$result = $GLOBALS['dbh']->Execute("select CURRENT_DATE");
$row = $result->FetchRow();
$current_date = $row["CURRENT_DATE"];

$error = 0;

if( $submit == '')
{
	//no form has been submited yet, so show first screen
	include('job_descriptions/search_job/admin/search_admin_form1.inc');
}
if( $submit == 'View search results' )
{

	$error = 0;

	if( $search_job_codes_m )
	{
		$search_job_codes = $search_job_codes_m;
	}
    
	if( $search_company_name_m )
	{
		$search_company_name = $search_company_name_m;
	}
    
	if( $search_close_date == '' )
	{
		$search_close_date_as = '';
	} 
    elseif (validDate($search_close_date) != -1)
	{
		$search_close_date = validDate($search_close_date);
	}
    
	if ($search_entry_date == '')
	{
		$search_entry_date_as = '';
	} 
    elseif (validDate($search_entry_date) != -1)
	{
		$search_entry_date = validDate($search_entry_date);
	}
	if ($search_min_academic == '')
	{
		$search_academic_as = '';
	}
	if( $search_min_workterms == '' )
	{
		$search_workterms_as = '';
	}
	if( $search_keywords != '' )
	{
		$search_keywords = explode(",", $search_keywords);
		for( $i = 0; $i < sizeof($search_keywords); $i++ )
		{
			$search_keywords[$i] = trim($search_keywords[$i]);
		}
	}
    
	if( $search_not_keywords != '' )
	{
		$search_not_keywords = explode(",", $search_not_keywords);
		for( $i = 0; $i < sizeof($search_not_keywords); $i++ )
		{
			$search_not_keywords[$i] = trim($search_not_keywords[$i]);
		}
	}
	
	/*
	 Trim all of the empty elements out of search_status, and search_order.
	*/
	
	$search_status = trim_array($search_status);
    $apply_method = trim_array($apply_method);
	$search_order = trim_array($search_order);
	$yes_industries = trim_array($yes_industries);
	$no_industries = trim_array($no_industries);
	$search_disciplines = trim_array($search_disciplines);

	$search_job = new SearchJob('');
	$search_job->set_admin_search_values($search_start_term, $search_start_year, $search_future_jobs, 
        $search_on_jobs, $search_status, $apply_method, $search_area, $search_country, $search_provstate, 
        $search_region, $search_job_codes, $search_job_codes_m, $search_company_name, $search_company_name_m, 
        $search_close_date_as, $search_close_date, 
        $search_entry_date_as, $search_entry_date,
        $search_division_id, $search_contact_id, 
        $search_academic_as, 
        $search_min_academic, $search_workterms_as, $search_min_workterms, $search_yes_industries,
        $search_no_industries, $search_keywords, $search_not_keywords, $search_disciplines, $search_order);

	if( ($search_close_date) && (validDate($search_close_date) == -1) )
	{
		$error = 1;
		error("If you wish to search by closing date, you must provide a valid closing date in the form YYYY-MM-DD.");
		$search_close_date = '';
		include('job_descriptions/search_job/admin/search_admin_form1.inc');
	}	
	if (($search_entry_date) && (validDate($search_entry_date) == -1))
	{
		$error = 1;
		error("If you wish to search by entry date, you must provide a valid entry date in the form YYYY-MM-DD.");
		$search_entry_date = '';
		include('job_descriptions/search_job/admin/search_admin_form1.inc');
	}
	if( ($search_start_term && !$search_start_year) || (!$search_start_term && $search_start_year) )
	{
		$error = 1;
		error("If you wish to search by the start of the workterm, you must provide both a starting term and a starting year.");
		include('job_descriptions/search_job/admin/search_admin_form1.inc');
	}
    
	if( $error == 0 ){
        include('job_descriptions/search_job/admin/search_admin_results.inc');

	}
}
elseif( $submit == 'Contact Division Company Search' )
{
    //We need to create an instance of a SearchJob object without going through the form
    $search_job = new SearchJob('');
    
    //Create some default search values for the new instance
    $array_next_termyear = getNextTermYear();
    $search_start_term = $array_next_termyear["term"];
    $search_start_year = $array_next_termyear["year"];
     
    if($search_on_jobs == "")
    {
        $sql = ("
            SELECT pulldown_menu_group
            FROM contact_internal
            WHERE department_id = '".$auth->department."' AND (login_id = '".$login_id."' OR netlink_id = '".$login_id."')
            ");

        $results = $GLOBALS['dbh']->Execute($sql);
        $row = $results->FetchRow();

        if ($row['pulldown_menu_group'] == 1)
        {
            //GROUP default dropdown menu
            $search_on_jobs = 'group';
        }
        else
        {
            //DEPT default dropdown menu
            $search_on_jobs = 'department';
        }
    }

    //Determine what to search by
    if( $show_quick )                                  //they just used a quickmenu
    {
        if( $quick_type == "contact_quickview" )       //they selected "view contact's job descriptions"
        {
            $search_contact_id = $contact_id;          //so add the contact's id for search
        }
        elseif( $quick_type == "division_quickview" )  //they selected "view division's job descriptions"
        {
            $search_division_id = $department_id;      //so add the department's id for search
        }
        elseif( $quick_type == "company_quickview" )   //they selected "view company's job descriptions"
        {
            $search_company_name = getCompanyName($employer_id);  //add the company name for search
            $search_company_name_m = $search_company_name;   
        }
    }
    
    //Use those default values in the new instance
    $search_job->set_admin_search_values($search_start_term, $search_start_year, $search_future_jobs, 
        $search_on_jobs, $search_status, $apply_method, $search_area, $search_country, $search_provstate, 
        $search_region, $search_job_codes, $search_job_codes_m, $search_company_name, $search_company_name_m, 
        $search_close_date_as, $search_close_date, 
        $search_entry_date_as, $search_entry_date,
        $search_division_id, $search_contact_id, 
        $search_academic_as, 
        $search_min_academic, $search_workterms_as, $search_min_workterms, $search_yes_industries,
        $search_no_industries, $search_keywords, $search_not_keywords, $search_disciplines, $search_order);
    
    //Create variables not included in search_job object but necessary for search
    //Set which fields are to be Displayed
    $display_inputs[0] = "Job Code";
    $display_inputs[1] = "Company";
    $display_inputs[2] = "Min Academic Yrs";
    $display_inputs[3] = "Closing Date";
    $display_inputs[4] = "Disciplines";
    $display_inputs[5] = "Status";
    
    $quickmenu_search = true; //indicates that we are are coming to search via quickmenu
      
    include('job_descriptions/search_job/admin/search_admin_results.inc');
}
elseif( $submit == 'Go' )
{
    /*
       They just want to change the limit on the jobs returned.  Restore search_job
       and reload the page.
     */

    if ($search_job && is_string($search_job))
    {
        $search_job = unpackObject($search_job);
    }

    include('job_descriptions/search_job/admin/search_admin_results.inc');
}
elseif( $submit == 'Sort' )
{
    /*
     They just want to change what term, year, or department they are looking at.  
     Restore search_job, copy in the new values, and reload the page.
     */
    if ($search_job && is_string($search_job))
    {
        $search_job = unpackObject($search_job);
    }
    
    if( $search_on_jobs )
    {
        $search_job->search_on_jobs = $search_on_jobs;
    }

    if( $search_start_term )
    {
        $search_job->search_start_term = $search_start_term;
    }

    if( $search_start_year )
    {
        $search_job->search_start_year = $search_start_year;
    }

    if( $contact_id )        
    {
        $search_job->search_contact_id = $contact_id;          //so add the contact's id for search
    }
    elseif( $department_id )
    {
        $search_job->search_division_id = $department_id;      //so add the department's id for search
    }
    elseif( $employer_id ) 
    {
        $search_job->search_company_name = getCompanyName($employer_id);    //add the company name for search
        $search_job->search_company_name_m = getCompanyName($employer_id);  //add the company name for search
    }

    include('job_descriptions/search_job/admin/search_admin_results.inc');
}
