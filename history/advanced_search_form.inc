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
 | Filename: advanced_search_form.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: search_form.inc is the advanced search for the history          |
 | module. This file relies on SearchHistory.class (object structure) and       |
 | advanced_search_results.inc which is used to display the search results.     |
 +------------------------------------------------------------------------------+

*/

if($btnSearch == "Manage My Saved Searches")          //allows the Manage My Saved Searches button to work without displaying the following search form
{
    unset($btnSearch);
    include_once("search/manage_searches.inc");
}
else
{
    if ($show_sub_menus=='true' || $searchStudents<>NULL)
    {
        echo("&nbsp;</td>");
        echo("<form name='student_info' action='".$PHP_SELF."' method='post'>");
        echo("<input type='hidden' name='select' value='".urlencode('View Student')."' />");
        echo("<input type='hidden' name='student_num' value='".$student_num."' />");
        echo("<input type='hidden' name='record_id' value='".$record_id."' />");
        echo("<input type='hidden' name='student_list' value='".$student_list."' />");
        echo("<input type='hidden' name='student_department' value='".$student_department."' />");
        echo("<input type='hidden' name='menu_select' value='".$menu_select."' />");
        echo("<input type='hidden' name='searchStudents' value='".$searchStudents."' />");
        echo("<td width='5'>&nbsp;</td>");
        echo("<td valign='middle'><b class='black'>View:</b>&nbsp;<a href='javascript:document.student_info.submit()'><b class='blue'>Student Info&nbsp;</b></a>");
        echo("</form>");
    }
    
    echo("</td><td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td></tr></table>");

    $department_in_str=implode("','",$departments_in_group);

    include('history/functions.inc');

    // columns_array[] populates the "order" and "column" drop down menus. Column determines which fields you want to view on the search results. Order determines
    // how the data will be sorted. The 'name' key in columns_array is what the user will see in the order and column drop down menus. In addition, the 'name' key
    // will be used for the column title of the search results page. The 'value' key in columns_array is directly placed in a SQL select statement. Lastly,
    // the 'order' key is used in the SQL order by statement. 

    // Note: Array elements have to be placed in alphabetical order. Therefore, the order of columns_array declarations do matter. If someone wants to implement
    //       a multidimensional sorting algorithm, please do so here.

    $columns_array[] = array(
            "name" => "&nbsp;",
            "value" => "",
            "order" => ""
            );
    $columns_array[] = array(
            "name" => "Academic Department Name",
            "value" => "d.department_name",
            "order" => "d.department_name"
            );
    $columns_array[] = array(
            "name" => "Academic Year",			
            "value" => "sd.academic_year",
            "order" => "sd.academic_year"
            );
    $columns_array[] = array(
            "name" => "Company Name",
            "value" => "h.company_name",
            "order" => "h.company_name"
            );
    $columns_array[] = array(
            "name" => "Co-op Advisor",
            "value" => "coopadvc.first_name AS coopadvc_first_name,coopadvc.last_name AS coopadvc_last_name",
            "order" => "coopadvc.last_name"
            );
    $columns_array[] = array(
            "name" => "Division Name",
            "value" => "h.department_name AS division_name",
            "order" => "h.department_name"
            );	
    $columns_array[] = array(
            "name" => "Division Postal Code",
            "value" => "h.company_postal_code",
            "order" => "h.company_postal_code"
            );
    $columns_array[] = array(
            "name" => "Emergency Contact E-mail",
            "value" => "s.emergency_email",
            "order" => "s.emergency_email"
            );
    $columns_array[] = array(
            "name" => "Emergency Contact Home Fax",
            "value" => "s.emergency_home_fax",
            "order" => "s.emergency_home_fax"
            );	
    $columns_array[] = array(
            "name" => "Emergency Contact Home Phone",
            "value" => "s.emergency_home_phone",
            "order" => "s.emergency_home_phone"
            );	
    $columns_array[] = array(
            "name" => "Emergency Contact Name",
            "value" => "s.emergency_name",
            "order" => "s.emergency_name"
            );	
    $columns_array[] = array(
            "name" => "Emergency Contact Relationship",
            "value" => "s.emergency_relationship",
            "order" => "s.emergency_relationship"
            );	
    $columns_array[] = array(
            "name" => "Emergency Contact Work Fax",
            "value" => "s.emergency_work_fax",
            "order" => "s.emergency_work_fax"
            );	
    $columns_array[] = array(
            "name" => "Emergency Contact Work Phone",
            "value" => "s.emergency_work_phone",
            "order" => "s.emergency_work_phone"
            );	
    $columns_array[] = array(
            "name" => "Faculty Advisor",
            "value" => "sd.advisor",
            "order" => "sd.advisor"
            );
    $columns_array[] = array(
            "name" => "Job Code",
            "value" => "ji.job_code",
            "order" => "ji.job_code"
            );	
    $columns_array[] = array(
            "name" => "Job Title",
            "value" => "h.job_title",
            "order" => "h.job_title"
            );	
    $columns_array[] = array(
            "name" => "Report Code",
            "value" => "h.report_code",
            "order" => "h.report_code"
            );
    $columns_array[] = array(
            "name" => "Report Marker",
            "value" => "h.report_marker",
            "order" => "h.report_marker"
            );
    $columns_array[] = array(
            "name" => "Report Subject",
            "value" => "hrs.report_subject_name",
            "order" => "hrs.report_subject_name"
            );	
    $columns_array[] = array(
            "name" => "Report Title",
            "value" => "h.report_title",
            "order" => "h.report_title"
            );	
    $columns_array[] = array(
            "name" => "Salary",
            "value" => "sp.salary_period_name, h.salary",
            "order" => "sp.salary_period_name, h.salary"
            );
    $columns_array[] = array(
            "name" => "Student Citizenship",
            "value" => "s.citizen",
            "order" => "s.citizen"
            );	
    $columns_array[] = array(
            "name" => "Student Discipline",
            "value" => "disc.discipline_name",
            "order" => "disc.discipline_name"
            );
    $columns_array[] = array(
            "name" => "Student First Name",
            "value" => "s.first_name",
            "order" => "s.first_name"
            );
    $columns_array[] = array(
            "name" => "Student Gender",
            "value" => "s.gender",
            "order" => "s.gender"
            );	
    $columns_array[] = array(
            "name" => "Student Last Name",
            "value" => "s.last_name",
            "order" => "s.last_name"
            );
    $columns_array[] = array(
            "name" => "Student Middle Name",
            "value" => "s.middle_name",
            "order" => "s.middle_name"
            );	
    $columns_array[] = array(
            "name" => "Student Number",
            "value" => "s.student_number",
            "order" => "s.student_number"
            );
    $columns_array[] = array(
            "name" => "Student Primary E-mail",
            "value" => "s.email",
            "order" => "s.email"
            );
    $columns_array[] = array(
            "name" => "Student Preferred Name",
            "value" => "s.preferred_name",
            "order" => "s.preferred_name"
            );	
    $columns_array[] = array(
            "name" => "Student Secondary E-mail",
            "value" => "s.email2",
            "order" => "s.email2"
            );	
    $columns_array[] = array(
            "name" => "Student Work E-mail",
            "value" => "h.work_email",
            "order" => "h.work_email"
            );
    $columns_array[] = array(
            "name" => "Student Work Phone",
            "value" => "h.student_work_phone",
            "order" => "h.student_work_phone"
            );
    $columns_array[] = array( // stuc = student 'contact' table
            "name" => "Student Work Site Visit By",
            "value" => "stuc.first_name AS stuc_first_name,stuc.last_name AS stuc_last_name",
            "order" => "stuc.last_name,stuc.first_name"
            );
    $columns_array[] = array(
            "name" => "Student Work Site Visit Date",
            "value" => "h.site_visit_date",
            "order" => "h.site_visit_date"
            );
    $columns_array[] = array( // stusvt = student 'site_visit_type' table
            "name" => "Student Work Site Visit Type",
            "value" => "stusvt.description AS stusvt_description",
            "order" => "stusvt.description"
            );	
    $columns_array[] = array(
            "name" => "Student WT Home Address",
            "value" => "h.work_term_street_address_1,h.work_term_street_address_2",
            "order" => "h.work_term_street_address_1"
            );	
    $columns_array[] = array(
            "name" => "Student WT Home City",
            "value" => "h.work_term_city",
            "order" => "h.work_term_city"
            );	
    $columns_array[] = array( // stucl = student 'country_list' table
            "name" => "Student WT Home Country",
            "value" => "stucl.country_name AS stu_country_name",
            "order" => "stucl.country_name"
            );	
    $columns_array[] = array(
            "name" => "Student WT Home Postal Code",
            "value" => "h.work_term_postal_code",
            "order" => "h.work_term_postal_code"
            );
    $columns_array[] = array( // currl = student's current 'region_list' table
            "name" => "Student WT Home Region",
            "value" => "sturl.region_name",
            "order" => "sturl.region_name"
            );	
    $columns_array[] = array( // stupl = student 'provstate_list' table
            "name" => "Student WT Home Province",
            "value" => "stupl.provstate_name AS stu_provstate_name",
            "order" => "stupl.provstate_name"
            );
    $columns_array[] = array( 
            "name" => "Student WT Hours Per Week",
            "value" => "h.work_term_hours_per_week",
            "order" => "h.work_term_hours_per_week"
            );
    $columns_array[] = array(
            "name" => "Supervisor E-mail",
            "value" => "h.supervisor_email",
            "order" => "h.supervisor_email"
            );	
    $columns_array[] = array(
            "name" => "Supervisor Fax Number",
            "value" => "h.supervisor_fax_number",
            "order" => "h.supervisor_fax_number"
            );	
    $columns_array[] = array(
            "name" => "Supervisor Name",
            "value" => "h.supervisor_name",
            "order" => "h.supervisor_name"
            );
    $columns_array[] = array(
            "name" => "Supervisor Phone Number",
            "value" => "h.supervisor_phone_number",
            "order" => "h.supervisor_phone_number"
            );
    $columns_array[] = array(
            "name" => "Supervisor Position",
            "value" => "h.supervisor_position",
            "order" => "h.supervisor_position"
            );	
    $columns_array[] = array( // supc = supervisor 'contact' table
            "name" => "Supervisor Work Site Visit By",
            "value" => "supc.first_name AS supc_first_name,supc.last_name AS supc_last_name",
            "order" => "supc.last_name,supc.first_name"
            );	
    $columns_array[] = array(
            "name" => "Supervisor Work Site Visit Date",
            "value" => "h.site_visit_date_supervisor",
            "order" => "h.site_visit_date_supervisor"
            );	
    $columns_array[] = array( // supsvt = supervisor 'site_visit_type' table
            "name" => "Supervisor Work Site Visit Type",
            "value" => "supsvt.description AS supsvt_description",
            "order" => "supsvt.description"
            );	
    $columns_array[] = array(
            "name" => "Term",
            "value" => "t.term_name",
            "order" => "t.term_name"
            );
    /* Temporarily disabled because our history records to not record work_term_length
    $columns_array[] = array(
            "name" => "Work Term Length",
            "value" => "h.work_term_length",
            "order" => "h.work_term_length"
            );
    */
    $columns_array[] = array(
            "name" => "Work Site City",
            "value" => "h.company_city",
            "order" => "h.company_city"
            );
    $columns_array[] = array(
            "name" => "Work Site Country",
            "value" => "divcl.country_name AS div_country_name",
            "order" => "divcl.country_name"
            );
    $columns_array[] = array(
            "name" => "Work Site Province",
            "value" => "divpl.provstate_name AS div_provstate_name",
            "order" => "divpl.provstate_name"
            );	
    $columns_array[] = array(
            "name" => "Work Site Region",
            "value" => "divrl.region_name",
            "order" => "divrl.region_name"
            );
    $columns_array[] = array(
            "name" => "Work Site Street Address",
            "value" => "h.company_street_address_1,h.company_street_address_2",
            "order" => "h.company_street_address_1"
            );
    $columns_array[] = array(
            "name" => "Work Term Number",
            "value" => "IF (hc.workterm_code, hc.workterm_code, h.work_term_number) AS work_term_number",
            "order" => "work_term_number"
            );
    $columns_array[] = array(
            "name" => "Work Term Status",
            "value" => "hs.history_status_name",
            "order" => "hs.history_status_name"
            );
    $columns_array[] = array(
            "name" => "Year",
            "value" => "h.year",
            "order" => "h.year"
            );	

    $error_array = array();
    $error = 0;
    
    if($btnSearch == "Manage My Saved Searches")
    {
        unset($module_id);
        include_once("search/manage_searches.inc");
    }
    
    // Form validation for search form. Every statement below must be passed before the search results are displayed. $error_array holds error messages for the user
    // which will be displayed later on in the code. $error is a flag. If $error = 0 by the end of this block, then no errors have occurred. 
    if ($validate_form && $btnSearch=="Search")
    {
        // Any time we have a regular expression like /%/ it means we want to reject the % character because it's a wild card in SQL. 
        
        // Searches for anything that is NOT alphanumeric in the student number. 
        if (strlen(trim($student_num)) && !preg_match("/^\w+$/",removeSlashes(trim($student_num))))
        {
            array_push($error_array,"The student number you have entered contains characters that are not numbers or letters. Please re-enter the student number.");
            $error = 1;
        }

        if (strlen(trim($student_num)) && !$student_num_search_type)
        {
            array_push($error_array,"You must specify whether you want your student number to start with or end in the number you entered. Please select starts with or ends in.");
            $error = 1;
        }

        if (!strlen(trim($student_num)) && $student_num_search_type)
        {
            array_push($error_array,"Please enter a student number for your search.");
            $error = 1;
        }

        if ($first_name && preg_match("/%/",removeSlashes(trim($first_name))))
        {
            array_push($error_array,"The first name you have entered contains characters that are not numbers or letters. Please re-enter the first name.");
            $error = 1;
        }

        if ($last_name && preg_match("/%/",removeSlashes(trim($last_name))))
        {
            array_push($error_array,"The last name you have entered contains characters that are not numbers or letters. Please re-enter the last name.");
            $error = 1;
        }

        // Make sure the e-mail address does not contain the following characters: + ? * ^ $ ( ) [ ] { } | \ / ` ! # % & = " '
        if (strlen(trim($email)) && preg_match("/[\+|\?|\*|\^|\$|\(|\)|\[|\]|\{|\}|\||\\|\/|`|!|#|%|&|=|\"|']/",removeSlashes(trim($email))))
        {
            array_push($error_array,"The student e-mail address you entered contains invalid characters. Please re-enter the student e-mail address.");
            $error = 1;
        }

        if (strlen(trim($email)) && !$email_search_type)
        {
            array_push($error_array,"You must specify whether you want your student e-mail address to start with or contain the e-mail you entered. Please select starts with or contains for the student e-mail address.");
            $error = 1;
        }

        if (!strlen(trim($email)) && $email_search_type)
        {
            array_push($error_array,"Please enter a student e-mail address for your search.");
            $error = 1;
        }

        // Make sure the e-mail address does not contain the following characters: + ? * ^ $ ( ) [ ] { } | \ / ` ! # % & = " '
        if (strlen(trim($work_email)) && preg_match("/[\+|\?|\*|\^|\$|\(|\)|\[|\]|\{|\}|\||\\|\/|`|!|#|%|&|=|\"|']/",removeSlashes(trim($work_email))))
        {
            array_push($error_array,"The student work e-mail address you entered contains invalid characters. Please re-enter the student work e-mail address.");
            $error = 1;
        }

        if (strlen(trim($work_email)) && !$work_email_search_type)
        {
            array_push($error_array,"You must specify whether you want your student work e-mail address to start with or contain the e-mail you entered. Please select starts with or contains for the student work e-mail address.");
            $error = 1;
        }

        if (!strlen(trim($work_email)) && $work_email_search_type)
        {
            array_push($error_array,"Please enter a student work e-mail address for your search.");
            $error = 1;
        }

        if ($job_code && preg_match("/%/",removeSlashes(trim($job_code))))
        {
            array_push($error_array,"The job code you have entered contains characters that are not numbers or letters. Please re-enter the job code.");
            $error = 1;
        }

        if ($work_term_city && preg_match("/%/",removeSlashes(trim($work_term_city))))
        {
            array_push($error_array,"The student work term city you have entered contains characters that are not numbers or letters. Please re-enter the student work term city.");
            $error = 1;
        }

        if ($company_city && preg_match("/%/",removeSlashes(trim($company_city))))
        {
            array_push($error_array,"The division city you have entered contains characters that are not numbers or letters. Please re-enter the division city.");
            $error = 1;
        }
        
        if ($site_visit_date_supervisor_equality && !$site_visit_date_supervisor)
        {
            array_push($error_array,"You must enter a date for the work site visit of the supervisor. Please re-enter the information.");
            $error = 1;
        }
 
        if (!$site_visit_date_supervisor_equality && $site_visit_date_supervisor)
        {
            array_push($error_array,"You have entered a date of the work site visit for the supervisor; however, you must specify if you are searching before/on/after the date you specified. Please select a choice.");
            $error = 1;
        }

        if ($supervisor_name && preg_match("/%/",removeSlashes(trim($supervisor_name))))
        {
            array_push($error_array,"The supervisor name you have entered contains characters that are not numbers or letters. Please re-enter the supervisor name.");
            $error = 1;
        }

        // regular expression detects dates that do not follow YYYY-MM-DD format. 
        if ($site_visit_date_supervisor && !preg_match("/(\d{4})-(\d{2})-(\d{2})/",$site_visit_date_supervisor))
        {
            array_push($error_array,"You have entered a date of the work site visit for the supervisor that is not of the format (YYYY-MM-DD). Please re-enter the work site visit date for the supervisor.");
            $error = 1;
        }

        if ($site_visit_date_equality && !$site_visit_date)
        {
            array_push($error_array,"You must enter a date for the work site visit of the student. Please re-enter the date information.");
            $error = 1;
        }
 
        if (!$site_visit_date_equality && $site_visit_date)
        {
            array_push($error_array,"You have entered a date of the work site visit for the student; however, you must specify if you are searching before/on/after the date you specified. Please select a choice.");
            $error = 1;
        }

        // regular expression detects dates that do not follow YYYY-MM-DD format. 
        if ($site_visit_date && !preg_match("/(\d{4})-(\d{2})-(\d{2})/",$site_visit_date))
        {
            array_push($error_array,"You have entered a date of the work site visit for the student that is not of the format (YYYY-MM-DD). Please re-enter the work site visit date for the student.");
            $error = 1;
        }

        if ($report_marker && preg_match("/%/",removeSlashes(trim($report_marker))))
        {
            array_push($error_array,"The report marker you have entered contains characters that are not numbers or letters. Please re-enter the report marker.");
            $error = 1;
        }
    }
    if($search_id  && $edit_page != "yes")
    {
        $search_saved = true;
        $sql = "Select search
            FROM search
            WHERE search_id = '$search_id'
            ";
        $results = $GLOBALS['dbh']->Execute($sql);
        $row = $results->FetchRow();
        $searchHistory  = $row["search"];
        //unset($search_id);
    }
    // User has clicked Search and has passed form validation. Display the search results page. 
    if (($btnSearch=="Search" || $btnSearch=="Save Search") && !$error)
    { 
        include_once ('history/SearchHistory.class');

        // searchHistory object exists and it has been packed. Re-initialize and unpack object.
        if ($searchHistory && is_string($searchHistory))
        {
            $searchHistory = new SearchHistory($searchHistory);
        }
        // Otherwise, this object does not exist yet, so we create a new one and store all user inputs into this object.  
        else
        {
            $searchHistory = new SearchHistory('');
            $searchHistory->setVariable(
                    NULL,NULL,NULL,NULL,$student_num_search_type,$student_num,$first_name,$last_name,$gender,$grouping,$discipline_id,$academic_year,$advisor
                    ,$coop_advisor,$citizen,$email_search_type,$email,$work_email_search_type,$work_email,$job_code,$work_term_number,$work_term_length,$salary
                    ,$salary_period,$wt_status,$work_term_city,$work_term_region, $work_term_province,$work_term_country,$company_city,$company_region
                    ,$company_province,$company_country,$supervisor_name,$site_visit_date_supervisor_equality,$site_visit_date_supervisor,$site_visit_by_supervisor
                    ,$site_visit_type_id_supervisor,$site_visit_date_equality,$site_visit_date,$site_visit_by,$site_visit_type_id,$report_code,$report_subject
                    ,$report_marker,$termsused,$term,$year,$flags,$nflags,$column,$order
                    );
        }

        if($btnSearch=="Save Search" && !$error)
        {
            //if the user wishes to save or edit the search
            $module_id = HISTORY_ADV_SEARCH;
            $search = $searchHistory;
            if ($searchHistory && !is_string($searchHistory))
            {
                $search = packObject($searchHistory);
            }
            if($edit_page)
            {
                $continue="Edit Search2";
                include_once("search/manage_searches.inc");

            }
            else
            {
                include_once("search/save_search.inc");
            }
        }
        else
        {
            include ('history/advanced_search_results.inc');
        }
    } 
    
    // Advanced Search Form begins. User has just come to the advanced search form, or they tried to submit this form but had input that did not pass the form
    // validation. 
    elseif ($btnSearch<>"Search" || $error)
    { 
        // Get all countries and provinces and store them in an array because they'll be used more than once. 
        $country_sql = ("
            SELECT country_name,country_id 
            FROM country_list 
            ORDER BY order_by
            ");

        $country_results = $GLOBALS['dbh']->Execute($country_sql);
        
        $count=0;
        while ($country_row = $country_results->FetchRow())
        {
            $countryarray[$count][0]=$country_row["country_name"];
            $countryarray[$count][1]=$country_row["country_id"];
            $count=$count+1;
        }
        
        $provstate_sql = ("
            SELECT provstate_id, provstate_name, country_id 
            FROM provstate_list
            ORDER BY country_id, provstate_name
            ");

        $provstate_results = $GLOBALS['dbh']->Execute($provstate_sql);

        $count=0;
        while ($provstate_row = $provstate_results->FetchRow())
        {
            $provincearray[$count][0]=$provstate_row["provstate_name"];
            $provincearray[$count][1]=$provstate_row["provstate_id"];
            $provincearray[$count][2]=$provstate_row["country_id"];
            $count=$count+1;
        }

        $count=0;
        $region_sql = ("
                SELECT p.provstate_id, r.region_id, r.region_name
                FROM provstate_list as p LEFT JOIN region_list as r ON p.provstate_id = r.provstate_id
                ORDER BY p.provstate_id
                ");

        $region_results = $GLOBALS['dbh']->Execute($region_sql);

        $count=0;
        while ($region_row = $region_results->FetchRow())
        {
            if($region_row["region_name"]){
                $regionarray[$count][0]=$region_row["region_name"];
                $regionarray[$count][1]=$region_row["region_id"];
                $regionarray[$count][2]=$region_row["provstate_id"];
                $count=$count+1;
            }
        }

        ?>
        <script type='text/javascript' language='javascript'>

        <!--javascript

        function clearOptions(select_name,skip_column,max_columns)
        {
            var current_column = 1 + skip_column;
            for (var i=0; i < document.myform.elements.length; i++)
            {
                var e = document.myform.elements[i];
                var string = select_name + "[" + current_column + "]";
                if (e.name == string)
                {
                    e.selectedIndex = 0;
                    current_column++;
                }
                if (current_column > max_columns)
                {
                    break;
                }
            }
        }

        //-->

        </script>

  <script type='text/javascript' language='javascript'>
                <!--javascript

                function clearOptions(select_name,skip_column,max_columns)
                {
                    var current_column = 1 + skip_column;
                    for (var i=0; i < document.myform.elements.length; i++)
                    {
                        var e = document.myform.elements[i];
                        var string = select_name + "[" + current_column + "]";
                        if (e.name == string)
                        {
                            e.selectedIndex = 0;
                            current_column++;
                        }
                        if (current_column > max_columns)
                        {
                            break;
                        }
                    }
                }

            //-->
            </script>


<?php
        echo("<h3> Student Information - Search Placement History</h3>");
       // echo("<center>");
        echo("<table width='100%' cellpadding='5' cellspacing='0' border='0' class='row1'>");

        if (sizeof($error_array) > 0 && $error_array && is_array($error_array))
        {
            echo("<tr>");
                echo("<td colspan='2'>");
                // Output error messages from form validation. 
                foreach($error_array as $error_msg)
                {
                    error($error_msg);
                }
                echo("</td>");
            echo("</tr>");
        }

            echo("<tr>");
                echo("<td colspan='2' align='center'>");
                    echo("<form action='".$PHP_SELF."&amp;select=history_choose' method='post' name='search_form'>");
                    echo("<input type='hidden' name='searchmode' value='Simple Search' />");
                    echo("<br /><center><input type='submit' value='Go to Simple Search' /></center>");
                    echo("</form>");
                echo("</td>");
            echo("</tr>");
        
            echo("<tr>");
                echo("<td colspan='2' align='center'>");
                    echo("<b class='black'>");
                    echo("<br />The list of history records returned will match the criteria entered below.");
                    echo("<br />Leave any fields blank that you do not wish to be used in the search.");
                    echo("</b>");
                    echo("<br />&nbsp;");
                echo("</td>");
            echo("</tr>");

        
            $s_sql = "  SELECT search_name, search_id
                FROM search
                WHERE contact_id = '".$auth->contact_id."'
                AND search_module_id = '".HISTORY_ADV_SEARCH."'
                ORDER BY search_name";
            $s_results = $GLOBALS['dbh']->Execute($s_sql);
            //if there are searches for this advance search display it here
            if($s_results->RecordCount() > 0)
            {
                echo("<tr>");
                                      //echo("<td align='right'>");
                    echo("<td align='center' nowrap='nowrap' colspan='2'>");
                    echo("<form name='myform2' action='".$PHP_SELF."&amp;select=history_advanced_search' method='post'>");
                    echo("<input type='hidden' name='searchmode' value='advanced' />");
                    echo("<input type='hidden' name='validate_form' value='1' />");
                    echo("<input type='hidden' name='btnSearch' value='Search' />");
                    echo("Search: &nbsp;");
                    //echo("</td>");
                    
                    //echo("<td align='left'>");
                    echo("<select name='search_id'>");
                    while ($s_row = $s_results->FetchRow())
                    {
                        echo("<option value='".$s_row["search_id"]."'>".$s_row["search_name"]."</option>");
                    }
                    echo("</select>");
                    echo("&nbsp;&nbsp;<input type='submit' value='Perform Search' />&nbsp;");
                    echo("<input type='submit' name='btnSearch' value='Manage My Saved Searches' />");
                    echo("</form>");
                    echo("</td>");

                echo("</tr>");
            }
            //Here the saved search is loaded on the form
            //the variables are being set here.
            if ($searchHistory && $btnSearch="Edit Search Criteria")
            {
                include_once ('history/SearchHistory.class');
                $searchHistory = new SearchHistory($searchHistory);
                $search_by_term = $searchHistory->search_by_term;
                $search_by_student = $searchHistory->search_by_student;
                $student_number_select = $searchHistory->student_number_select;
                $student_name_select = $searchHistory->student_name_select;
                
                $student_num_search_type = $searchHistory->student_num_search_type;
                $student_num = $searchHistory->student_num;
                $first_name = $searchHistory->first_name;
                $last_name = $searchHistory->last_name;
                $gender = $searchHistory->gender;
                $grouping = $searchHistory->grouping;
                $discipline_id = $searchHistory->discipline_id;
                $academic_year = $searchHistory->academic_year;
                $advisor = $searchHistory->advisor;
                $coop_advisor = $searchHistory->coop_advisor;
                $citizen = $searchHistory->citizen;
                $email_search_type = $searchHistory->email_search_type;
                $email = $searchHistory->email;
                $work_email_search_type = $searchHistory->work_email_search_type;
                $work_email = $searchHistory->work_email;
                $job_code = $searchHistory->job_code;
                $work_term_number = $searchHistory->work_term_number;
                $work_term_length = $searchHistory->work_term_length;
                $salary = $searchHistory->salary;
                $salary_period = $searchHistory->salary_period;
                $wt_status = $searchHistory->wt_status;

                $work_term_city = $searchHistory->work_term_city;
                $work_term_region = $searchHistory->work_term_region;
                $work_term_province = $searchHistory->work_term_province;
                $work_term_country = $searchHistory->work_term_country;

                $company_city = $searchHistory->company_city;
                $company_region = $searchHistory->company_region;
                $company_province = $searchHistory->company_province;
                $company_country = $searchHistory->company_country;

                $supervisor_name = $searchHistory->supervisor_name;
                $site_visit_date_supervisor_equality = $searchHistory->site_visit_date_supervisor_equality;
                $site_visit_date_supervisor = $searchHistory->site_visit_date_supervisor;
                $site_visit_by_supervisor = $searchHistory->site_visit_by_supervisor;
                $site_visit_type_id_supervisor = $searchHistory->site_visit_type_id_supervisor;

                $site_visit_date_equality = $searchHistory->site_visit_date_equality;
                $site_visit_date = $searchHistory->site_visit_date;
                $site_visit_by = $searchHistory->site_visit_by;
                $site_visit_type_id = $searchHistory->site_visit_type_id;

                $report_code = $searchHistory->report_code;
                $report_subject = $searchHistory->report_subject;
                $report_marker = $searchHistory->report_marker;

                $termsused = $searchHistory->termsused;
                $term = $searchHistory->term;
                $year = $searchHistory->year;

                $flags = $searchHistory->flags;
                $nflags = $searchHistory->nflags;

                $column = $searchHistory->column;
                $order = $searchHistory->order;
            }
     echo("<tr>");
        echo("<td>");
        echo("<form name='myform' action='".$PHP_SELF."&amp;select=history_advanced_search' method='post'>");
        echo("<table>");        

        //    echo("<form name='myform' action='".$PHP_SELF."&amp;select=history_advanced_search' method='post'>");
            echo("<tr>");
                echo("<td>&nbsp;</td><td>&nbsp;</td>");
            echo("</tr>");
            echo("<tr>");
                echo("<td align='right'>Student Number:&nbsp;&nbsp;</td>");
                echo("<td>");
            echo("<input type='hidden' name='student_num' value='".$student_num."' />");
            echo("<input type='hidden' name='student_list' value='".$student_list."' />");
            echo("<input type='hidden' name='student_department' value='".$student_department."' />");
            echo("<input type='hidden' name='searchStudents' value='".$searchStudents."' />");
            echo("<input type='hidden' name='searchmode' value='advanced' />");
            echo("<input type='hidden' name='page' value='".$page."' />");
            if($btnSearch == "Edit Search Criteria")
            {
                echo("<input type='hidden' name='search_id' value='$search_id' />");
                echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
                echo("<input type='hidden' name='edit_page' value='yes' />");
            }

            echo("<input type='hidden' name='validate_form' value='1' />");


                    echo("<input type='radio' name='student_num_search_type' value='starts_with'".(($student_num_search_type == 'starts_with') ? "checked='checked'" : "")." />Starts with ");
                    echo("<input type='radio' name='student_num_search_type' value='ends_with'".(($student_num_search_type == 'ends_with') ? "checked='checked'" : "")." />Ends with ");
                    echo("<input type='text' name='student_num' value=\"".removeSlashes($student_num)."\" />");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>First Name:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='first_name' value=\"".removeSlashes($first_name)."\" /></td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Last Name:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='last_name' value=\"".removeSlashes($last_name)."\" /></td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Gender:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='gender'>");
                        echo("<option value='A'>&nbsp;</option>");
                        echo("<option value='M' ");
                        if ($gender == "M") 
                        { 
                            echo ("selected='selected'"); 
                        } 
                        echo(">Male</option>");
                        
                        echo("<option value='F' ");
                        if ($gender == "F") 
                        { 
                            echo ("selected='selected'"); 
                        } 
                        echo(">Female</option>");
                    echo("</select>");
                echo("</td>");
            echo("</tr>");
            
            // If we just came to this screen, we pre-select a default department/group for the user. 
            if(!strlen($grouping))
            {
                $group_sql = ("
                    SELECT pulldown_menu_group 
                    FROM contact_internal 
                    WHERE department_id=".$auth->department." AND (login_id='".$login_id."' OR netlink_id='".$login_id."')
                    ");
                $group_results = $GLOBALS['dbh']->Execute($group_sql);
                $group_row = $group_results->FetchRow();	

                if ($group_row['pulldown_menu_group']==1)
                {
                    $default_department = 0;
                }
                else
                {
                    $default_department = $auth->department;
                }
            }
            
            echo("<tr>");
                echo("<td align='right'>Department:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='grouping'>");
                    //Display "All In Group" option only if user is in a group 
                    if(sizeof($departments_in_group) > 1)
                    {
                        if($grouping == '0')
                        {
                            $selected = "selected='selected'";
                        }
                        elseif($default_department == '0' && !(strlen($grouping)))
                        {
                            $selected = "selected='selected'";
                        }
                        echo("<option value='0' ".$selected.">All In Group</option>");
                    }
                    elseif($default_department == 0 && !(strlen($grouping)))
                    {
                        $selected = "selected='selected'";
                    }
                    $selected = "";
                    
                    $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
                    $department_in_str="(".$department_in_str.")";
                    $departments_sql = ("
                        SELECT department_name,department_id 
                        FROM department 
                        WHERE department_id in ".$department_in_str."
                        ");
                    $departments_results = $GLOBALS['dbh']->Execute($departments_sql);

                    $selected = "";
                    while($departments_row = $departments_results->FetchRow())
                    {
                        if ($grouping == $departments_row['department_id'])
                        {
                            $selected = "selected='selected'";
                        }
                        elseif ($default_department==$departments_row['department_id'] && !(strlen($grouping)))
                        {
                            $selected = "selected='selected'";
                        }

                        echo("<option value='".$departments_row['department_id']."' ".$selected.">".$departments_row['department_name']."</option>");
                        $selected="";
                    }

                    echo("</select>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td class='row1' colspan='2' align='center'><br /><br />Discipline:<br /><br />");
                    echo("<table cellspacing='0' cellpadding='4' border='0' class='row0'>");


                    $discipline_sql = ("
                            SELECT disc.discipline_name, disc.discipline_id, disc.department_id, d.department_code
                            FROM discipline disc
                            LEFT JOIN department d
                            ON disc.department_id = d.department_id
                            WHERE disc.department_id IN ".$department_in_str."
                            ORDER BY disc.department_id, disc.discipline_name
                            ");
                    $discipline_results = $GLOBALS['dbh']->Execute($discipline_sql);
                    $size = $discipline_results->RecordCount();

                    // Everytime we hit more columns, we will get x more columns.
                    $discipline_iteration = 3;

                    // This figures out the maximum number of columns an individual can view. We take the number of viewable columns (from $columns_array)
                    // and round up so it can be divided by $column_iteration. IE If column_iteration = 8 and length of $columns_array = 15, we would make
                    // the maximum number of viewable columns to be 16.
                    $remainder = $size  % $discipline_iteration;
                    $difference = $discipline_iteration - $remainder;
                    $max_disciplines = $size + $difference;
                    // The minimum number of columns viewable.
                    $min_disciplines = $discipline_iteration;

                    // We just got to this page, so display only $column_iteration number of columns.
                    if (!$num_discipline)
                    {
                        if(sizeof($discipline_id) > 3)
                        {
                            $num_discipline = sizeof($discipline_id);
                        }
                        else
                        {
                            $num_discipline = $discipline_iteration;
                        }
                    }

                    // Show more columns
                    elseif($more_disciplines && $num_discipline <= $max_disciplines)
                    {
                        $num_discipline = $num_discipline + $discipline_iteration;
                    }

                    // Show less columns
                    elseif($less_disciplines && $num_discipline >= $min_disciplines)
                    {
                        $num_discipline = $num_discipline - $discipline_iteration;
                    }
                    
                    $temp_discipline_array = array();
                    for ($discipline_number=1;$discipline_number<=$num_discipline;$discipline_number++)
                    {
                        // Start new table row.
                        if ($discipline_number%2 == 1)
                        {
                            echo("<tr>");
                        }
                        echo("<td>Discipline ".$discipline_number.":</td>");

                        echo("<td>");

                        // reuse our results
                        $discipline_results->Move(0);

                        $i = 0;
                        echo("<select name='discipline_id[".$discipline_number."]'>\n");
                        $selected = "";
                        echo("<option value=''>&nbsp;</option>\n");
                        while ($discipline_row=$discipline_results->FetchRow())
                        {
                            if ($i == 0)
                            {
                                $temp_department_id = $discipline_row['department_id'];
                            }

                            elseif ($temp_department_id != $discipline_row['department_id'])
                            {
                                $temp_department_id = $discipline_row['department_id'];
                                echo("<option value=''>--------------</option>\n");
                            }
                            if(is_array($discipline_id))
                            {
                                if (in_array($discipline_row['discipline_id'],$discipline_id)
                                        && !in_array($discipline_row['discipline_id'],$temp_discipline_array)
                                        && !$selected)
                                {
                                    $option = "selected";
                                    array_push($temp_discipline_array, $discipline_row['discipline_id']);
                                    $selected = true;

                                }
                            }

                            echo("<option value='".$discipline_row['discipline_id']."' ".$option.">".$discipline_row['discipline_name']." (".$discipline_row['department_code'].")</option>\n");
                            $i++;
                            $option = "";
                        }
                        unset($selected);
                        echo("</select>");
                        echo("</td>");

                        if ($discipline_number%2 == 0)
                        {
                            echo("</tr>");
                        }
                    }
                    unset($temp_discipline_array);

                    echo("<tr>");
                   // echo("<input type='hidden' name='num_discipline' value='".$num_discipline."' />");
                    echo("<td colspan='4' align='right' class='row1'>");
                    echo("<input type='hidden' name='num_discipline' value='".$num_discipline."' />");
                    echo("<input type='button' name='order' value='Clear Disciplines' onclick='javascript:clearOptions(\"discipline_id\",0,".$num_discipline.")' />\n"); 
                    if ($num_discipline > $min_disciplines)
                    {
                        echo("<input type='submit' name='less_disciplines' value='Less Disciplines' />\n");
                    }

                    if ($num_discipline < $max_disciplines)
                    {
                        echo("<input type='submit' name='more_disciplines' value='More disciplines' />\n");
                    }
                    echo("</td>");
                    echo("</tr>");
                echo("</table>");
            echo("</td>");
        echo("</tr>");
            echo("<tr>");
                // Grab the minimum and maximum academic years available in the department and create a dropdown menu. 
                echo("<td align='right'>Academic Year:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='academic_year'>");
                    $academic_sql = ("
                        SELECT MIN(academic_year) AS min_year
                        FROM student_department
                        WHERE department_id IN ".$department_in_str."
                        ");
                    $academic_result = $GLOBALS['dbh']->Execute($academic_sql);
                    if ($academic_result->RecordCount() > 0)
                    {
                        $academic_row = $academic_result->FetchRow();
                        $min_academic_year = $academic_row['min_year'];

                        $academic_sql = ("
                            SELECT MAX(academic_year) AS max_year
                            FROM student_department
                            WHERE department_id IN ".$department_in_str."
                            ");
                        $academic_result = $GLOBALS['dbh']->Execute($academic_sql);
                        $academic_row = $academic_result->FetchRow();
                        $max_academic_year = $academic_row['max_year'];
                        
                        echo("<option value=''>&nbsp;</option>");
                        $selected = "";
                        for($i=$min_academic_year;$i<=$max_academic_year;$i++)
                        {
                            if ($i == $academic_year)
                            {
                                $selected = "selected='selected'";
                            }
                            echo("<option value='".$i."' ".$selected.">".$i."</option>");
                            $selected = "";
                        }
                    }
                    echo("</select>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Faculty Advisor:&nbsp;&nbsp;</td>");
                    echo("<td>");
                        echo("<select name='advisor'>");
                        echo("<option value=''>&nbsp;</option>");

                        $advisor_sql = ("
                                SELECT DISTINCT advisor
                                FROM student_department
                                WHERE advisor != '' AND advisor IS NOT NULL AND department_id IN ".$department_in_str."
                                ORDER BY advisor
                                ");
                        $advisor_results=$GLOBALS['dbh']->Execute($advisor_sql);

                        while ($advisor_row=$advisor_results->FetchRow())
                        {
                            if ($advisor_row["advisor"] == $advisor)
                            {
                                $option = "selected='selected'";
                            }

                            else
                            {
                                $option = "";
                            }

                            echo("<option value='".htmlentities($advisor_row['advisor'], ENT_QUOTES)."' $option>".$advisor_row['advisor']."</option>");
                        }
                        echo("</select>");
                    echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Co-op Advisor:&nbsp;&nbsp;</td>");
                    echo("<td>");
                        echo("<select name='coop_advisor'>");
                        echo("<option value=''>&nbsp;</option>");

                        $coop_advisor_sql = ("
                                SELECT DISTINCT sd.coop_advisor, c.first_name, c.last_name, d.department_code
                                FROM student_department sd
                                INNER JOIN contact c 
                                ON sd.coop_advisor = c.contact_id
                                INNER JOIN contact_internal ci
                                ON c.contact_id = ci.contact_id
                                INNER JOIN department d
                                ON ci.department_id = d.department_id
                                WHERE sd.coop_advisor != '' AND sd.coop_advisor IS NOT NULL AND ci.department_id IN ".$department_in_str."
                                ORDER BY c.last_name, c.first_name
                                ");
                        $coop_advisor_results=$GLOBALS['dbh']->Execute($coop_advisor_sql);

                        while ($coop_advisor_row=$coop_advisor_results->FetchRow())
                        {
                            if ($coop_advisor_row["coop_advisor"] == $coop_advisor)
                            {
                            $option = "selected='selected'";
                            }

                            else
                            {
                                $option = "";
                            }

                            echo("<option value='".htmlentities($coop_advisor_row['coop_advisor'], ENT_QUOTES)."' ".$option.">");
                            echo($coop_advisor_row['first_name']." ".$coop_advisor_row['last_name']." (".$coop_advisor_row['department_code'].")");
                            echo("</option>");
                        }
                        echo("</select>");
                    echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Citizenship:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='citizen' value=\"".removeSlashes($citizen)."\" /></td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Student E-mail:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<input type='radio' name='email_search_type' value='starts_with'".(($email_search_type == 'starts_with') ? "checked" : "")." />Starts with ");
                    echo("<input type='radio' name='email_search_type' value='contains'".(($email_search_type == 'contains') ? " checked" : "")." />Contains ");
                    echo("<input type='text' name='email' value=\"".removeSlashes($email)."\" />");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Student Work E-mail:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<input type='radio' name='work_email_search_type' value='starts_with'".(($work_email_search_type == 'starts_with') ? " checked" : "")." />Starts with ");
                    echo("<input type='radio' name='work_email_search_type' value='contains'".(($work_email_search_type == 'contains') ? " checked" : "")." />Contains ");
                    echo("<input type='text' name='work_email' value=\"".removeSlashes($work_email)."\" />");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Job Code:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='job_code' value=\"".removeSlashes($job_code)."\" /></td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Work Term Number:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='work_term_number'>");
                    echo("<option value=''>&nbsp;</option>");

                    $wt_number_sql = ("
                        SELECT MIN(work_term_number)
                        FROM history
                        WHERE department_id IN ".$department_in_str." 
                        ");
                    $min_wt_number = $GLOBALS['dbh']->GetOne($wt_number_sql);

                    if ($min_wt_number != null)
                    {
                        $wt_number_sql = ("
                            SELECT MAX(work_term_number)
                            FROM history
                            WHERE department_id IN ".$department_in_str." 
                            ");
                        $max_wt_number = $GLOBALS['dbh']->GetOne($wt_number_sql);

                        for($i=$min_wt_number;$i<=$max_wt_number;$i++)
                        {
                            echo("<option value='".$i."' ".(($work_term_number == $i) ? "selected='selected'" : "").">".$i."</option>");
                        }

                    }
                    echo("</select>");
                echo("</td>");
            echo("</tr>");

            /* Temporarily Disabled because none of the history records have their work_term_length entered. 
            echo("<tr>");
                echo("<td align='right'>Work Term Length:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='work_term_length' value='".$work_term_length."'> Months</td>");
            echo("</tr>");
            */

            /* Temporarily disabled because salary is incorrectly stored in the history table. Current the salary is saved as: 2200/month for instance. 
               This makes it impossible to search based on salary because the field is a string. If I wanted to check for students that makes less than 
               2200.01/month, 2200/month would not be returned. 

            echo("<tr>");
                echo("<td align='right'>Salary:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("Equal to<input type='radio' class='row1' name='salary_equality' value='='>&nbsp;&nbsp;");
                    echo("Less than<input type='radio' class='row1' name='salary_equality' value='<'>&nbsp;&nbsp;");
                    echo("Greater than<input type='radio' class='row1' name='salary_equality' value='>'>&nbsp;&nbsp;");
                    echo("<input type='text' name='salary' value='".$salary."'>");
                    echo("&nbsp;per&nbsp;");
                    echo("<select name='salary_period'>");
                        echo("<option value='hour' ".(($salary_period == 'hour') ? "selected='selected'" : "").">Hour</option>");
                        echo("<option value='week' ".(($salary_period == 'week') ? "selected='selected'" : "").">Week</option>");
                        echo("<option value='2 weeks' ".(($salary_period == '2 weeks') ? "selected='selected'" : "").">2 Weeks</option>");
                        echo("<option value='year' ".(($salary_period == 'year') ? "selected='selected'" : "").">Year</option>");
                    echo("</select>");
                echo("</td>");
            echo("</tr>");
            */
            
            echo("<tr>");
                echo("<td align='right'>Work Term Status:&nbsp;&nbsp;</td>");
                echo("<td>");
                    $status_sql = ("
                        SELECT history_status_id, history_status_name
                        FROM history_status
                        ORDER BY history_status_name
                        ");
                    $status_result = $GLOBALS['dbh']->Execute($status_sql);

                    echo("<select name='wt_status'>");
                    echo("<option value=''>&nbsp;</option>");
                    if ($status_result->RecordCount() > 0)
                    {
                        while ($status_row = $status_result->FetchRow())
                        {
                            echo("<option value='".$status_row['history_status_id']."' ".(($wt_status == $status_row['history_status_id']) ? "selected='selected'" : "").">".$status_row['history_status_name']."</option>");
                        }
                    }
                    echo("</select>");
                echo("</td>");
            echo("</tr>");
            
            // :TODO: Should be able to search based on region. Add soon.
            echo("<tr>");
                echo("<td align='right'>Work Site City:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='company_city' value=\"".removeSlashes($company_city)."\" /></td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Work Site Region:&nbsp;&nbsp;</td>");
            
                echo("<td>");
                    echo("<select name='company_region'>");
                        echo("<option value=''>&nbsp;</option>");
                        if (sizeof($regionarray) > 0 && is_array($regionarray))
                        {
                            $i = 0;
                            $selected = "";
                            foreach ($regionarray as $x)
                            {
                                if ($i == 0)
                                {
                                    $temp_province_id = $x[2];
                                    $i++;
                                }

                                else
                                {
                                    if ($temp_province_id != $x[2])
                                    {
                                        echo("<option value=''>--------------</option>");
                                        $temp_province_id = $x[2];
                                    }
                                }
                                // $x[1] is the region's region_id. If the user has already selected a region, and this page is reloaded, the region
                                // selected will be selected once the page loads.
                                if ($x[1] == $company_region)
                                {
                                    $option = "selected='selected'";
                                }

                                echo("<option value='".$x[1]."' ".$option.">".$x[0]."</option>");
                                $option = "";
                            }
                        }
                    echo("</select>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Work Site Province/State:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='company_province'>");
                        echo("<option value='0'>&nbsp;</option>");

                        if (sizeof($provincearray) > 0 && is_array($provincearray))
                        {
                            $i = 0;
                            $selected = "";
                            foreach ($provincearray as $x)
                            {
                                if ($i == 0)
                                {
                                    $temp_country_id = $x[2];
                                    $i++;
                                }

                                else
                                {
                                    if ($temp_country_id != $x[2])
                                    {
                                        echo("<option value=''>--------------</option>");
                                        $temp_country_id = $x[2]; 
                                    }
                                }
                                // $x[1] is the province's provstate_id. If the user has already selected a province, and this page is reloaded, the province
                                // selected will be selected once the page loads. 
                                if ($x[1] == $company_province) 
                                { 
                                    $selected = "selected='selected'"; 
                                } 

                                echo("<option value='".$x[1]."' ".$selected.">".$x[0]."</option>");
                                $selected = "";
                            }
                        }
                    echo("</select>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Work Site Country:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='company_country'>");
                        echo("<option value='0'>&nbsp;</option>");
                        if (sizeof($countryarray) > 0 && is_array($countryarray))
                        {
                            $selected = "";
                            foreach ($countryarray as $x)
                            {
                                if ($x[1] == $company_country) 
                                { 
                                    $selected = "selected='selected'"; 
                                } 
                                echo("<option value='".$x[1]."' ".$selected.">".$x[0]."</option>");
                                $selected = "";
                            }
                        }
                    echo("</select>");
                echo("</td>");
            echo("</tr>");
            
            echo("<tr>");
                echo("<td align='right'>Student WT Home City:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='work_term_city' value=\"".removeSlashes($work_term_city)."\" /></td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Student WT Home Region:&nbsp;&nbsp;</td>");

                echo("<td>");
                    echo("<select name='work_term_region'>");
                        echo("<option value=''>&nbsp;</option>");
                        if (sizeof($regionarray) > 0 && is_array($regionarray))
                        {
                            $i = 0;
                            $option = "";
                            foreach ($regionarray as $x)
                            {
                                if ($i == 0)
                                {
                                    $temp_province_id = $x[2];
                                    $i++;
                                }

                                else
                                {
                                    if ($temp_province_id != $x[2])
                                    {
                                        echo("<option value=''>--------------</option>");
                                        $temp_province_id = $x[2];
                                    }
                                }
                                // $x[1] is the region's region_id. If the user has already selected a region, and this page is reloaded, the region
                                // selected will be selected once the page loads.
                                if ($x[1] == $work_term_region)
                                {
                                    $option = "selected='selected'";
                                }

                                echo("<option value='".$x[1]."' ".$option.">".$x[0]."</option>");
                                $option = "";

                            }
                        }
                    echo("</select>");
                echo("</td>");
            echo("</tr>");

            // :TODO: Add dynamic drop down menus for Country/Province.
            echo("<tr>");
                echo("<td align='right'>Student WT Home Province/State:&nbsp;&nbsp;</td>");
                echo("<td>");

                    echo("<select name='work_term_province'>");
                        echo("<option value='0'>&nbsp;</option>");
                        if (sizeof($provincearray) > 0 && is_array($provincearray))
                        {
                            $i = 0;
                            $selected = "";
                            foreach ($provincearray as $x)
                            {
                                if ($i == 0)
                                {
                                    $temp_country_id = $x[2];
                                    $i++;
                                }

                                else
                                {
                                    if ($temp_country_id != $x[2])
                                    {
                                        echo("<option value=''>--------------</option>");
                                        $temp_country_id = $x[2]; 
                                    }
                                }
                                // $x[1] is the province's provstate_id. If the user has already selected a province, and this page is reloaded, the province
                                // selected will be selected once the page loads. 
                                if ($x[1] == $work_term_province) 
                                { 
                                    $selected = "selected='selected'"; 
                                } 

                                echo("<option value='".$x[1]."' ".$selected.">".$x[0]."</option>");
                                $selected = "";
                            }
                        }

                    echo("</select>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Student WT Home Country:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='work_term_country'>");
                        echo("<option value='0'>&nbsp;</option>");

                        if (sizeof($countryarray) > 0 && is_array($countryarray))
                        {
                            $selected = "";
                            foreach ($countryarray as $x)
                            {
                                if ($x[1] == $work_term_country) 
                                { 
                                    $selected = "selected='selected'"; 
                                }
                                
                                echo("<option value='".$x[1]."' ".$selected.">".$x[0]."</option>");
                                $selected = "";
                            }
                        }

                    echo("</select>");
                echo("</td>");
            echo("</tr>");
            
            echo("<tr>");
                echo("<td align='right'>Supervisor Name:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='supervisor_name' value=\"".removeSlashes($supervisor_name)."\" /></td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Supervisor Work Site Visit Date (YYYY-MM-DD):&nbsp;&nbsp;</td>");
                $pop_up_string = popup('site_visit_date_supervisor','myform');
                echo("<td>");
                    echo("<input type='radio' class='row1' name='site_visit_date_supervisor_equality' value='<' ".(($site_visit_date_supervisor_equality == '<') ? "checked='checked'" : "")." />Before &nbsp;&nbsp;");
                    echo("<input type='radio' class='row1' name='site_visit_date_supervisor_equality' value='=' ".(($site_visit_date_supervisor_equality == '=') ? "checked='checked'" : "")." />On &nbsp;&nbsp;");
                    echo("<input type='radio' class='row1' name='site_visit_date_supervisor_equality' value='>' ".(($site_visit_date_supervisor_equality == '>') ? "checked'checked'" : "")." />After &nbsp;&nbsp;");
                    echo("<input type='text' name='site_visit_date_supervisor' value=\"".removeSlashes($site_visit_date_supervisor)."\" />&nbsp;".$pop_up_string."");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                // Create a list of all staff that have done work site visits for supervisors in a certain department. We list the first name, last name and 
                // department name of the staff members. 
                echo("<td align='right'>Supervisor Work Site Visit By:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='site_visit_by_supervisor'>");
                        echo("<option value=''>&nbsp;</option>");

                        $contact_sql = ("
                            SELECT DISTINCT h.site_visit_by_supervisor, d.department_name, c.first_name, c.last_name
                            FROM history h
                            INNER JOIN contact_internal ci
                            ON h.site_visit_by_supervisor = ci.contact_id
                            INNER JOIN contact c
                            ON ci.contact_id = c.contact_id
                            INNER JOIN department d
                            ON ci.department_id = d.department_id
                            WHERE h.department_id IN ".$department_in_str."
                            ORDER BY ci.department_id, c.last_name, c.first_name
                            ");

                        $contact_results = $GLOBALS['dbh']->Execute($contact_sql);
                        
                        if ($contact_results->RecordCount() > 0)
                        {
                            $i = 0;
                            $selected = "";
                            $temp_department_name = "";
                            while ($contact_row = $contact_results->FetchRow())
                            {
                                if ($i == 0)
                                {
                                    $temp_department_name = $contact_row['department_name'];
                                    $i++;
                                }

                                else
                                {
                                    // Visually divide staff by department.
                                    if ($temp_department_name != $contact_row['department_name'])
                                    {
                                        echo("<option value=''>--------------</option>");
                                        $temp_department_name = $contact_row['department_name'];
                                    }
                                }

                                if ($contact_row["site_visit_by_supervisor"] == $site_visit_by_supervisor)
                                {
                                    $selected = "selected='selected'";
                                }

                                if ($contact_row['site_visit_by_supervisor']==SITE_VISIT_OTHER)
                                {
                                    echo "<option value='".SITE_VISIT_OTHER."' $selected>Other</option>";
                                }
                                elseif ($contact_row['site_visit_by_supervisor']==SITE_VISIT_FACULTY)
                                {
                                    echo "<option value='".SITE_VISIT_FACULTY."' $selected>Faculty</option>";
                                }
                                else
                                {
                                    echo ("<option ".$selected." value='".$contact_row['site_visit_by_supervisor']."'>".$contact_row['first_name']." ".$contact_row['last_name']." (".$contact_row['department_name'].")</option>");
                                }
                                $selected = "";
                                }
                            }

                            echo("</select>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Supervisor Work Site Visit Type:&nbsp;&nbsp;</td>");
                echo("<td>");
                
                    echo("<select name='site_visit_type_id_supervisor'>");
                        echo("<option value='0'>&nbsp;</option>");
                        $site_visit_sql = ("
                            SELECT site_visit_type_id, description
                            FROM site_visit_type
                            ORDER BY description
                            ");

                        $site_visit_results = $GLOBALS['dbh']->Execute($site_visit_sql);
                        if ($site_visit_results->RecordCount() > 0)
                        {
                            $selected = "";
                            while ($site_visit_row = $site_visit_results->FetchRow())
                            {
                                if ($site_visit_type_id_supervisor == $site_visit_row['site_visit_type_id'])
                                {
                                    $selected = "selected='selected'";
                                }
                                echo("<option ".$selected." value='".$site_visit_row['site_visit_type_id']."'>".$site_visit_row['description']."</option>");
                                $selected = "";
                            }
                        }

                    echo("</select>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Student Work Site Visit Date (YYYY-MM-DD):&nbsp;&nbsp;</td>");
                $pop_up_string = popup('site_visit_date','myform');
                echo("<td>");
                    echo("<input type='radio' class='row1' name='site_visit_date_equality' value='<' ".(($site_visit_date_equality == '<') ? "checked='checked'" : "")." />Before &nbsp;&nbsp;");
                    echo("<input type='radio' class='row1' name='site_visit_date_equality' value='=' ".(($site_visit_date_equality == '=') ? "checked='checked'" : "")." />On &nbsp;&nbsp;");
                    echo("<input type='radio' class='row1' name='site_visit_date_equality' value='>' ".(($site_visit_date_equality == '>') ? "checked='checked'" : "")." />After &nbsp;&nbsp;");
                    echo("<input type='text' name='site_visit_date' value=\"".removeSlashes($site_visit_date)."\" />&nbsp;".$pop_up_string."");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                // This generates a list of all staff members that have performed work site visits for students in a certain department. 
                echo("<td align='right'>Student Work Site Visit By:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='site_visit_by'>");
                        echo("<option value=''>&nbsp;</option>");

                        $contact_sql = ("
                            SELECT DISTINCT h.site_visit_by, d.department_name, c.first_name, c.last_name
                            FROM history h
                            INNER JOIN contact_internal ci
                            ON h.site_visit_by = ci.contact_id
                            INNER JOIN contact c
                            ON ci.contact_id = c.contact_id
                            INNER JOIN department d
                            ON ci.department_id = d.department_id
                            WHERE h.department_id IN ".$department_in_str."
                            ORDER BY ci.department_id, c.last_name, c.first_name
                            ");
                        $contact_results = $GLOBALS['dbh']->Execute($contact_sql);
                        
                        if ($contact_results->RecordCount() > 0)
                        {
                            $i = 0;
                            $temp_department_name = "";
                            $selected = "";
                            while ($contact_row = $contact_results->FetchRow())
                            {
                                if ($i == 0)
                                {
                                    $temp_department_name = $contact_row['department_name'];
                                    $i++;
                                }

                                else
                                {
                                    // Visually divide staff by department.
                                    if ($temp_department_name != $contact_row['department_name'])
                                    {
                                        echo("<option value=''>--------------</option>");
                                        $temp_department_name = $contact_row['department_name'];
                                    }
                                }

                                if ($contact_row["site_visit_by"] == $site_visit_by)
                                {
                                    $selected = "selected='selected'";
                                }

                                if ($contact_row['site_visit_by']==SITE_VISIT_OTHER)
                                {
                                    echo "<option value='".SITE_VISIT_OTHER."' $selected>Other</option>";
                                }
                                elseif ($contact_row['site_visit_by']==SITE_VISIT_FACULTY)
                                {
                                    echo "<option value='".SITE_VISIT_FACULTY."' $selected>Faculty</option>";
                                }
                                else
                                {
                                    echo ("<option ".$selected." value='".$contact_row['site_visit_by']."'>".$contact_row['first_name']." ".$contact_row['last_name']." (".$contact_row['department_name'].")</option>");
                                }
                                $selected = "";
                            }
                        }

                        echo("</select>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Student Work Site Visit Type:&nbsp;&nbsp;</td>");
                echo("<td>");
                    echo("<select name='site_visit_type_id'>");
                        echo("<option value='0'>&nbsp;</option>");
                        $site_visit_sql = ("
                            SELECT site_visit_type_id, description
                            FROM site_visit_type
                            ORDER BY description
                            ");

                        $site_visit_results = $GLOBALS['dbh']->Execute($site_visit_sql);
                        if ($site_visit_results->RecordCount() > 0)
                        {
                            $selected = "";
                            while ($site_visit_row = $site_visit_results->FetchRow())
                            {
                                if ($site_visit_type_id == $site_visit_row['site_visit_type_id'])
                                {
                                    $selected = "selected='selected'";
                                }
                                echo("<option ".$selected." value='".$site_visit_row['site_visit_type_id']."'>".$site_visit_row['description']."</option>");
                                $selected = "";
                            }
                        }

                    echo("</select>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td align='right'>Report Code:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='report_code' value=\"".removeSlashes($report_code)."\" /></td>");
            echo("</tr>");

            $report_subject_sql = ("
                    SELECT report_subject_id, report_subject_name, department_id
                    FROM history_report_subject
                    WHERE department_id IN ".$department_in_str."
                    ORDER BY department_id, report_subject_name
                    ");
            $report_subject_result = $GLOBALS['dbh']->Execute($report_subject_sql);
            if ($report_subject_result->RecordCount() > 0)
            {
                echo("<tr>");
                    echo("<td align='right'>Report Subject:&nbsp;&nbsp;</td>");
                    echo("<td>");
                        echo("<select name='report_subject'>");
                        echo("<option value=''>&nbsp;</option>");
                        $selected = "";
                        $i = 0;
                        $temp_department_id = "";
                        while ($report_subject_row = $report_subject_result->FetchRow())
                        {
                            if ($i == 0)
                            {
                                $temp_department_id = $report_subject_row['department_id'];
                                $i++;
                            }

                            else
                            {
                                if ($temp_department_id != $report_subject_row['department_id'])
                                {
                                    echo("<option value=''>--------------</option>");
                                    $temp_department_id = $report_subject_row['department_id'];
                                }
                            }
                            
                            if ($report_subject_row['report_subject_id'] == $report_subject) 
                            { 
                                $selected = "selected"; 
                            } 
                            echo("<option value='".$report_subject_row['report_subject_id']."' ".$selected.">".$report_subject_row['report_subject_name']."</option>");
                            $selected = "";
                        }

                        echo("</select>");
                    echo("</td>");
                echo("</tr>");
            }

            echo("<tr>");
                echo("<td align='right'>ReportMarker:&nbsp;&nbsp;</td>");
                echo("<td><input type='text' name='report_marker' value=\"".removeSlashes($report_marker)."\" /></td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td colspan='2'>&nbsp;</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td colspan='2' align='center'>");
                echo("Show placements for:");
                echo("</td>");
            echo("</tr>");

            // This generates drop down boxes which allows you to search for history records from the earliest history record to any history records 
            // one year from today in the future.
            echo("<tr>");
                echo("<td colspan='2'>");
                    echo("<table cellspacing='0' align='center' class='row0' border='0' cellpadding='10'>");

                        $year_sql = ("
                                SELECT MIN(year)
                                FROM history
                                WHERE year != 0 
                                ");
                        $start_year = $GLOBALS['dbh']->GetOne($year_sql);

                        $term_sql = ("
                                SELECT term_id, term_name
                                FROM term
                                ORDER BY year_order
                                ");
                        $term_results = $GLOBALS['dbh']->Execute($term_sql);

                        if ($start_year != NULL && $term_results->RecordCount() > 0)
                        {
                            $num_terms = $term_results->RecordCount();

                            for ($i=1;$i<=$num_terms;$i++)
                            {
                                echo("<tr>");
                                    echo("<td class='row0'>");
                                        echo("<center>");
                                            $selected = "";
                                            if ($termsused[$i])
                                            {
                                                $selected = "checked='checked'";
                                            }

                                            echo("<input type='checkbox' name='termsused[".$i."]' value='".$i."' ".$selected." />");
                                            $selected = "";

                                            echo("<select name='term[".$i."]'>");

                                            // term_id_list keeps a list of all terms in a year. This allows the user to search for any term in a given year.
                                            // option_string is used to save the <option></option> options because I didn't want to loop through $term_row again
                                            // to build the term_id_list. 
                                            $option_string = "";
                                            $term_id_list = "";
                                            // Display terms for drop down menu.
                                            while ($term_row = $term_results->FetchRow())
                                            {
                                                // If they reload this form and have chosen a term, then that selection will be preserved. 
                                                if ($term[$i] == $term_row['term_id'])
                                                {
                                                    $selected = 'selected=\'selected\'';
                                                }
                                                // Otherwise default terms by year order. IE: Term 1 = Spring, Term 2 = Summer, Term 3 = Fall.
                                                elseif ($i==$term_row['term_id'])
                                                {
                                                    $selected = 'selected=\'selected\'';
                                                }
                                                $option_string .= "<option value='".$term_row['term_id']."' ". $selected.">".$term_row['term_name']."</option>";
                                                $selected = "";
                                                $term_id_list .= $term_row['term_id'].",";
                                            }
                                            // strip last comma from term_id_list
                                            $term_id_list = substr($term_id_list,0,strlen($term_id_list)-1);

                                            if ($term[$i] == $term_id_list)
                                            {
                                                $selected = 'selected=\'selected\'';
                                            }

                                            echo("<option value='".$term_id_list."' ".$selected.">Any Term</option>");
                                            $selected = "";
                                            echo($option_string);

                                            $term_results->Move(0);
                                            echo("</select>");

                                            echo("<select name='year[".$i."]'>");

                                            // Create year for drop down menu. The range is the minimum year in the history table to next year. 
                                            $this_year=date("Y");
                                            $selected = "";
                                            for ($the_year=$this_year + 1;$the_year>(int)$start_year;$the_year--)
                                            {
                                                // If they reload this form and have chosen a year, then that selection will be preserved. 
                                                if ($the_year == $year[$i])
                                                {
                                                    $selected = "selected='selected'";
                                                }
                                                // Otherwise, default year to this year. 
                                                elseif (!$year[$i] && $the_year == $this_year)
                                                {
                                                    $selected = "selected='selected'";
                                                }
                                                echo ("<option value='".$the_year."' ".$selected.">".$the_year."</option>");
                                                $selected = "";
                                            }
                                            echo("</select>");
                                        echo("</center>");
                                    echo("</td>");
                                echo("</tr>");
                            }
                        }

                        elseif ($start_year == NULL || $year_results->RecordCount() < 1)
                        {
                            echo("<tr>");
                                echo("<td class='row0'>No students have been placed yet.</td>");
                            echo("</tr>");
                        }

                        elseif ($term_results->RecordCount() < 1)
                        {
                            echo("<tr>");
                                echo("<td class='row0'>No terms have been entered into the database yet.</td>");
                            echo("</tr>");
                        }

                        else
                        {
                            echo("<tr>");
                                echo("<td class='row0'>");
                                error("An error has occurred while retrieving terms and years for student placement records.");
                                echo("</td>");
                            echo("</tr>");
                        }

                    echo("</table>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td colspan='2' align='center'><br /><br /> History Flags:</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td colspan='2' align='center'>");
                    echo("<table border='0' cellpadding='5' cellspacing='0' class='row0'>");
                        // Grab all the history flags for this department. 
                        $history_flags_sql = ("
                            SELECT DISTINCT hf.history_flags_id, hf.description
                            FROM department_history_flags dhf
                            INNER JOIN history_flags hf
                            ON dhf.history_flags_id = hf.history_flags_id
                            WHERE dhf.department_id = '".$auth->department."'
                            ORDER BY hf.description
                            ");
                        $history_flags_results = $GLOBALS['dbh']->Execute($history_flags_sql);

                        if ($history_flags_results->RecordCount() > 0)
                        {
                            echo("<tr class='row0'>");
                                echo("<td>Yes/No</td>");
                                echo("<td>Yes/No</td>");
                                echo("<td>Yes/No</td>");
                            echo("</tr>");

                            $history_flags_count = 0;
                            while ($history_flags_row = $history_flags_results->FetchRow())
                            {
                                $history_flags_count++;
                                // history flag count is divisible by 3 with 1 as a remainder. Start new row. 
                                if ($history_flags_count%3 == 1)
                                {
                                    echo("<tr class='row0'>");
                                }

                                $temp = ($history_flags_count/2)-1;

                                echo("<td class='row0'>");
                                    $selected = "";
                                    if ($flags[$history_flags_count] == $history_flags_row['history_flags_id'])
                                    {
                                        $selected = "checked='checked'";
                                    }
                                    echo("<input type='checkbox' ".$selected." name='flags[".$history_flags_count."]' value='".$history_flags_row['history_flags_id']."' />");
                                    $selected = "";
                                    if ($nflags[$history_flags_count] == $history_flags_row['history_flags_id'])
                                    {
                                        $selected = "checked='checked'";
                                    }
                                    echo("<input type='checkbox' ".$selected." name='nflags[".$history_flags_count."]' value='".$history_flags_row['history_flags_id']."' />".$history_flags_row['description']);
                                    $selected = "";
                                echo("</td>");

                                // history flag count is divisible exactly by 3. End row. 
                                if ($history_flags_count%3 == 0)
                                {
                                    echo("</tr>");
                                }
                            }

                            // Fill in empty cells. Say we had 14 flags, and we display 3 flags per row. The last row would have one missing cell, so we fill it in. 
                            if ($history_flags_count%3 != 0)
                            {
                                while ($history_flags_count%3 != 0)
                                {
                                    echo("<td>&nbsp;</td>");
                                    $history_flags_count++;
                                }
                                echo("</tr>");
                            }
                        }

                        else
                        {
                            echo("<tr class='row0'>");
                                echo("<td class='row0'>");
                                echo("No history flags are available.");
                                echo("</td>");
                            echo("</tr>");
                        }

                        //echo("</tr>");
                    echo("</table>");
                echo("</td>");
            echo("</tr>");

            // function clearOptions is used to remove all pre-selected ordering/display columns and set them to blank. Notice
            // that below, I the drop down menus that controls displaying of columns are named column[1],column[2],etc, while
            // ordering is named order[1],order[2], etc. So, the first parameter in clearOptions is the array name. ie: column
            // or order. Next parameter is skip_column. Some ordering/display columns may have a pre-selected field that
            // cannot be changed. For instance, for displaying columns, you will ALWAYS have last name and first name. These
            // occupy column[1] and column[2]. So by setting skip_column = 2, we skip over these fields that we cannot reset.
            // Last is max_columns, and that's the number of drop down menus in display/order. Once we hit this upper limit, we
            // stop.
            ?>

                          <?php


            echo("<tr>");
                echo("<td class='row1' colspan='2' align='center'><br /><br />Display the following fields in the search results:<br /><br />");
                    echo("<table cellspacing='0' cellpadding='4' border='0' class='row0'>");

                        // Everytime we hit more columns, we will get x more columns. 
                        $column_iteration = 8;
                        
                        // This figures out the maximum number of columns an individual can view. We take the number of viewable columns (from $columns_array)
                        // and round up so it can be divided by $column_iteration. IE If column_iteration = 8 and length of $columns_array = 15, we would make
                        // the maximum number of viewable columns to be 16. 
                        $remainder = sizeof($columns_array) % $column_iteration;
                        $difference = $column_iteration - $remainder;
                        $max_columns = sizeof($columns_array) + $difference;
                        // The minimum number of columns viewable.  
                        $min_columns = $column_iteration;
                        
                        // We just got to this page, so display only $column_iteration number of columns.  
                        if (!$num_column)
                        {
                            if(sizeof($column) > $column_iteration)
                            {
                                $num_column = sizeof($column);
                            }
                            else
                            {
                                $num_column = $column_iteration;
                            }
                        }

                        // Show more columns
                        elseif($more_columns && $num_column <= $max_columns)
                        {
                            $num_column = $num_column + $column_iteration;
                        }

                        // Show less columns
                        elseif($less_columns && $num_column >= $min_columns)
                        {
                            $num_column = $num_column - $column_iteration;
                        }

                        for ($column_number=1;$column_number<=$num_column;$column_number++)
                        {
                            // Start new table row. 
                            if ($column_number%2 == 1)
                            {
                                echo("<tr>");
                            }
                            echo("<td>Column ".$column_number.":</td>");
                            echo("<td>");
                            if ($column_number == 1)
                            {
                                echo("<input type='hidden' name='column[".$column_number."]' value='s.first_name' />Student First Name");
                            }
                            elseif ($column_number == 2)
                            {
                                echo("<input type='hidden' name='column[".$column_number."]' value='s.last_name' />Student Last Name");
                            }

                            else
                            {
                                echo("<select name='column[".$column_number."]'>");
                                $selected = "";
                                for ($i=0;$i<sizeof($columns_array);$i++)
                                {
                                    // If user reloads this page and has selected an option here, preserve that option.
                                    if(is_array($column))
                                    {
                                        if ($column[$column_number] && ($column[$column_number] == $columns_array[$i]["value"]))
                                        {
                                            $selected = "selected='selected'";
                                        }
                                    }
                                    // Otherwise, here are the default settings for the drop down boxes. 
                                    elseif ($column_number >= 3 && $column_number <= 7 && !$more_columns && !$less_columns)
                                    {
                                        switch ($column_number)
                                        {
                                            case 3:
                                                $selected_value = "IF (hc.workterm_code, hc.workterm_code, h.work_term_number) AS work_term_number";
                                                break;
                                            case 4:
                                                $selected_value = "t.term_name";
                                                break;
                                            case 5:
                                                $selected_value = "h.year";
                                                break;
                                            case 6:
                                                $selected_value = "h.company_name";
                                                break;
                                            case 7:
                                                $selected_value = "h.company_city";
                                                break;
                                            default:
                                                $selected_value = "";
                                                
                                        }

                                        if ($columns_array[$i]["value"] == $selected_value && $selected_value != "")
                                        {
                                            $selected = "selected='selected'";
                                        }
                                    }
                                    echo("<option value='".$columns_array[$i]["value"]."' ".$selected.">".$columns_array[$i]["name"]."</option>");
                                    $selected = "";
                                }
                                echo("</select>");
                            }
                            echo("</td>");

                            if ($column_number%2 == 0)
                            {
                                echo("</tr>");
                            }
                        }
                        echo("<tr>");
                           // echo("<input type='hidden' name='num_column' value='".$num_column."' />");
                            echo("<td colspan='4' align='right' class='row1'>");
                            echo("<input type='hidden' name='num_column' value='".$num_column."' />");
   // This button clears the selected columns to display. 
                                echo("<input type='button' name='order' value='Clear Columns' onclick='javascript:clearOptions(\"column\",2,".$num_column.")' />");
                                // Buttons that control how many columns will be displayed. 
                                if ($num_column > $min_columns)
                                {
                                    echo("<input type='submit' name='less_columns' value='Less Columns' /> ");
                                }

                                if ($num_column < $max_columns)
                                {
                                    echo("<input type='submit' name='more_columns' value='More Columns' />");
                                }
                            echo("</td>");
                        echo("</tr>");
                    echo("</table>");
                echo("</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td colspan='2' align='center'><br /><br />Order search results using:</td>");
            echo("</tr>");

            echo("<tr>");
                echo("<td colspan='2'>");
                    echo("<table align='center' class='row0' cellpadding='4' cellspacing='0' border='0'>");

                        // Same as columns up above. Check up a few blocks up for more details.  
                        $order_iteration = 4;
                        
                        $max_orders = $order_iteration * 2;
                        $min_orders = $order_iteration;
                        
                        if (!$num_order)
                        {
                            if(sizeof($order) > $order_iteration)
                            {
                                $num_order = sizeof($order);
                            }
                            else
                            {
                                $num_order = $order_iteration;
                            }
                        }

                        elseif($more_orders && $num_order <= $max_orders)
                        {
                            $num_order = $num_order + $order_iteration;
                        }

                        elseif($less_orders && $num_order >= $min_orders)
                        {
                            $num_order = $num_order - $order_iteration;
                        }

                        for ($order_number=1;$order_number<=$num_order;$order_number++)
                        {
                            if ($order_number%2 == 1)
                            {
                                echo("<tr>");
                            }
                            echo("<td>Order ".$order_number.":</td>");
                            echo("<td>");
                                echo("<select name='order[".$order_number."]'>");
                                $selected = "";
                                for ($i=0;$i<sizeof($columns_array);$i++)
                                {
                                    if(is_array($order))
                                    {
                                        if ($order[$order_number] && ($order[$order_number] == $columns_array[$i]["order"]))
                                        {
                                            $selected = "selected='selected'";
                                        }
                                    }

                                    elseif ($order_number >= 1 && $order_number <= 4 && !$more_orders && !$less_orders)
                                    {
                                        switch ($order_number)
                                        {
                                            case 1:
                                                $selected_value = "s.last_name";
                                                break;
                                            case 2:
                                                $selected_value = "s.first_name";
                                                break;
                                            case 3:
                                                $selected_value = "work_term_number";
                                                break;
                                            case 4:
                                                $selected_value = "h.company_name";
                                                break;
                                            default:
                                                $selected_value = "";
                                        }

                                        if ($columns_array[$i]["order"] == $selected_value && $selected_value != "")
                                        {
                                            $selected = "selected='selected'";
                                        }
                                    }
                                    echo("<option value='".$columns_array[$i]["order"]."' ".$selected.">".$columns_array[$i]["name"]."</option>");
                                    $selected = "";
                                }
                                echo("</select>");
                            echo("</td>");

                            if ($order_number%2 == 0)
                            {
                                echo("</tr>");
                            }
                        }
                        echo("<tr>");
                           // echo("<input type='hidden' name='num_order' value='".$num_order."' />");
                            echo("<td colspan='4' align='right' class='row1'>");
                                echo("<input type='hidden' name='num_order' value='".$num_order."' />");
                                echo("<input type='button' name='order' value='Clear Columns' onclick='javascript:clearOptions(\"order\",0,".$num_order.")' />");
                                if ($num_order > $min_orders)
                                {
                                    echo("<input type='submit' name='less_orders' value='Less Columns' />");
                                }
                                if ($num_order < $max_orders)
                                {
                                    echo("<input type='submit' name='more_orders' value='More Columns' />");
                                }
                            echo("</td>");
                        echo("</tr>");

                    echo("</table>");
                echo("</td>");
            echo("</tr>");
            
            echo("<tr>");
                echo("<td colspan='2' align='center'><br /><hr /><br /><input type='submit' name='btnSearch' value='Search' />");
                    echo("&nbsp;&nbsp;&nbsp;");
                    echo("<input type='button' name='ClearForm' value='Reset Form' onclick='javascript:document.reset_page.submit()' />");
                    echo("&nbsp;&nbsp;&nbsp;");
                    //if the page is loaded on the intent to edit a saved search add theses variables in
                    if($btnSearch == "Edit Search Criteria")
                    {
                        echo("<input type='hidden' name='search_id' value='$search_id' />");
                        echo("<input type='hidden' name='edit_page' value='yes' />");
                    }
                    echo("<input type='submit' name='btnSearch' value='Save Search' />");
                echo("</td>");
            echo("</tr>");
        echo("</table>");
        echo("</form>");
        echo("</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td>");
        echo("<form name='reset_page' method='post' action='".$PHP_SELF."'>");
        echo("<input type='hidden' name='continue' value='Advanced Search' />");
        echo("<input type='hidden' name='select' value='history_advanced_search' />");
        if (strlen($search_id))
        {
            echo("<input type='hidden' name='search_id' value='".$search_id."' />");
            echo("<input type='hidden' name='edit_page' value='yes' />");
            echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
        }
        echo("</form>");
        echo("</td>");
        echo("</tr>");
            
    echo("</table>");
 
    }
}
