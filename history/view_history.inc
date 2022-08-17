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
 | Filename: view_history.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: To view the history information                                 |
 +------------------------------------------------------------------------------+

*/
include_once('history/container_functions.inc');

$department_in_str=implode("', '",$departments_in_group);
if($from_edit_page != 1)
{
    include('history/functions.inc');
}

//get the history ids of the records we want to go to with the next and previous buttons
if ($history_list != NULL) {
    $history_array = explode(",", $history_list);
}

if (!isset($position) && $history_id)
{
    for ($i=0; $i < sizeof($history_array); $i++)
    {
        if ($history_array[$i] == $history_id)
        {
            $position = $i;
            break;
        }
    }
}

// check position bounds
if ($position > sizeof($history_array)-1)
{
    $position = 0;
}
elseif ($position < 0)
{
    $position = sizeof($history_array) - 1;
}

$left_pos = $position - 1;
$right_pos = $position + 1;

// check bounds again. 
if ($left_pos < 0) { $left_pos = sizeof($history_array) - 1; }
if ($right_pos > sizeof($history_array) - 1) { $right_pos = 0; }

$left_number = $history_array[$left_pos];
$right_number = $history_array[$right_pos];
//get the history information from all the different tables
$history_sql=("
        SELECT h.options, h.wt_status, h.company_name, h.department_name, h.supervisor_name, h.year, h.supervisor_position
        , h.supervisor_email, h.supervisor_phone_number, h.supervisor_cellphone_number, h.supervisor_fax_number, h.work_term_street_address_1
        , h.work_term_street_address_2, h.work_term_country, h.work_term_province, h.work_term_city, h.work_term_postal_code
        , h.company_city, h.company_country, h.company_province, h.company_postal_code, h.company_street_address_1
        , h.company_street_address_2, h.site_visit_type_id_supervisor, h.work_term_home_phone, h.site_visit_by_supervisor
        , h.site_visit_date_supervisor, h.site_visit_type_id, h.work_site_visit_notes, h.student_number, h.comment, h.report_title
        , h.report_subject, h.report_marker, h.report_code, h.site_visit_date, h.site_visit_by, h.student_work_phone
        , h.employer_id, h.contact_id, h.job_title, h.term_id, h.work_email, h.salary, h.company_region_id, ji.job_code
        , sp.salary_period_name, h.work_term_hours_per_week, h.start_date, h.end_date, h.work_term_length, h.work_term_number
        FROM history h
        LEFT JOIN job_info ji
        ON h.job_id = ji.job_id
        LEFT JOIN salary_period sp
        ON h.salary_period_id = sp.salary_period_id
        WHERE h.history_id='".$history_id."'
        ");

$history_results=$GLOBALS['dbh']->Execute($history_sql);
$history_row=$history_results->FetchRow();
$history_row['term_name']=getTermName($history_row['term_id']);

$wt_code_sql = ("
    SELECT workterm_code
    FROM history_container
    WHERE history_id = '".$history_id."'
    ORDER BY workterm_code
    ");
$wt_code_result = $GLOBALS['dbh']->Execute($wt_code_sql);

// create comma delimited list of workterm codes if the student is using container based history records. 
if ($wt_code_result->RecordCount())
{
    while ($wt_code_row = $wt_code_result->FetchRow())
    {
        $wt_codes[] = $wt_code_row['workterm_code'];
    }
    $wt_codes = implode(",",$wt_codes);
    $history_row['work_term_number'] = $wt_codes;
}
if ($history_row['report_subject']<>NULL){
    $report_subject_sql= "
        SELECT report_subject_name 
        FROM history_report_subject 
        WHERE report_subject_id='".$history_row['report_subject']."'
        ";
    $report_subject_results=$GLOBALS['dbh']->Execute($report_subject_sql);
    $report_subject_row=$report_subject_results->FetchRow();
}
if ($history_row['wt_status']<>NULL){
    $wt_status_sql = "SELECT history_status_name 
        FROM history_status 
        WHERE history_status_id=".$history_row['wt_status'];
    $wt_status_results=$GLOBALS['dbh']->Execute($wt_status_sql);
    $wt_status_row=$wt_status_results->FetchRow();
}
if ($history_row['options']<>NULL){
    $options_sql = "SELECT history_options_name 
        FROM history_options 
        WHERE history_options_id=".$history_row['options'];
    $options_results = $GLOBALS['dbh']->Execute($options_sql); 
    $options_row = $options_results->FetchRow();   
}
if ($history_row['site_visit_type_id']<>NULL){
    $site_visit_type_sql = "Select description from site_visit_type where site_visit_type_id=".$history_row['site_visit_type_id']; 
    $site_visit_type_results = $GLOBALS['dbh']->Execute($site_visit_type_sql);
    $site_visit_type_row = $site_visit_type_results->FetchRow();
}
if ($history_row['site_visit_type_id_supervisor']<>NULL){
    $site_visit_type_sql = "Select description from site_visit_type where site_visit_type_id=".$history_row['site_visit_type_id_supervisor'];
    $site_visit_type_results = $GLOBALS['dbh']->Execute($site_visit_type_sql);
    $supervisor_site_visit_type_row = $site_visit_type_results->FetchRow();
}

$sql = ("
    SELECT s.student_number, s.first_name, sd.record_id, s.photo, sd.department_id, s.last_name, s.email, s.gender, d.discipline_name
    FROM student s
    INNER JOIN student_department sd
    ON s.student_number = sd.student_number
    INNER JOIN history h
    ON h.student_number = sd.student_number AND h.department_id = sd.department_id
    LEFT JOIN discipline d
    ON sd.discipline_id = d.discipline_id
    WHERE h.history_id = '".$history_id."' 
    ");

$results = $GLOBALS['dbh']->Execute($sql);
$student_row = $results->FetchRow();

if ($history_row['employer_id']<>NULL){
    $employer_sql="SELECT company_name,street_address_1,street_address_2,city,provstate_id,country_id,postal_code 
        FROM employer_company 
        WHERE employer_id='".$history_row['employer_id']."'";
    $employer_results=$GLOBALS['dbh']->Execute($employer_sql);
    $employer_row=$employer_results->FetchRow();
}

if ($history_row['site_visit_by']<>NULL){
    if($history_row['site_visit_by'] == SITE_VISIT_OTHER)
    {
        $visitor_row["first_name"] = "Other";
        $visitor_row["last_name"] = "";
    }
    elseif($history_row['site_visit_by'] == SITE_VISIT_FACULTY)
    {
        $visitor_row["first_name"] = "Faculty";
        $visitor_row["last_name"] = "";
    }
    else
    {
        $visitor_row=getContactName($history_row['site_visit_by']);
    }
}

if ($history_row['site_visit_by_supervisor']<>NULL){
    if($history_row['site_visit_by_supervisor'] == SITE_VISIT_OTHER)
    {
        $supervisor_visitor_row["first_name"] = "Other";
        $supervisor_visitor_row["last_name"] = "";
    }
    elseif($history_row['site_visit_by_supervisor'] == SITE_VISIT_FACULTY)
    {
        $supervisor_visitor_row["first_name"] = "Faculty";
        $supervisor_visitor_row["last_name"] = "";
    }
    else
    {
        $supervisor_visitor_row=getContactName($history_row['site_visit_by_supervisor']);
    }
}

$sql = "Select notes, contact_id, date_entered from history_notes where history_id='".$history_id."' order by date_entered desc";
$notesresults=$GLOBALS['dbh']->Execute($sql);
$company_street_address_1=$history_row['company_street_address_1'];
$company_street_address_2=$history_row['company_street_address_2'];
$company_city=$history_row['company_city'];

if ($history_row['company_country']<>NULL){
    $company_country=getCountryName($history_row['company_country']);
}

if ($history_row['company_province']<>NULL){
    $company_province=getProvStateName($history_row['company_province']);
}

$company_postal_code=$history_row['company_postal_code'];

if ($history_row['company_name']==NULL){
    $history_row['company_name']=$employer_row['company_name'];
}

$work_term_street_address_1 = $history_row['work_term_street_address_1'];
$work_term_street_address_2 = $history_row['work_term_street_address_2'];
$work_term_city=$history_row['work_term_city'];
$work_term_province = getProvStateName($history_row['work_term_province']);
$work_term_country = getCountryName($history_row['work_term_country']);
$work_term_postal_code = $history_row['work_term_postal_code'];

if ($province<>NULL){
    $province_sql="Select provstate_name from provstate_list where provstate_id='".$province."'";
    $province_results=$GLOBALS['dbh']->Execute($province_sql);
    $province_row=$province_results->FetchRow();
    $province=$province_row['provstate_name'];
}

if ($country<>NULL){
    $country_sql="Select country_name from country_list where country_id='".$country."'";
    $country_results=$GLOBALS['dbh']->Execute($country_sql);
    $country_row=$country_results->FetchRow();
    $country=$country_row['country_name'];
}

if ($student_row['department_id']<>NULL){
    $department_sql="Select department_name from department where department_id='".$student_row['department_id']."'";
    $department_results=$GLOBALS['dbh']->Execute($department_sql);
    $department_row=$department_results->FetchRow();
}

if($from_edit_page == 1)
{
    notify("The following history record was successfully updated<br />");                                    
}
if ($new_container)
{
    notify("Additional history records have been created as the duration of the work term has changed.");
}
if ($term_has_changed_flag && $term_has_changed_msg) {
    notify($term_has_changed_msg);
}

if ($search_by_term=="true" || $history_list<>NULL){
    echo ("
            <table width='96%' border='0'>
            <tr align='center'>
            <td align='left'>
            <form method='post' action='" . $PHP_SELF . "'>");
    if ($fromPage == "simple search")
    {
        echo("<input type='hidden' name='btnSubmit' value='View Placements' />
                <input type='hidden' name='searchmode' value='simple' />
                <input type='hidden' name='search_by_student' value='true' />
                <input type='hidden' name='search_by_term' value='true' />");
    }
    echo("<input type='hidden' name='searchHistory' value='" . ($searchHistory) . "' />");
    echo("  <input type='hidden' name='select' value='history' />
            <input type='hidden' name='btnSearch' value='Search' />
            <input type='hidden' name='student_num' value='".$student_row['student_number']."' />
            <input type='hidden' name='student_list' value='".$student_list."' />
            <input type='hidden' name='student_department' value='".$student_department."' />
            <input type='hidden' name='searchStudents' value='".$searchStudents."' />
            <input type='hidden' name='history_list' value='" . $history_list . "' />
            <input type='hidden' name='per_page_max' value='". $per_page_max . "' />
            <input type='hidden' name='history_id' value='" . $left_number . "' />
            <input type='hidden' name='position' value='".($position-1)."' />
            ");
            
    if ($search_by_term=="true"){
        echo ("<input type='hidden' name='search_by_term' value='".$search_by_term."' />");
    }
    echo ("<input type='image' src='misc/images/previous.gif' />
            </form>
            </td>");
    
    echo ("<td align='center'>
            <form method='post' action='" . $PHP_SELF . "'>
            <input type='hidden' name='searchHistory' value='" . $searchHistory . "' />
            <input type='hidden' name='per_page_max' value='" . $per_page_max ."' />
            <input type='hidden' name='btnSearch' Value='Search' />");
    if ($fromPage == "simple search")
    {
        echo("<input type='hidden' name='btnSubmit' value='View Placements' />
                <input type='hidden' name='select' value='history_choose' />
                <input type='hidden' name='searchmode' value='simple' />
                <input type='hidden' name='btnSumbit' value='View Placements' />
                <input type='hidden' name='search_by_student' value='true' />
                <input type='hidden' name='search_by_term' value='true' />");
    }
    elseif ($search_by_term=='true' || $search_by_student=='true'){
        echo(" <input type='hidden' name='search_by_term' value='true' />");
        echo(" <input type='hidden' name='search_by_term' value='".$search_by_term."' />");
        echo(" <input type='hidden' name='search_by_student' value='".$search_by_student."' />");
        echo(" <input type='hidden' name='history_list' value='".$history_list."' />");
        echo(" <input type='hidden' name='select' value='history_advanced_search' />");
        echo(" <input type='hidden' name='btnSearch' value='Search' />");
    }else{
        echo(" <input type='hidden' name='select' value='history_advanced_search' />");
    }
    echo("<input type='submit' name='back_to_list' value='Back to List' />");

    // These two hidden fields are for paging. 

    echo("<input type='hidden' name='record_id' value='". $student_row['record_id']. "' />");
    echo("<input type='hidden' name='student_num' value='". $student_row['student_number']. "' />");
    echo("<input type='hidden' name='start_row' value='" . $start_row . "' />");
    echo("<input type='hidden' name='per_page_max' value='" . $per_page_max . "' />");

    echo("<input type='hidden' name='showquick' value='".$showquick."' />");
    echo("<input type='hidden' name='student_list' value='".$student_list."' />");
    echo("<input type='hidden' name='student_department' value='".$student_department."' />");
    echo("<input type='hidden' name='searchStudents' value='".$searchStudents."' />");
    echo("<input type='hidden' name='searchHistory' value='" . $searchHistory . "' />");
    echo("</form>");
    echo("</td>");
    echo("<td align='right'>");

    
    echo(" <form method='post' action='" . $PHP_SELF . "'>");
    echo("<input type='hidden' name='select' value='history' />");

    if ($fromPage == "simple search")
    {
        echo("<input type='hidden' name='btnSubmit' value='View Placements' />
                <input type='hidden' name='searchmode' value='simple' />
                <input type='hidden' name='search_by_student' value='true' />
                <input type='hidden' name='fromPage' value='simple search' />
                <input type='hidden' name='search_by_term' value='true' />");

    }

    echo("  <input type='hidden' name='student_num' value='".$student_row['student_number']."' />
            <input type='hidden' name='student_list' value='".$student_list."' />
            <input type='hidden' name='student_department' value='".$student_department."' />
            <input type='hidden' name='searchStudents' value='".$searchStudents."' />
            <input type='hidden' name='searchHistory' value='" . $searchHistory . "' />
            <input type='hidden' name='history_list' value='" . $history_list . "' />
            <input type='hidden' name='per_page_max' value='". $per_page_max . "' />
            <input type='hidden' name='history_id' value='" . $right_number . "' />
            <input type='hidden' name='position' value='".($position+1)."' />
            ");
    if ($search_by_term=="true"){
        echo("<input type='hidden' name='search_by_term' value='".$search_by_term."' />");
    }
    echo("<input type='image' src='misc/images/next.gif' />
            </form>
            </td>
            </tr>
            </table>
            ");
}
//start history information output
$historyOutput .= "<table border='1' cellpadding='10' width='96%'><tr><td>
<table border='0' cellpadding='5' cellspacing='0' width='100%'>";

// Student Name
$historyOutput .= "<tr><td colspan='3' align='center'><h4>&nbsp;". $student_row["first_name"] . " " . $student_row["last_name"]." (".$student_row['student_number'].") at ". $history_row['company_name'] . "</h4><hr /></td></tr>";
$historyOutput .= "<tr><td colspan='3' align='center'><h4>&nbsp;".$history_row["term_name"] . " " . $history_row["year"]. "</h4><hr /></td></tr>";

// Didn't want to hardcode column widths.. may change later.
$col1width = "35%"; $col2width = "35%"; $col3width = "30%";

/****** Main Info Section of View History
  On Left : Current/Permanent Address, Current/Permanent Phone #, E-mail, High School, Birthdate, Citizenship, Gender.
  On Right : Photo if it exists
 *******/
// Check Photo's existence.
if ($student_row["photo"] == NULL) {
    $photo_output = "<b>No Photo Available</b>";
} else { 
    // hack to fix an IE image cache bug
    $num = rand(0,100);

    $photo_output_html ="<img src='".$PHP_SELF_MENU."?select=student_photo_viewer&amp;no_html=1&amp;record_id=".$student_row['record_id']."&amp;num=".$num."' alt='". $student_row["first_name"] . " " . $student_row["last_name"] . "' />";
    $photo_output_pdf =  ("<img src='".$student_row['student_number'].".jpg' />");
    $photo_output = STUDENT_PHOTO_PLACEHOLDER;
}

// Get Current Address (build the output string..)
if ($company_street_address_1) $current_address_output .= $company_street_address_1 . "<br />";
if ($company_street_address_2) $current_address_output .= $company_street_address_2 . "<br />";
if ($company_city) $current_address_output .= $company_city . ", ";
if ($company_province)	{	$current_address_output .= $company_province . "<br />"; }
else {	$current_address_output .=  ($province) ? "<br />" : ""; }
if ($company_country) $current_address_output .= $company_country;
if ($company_postal_code)$current_address_output .= "<br />" .  $company_postal_code;

if($current_address_output == "NULL")
    $current_address_output = "";

// Get Permanent Address (build the output string..)
if ($row["street_address_permanent"]) $permanent_address_output .= $row["street_address_permanent"] . "<br />";
if ($row["street_address_permanent2"]) $permanent_address_output .= $row["street_address_permanent2"] . "<br />";
if ($row["city_permanent"]) $permanent_address_output .= $row["city_permanent"] . ", ";
if ($row["province_permanent"])	{	$permanent_address_output .= $provincepermanentrow["provstate_name"] . "<br />"; }
else {	$permanent_address_output .=  ($row["city_permanent"]) ? "<br />" : ""; }
if ($row["country_permanent"]) $permanent_address_output .= $countrypermanentrow["country_name"];
if ($row["postal_code"])$permanent_address_output .= "<br />" . $row["postal_code_permanent"];

$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Company Name : </b></td>
<td width='$col2width' align='left'>". $history_row['company_name'] . "&nbsp;</td>
<td width='$col3width' align='center' valign='middle' rowspan=9>$photo_output</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Division Name : </b></td>
<td width='$col2width' align='left'>". $history_row['department_name'] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Current Address : </b></td>
<td width='$col2width' align='left'>". $current_address_output . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Region : </b></td>
<td width='$col2width' align='left'>". getRegionName($history_row['company_region_id']) . "&nbsp;</td></tr>";

if (!$history_row['job_code'])
{
    $history_row['job_code'] = 'Own Job / Returning Job';
}

$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Job Code : </b></td>
<td width='$col2width' align='left'>". $history_row['job_code'] . "&nbsp;</td></tr>";

$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b> Supervisor : </b></td>
<td width='$col2width' align='left'>". $history_row["supervisor_name"]."&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor Title : </b></td>
<td width='$col2width' align='left'>". $history_row['supervisor_position'] . "&nbsp;</td></tr>";

$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor's Phone Number : </b></td>
<td width='$col2width' align='left'>". $history_row['supervisor_phone_number'] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor's Cell Number : </b></td>
<td width='$col2width' align='left'>". $history_row['supervisor_cellphone_number'] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor's Fax Number : </b></td>
<td width='$col2width' align='left'>". $history_row["supervisor_fax_number"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor's E-mail : </b></td>
<td width='$col2width' align='left'>". $history_row["supervisor_email"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Salary : </b></td>
<td width='$col2width' align='left'>". $history_row["salary"].""; 
if ($history_row["salary_period_name"])
{
    $historyOutput .=  "/".$history_row["salary_period_name"]."&nbsp;";
}
$historyOutput .= "</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Hours per Week : </b></td>
<td width='$col2width' align='left'>". $history_row["work_term_hours_per_week"].""; 
$historyOutput .= "</td></tr>";

$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Start Date : </b></td>
<td width='$col2width' align='left'>". $history_row["start_date"].""; 
$historyOutput .= "</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>End Date : </b></td>
<td width='$col2width' align='left'>". $history_row["end_date"].""; 
$historyOutput .= "</td></tr>";

//container specific output
$container_mode = useContainers($student_row['student_number'],$student_row['department_id']);
if ($container_mode == CONTAINER_TERM)
{
    $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Workterm Length : </b></td>
    <td width='$col2width' align='left'>". $history_row["work_term_length"]." month".(($history_row['work_term_length']==1) ? "" : "s"); 
    $historyOutput .= "</td></tr>";
}
elseif ($container_mode == CONTAINER_HOURS)
{
    $total_days = count_days($history_row["start_date"],$history_row["end_date"]);
    $hours_per_day = $history_row["work_term_hours_per_week"] / 7;
    $total_hours = round($total_days * $hours_per_day);

    $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Total Hours : </b></td>
    <td width='$col2width' align='left'>". $total_hours.""; 
    $historyOutput .= "</td></tr>";
}

// spacer
$historyOutput .= "<tr><td colspan='3'><br /><hr /><br /></td></tr>";

/***** Start Student Information
 ******/
// Get Current Address (build the output string..)
if ($work_term_street_address_1) $address_output .= $work_term_street_address_1 . "<br />";
if ($work_term_street_address_2) $address_output .= $work_term_street_address_2 . "<br />";
if ($work_term_city) $address_output .= $work_term_city . ", ";
if ($work_term_province)	{	$address_output .= $work_term_province . "<br />"; }
else {	$address_output .=  ($province) ? "<br />" : ""; }
if ($work_term_country) $address_output .= $work_term_country;
if ($work_term_postal_code)$address_output .= "<br />" .  $work_term_postal_code;

if($address_output == NULL)
    $address_output = "";

$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Discipline : </b></td>
<td width='$col2width' align='left'>". $student_row["discipline_name"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Number : </b></td>
<td width='$col2width' align='left'>". $history_row["work_term_number"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Status : </b></td>
<td width='$col2width' align='left'>". $wt_status_row["history_status_name"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Job Title : </b></td>
<td width='$col2width' align='left'>". $history_row["job_title"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Department : </b></td>
<td width='$col2width' align='left'>". $department_row['department_name'] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Home Address : </b></td>
<td width='$col2width' align='left'>". $address_output . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Phone Number : </b></td>
<td width='$col2width' align='left'>". $history_row["student_work_phone"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Home Phone Number : </b></td>
<td width='$col2width' align='left'>". $history_row["work_term_home_phone"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student E-mail :</b></td>
<td width='$col2width' align='left'>". $student_row["email"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Work Term E-mail : </b></td>
<td width='$col2width' align='left'>". $history_row["work_email"] . "&nbsp;</td></tr>";
// spacer
$historyOutput .= "<tr><td colspan='3'><br /><hr /><br /></td></tr>";

/***** Start Work Term Information
 ******/

$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Work Term Visit On : </b></td>
<td width='$col2width' align='left'>". $history_row["site_visit_date"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Work Term Visit by : </b></td>
<td width='$col2width' align='left'>". $visitor_row['first_name'] ." ".$visitor_row['last_name']."&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Student Work Term Visit Type : </b></td>
<td width='$col2width' align='left'>".$site_visit_type_row['description']."&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor Work Term Visit On : </b></td>
<td width='$col2width' align='left'>". $history_row["site_visit_date_supervisor"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor Work Term Visit by : </b></td>
<td width='$col2width' align='left'>". $supervisor_visitor_row['first_name'] ." ".$supervisor_visitor_row['last_name']."&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Supervisor Work Term Visit Type : </b></td>
<td width='$col2width' align='left'>".$supervisor_site_visit_type_row['description']."&nbsp;</td></tr>";

$work_site_visit_notes = preg_replace("/\n/","<br />",$history_row['work_site_visit_notes']);
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Visit Notes : </b></td>
<td width='$col2width' align='left'>". $work_site_visit_notes. "&nbsp;</td></tr>";

// spacer
$historyOutput .= "<tr><td colspan='3'><br /><hr /><br /></td></tr>";

/***** Start Work Term Report Information
 ******/

$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Report Title : </b></td>
<td width='$col2width' align='left'>". $history_row["report_title"] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Subject : </b></td>
<td width='$col2width' align='left'>". $report_subject_row['report_subject_name'] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Work Term Report Marker : </b></td>
<td width='$col2width' align='left'>". $history_row['report_marker'] . "&nbsp;</td></tr>";
$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Report Code : </b></td>
<td width='$col2width' align='left'>". $history_row["report_code"] . "&nbsp;</td></tr>";

// spacer
$historyOutput .= "<tr><td colspan='3'><br /><hr /><br /></td></tr>";

// Flags : Student flags
$arrFlags=history_flags($history_id,$student_row['department_id']);
foreach ($arrFlags as $id => $flag){
    if ($flag['checked']){
        $flags_output .= $flag['description']."<br />";
    }
}

$historyOutput .= "<tr><td valign='top' align='right'><b>History Flags :</b></td>
<td>$flags_output</td>";

// spacer
$historyOutput .= "<tr><td colspan='3'><br /><hr /><br /></td></tr>";

// Last Section : Comments, Notes, Resumes, Coverletters, Work Term History

$historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Notes : </b></td>
</tr>";

while($rstNotes=$notesresults->FetchRow()){
    $historyOutput .= "<tr><td>&nbsp;&nbsp;</td><td>";
    $contact_id_row=getContactInfoByID($rstNotes['contact_id']);
    $contact_name=$contact_id_row['first_name']." ".$contact_id_row['last_name'];
    $historyOutput .= "<b class='blue'>".$contact_name."</b><br /><b class='blue'>".$rstNotes['date_entered']."</b><br /><p>";
    $temp=explode("\n",$rstNotes["notes"]);
    $historyOutput .= implode("<br />",$temp);
    $historyOutput .= "</td></tr>";
    $count=$count+1;
}

$changes_sql = "SELECT changes_recorded_1,changes_recorded_2,changes_recorded_3,change_by_1,change_by_2,change_by_3,change_date_1
,change_date_2,change_date_3 
FROM history 
WHERE history_id='".$history_id."'";
$changes_results = $GLOBALS['dbh']->Execute($changes_sql);
$changes_row = $changes_results->FetchRow(); 

$historyOutput .= "<tr><td colspan='3'><br /><hr /><br /></td></tr>";

if ($changes_row['changes_recorded_1']<>NULL){

    $contact_name= getContactName($changes_row['change_by_1']);	
    $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Fields Changed: </b></td>
        <td width='$col2width' align='left'>". $changes_row['changes_recorded_1'] . "&nbsp;</td></tr>";
    $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Changed By: </b></td>
        <td width='$col2width' align='left'>". $contact_name['first_name']." ".$contact_name['last_name'] . "&nbsp;</td></tr>";
    $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Change Date: </b></td>
        <td width='$col2width' align='left'>". $changes_row['change_date_1'] . "&nbsp;</td></tr>";

    if ($changes_row['changes_recorded_2']<>NULL){
        $historyOutput .= "<tr><td colspan='3'><br /><hr /><br /></td></tr>";
        $contact_name= getContactName($changes_row['change_by_2']);	
        $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Fields Changed: </b></td>
            <td width='$col2width' align='left'>". $changes_row['changes_recorded_2'] . "&nbsp;</td></tr>";
        $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Changed By: </b></td>
            <td width='$col2width' align='left'>".$contact_name['first_name']." ".$contact_name['last_name'] . "&nbsp;</td></tr>";
        $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Change Date: </b></td>
            <td width='$col2width' align='left'>". $changes_row['change_date_2'] . "&nbsp;</td></tr>";
    }
    if ($changes_row['changes_recorded_3']<>NULL){
        $historyOutput .= "<tr><td colspan='3'><br /><hr /><br /></td></tr>";
        $contact_name= getContactName($changes_row['change_by_3']);	
        $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Fields changed: </b></td>
            <td width='$col2width' align='left'>". $changes_row['changes_recorded_3'] . "&nbsp;</td></tr>";
        $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Changed By: </b></td>
            <td width='$col2width' align='left'>". $contact_name['first_name']." ".$contact_name['last_name'] . "&nbsp;</td></tr>";
        $historyOutput .= "<tr><td width='$col1width' align='right' valign='top'><b>Change Date: </b></td>
            <td width='$col2width' align='left'>". $changes_row['change_date_3'] . "&nbsp;</td></tr>";
    }
}

// End of Student Info Table

$historyOutput .= "</table></td></tr></table>";

$pdfOutput = $historyOutput;
$htmlOutput = $historyOutput;

$pdfOutput = str_replace(STUDENT_PHOTO_PLACEHOLDER, $photo_output_pdf, $pdfOutput);
$htmlOutput = str_replace(STUDENT_PHOTO_PLACEHOLDER, $photo_output_html, $htmlOutput);
print $htmlOutput;

echo ("
        <table width='96%' border='0'>
        <tr>");

if ($searchHistory<>NULL){
    echo("<td align='left'>
            <form method='post' action='" . $PHP_SELF . "'>");
    if ($fromPage == "simple search")
    {
        echo("<input type='hidden' name='btnSubmit' value='View Placements' />
                <input type='hidden' name='searchmode' value='simple' />
                <input type='hidden' name='search_by_student' value='true' />
                <input type='hidden' name='search_by_term' value='true' />");
    }

    echo("<input type='hidden' name='searchHistory' value='" . ($searchHistory) . "' />");
    echo("  <input type='hidden' name='select' value='history' />
            <input type='hidden' name='btnSearch' value='Search' />
            <input type='hidden' name='student_num' value='".$student_num."' />
            <input type='hidden' name='student_list' value='".$student_list."' />
            <input type='hidden' name='student_department' value='".$student_department."' />
            <input type='hidden' name='searchStudents' value='".$searchStudents."' />
            <input type='hidden' name='history_list' value='" . $history_list . "' />
            <input type='hidden' name='per_page_max' value='". $per_page_max . "' />
            <input type='hidden' name='history_id' value='" . $left_number . "' />
            <input type='hidden' name='position' value='".($position-1)."' />
            ");
    if ($search_by_term=="true"){
        echo ("<input type='hidden' name='search_by_term' value='".$search_by_term."' />");
    }
    echo ("<input type='image' src='misc/images/previous.gif' />
            </form>
            </td>");
}
echo("<td align='right'>
        <form method='post' action='" . $PHP_SELF ."'>
        <input type='hidden' name='select' value='verify_history_delete' />
        <input type='hidden' name='history_id' value='$history_id' />
        <input type='hidden' name='history_list' value='".$history_list."' />
        <input type='hidden' name='student_list' value='".$student_list."' />
        <input type='hidden' name='student_department' value='".$student_department."' />
        <input type='hidden' name='student_num' value='".$student_num."' />
        <input type='hidden' name='searchStudents' value='".$searchStudents."' />
        <input type='hidden' name='menu_select' value='".$menu_select."' />
        <input type='hidden' name='searchHistory' value='".$searchHistory."' />
        <input type='submit' name='edit_record' Value='Delete Record' />
        </form>
        </td>");
if ($searchHistory<>NULL){
    echo("<td align='center'>
            <form method='post' action='" . $PHP_SELF . "'>
            <input type='hidden' name='searchHistory' value='" . $searchHistory . "' />
            <input type='hidden' name='per_page_max' value='" . $per_page_max ."' />
            ");
    if ($fromPage == "simple search")
    {
        echo("<input type='hidden' name='btnSubmit' value='View Placements' />
                <input type='hidden' name='select' value='history_choose' />
                <input type='hidden' name='searchmode' value='simple' />
                <input type='hidden' name='btnSumbit' value='View Placements' />
                <input type='hidden' name='search_by_student' value='true' />
                <input type='hidden' name='search_by_term' value='true' />");
    }
    elseif ($search_by_term=='true' || $search_by_student=='true'){
        echo(" <input type='hidden' name='search_by_term' value='true' />");
        echo(" <input type='hidden' name='search_by_term' value='".$search_by_term."' />");
        echo(" <input type='hidden' name='search_by_student' value='".$search_by_student."' />");
        echo(" <input type='hidden' name='history_list' value='".$history_list."' />");
        echo(" <input type='hidden' name='select' value='history_advanced_search' />");
        
        echo(" <input type='hidden' name='btnSearch' value='Search' />");
    }else{
        echo(" <input type='hidden' name='select' value='history_advanced_search' />");
            echo("<input type='hidden' name='btnSearch' Value='Search' />");
    }
    echo("<input type='submit' name='back_to_list' value='Back to List' />
            <input type='hidden' name='record_id' value='". $student_row['record_id']. "' />
            <input type='hidden' name='student_num' value='". $student_row['student_number']. "' />

            <input type='hidden' name='start_row' value='" . $start_row . "' />
            <input type='hidden' name='per_page_max' value='" . $per_page_max . "' />

            <input type='hidden' name='showquick' value='".$showquick."' />
            <input type='hidden' name='student_list' value='".$student_list."' />
            <input type='hidden' name='student_department' value='".$student_department."' />
            <input type='hidden' name='searchStudents' value='".$searchStudents."' />
            </form>
            </td>");
}
echo("<td align='left'><form method='post' action='" . $PHP_SELF . "'>
        <input type='hidden' name='select' value='historyOutput' />
        <input type='hidden' name='HTMLhistory' value='". urlencode($pdfOutput) . "' />
        <input type='hidden' name='PDF' value='1' />
        <input type='submit' name='viewpdf' value='View in PDF' />
        </form>
        </td>");
if ($searchHistory<>NULL){
    echo("<td align='right'>");
    echo(" <form method='post' action='" . $PHP_SELF . "'>");
    echo("<input type='hidden' name='select' value='history' />");
    if ($fromPage == "simple search")
    {
        echo("<input type='hidden' name='btnSubmit' value='View Placements' />
                <input type='hidden' name='searchmode' value='simple' />
                <input type='hidden' name='search_by_student' value='true' />
                <input type='hidden' name='fromPage' value='simple search' />
                <input type='hidden' name='search_by_term' value='true' />");

    }
    echo("  <input type='hidden' name='student_num' value='".$student_num."' />
            <input type='hidden' name='student_list' value='".$student_list."' />
            <input type='hidden' name='student_department' value='".$student_department."'/>
            <input type='hidden' name='searchStudents' value='".$searchStudents."' />
            <input type='hidden' name='searchHistory' value='" . $searchHistory . "' />
            <input type='hidden' name='history_list' value='" . $history_list . "' />
            <input type='hidden' name='per_page_max' value='". $per_page_max . "' />
            <input type='hidden' name='history_id' value='" . $right_number . "' />
            <input type='hidden' name='position' value='".($position+1)."' />
            ");
    if ($search_by_term=="true"){
        echo("<input type='hidden' name='search_by_term' value='".$search_by_term."' />");
    }
    echo("<input type='image' src='misc/images/next.gif' />
            </form>
            </td>");
}
echo("</tr>
        </table>");

?>

