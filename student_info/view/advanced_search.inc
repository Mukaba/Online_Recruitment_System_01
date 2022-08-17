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
 | Filename: advanced_search.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: Advanced Search file for the 'Student Info' Section...          |
 | Displays a form for the user to fill in for his search, an option for a      |
 | simple search is also available. To Display the Search Results...            |
 | advanced_search_results.inc is called.                                       |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/

//include('student_info/functions.inc');	
$departments_in_str = implode("','",$departments_in_group);
$departments_in_str = "('".$departments_in_str."')";

// The columns_array controls the SQL query that will be formed. The name key populates the 'column' and 'order' drop down menus near the bottom of this code. 
// Meanwhile, each name in the 'column' drop down menu uses the value key from the array. The value is used in the SQL select statement. Lastly, each name in the
// 'order' drop down menu uses the order key from the array. The order is used in the SQL order by statement. There are some fields that require the group key 
// from the array, and this is used for the query's GROUP BY clause. Typically, a group key is required for aggregate SQL functions. 

// Note: The order of these columns_array declarations are important. They're sorted by name.
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
        "name" => "Admit Date",
        "value" => "sd.admit",
        "order" => "sd.admit"
        );
$columns_array[] = array(
        "name" => "Birth Date",
        "value" => "s.birth",
        "order" => "s.birth"
        );
$columns_array[] = array(
        "name" => "Citizenship",
        "value" => "s.citizen",
        "order" => "s.citizen"
        );	
$columns_array[] = array(
        "name" => "Convocation Date",
        "value" => "sd.convocation_year,sd.convocation_month",
        "order" => "sd.convocation_year,sd.convocation_month"
        );
$columns_array[] = array(
        "name" => "Co-op Advisor",
        "value" => "coopadvc.first_name AS coopadvc_first_name,coopadvc.last_name AS coopadvc_last_name",
        "order" => "coopadvc.last_name"
        );
$columns_array[] = array(
        "name" => "Current Address",
        "value" => "s.street_address_current,s.street_address_current2,s.street_address_current3",
        "order" => "s.street_address_current"
        );	
$columns_array[] = array(
        "name" => "Current City",
        "value" => "s.city_current",
        "order" => "s.city_current"
        );	
$columns_array[] = array( // curcl = student's current 'country_list' table
        "name" => "Current Country",
        "value" => "curcl.country_name AS cur_country_name",
        "order" => "curcl.country_name"
        );	
$columns_array[] = array(
        "name" => "Current Fax Number",
        "value" => "s.fax_current",
        "order" => "s.fax_current"
        );	
$columns_array[] = array(
        "name" => "Current Phone Number",
        "value" => "s.phone_current",
        "order" => "s.phone_current"
        );	
$columns_array[] = array(
        "name" => "Current Postal Code",
        "value" => "s.postal_code_current",
        "order" => "s.postal_code_current"
        );	
$columns_array[] = array( // curpl = student's current 'provstate_list' table
        "name" => "Current Province",
        "value" => "curpl.provstate_name AS cur_provstate_name",
        "order" => "curpl.provstate_name"
        );
$columns_array[] = array( // currl = student's current 'region_list' table
        "name" => "Current Region",
        "value" => "currl.region_name AS cur_region_name",
        "order" => "currl.region_name"
        );
$columns_array[] = array(
        "name" => "Discipline",
        "value" => "disc.discipline_name",
        "order" => "disc.discipline_name"
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
        "name" => "Expected Grad Year",
        "value" => "sd.grad_year",
        "order" => "sd.grad_year"
        );
$columns_array[] = array(
        "name" => "Faculty Advisor",
        "value" => "sd.advisor",
        "order" => "sd.advisor"
        );
$columns_array[] = array(
        "name" => "First Name",
        "value" => "s.first_name",
        "order" => "s.first_name"
        );
$columns_array[] = array(
        "name" => "Gender",
        "value" => "s.gender",
        "order" => "s.gender"
        );
$columns_array[] = array(
        "name" => "Grad GPA",
        "value" => "sd.grad_gpa",
        "order" => "sd.grad_gpa"
        );
$columns_array[] = array(
        "name" => "High School",
        "value" => "s.high_school",
        "order" => "s.high_school"
        );	
$columns_array[] = array(
        "name" => "Last Name",
        "value" => "s.last_name",
        "order" => "s.last_name"
        );
$columns_array[] = array(
        "name" => "Middle Name",
        "value" => "s.middle_name",
        "order" => "s.middle_name"
        );	
$columns_array[] = array(
        "name" => "Permanent Address",
        "value" => "s.street_address_permanent,s.street_address_permanent2,s.street_address_permanent3",
        "order" => "s.street_address_permanent"
        );	
$columns_array[] = array(
        "name" => "Permanent City",
        "value" => "s.city_permanent",
        "order" => "s.city_permanent"
        );	
$columns_array[] = array( // permcl = student's permanent 'country_list' table
        "name" => "Permanent Country",
        "value" => "permcl.country_name AS perm_country_name",
        "order" => "permcl.country_name"
        );	
$columns_array[] = array(
        "name" => "Permanent Fax Number",
        "value" => "s.fax_permanent",
        "order" => "s.fax_permanent"
        );	
$columns_array[] = array(
        "name" => "Permanent Phone Number",
        "value" => "s.phone_permanent",
        "order" => "s.phone_permanent"
        );	
$columns_array[] = array(
        "name" => "Permanent Postal Code",
        "value" => "s.postal_code_permanent",
        "order" => "s.postal_code_permanent"
        );	
$columns_array[] = array( // permpl = student's permanent 'provstate_list' table
        "name" => "Permanent Province",
        "value" => "permpl.provstate_name AS perm_provstate_name",
        "order" => "permpl.provstate_name"
        );
$columns_array[] = array( // permpl = student's permanent 'provstate_list' table
        "name" => "Permanent Region",
        "value" => "permrl.region_name AS perm_region_name",
        "order" => "permrl.region_name"
        );
$columns_array[] = array(
        "name" => "Preferred Name",
        "value" => "s.preferred_name",
        "order" => "s.preferred_name"
        );	
$columns_array[] = array(
        "name" => "Primary E-mail",
        "value" => "s.email",
        "order" => "s.email"
        );
$columns_array[] = array(
        "name" => "Secondary E-mail",
        "value" => "s.email2",
        "order" => "s.email2"
        );	
$columns_array[] = array(
        "name" => "Start year",
        "value" => "sd.start_year",
        "order" => "sd.start_year"
        );
$columns_array[] = array(
        "name" => "Student Number",
        "value" => "s.student_number",
        "order" => "s.student_number"
        );

$columns_array[] = array(
        "name" => "Transcript Last Updated",
        "value" => "tr.last_modified",
        "order" => "tr.last_modified"
        );

$columns_array[] = array(
        "name" => "Withdraw Date",
        "value" => "sd.withdraw",
        "order" => "sd.withdraw"
        );
$columns_array[] = array(
        "name" => "Work Term Number",
        "value" => "MAX(h.work_term_number) AS num_work_terms",
        "order" => "num_work_terms",
        "group" => "s.student_number"
        );


if($search_id && $edit_page != "yes")
{
    include_once ('student_info/view/SearchStudents.class');
    $search_saved = true;
    $sql = "Select search
            FROM search
            WHERE search_id = '$search_id'
            ";
    $results = $GLOBALS['dbh']->Execute($sql);
    $row = $results->FetchRow();
    $searchStudents  = $row["search"];
    //unset($search_id);
}

// User has clicked Search
if ($btnSearch=="Search" || $btnSearch=="Save Search")
{ 
    if ($message_sent=="yes")
    {
        echo("<br />".notify("The e-mail has been sent successfully.")."<br />");
    }

    // User has clicked Search and has passed form validation. Display the search results page.
    if ($btnSearch=="Search" || $btnSearch=="Save Search")
    {
        include_once ('student_info/view/SearchStudents.class');

        // searchStudents object exists and it has been packed. Re-initialize and unpack object.
        if ($searchStudents && is_string($searchStudents))
        {
            $searchStudents = new SearchStudents($searchStudents);
            $error_array = $searchStudents->checkInputs();
        }
        // Otherwise, this object does not exist yet, so we create a new one and store all user inputs into this object.
        else
        {
            // the current flag is inserted into either the flags array or the nflags array. 
            if ($current_flag == "YES")
            {
                if(!is_array($flags) && sizeof($flags) < 1)
                {
                    $flags = array();
                }

                array_unshift ($flags, CURRENT_FLAG);
            }
            elseif ($current_flag == "NO")
            {
                if(!is_array($nflags) && sizeof($nflags) < 1)
                {
                    $nflags = array();
                }

                array_unshift ($nflags, CURRENT_FLAG);
            }

            // The ep_used, ep_flags, ep_term, and ep_year are put into eligible_placed and not_eligible_placed arrays. 
            if (is_array($ep_used) && sizeof($ep_used) > 0)
            {
                $eligible_placed_used = array();
                $eligible_placed_flags = array();
                $eligible_placed_term = array();
                $eligible_placed_year = array();
                
                $not_eligible_placed_used = array();
                $not_eligible_placed_flags = array();
                $not_eligible_placed_term = array();
                $not_eligible_placed_year = array();
                foreach($ep_used as $index => $value)
                {
                    // If a user is searching for someone that is NOT placed, the $ep_flags[$i] value would be !PLACED_FLAG. If we detect the exclamation mark in 
                    // $ep_flags, then we push all the information onto the not eligible placed arrays. Otherwise, it goes into the eligible placed arrays. 
                    if (preg_match("/!/",$ep_flags[$index]))
                    {
                        // get rid of ! from flag.
                        $not_flag = preg_replace("/!/","",$ep_flags[$index]);
                            
                        array_push($not_eligible_placed_used,$ep_used[$index]);
                        array_push($not_eligible_placed_flags,$not_flag);
                        array_push($not_eligible_placed_term,$ep_term[$index]);
                        array_push($not_eligible_placed_year,$ep_year[$index]);
                    }

                    else
                    {
                        array_push($eligible_placed_used,$ep_used[$index]);
                        array_push($eligible_placed_flags,$ep_flags[$index]);
                        array_push($eligible_placed_term,$ep_term[$index]);
                        array_push($eligible_placed_year,$ep_year[$index]);
                    }
                }
            }
            $searchStudents = new SearchStudents('');
            $searchStudents->setVariables(
                    $student_num_search_type, $student_num, $first_name, $last_name, $gender, $department, $discipline_id, $academic_year_equality
                    , $academic_year, $grad_year_equality, $grad_year, $start_year_equality, $start_year, $primary_email_search_type, $primary_email
                    , $secondary_email_search_type, $secondary_email, $coop_advisor, $advisor, $citizen, $current_city, $current_province, $current_region, $current_country
                    , $permanent_city, $permanent_province, $permanent_region, $permanent_country, $high_school, $grad_gpa_equality, $grad_gpa, $work_terms_completed_equality
                    , $work_terms_completed, $admit_equality, $admit, $birth_equality, $birth, $convocation_equality, $convocation_month, $convocation_year, $grad, $withdraw_equality, $withdraw
                    , $flags, $nflags, $eligible_placed_used, $eligible_placed_flags, $eligible_placed_term, $eligible_placed_year
                    , $not_eligible_placed_used, $not_eligible_placed_flags, $not_eligible_placed_term, $not_eligible_placed_year, $semester_id
                    , $semester_term, $semester_year, $column, $order, $current_flag
                    );
            $error_array = $searchStudents->checkInputs();
        }
    }
   
    if($btnSearch=="Save Search" && !$error_array)
    {
        $search = $searchStudents;
        $module_id = STUDENT_ADV_SEARCH;
        if ($search && !is_string($search))
        {
            $search = packObject($search);
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
    elseif(sizeof($error_array) == 0)
    {
        // Display all results, and split by pages of results..
        include ('student_info/view/advanced_search_results.inc');
    }
} 

// User must fill out Search Form
if (($btnSearch<>"Search"  && $btnSearch!="Save Search") || $error_array)
{
    $countrysql = ("
        SELECT country_name, country_id 
        FROM country_list 
        ORDER BY order_by
        ");
    $count=0;
    $countryresults=$GLOBALS['dbh']->Execute($countrysql);
    while ($countryrow=$countryresults->FetchRow())
    {
        $countryarray[$count][0]=$countryrow["country_name"];
        $countryarray[$count][1]=$countryrow["country_id"];
        $count=$count+1;
    }
    
    $count=0;
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




    $term_sql = ("
        SELECT term_id, term_name 
        FROM term
        ");
    $term_results = $GLOBALS['dbh']->Execute($term_sql);
    $count=1;
    while ($term_row = $term_results->FetchRow())
    {
        $any_term .= $term_row['term_id'].",";
        $termarray[$count][0]=$term_row['term_name'];
        $termarray[$count][1]=$term_row['term_id'];
        $count++;
    }
    // Some people want to search for students placed in any term. So, the any_string is a comma delimited list of all the terms. 
    $any_term = substr($any_term,0,strlen($any_term)-1);
    $count=0;
    $termarray[$count][0]="Any Term";
    $termarray[$count][1]=$any_term;
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


<?php      
echo("<table width='100%' cellpadding='5' cellspacing='0' border='0' class='row1'>");
    echo("<tr>");
        echo("<td colspan='2' align='center'>");
            echo("<form action='".$PHP_SELF."' method='post' name='search_form'><br />");
                        echo("<input type='hidden' name='page' value='".$page."' />");
                        echo("<input type='hidden' name='continue' value='Simple Search' />");
                        echo("<input type='submit' value='Go to Simple Search' />");
                echo("</form>");
            echo("</td>");
        echo("</tr>");

        if (is_array($error_array) && sizeof($error_array) > 0)
        {
            echo("<tr>");
                echo("<td colspan='2'>");
                    foreach($error_array as $error_msg)
                    {
                        error($error_msg);
                        if($error_msg == "Please enter a student number for your search.")
                        {
                            unset($student_num_search_type);
                        }
                    }
                echo("</td>");
            echo("</tr>");
        }
        echo("<tr>");
            echo("<td colspan='2' align='center'>");
                echo("<b class='black'>");
                    echo("The list of students returned will match the criteria entered below.  ");
                    echo("<br />Leave any fields blank that you do not wish to be used in the search.");
                echo("</b>");
            echo("</td>");
        echo("</tr>");
       
        //if there are any saved searches for student advanced search they are now shown here
        //in a drop down box
        $s_sql = "  SELECT search_name, search_id
        FROM search
        WHERE contact_id = '".$auth->contact_id."'
        AND search_module_id = '".STUDENT_ADV_SEARCH."' 
        ORDER BY search_name";
    $s_results = $GLOBALS['dbh']->Execute($s_sql);
    if($s_results->RecordCount() > 0)
    {
        echo("<tr>");
        echo("<td align='center' colspan='2' nowrap='nowrap'>");
        echo("<form name='myform2' action='".$PHP_SELF."&amp;select=view_student' method='post'>");
        echo("Search:&nbsp;&nbsp;");
        echo("<input type='hidden' name='continue' value='Advanced Search' />");
        echo("<input type='hidden' name='page' value='".$page."' />");
        echo("<input type='hidden' name='btnSearch' value='Search' />");
            echo("<input type='hidden' name='continue' value='Advanced Search' />");
            echo("<input type='hidden' name='page' value='".$page."' />");
            echo("<input type='hidden' name='btnSearch' value='Search' />");
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
        
        //this part of the code is used when a search is loaded in a form for editing
        //here we are extracting the variable from the search object.
        if ($searchStudents && $btnSearch=="Edit Search Criteria")
        {
            include_once ('student_info/view/SearchStudents.class');
            $searchStudents = new SearchStudents($searchStudents);
            $current_flag = $searchStudents->current_flag;
            $student_num_search_type = $searchStudents->student_num_search_type;
            $student_num = $searchStudents->student_num;
            $first_name = $searchStudents->first_name;
            $last_name = $searchStudents->last_name;
            $gender = $searchStudents->gender;
            $department = $searchStudents->department;
            $discipline_id = $searchStudents->discipline_id;
            $academic_year_equality = $searchStudents->academic_year_equality;
            $academic_year = $searchStudents->academic_year;
            $grad_year_equality = $searchStudents->grad_year_equality;
            $grad_year = $searchStudents->grad_year;
            $start_year_equality = $searchStudents->start_year_equality;
            $start_year = $searchStudents->start_year;
            $primary_email_search_type = $searchStudents->primary_email_search_type;
            $primary_email = $searchStudents->primary_email;
            $secondary_email_search_type = $searchStudents->secondary_email_search_type;
            $secondary_email = $searchStudents->secondary_email;
            $coop_advisor = $searchStudents->coop_advisor;
            $advisor = $searchStudents->advisor;
            $citizen = $searchStudents->citizen;
            $current_city = $searchStudents->current_city;
            $current_province = $searchStudents->current_province;
            $current_region = $searchStudents->current_region;
            $current_country = $searchStudents->current_country;
            $permanent_city = $searchStudents->permanent_city;
            $permanent_province = $searchStudents->permanent_province;
            $permanent_region = $searchStudents->permanent_region;
            $permanent_country = $searchStudents->permanent_country;
            $high_school = $searchStudents->high_school;
            $work_terms_completed_equality = $searchStudents->work_terms_completed_equality;
            $work_terms_completed = $searchStudents->work_terms_completed;
            $grad_gpa_equality = $searchStudents->grad_gpa_equality;
            $grad_gpa = $searchStudents->grad_gpa;

            $admit_equality = $searchStudents->admit_equality;
            $admit = $searchStudents->admit;
            $birth_equality = $searchStudents->birth_equality;
            $birth = $searchStudents->birth;
            $convocation_equality = $searchStudents->convocation_equality;
            
            $convocation_month = $searchStudents->convocation_month;
            $convocation_year = $searchStudents->convocation_year;
            $grad = $searchStudents->grad;
            /* :EXPIRY: Date to Expire: Undetermined
                        Entered by:     Shaun
                        Entered on:     Jun-02-2003
                        
                - This section is essential to keeping grad in the db for historical reasons.
                - It should only be deleted when it is decided that keeping grad for historical records is no longer needed
                - a copy of the line to be deleted is here: 
                
            $grad = $searchStudents->grad;
            */
            $withdraw_equality = $searchStudents->withdraw_equality;
            $withdraw = $searchStudents->withdraw;

            $flags = $searchStudents->flags;
            $nflags = $searchStudents->nflags;

            $ep_used = $searchStudents->eligible_placed_used;
            $ep_flags = $searchStudents->eligible_placed_flags;
            $ep_term = $searchStudents->eligible_placed_term;
            $ep_year = $searchStudents->eligible_placed_year;

            $i = 1;

            if (is_array($searchStudents->eligible_placed_used) && sizeof($searchStudents->eligible_placed_used) > 0)
            {
                foreach($searchStudents->eligible_placed_used as $key => $key_value)
                {
                    $ep_used[$i] = $searchStudents->eligible_placed_used[$key];
                    $ep_flags[$i] = $searchStudents->eligible_placed_flags[$key];
                    $ep_term[$i] = $searchStudents->eligible_placed_term[$key];
                    $ep_year[$i] = $searchStudents->eligible_placed_year[$key];
                    $i++;
                }
                $key = "";
            }

            if (is_array($searchStudents->not_eligible_placed_used) && sizeof($searchStudents->not_eligible_placed_used) > 0)
            {
                foreach($searchStudents->not_eligible_placed_used as $key => $key_value)
                {
                    $ep_used[$i] = $searchStudents->not_eligible_placed_used[$key];
                    // Not eligible placed flags are prepended by an !. 
                    $ep_flags[$i] = "!".$searchStudents->not_eligible_placed_flags[$key];
                    $ep_term[$i] = $searchStudents->not_eligible_placed_term[$key];
                    $ep_year[$i] = $searchStudents->not_eligible_placed_year[$key];
                    $i++;
                }
                $key = "";
            }

            $not_eligible_placed_used = $searchStudents->not_eligible_placed_used;
            $not_eligible_placed_flags = $searchStudents->not_eligible_placed_flags;
            $not_eligible_placed_term = $searchStudents->not_eligible_placed_term;
            $not_eligible_placed_year = $searchStudents->not_eligible_placed_year;

            $semester_id = $searchStudents->semester_id;
            $semester_term = $searchStudents->semester_term;
            $semester_year = $searchStudents->semester_year;

            $column = $searchStudents->column;
            $order = $searchStudents->order;
        }
     echo("<tr>");
        echo("<td>");
        echo("<form name='myform' action='".$PHP_SELF."&amp;select=view_student' method='post'>");
        echo("<table align='center'>");        
        echo("<tr>");
            echo("<td>&nbsp;</td><td>&nbsp;</td>");
        echo("</tr>");
        echo("<tr>");
            echo("<td align='right'>Current:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<input type='hidden' name='continue' value='Advanced Search' />");
                echo("<select name='current_flag'>");
                if ($current_flag == 'ALL') 
                {
                    $option = "selected='selected'";                   //then we want yes to be selected
                }
                echo("<option value='ALL' ". $option .">All</option>");
                $option = '';
                
                if ($current_flag == 'YES' || empty($current_flag))   //if the current_flag was set in saved search OR flags is not set (i.e. not a saved search)
                {
                    $option = "selected='selected'";                   //then we want yes to be selected
                }
                if(is_array($flags))
                {
                    if(in_array(CURRENT_FLAG, $flags))
                    {
                        $option = "selected='selected'";
                    }
                }
                echo("<option value='YES' ".$option.">Yes</option>");
                $option = "";

                if ($current_flag == 'NO')
                {
                    $option = "selected='selected'";
                }
                if(is_array($nflags))
                {
                    if(in_array(CURRENT_FLAG, $nflags))
                    {
                        $option = "selected='selected'";
                    }
                }
                echo("<option value='NO' ".$option.">No</option>");
                $option = "";
                echo("</select>");
            echo("</td>");
        echo("</tr>");


        echo("<tr>");
            echo("<td align='right'>Student Number:&nbsp;&nbsp;</td>");
            echo("<td align='left'>");
                echo("<input type='radio' name='student_num_search_type' value='starts_with'".(($student_num_search_type == 'starts_with') ? "checked='checked'" : "")." />Starts with ");
                echo("<input type='radio' name='student_num_search_type' value='ends_with'".(($student_num_search_type == 'ends_with') ? "checked='checked'" : "")." />Ends with &nbsp;&nbsp;");
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
                    if (!$gender)
                    {
                        $option = "selected='selected'";
                    }
                    echo("<option value='' ".$option.">&nbsp;</option>");
                    $option = "";
                    
                    if ($gender == "M") 
                    { 
                        $option = "selected='selected'"; 
                    } 
                    echo("<option value='M' ".$option.">Male</option>");
                    $option = "";

                    if ($gender == "F") 
                    { 
                        $option = "selected='selected'";
                    } 
                    echo("<option value='F' ".$option.">Female</option>");
                    $option = "";
                echo("</select>");
            echo("</td>");
        echo("</tr>");
        
        //if $department_id is not set, then set it to the users dept and access the default settings (either GROUP or DEPT) of their drop-down menus for 
        //proper drop-down menu display later
        if($department_id=="")
        {    
            $sql = ("
                SELECT pulldown_menu_group 
                FROM contact_internal 
                WHERE department_id=".$auth->department." AND (login_id='".$login_id."' OR netlink_id='".$login_id."')
            ");
            $results=$GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();

            if ($row['pulldown_menu_group'] == 1)
            {
                //GROUP drop-down menu default
                $department_id = '0';
            }   
            else
            {
                //DEPT drop-down menu default
                $department_id = $auth->department;
            }   
        }       

        // We place each department_id from $departments_in_str into an array. If the size of the array is bigger than one, that means this department is in a group.
        // So, in the drop down menu, we let them search for 'All in group'. Otherwise, we don't give them the option. 
        $num_department_groups = explode(",",$departments_in_str);
        $num_department_groups = sizeof($num_department_groups);
        echo("<tr>");
            echo("<td align='right'>Department:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<select name='department'>");
                if ($num_department_groups > 1)
                {
                    if (strlen($department))
                    {
                        echo("<option value='0' ". (($department=='0') ? "selected='selected'" : "").">All in group</option>");
                    }
                    else
                    {
                        echo("<option value='0' ". (($department_id=='0') ? "selected='selected'" : "").">All in group</option>");
                    }
                }
                $department_sql = ("
                        SELECT department_name,department_id
                        FROM department
                        WHERE department_id IN ".$departments_in_str."
                        ");
                $department_results = $GLOBALS['dbh']->Execute($department_sql);
                $option = NULL;
                while ($department_row=$department_results->FetchRow())
                {
                    // The department variable is derived from the searchStudent class. If someone is loading values
                    // from this object, we want to use the department chosen in searchStudent.
                    if (strlen($department) && $department == $department_row['department_id'])
                    {
                        $option = "selected='selected'";
                    }
                    // Otherwise, if a user doesn't have a searchStudent object yet, we'll select the user's department
                    // as the default value of this select box.
                    elseif (!$department && $department_row['department_id']==$department_id)
                    {
                        $option = "selected='selected'";
                    }
                    echo("<option ".$option." value='".$department_row['department_id']."'>".$department_row['department_name']."</option>");
                    $option = NULL;
                }

                echo("</select>");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td class='row1' colspan='2' align='center'><br />Discipline:<br /><br />");
            echo("<table cellspacing='0' cellpadding='4' border='0' class='row0'>");


            $discipline_sql = ("
                    SELECT disc.discipline_name, disc.discipline_id, disc.department_id, d.department_code
                    FROM discipline disc
                    LEFT JOIN department d
                    ON disc.department_id = d.department_id
                    WHERE disc.department_id IN ".$departments_in_str."
                    ORDER BY disc.department_id, disc.discipline_name
                    ");
            $discipline_result = $GLOBALS['dbh']->Execute($discipline_sql);
            $size = $discipline_result->RecordCount();
            
            // Everytime we hit more columns, we will get x more columns.
            $discipline_iteration = 4;

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

                $discipline_results = $GLOBALS['dbh']->Execute($discipline_sql);

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
                            $option = "selected='selected'";
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
            echo("<td colspan='4' align='right' class='row1'>");
             echo("<input type='hidden' name='num_discipline' value='".$num_discipline."' />");
            if($btnSearch == "Edit Search Criteria")
            {
                echo("<input type='hidden' name='search_id' value='$search_id' />");
                echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
                echo("<input type='hidden' name='edit_page' value='yes' />");
            }
            // Buttons that control how many columns will be displayed.
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
            echo("<td align='right'>Faculty Advisor:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<select name='advisor'>");
                   echo("<option value=''>&nbsp;</option>");
                
                    $advisor_sql = ("
                        SELECT DISTINCT advisor 
                        FROM student_department 
                        WHERE advisor != '' AND advisor IS NOT NULL AND department_id IN ".$departments_in_str."
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
                        
                        echo("<option value='".$advisor_row['advisor']."' $option>".$advisor_row['advisor']."</option>");
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
                        WHERE sd.coop_advisor != '' AND sd.coop_advisor IS NOT NULL AND ci.department_id IN ".$departments_in_str."
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

                        echo("<option value=\"".addslashes($coop_advisor_row['coop_advisor'])."\" ".htmlentities($option).">");
                        echo($coop_advisor_row['first_name']." ".$coop_advisor_row['last_name']." (".$coop_advisor_row['department_code'].")");
                        echo("</option>");
                    }
                echo("</select>");
            echo("</td>");
        echo("</tr>");
        echo("<tr>");
            echo("<td align='right'>Academic Year:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<input type='radio' name='academic_year_equality' value='<' ".(($academic_year_equality == '<') ? "checked='checked'" : "")." />Before &nbsp;&nbsp;");
                echo("<input type='radio' name='academic_year_equality' value='=' ".(($academic_year_equality == '=') ? "checked='checked'" : "")." />On &nbsp;&nbsp;");
                echo("<input type='radio' name='academic_year_equality' value='>' ".(($academic_year_equality == '>') ? "checked='checked'" : "")." />After &nbsp;&nbsp;");
                echo("<input type='text' name='academic_year' value='".removeSlashes($academic_year)."' />");
            echo("</td>");
        echo("</tr>");
                    
        echo("<tr>");
            echo("<td align='right'>Expected Graduating Year:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<input type='radio' name='grad_year_equality' value='<' ".(($grad_year_equality == '<') ? "checked='checked'" : "")." />Before &nbsp;&nbsp;");
                echo("<input type='radio' name='grad_year_equality' value='=' ".(($grad_year_equality == '=') ? "checked='checked'" : "")." />On &nbsp;&nbsp;");
                echo("<input type='radio' name='grad_year_equality' value='>' ".(($grad_year_equality == '>') ? "checked='checked'" : "")." />After &nbsp;&nbsp;");
                echo("<input type='text' name='grad_year' value=\"".removeSlashes($grad_year)."\" />&nbsp;");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>Start Year:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<input type='radio' name='start_year_equality' value='<' ".(($start_year_equality == '<') ? "checked='checked'" : "")." />Before &nbsp;&nbsp;");
                echo("<input type='radio' name='start_year_equality' value='=' ".(($start_year_equality == '=') ? "checked='checked'" : "")." />On &nbsp;&nbsp;");
                echo("<input type='radio' name='start_year_equality' value='>' ".(($start_year_equality == '>') ? "checked='checked'" : "")." />After &nbsp;&nbsp;");
                echo("<input type='text' name='start_year' value=\"".removeSlashes($start_year)."\" />&nbsp;");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>Primary E-mail:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<input type='radio' name='primary_email_search_type' value='starts_with'".(($primary_email_search_type == 'starts_with') ? " checked='checked'" : "")." />Starts with ");
                echo("<input type='radio' name='primary_email_search_type' value='contains'".(($primary_email_search_type == 'contains') ? " checked='checked'" : "")." />Contains&nbsp;&nbsp;");
                echo("<input type='text' name='primary_email' value=\"".removeSlashes($primary_email)."\" />");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>Secondary E-mail:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<input type='radio' name='secondary_email_search_type' value='starts_with'".(($secondary_email_search_type == 'starts_with') ? " checked='checked'" : "")."/>Starts with ");
                echo("<input type='radio' name='secondary_email_search_type' value='contains'".(($secondary_email_search_type == 'contains') ? " checked='checked'" : "")." />Contains&nbsp;&nbsp;");
                echo("<input type='text' name='secondary_email' value=\"".removeSlashes($secondary_email)."\" />");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>Citizenship:&nbsp;&nbsp;</td>");
            echo("<td><input type='text' name='citizen' value=\"".removeSlashes($citizen)."\" /></td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>Current City:&nbsp;&nbsp;</td>");
            echo("<td><input type='text' name='current_city' value=\"".removeSlashes($current_city)."\" /></td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>Current Province/State:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<select name='current_province'>");
                    echo("<option value=''>&nbsp;</option>");
                    if (sizeof($provincearray) > 0 && is_array($provincearray))
                    {
                        $i = 0;
                        $option = "";
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
                                    echo("<option value=''>--------------</option>\n");
                                    $temp_country_id = $x[2];
                                }
                            }
                            // $x[1] is the province's provstate_id. If the user has already selected a province, and this page is reloaded, the province
                            // selected will be selected once the page loads.
                            if ($x[1] == $current_province)
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
            echo("<td align='right'>Current Region:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<select name='current_region'>");
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
                                    echo("<option value=''>--------------</option>\n");
                                    $temp_province_id = $x[2];
                                }
                            }
                            // $x[1] is the region's region_id. If the user has already selected a region, and this page is reloaded, the region
                            // selected will be selected once the page loads.
                            if ($x[1] == $current_region)
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
            echo("<td align='right'>Current Country:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<select name='current_country'>");
                    echo("<option value=''>&nbsp;</option>");
                    if (is_array($countryarray) && sizeof($countryarray) > 0)
                    {
                        for($i=0;$i<sizeof($countryarray);$i++)
                        {
                            if ($current_country == $countryarray[$i][1]) 
                            { 
                                $option = "selected='selected'"; 
                            } 
                            
                            else 
                            { 
                                $option = ""; 
                            }
                            echo("<option value='".$countryarray[$i][1]."' ".$option.">".$countryarray[$i][0]."</option>");
                        }
                    }
                echo("</select>");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>Permanent City:&nbsp;&nbsp;</td>");
            echo("<td><input type='text' name='permanent_city' value='".removeSlashes($permanent_city)."' /></td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>Permanent Province/State:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<select name='permanent_province'>");
                    echo("<option value=''>&nbsp;</option>");
                    if (sizeof($provincearray) > 0 && is_array($provincearray))
                    {
                        $i = 0;
                        $option = "";
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
                                    echo("<option value=''>--------------</option>\n");
                                    $temp_country_id = $x[2];
                                }
                            }
                            // $x[1] is the province's provstate_id. If the user has already selected a province, and this page is reloaded, the province
                            // selected will be selected once the page loads.
                            if ($x[1] == $permanent_province)
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
            echo("<td align='right'>Permanent Region:&nbsp;&nbsp;</td>");
             echo("<td>");
                echo("<select name='permanent_region'>");
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
                                    echo("<option value=''>--------------</option>\n");
                                    $temp_province_id = $x[2];
                                }
                            }
                            // $x[1] is the region's region_id. If the user has already selected a region, and this page is reloaded, the region
                            // selected will be selected once the page loads.
                            if ($x[1] == $permanent_region)
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
            echo("<td align='right'>Permanent Country:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<select name='permanent_country'>");
                    echo("<option value=''>&nbsp;</option>");
                    if (is_array($countryarray) && sizeof($countryarray) > 0)
                    {
                        for($i=0;$i<sizeof($countryarray);$i++)
                        {
                            if ($permanent_country == $countryarray[$i][1]) 
                            { 
                                $option = "selected='selected'"; 
                            } 
                            echo("<option value='".$countryarray[$i][1]."' ".$option.">".$countryarray[$i][0]."</option>");
                            $option = "";
                        }
                    }
                echo("</select>");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>High School:&nbsp;&nbsp;</td>");
            echo("<td><input type='text' size='35' name='high_school' value='".removeSlashes($high_school)."' /></td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'>No. of Work Terms Completed:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<input type='radio' name='work_terms_completed_equality' value='<' ".(($work_terms_completed_equality == '<') ? "checked='checked'" : "")." />Less than &nbsp;&nbsp;");
                echo("<input type='radio' name='work_terms_completed_equality' value='=' ".(($work_terms_completed_equality == '=') ? "checked='checked'" : "")." />Equal to &nbsp;&nbsp;");
                echo("<input type='radio' name='work_terms_completed_equality' value='>' ".(($work_terms_completed_equality == '>') ? "checked='checked'" : "")." />Greater than &nbsp;&nbsp;");
                echo("<input type='text' name='work_terms_completed' value='".removeSlashes($work_terms_completed)."' />");
            echo("</td>");
        echo("</tr>");
        
        echo("<tr>");
            echo("<td align='right'>Grad GPA:&nbsp;&nbsp;</td>");
            echo("<td>");
                echo("<input type='radio' name='grad_gpa_equality' value='<' ".(($grad_gpa_equality == '<') ? "checked='checked'" : "")." />Less than &nbsp;&nbsp;");
                echo("<input type='radio' name='grad_gpa_equality' value='=' ".(($grad_gpa_equality == '=') ? "checked='checked'" : "")." />Equal to &nbsp;&nbsp;");
                echo("<input type='radio' name='grad_gpa_equality' value='>' ".(($grad_gpa_equality == '>') ? "checked='checked'" : "")." />Greater than &nbsp;&nbsp;");
                echo("<input type='text' name='grad_gpa' value='".removeSlashes($grad_gpa)."' />");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td colspan='2' align='center'><br />Search by Dates : <br /><br />");
                echo("<table class='row0' border='0' cellspacing='0' cellpadding='4'>");
                    echo("<tr>");
                        echo("<td align='center'>Admit Date</td>");
                        echo("<td align='center'><input type='radio' name='admit_equality' value='<' ".(($admit_equality == '<') ? "checked='checked'" : "")." />Before &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='radio' name='admit_equality' value='=' ".(($admit_equality == '=') ? "checked='checked'" : "")." />On &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='radio' name='admit_equality' value='>' ".(($admit_equality == '>') ? "checked='checked'" : "")." />After &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='text' name='admit' value='".removeSlashes($admit)."' />".popup('admit','myform')."</td>");
                    echo("</tr>");

                    echo("<tr>");
                        echo("<td align='center'>Birth Date</td>");
                        echo("<td align='center'><input type='radio' name='birth_equality' value='<' ".(($birth_equality == '<') ? "checked='checked'" : "")." />Before &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='radio' name='birth_equality' value='=' ".(($birth_equality == '=') ? "checked='checked'" : "")." />On &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='radio' name='birth_equality' value='>' ".(($birth_equality == '>') ? "checked='checked'" : "")." />After &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='text' name='birth' value='".removeSlashes($birth)."' />".popup('birth','myform')."</td>");
                    echo("</tr>");
                    
                    echo("<tr>");
                        echo("<td align='center'>Withdrawal Date</td>");
                        echo("<td align='center'><input type='radio' name='withdraw_equality' value='<' ".(($withdraw_equality == '<') ? "checked='checked'" : "")." />Before &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='radio' name='withdraw_equality' value='=' ".(($withdraw_equality == '=') ? "checked='checked'" : "")." />On &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='radio' name='withdraw_equality' value='>' ".(($withdraw_equality == '>') ? "checked='checked'" : "")." />After &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='text' name='withdraw' value='".removeSlashes($withdraw)."' />".popup('withdraw','myform')."</td>");
                    echo("</tr>");
                    
                    echo("<tr>");
                        echo("<td align='center'>Convocation Date</td>");
                        echo("<td align='center'><input type='radio' name='convocation_equality' value='<' ".(($convocation_equality == '<') ? "checked='checked'" : "")." />Before &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='radio' name='convocation_equality' value='=' ".(($convocation_equality == '=') ? "checked='checked'" : "")." />On &nbsp;&nbsp;</td>");
                        echo("<td align='center'><input type='radio' name='convocation_equality' value='>' ".(($convocation_equality == '>') ? "checked='checked'" : "")." />After &nbsp;&nbsp;</td>");
                        echo("<td align='center'>");
          
                            //get all the months the students can go through convocation
                            $convocation_month_sql = "SELECT DISTINCT convocation_month
                                                      FROM student_department
                                                      WHERE convocation_month IS NOT NULL
                                                      ORDER BY convocation_month";

                            $convocation_month_results = $GLOBALS['dbh']->Execute($convocation_month_sql);

                            echo("<select name='convocation_month'>");
                            echo("<option value=''>&nbsp;</option>");
                            while( $convocation_month_row = $convocation_month_results->FetchRow() )   
                            {
                                $temp_month = $convocation_month_row['convocation_month'];
                                
                                //convert month numerical version to full name
                                echo("<option value='".$temp_month."'>".date("F", mktime(0,0,0,$temp_month+1,0,0)) ."</option>");
                            }
                            echo("</select>&nbsp;");

                            //get all the years any student has gone through convocation
                            $convocation_year_sql = "SELECT DISTINCT convocation_year 
                                                     FROM student_department
                                                     WHERE convocation_year IS NOT NULL
                                                     ORDER BY convocation_year DESC";
                            
                            $convocation_year_results = $GLOBALS['dbh']->Execute($convocation_year_sql);
                            
                            echo("<select name='convocation_year'>");
                            echo("<option value=''>&nbsp;</option>");
                            while( $convocation_year_row = $convocation_year_results->FetchRow() )
                            {
                                $temp_year = $convocation_year_row["convocation_year"];                  
                                echo("<option value='".$temp_year."'>".$temp_year."</option>");
                            }
                            echo("</select>&nbsp;");                            
                            
                        echo("</td>");
                    echo("</tr>");
                echo("</table>");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td colspan='2' align='center'><br />Student Flags<br /><br />");
                echo("<table border='0' cellpadding='5' cellspacing='0' class='row0'>");
                $count=0;
                    // Grab all the student flags for this department. 
                    $student_flags_sql = ("
                        SELECT DISTINCT sf.student_flags_id, sf.description
                        FROM student_flags sf
                        INNER JOIN department_student_flags dsf
                        ON dsf.student_flags_id = sf.student_flags_id
                        WHERE dsf.department_id='".$auth->department."' AND sf.student_flags_id != '".ELIGIBLE_FLAG."' AND sf.student_flags_id != '".PLACED_FLAG."'
                        AND sf.student_flags_id != '".CURRENT_FLAG."'
                        ORDER BY description
                        ");
                    $student_flags_results=$GLOBALS['dbh']->Execute($student_flags_sql);

                    if ($student_flags_results->RecordCount() > 0)
                    {
                        echo("<tr class='row0' align='left'>");
                            echo("<td>Yes/No</td>");
                            echo("<td>Yes/No</td>");
                            echo("<td>Yes/No</td>"); 
                        echo("</tr>");

                        $student_flags_count = 0;
                        while ($student_flags_row = $student_flags_results->FetchRow())
                        {
                            $student_flags_count++;
                            // student flag count is divisible by 3 with 1 as a remainder. Start new row. 
                            if ($student_flags_count%3 == 1)
                            {
                                echo("<tr class='row0' align='left'>");
                            }

                            $temp = ($student_flags_count/2)-1;

                            echo("<td class='row0'>");
                                $selected = "";
                                if (is_array($flags) && sizeof($flags) > 0)
                                {
                                    //for ($i = 0; $i < sizeof($flags); $i++)
                                    foreach($flags as $value)
                                    {
                                        if ($value == $student_flags_row['student_flags_id'])
                                        {
                                            $selected = "checked='checked'";
                                            break;
                                        }
                                    }
                                }
                                $value = NULL;
                                
                                echo("<input type='checkbox' ".$selected." name='flags[".$student_flags_count."]' value='".$student_flags_row['student_flags_id']."' />");
                                $selected = "";

                                if (is_array($nflags) && sizeof($nflags) > 0)
                                {
                                    foreach($nflags as $value)
                                    //for ($i = 0; $i < sizeof($nflags); $i++)
                                    {
                                        if ($value == $student_flags_row['student_flags_id'])
                                        {
                                            $selected = "checked='checked'";
                                            break;
                                        }
                                    }
                                    $value = "";
                                }
                                echo("<input type='checkbox' ".$selected." name='nflags[".$student_flags_count."]' value='".$student_flags_row['student_flags_id']."' />".$student_flags_row['description']);
                                $selected = "";
                            echo("</td>");

                            // student flag count is divisible exactly by 3. End row. 
                            if ($student_flags_count%3 == 0)
                            {
                                echo("</tr>");
                            }
                        }

                        // Fill in empty cells. Say we had 14 flags, and we display 3 flags per row. The last row would have one missing cell, so we fill it in. 
                        if ($student_flags_count%3 != 0)
                        {
                            while ($student_flags_count%3 != 0)
                            {
                                echo("<td>&nbsp;</td>");
                                $student_flags_count++;
                            }
                            echo("</tr>");
                        }
                    }

                    else
                    {
                        echo("<tr class='row0'>");
                            echo("<td class='row0'>");
                            echo("No student flags are available.");
                            echo("</td>");
                        echo("</tr>");
                    }

                echo("</table>");
            echo("</td>");
        echo("</tr>");
        
        echo("<tr>");
            echo("<td colspan='2' align='center'><br />Eligible/Placed Flags<br /><br />");
                echo("<table class='row0' border='0' cellspacing='0' cellpadding='4'>");
                    $eligible_sql = ("
                        SELECT MIN(year)
                        FROM eligible_placed
                        WHERE year != 0
                        ");
                    $min_eligible_year = $GLOBALS['dbh']->GetOne($eligible_sql); 

                    $eligible_sql = ("
                        SELECT MAX(year)
                        FROM eligible_placed
                        WHERE year != 0
                        ");
                    $max_eligible_year = $GLOBALS['dbh']->GetOne($eligible_sql); 
                    
                    for ($i=1;$i<=3;$i++)
                    {
                        echo("<tr>");
                            echo("<td>");
                                if ($ep_used[$i])
                                {
                                    $checkbox = "checked='checked'";
                                }
                                echo("<input type='checkbox' name='ep_used[".$i."]' value='".$i."' ".$checkbox." />");
                                $checkbox = "";
                                echo("<select name='ep_flags[".$i."]'>");
                                    if ($ep_flags[$i] == '')
                                    {
                                        $option = "selected='selected'";
                                    }
                                    echo("<option value='' ".$option.">&nbsp;</option>");
                                    $option = "";

                                    if ($ep_flags[$i] == PLACED_FLAG)
                                    {
                                        $option = "selected='selected'";
                                    }
                                    echo("<option value='".PLACED_FLAG."' ".$option.">Students placed for</option>");
                                    $option = "";
                                    
                                    if ($ep_flags[$i] == ELIGIBLE_FLAG)
                                    {
                                        $option = "selected='selected'";
                                    }
                                    echo("<option value='".ELIGIBLE_FLAG."' ".$option.">Students eligible for</option>");
                                    $option = "";

                                    // Notice when searching for people NOT placed or eligible/placed, we have an exclamation mark in front of it. This is important
                                    // when we create the searchStudents object.
                                    $not_placed_string = "!".PLACED_FLAG;
                                    if ($ep_flags[$i] == $not_placed_string)
                                    {
                                        $option = "selected='selected'";
                                    }
                                    echo("<option value='".$not_placed_string."' ".$option.">Students not placed for</option>");
                                    $option = "";

                                    $not_eligible_string = "!".ELIGIBLE_FLAG;
                                    if ($ep_flags[$i] == $not_eligible_string)
                                    {
                                        $option = "selected='selected'";
                                    }
                                    echo("<option value='".$not_eligible_string."' ".$option.">Students not eligible for</option>");
                                    $option = "";
                                echo("</select>");

                                echo("<select name='ep_term[".$i."]'>");
                                    echo("<option value=''>&nbsp;</option>");
                                    for ($j=0;$j<sizeof($termarray);$j++)
                                    {
                                        if ($ep_term[$i] == $termarray[$j][1])
                                        {
                                            $option = "selected='selected'";
                                        }
                                        echo("<option value='".$termarray[$j][1]."' ".$option.">".$termarray[$j][0]."</option>");
                                        $option = "";
                                    }
                                echo("</select>");

                                echo("<select name='ep_year[".$i."]'>");
                                    echo("<option value=''>&nbsp;</option>");
                                    for($year=(int)$max_eligible_year;$year>=(int)$min_eligible_year;$year--)
                                    {
                                        if ($ep_year[$i] == $year)
                                        {
                                            $option = "selected='selected'";
                                        }
                                        echo("<option value='".$year."' ".$option.">".$year."</option>");
                                        $option = "";
                                    }
                                echo("</select>");
                                
                            echo("</td>");
                        echo("</tr>");
                    }
                echo("</table>");
            echo("</td>");
        echo("</tr>");
        
        //=========================================================================================================================================================//
        // Begin dynamic drop down menus for searching on student semesters.                                                                                       //
        //                                                                                                                                                         //
        // Note: These dynamic drop down menus were derived from the contact advanced search. The javascript here was used to update countries, provinces,         // 
        //       and regions.                                                                                                                                      //
        //=========================================================================================================================================================//


        // Grab all the types of semesters associated with the department(s). 
        $semesters_sql = ("
            SELECT DISTINCT ds.semesters_id, s.display, s.description_type, sdt.description 
            FROM department_semesters ds 
            INNER JOIN semesters s 
            ON ds.semesters_id = s.semesters_id 
            INNER JOIN semesters_description_type sdt 
            ON s.description_type = sdt.description_type 
            WHERE department_id IN ".$departments_in_str."
            ORDER BY s.description_type, s.display
            ");
        $semesters_result = $GLOBALS['dbh']->Execute($semesters_sql);
        if ($semesters_result->RecordCount() > 0)
        {
            $arrSemesterType = array();     // An array that holds the different semester types. IE: Academic Term, Work Term, etc.
            $any_semesters = array();       // An array that holds a comma delimited list of ALL the semesters in a semester type. IE: 1,2,3,4
            $arrSemesterID = array();       // An array that holds a semester_id. IE: 1
            $arrSemesterDisplay = array();  // An array that holds a semester_id's meaning. IE: WT1
            $i = 0;
            $j = 0;
            while($semesters_row = $semesters_result->FetchRow())
            {
                // These if/else clauses allow us to grab every distinct semester type that is pulled back from query. $j keeps track of these distinct
                // semester types. semesters_id's and display's are kept in arrays associated with the semester type. 
                if ($i == 0)
                {
                    $temp_description_type = $semesters_row['description_type'];
                    $arrSemesterType[$j]['description'] = $semesters_row['description'];
                    $arrSemesterType[$j]['description_type'] = $semesters_row['description_type'];
                }
                
                if ($temp_description_type == $semesters_row['description_type'])
                {
                    $any_semesters[$j] = $any_semesters[$j].$semesters_row['semesters_id'].",";
                    $arrSemesterID[$j] = $arrSemesterID[$j]."\"".$semesters_row['semesters_id']."\",";
                    $arrSemesterDisplay[$j] = $arrSemesterDisplay[$j]."\"".$semesters_row['display']."\",";
                }
                else
                {
                    $temp_description_type = $semesters_row['description_type'];
                    $j++; 
                    $arrSemesterType[$j]['description'] = $semesters_row['description'];
                    $arrSemesterType[$j]['description_type'] = $semesters_row['description_type'];
                    $any_semesters[$j] = $semesters_row['semesters_id'].",";
                    $arrSemesterID[$j] = "\"".$semesters_row['semesters_id']."\",";
                    $arrSemesterDisplay[$j] = "\"".$semesters_row['display']."\",";
                }
                $i++;
            }

            // Strip ending comma from lists. 
            for($i=0;$i<sizeof($any_semesters);$i++)
            {
                $any_semesters[$i] = "\"".substr($any_semesters[$i],0,strlen($any_semesters[$i])-1)."\"";
                $arrSemesterID[$i] = substr($arrSemesterID[$i],0,strlen($arrSemesterID[$i])-1);
                $arrSemesterDisplay[$i] = substr($arrSemesterDisplay[$i],0,strlen($arrSemesterDisplay[$i])-1);
            }

            /* DEBUGGING PURPOSES. 
            echo("ANY SEMESTER STRING:<br />");
            for($i=0;$i<sizeof($any_semesters);$i++)
            {
                echo($any_semesters[$i]."<br />");
            }

            echo("SEMESTER ID:<br />");
            for($i=0;$i<sizeof($arrSemesterID);$i++)
            {
                echo($arrSemesterID[$i]."<br />");
            }
            
            echo("SEMESTER DISPLAY:<br />");
            for($i=0;$i<sizeof($arrSemesterDisplay);$i++)
            {
                echo($arrSemesterDisplay[$i]."<br />");
            }
            */

            // Construct javascript arrays from our results. We create two arrays. semester_display_array, and semester_id_array. When calling 
            // semester_id_array[$x][$y], $x represents the semester type, and $y represents a semester under that semester type. If you use
            // semester_display_array[$x][$y], the $x, $y coordinates should map you to the semester's display value. 
            $string = "\nvar semester_display_array = new Array(\n";
            for ($i=0;$i<sizeof($arrSemesterDisplay);$i++)
            {
                if ($i == 0)
                {
                    $string .= "new Array(\"NULL\"),\n";
                }

                $string .= "new Array(\"NULL\",\"any\",".$arrSemesterDisplay[$i]."),\n";

            }
            $string = substr($string,0,strlen($string)-2)."\n";
            $string .= ");\n";

            $string .= "\nvar semester_id_array = new Array(\n";
            for ($i=0;$i<sizeof($arrSemesterID);$i++)
            {
                if ($i == 0)
                {
                    $string .= "new Array(\"NULL\"),\n";
                }

                $string .= "new Array(\"NULL\",".$any_semesters[$i].",".$arrSemesterID[$i]."),\n";

            }
            $string = substr($string,0,strlen($string)-2)."\n";
            $string .= ");\n";
            
            /* FOR DEBUGGING PURPOSES
            echo($string."<br />");

            for($i=0;$i<sizeof($arrSemesterType);$i++)
            {
                echo($arrSemesterType[$i]['description']." ".$arrSemesterType[$i]['description_type']."<br />");
            }

            for($i=0;$i<sizeof($any_semesters);$i++)
            {
                echo($any_semesters[$i]."<br />");
            }
            */

?>
        <script type='text/javascript'language='javascript'>
        <!--javascript
        <?php echo($string) ?>

        function update_semester()
        {
            index = document.myform.semester_type[document.myform.semester_type.selectedIndex].value;
            id = semester_id_array[index];
            display = semester_display_array[index];
            if (document.myform.semester_type[document.myform.semester_type.selectedIndex].value == "")
            {
                message = "Choose a Semester Type";
                document.myform.semester_id.options.length = 1;
                document.myform.semester_id.options[0] = new Option(message, "");
                document.myform.semester_id.selectedIndex = 0;
            }

            else
            {
                document.myform.semester_id.options.length = 0;
                document.myform.semester_id.options.length = id.length;
                for($i=0;$i<id.length;$i++)
                {
                    if (id[$i] == "NULL")
                    {
                        document.myform.semester_id.options[$i] = new Option(" ", "");
                    }
                    else
                    {
                        document.myform.semester_id.options[$i] = new Option(display[$i],id[$i]);
                    }
                }
                document.myform.semester_id.selectedIndex = 0;
            }
        }

        //-->
        </script>

    <?php
        //========================================================================================================================================================//
        
        echo("<tr align='center'>");
            echo("<td colspan='2' align='center'><br />Student Schedule<br /><br />");
                echo("<table class='row0' border='0' cellspacing='0' cellpadding='4'>");
                    echo("<tr>");
                        echo("<td>");
                            echo("student scheduled for ");
                            // Enter this case when a user has to reload this page from a save search.  
                            if (!$semester_type && $semester_id)
                            {
                                // $semester_id could be an integer or a list like 1,2,3,4. So, we grab the first value.
                                if (preg_match("/,/",$semester_id))
                                {
                                    // REGEXP: Find first set of digits followed by a comma, and then anything else. Take this first
                                    //         set of digits and assign it to $temp_semester_id. 
                                    $temp_semester_id = preg_replace("/^(\d+),.*/","\\1",$semester_id);
                                }
                                else
                                {
                                    $temp_semester_id = $semester_id;
                                }

                                // Find what sort of term this semester is. It could be a work term, academic term, etc. 
                                $semester_type_sql = ("
                                    SELECT s.description_type
                                    FROM semesters s
                                    WHERE s.semesters_id = '".$temp_semester_id."'
                                    ");
                                $semester_type_results = $GLOBALS['dbh']->Execute($semester_type_sql);
                                while ($semester_type_row = $semester_type_results->FetchRow())
                                {
                                    $semester_type = $semester_type_row['description_type'];
                                }
                            }
                            echo("<select name='semester_type' onchange='update_semester();'>");
                            echo("<option value=''>&nbsp;</option>");
                            for ($i=0;$i<sizeof($arrSemesterType);$i++)
                            {
                                if ($semester_type == $arrSemesterType[$i]['description_type'])
                                {
                                    $option = "selected='selected'";
                                }
                                echo("<option value='".$arrSemesterType[$i]['description_type']."' ".$option.">".$arrSemesterType[$i]['description']."</option>");
                                $option = "";
                            }
                            echo("</select>");
                            echo("<select name='semester_id'>");
                                if ($semester_type)
                                {
                                    echo("<option value=''>&nbsp;</option>");
                                    $semester_sql = ("
                                        SELECT DISTINCT ds.semesters_id, s.display
                                        FROM department_semesters ds 
                                        INNER JOIN semesters s
                                        ON ds.semesters_id = s.semesters_id
                                        WHERE s.description_type = '".$semester_type."' AND ds.department_id IN ".$departments_in_str."
                                        ORDER BY s.display
                                        ");
                                    $semester_results = $GLOBALS['dbh']->Execute($semester_sql);
                                    $any_string = "";
                                    while ($semester_row = $semester_results->FetchRow())
                                    {
                                        if ($semester_id == $semester_row['semesters_id'])
                                        {
                                            $option = "selected='selected'";
                                        }
                                        $any_string .= $semester_row['semesters_id'].",";
                                        $temp_string .= ("<option value='".$semester_row['semesters_id']."' ".$option.">".$semester_row['display']."</option>");
                                        $option = "";
                                    }
                                    
                                    $any_string = substr($any_string,0,strlen($any_string)-1);
                                    if ($semester_id == $any_string)
                                    {
                                        $option = "selected='selected'";
                                    }
                                    echo("<option value='".$any_string."' ".$option.">any</option>");
                                    $option = "";
                                    echo($temp_string);
                                }
                                else
                                {
                                    echo("<option value=''>&nbsp;</option>");
                                    echo("<option value=''>Please choose a Semester Type</option>");
                                }
                            echo("</select>");
                            echo(" during ");
                            echo("<select name='semester_term'>");
                                echo("<option value=''>&nbsp;</option>");

                                for($i=0;$i<sizeof($termarray);$i++)
                                {
                                    if ($semester_term == $termarray[$i][1])
                                    {
                                        $option = "selected='selected'";
                                    }
                                    echo("<option value='".$termarray[$i][1]."' ".$option.">".$termarray[$i][0]."</option>");
                                    $option = "";
                                }
                            echo("</select>");

                            $year_sql = ("
                                SELECT MIN(year)
                                FROM semesters_table
                                WHERE year != 0
                                ");
                            $min_year = $GLOBALS['dbh']->GetOne($year_sql); 

                            $year_sql = ("
                                SELECT MAX(year)
                                FROM semesters_table
                                WHERE year != 0
                                ");
                            $max_year = $GLOBALS['dbh']->GetOne($year_sql); 
                            echo("<select name='semester_year'>");
                                echo("<option value=''>&nbsp;</option>");

                                for($i=(int)$max_year;$i>=(int)$min_year;$i--)
                                {
                                    if ($semester_year == $i)
                                    {
                                        $option = "selected='selected'";
                                    }
                                    echo("<option value='".$i."' ".$option.">".$i."</option>");
                                    $option = "";
                                }
                            echo("</select>");
                        echo("</td>");
                    echo("</tr>");
                echo("</table>");
            echo("</td>");
        echo("</tr>");
    }

    else
    {
        echo("<tr align='center'>");
            echo("<td colspan='2' align='center'><br />Search by Term<br /><br />");
                echo("<table class='row0' border='0' cellspacing='0' cellpadding='4'>");
                    echo("<tr>");
                        echo("<td>");
                            echo("No student semester schedules have been set, or your department has no semesters defined yet.");
                        echo("</td>");
                    echo("</tr>");
                echo("</table>");
            echo("</td>");
        echo("</tr>");
    }

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
        echo("<td class='row1' colspan='2' align='center'><br />Display the following fields in the search results:<br /><br />");
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
                            echo("<select name='column[".$column_number."]'>\n");
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
                                elseif ($column_number >= 3 && $column_number <= 6 && !$more_columns && !$less_columns)
                                {
                                    switch ($column_number)
                                    {
                                        case 3:
                                            $selected_value = "d.department_name";
                                            break;
                                        case 4:
                                            $selected_value = "s.phone_current";
                                            break;
                                        case 5:
                                            $selected_value = "s.student_number";
                                            break;
                                        case 6:
                                            $selected_value = "s.email";
                                            break;
                                        default:
                                            $selected_value = "";
                                            
                                    }

                                    if ($columns_array[$i]["value"] == $selected_value && $selected_value != "")
                                    {
                                        $selected = "selected='selected'";
                                    }
                                }
                                echo("<option value='".$columns_array[$i]["value"]."' ".$selected.">".$columns_array[$i]["name"]."</option>\n");
                                $selected = "";
                            }
                            echo("</select>\n");
                        }
                        echo("</td>");

                        if ($column_number%2 == 0)
                        {
                            echo("</tr>");
                        }
                    }
                    echo("<tr>");
                        echo("<td colspan='4' align='right' class='row1'>");
                            echo("<input type='hidden' name='num_column' value='".$num_column."' />");
                        if($btnSearch == "Edit Search Criteria")
                        {
                            echo("<input type='hidden' name='search_id' value='$search_id' />");
                            echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
                            echo("<input type='hidden' name='edit_page' value='yes' />");
                        }
                        // Buttons that control how many columns will be displayed. 
                            echo("<input type='button' name='order' value='Clear Columns' onclick='javascript:clearOptions(\"column\",2,".$num_column.")' />");
                            if ($num_column > $min_columns)
                            {
                                echo("<input type='submit' name='less_columns' value='Less Columns' />");
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
            echo("<td colspan='2' align='center'><br />Order search results using:<br /><br /></td>");
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
                                            $selected_value = "d.department_name";
                                            break;
                                        case 4:
                                            $selected_value = "s.student_number";
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
                       
                        echo("<td colspan='4' align='right' class='row1'>");
                            echo("<input type='hidden' name='num_order' value='".$num_order."' />");
                        if($btnSearch == "Edit Search Criteria")
                        {
                            echo("<input type='hidden' name='search_id' value='$search_id' />");
                            echo("<input type='hidden' name='btnSearch' value='Edit Search Criteria' />");
                            echo("<input type='hidden' name='edit_page' value='yes' />");
                        }

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
                echo("<td colspan='2' align='center'>");
                echo("<input type='hidden' name='page' value='".$page."' />");
                echo("<br /><hr /><br />");
                echo("<input type='submit' name='btnSearch' value='Search' />");
                echo("&nbsp;&nbsp;&nbsp;");
                echo("<input type='button' name='ClearForm' value='Reset Form' onclick='javascript:document.reload_page.submit()' />");
                echo("&nbsp;&nbsp;&nbsp;");
                if($btnSearch == "Edit Search Criteria")
                {
                    //These variables are only set if you are editing a saved search
                    echo("<input type='hidden' name='search_id' value='$search_id' />");
                    echo("<input type='hidden' name='edit_page' value='yes' />");
                }
                if ($auth->userlevel == OFFICE)
                {
                    echo("<input type='submit' name='btnSearch' value='Save Search' />");
                } 
                echo("</td>");
        echo("</tr>");

                echo("</table>");
                echo("</form>");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td>");
        echo("<form name='reload_page' method='post' action='".$PHP_SELF."'>");
        echo("<input type='hidden' name='continue' value='Advanced Search' />");
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
