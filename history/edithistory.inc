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
 | Filename: edithistory.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This file is the main handler for editing history records.      |
 +------------------------------------------------------------------------------+
 | TODO: The work_term_region_id needs to be added to this form eventually.     |
 |       This field records the region where the student is living while on     |
 |       a work term.                                                           |
 +------------------------------------------------------------------------------+

:TODO: Refactor all of the SQL update statements below to one single update statement,
       as the updates should be atomic. 
 
*/
if ($userlevel == OFFICE){
    if ($department_id == ""){
        $department_id = $auth->department;
    }

    $department_in_str = "('" . implode("','", $departments_in_group) . "')";
}
include_once('history/functions.inc');
include_once('history/container_functions.inc');

$sql = ("
        SELECT sd.record_id
        FROM student_department sd 
        INNER JOIN history h
        ON sd.student_number = h.student_number AND sd.department_id = h.department_id
        WHERE h.history_id = '".$history_id."' AND sd.department_id IN ".$department_in_str."
        ");
$results = $GLOBALS['dbh']->Execute($sql);
$record_row=$results->FetchRow(); 
$record_id=$record_row['record_id'];

assert($record_id);

if ($changed_company_division == 1)
{
    // We see if the company name has changed. We make sure $select does not equal to add_history because when we add a history record, we have no former 
    // company name to compare with.
    if ($company_name <> NULL && $select != "add_history") {
        // Get the modified company name
        $sql = "SELECT employer_id
            FROM employer_company
            WHERE company_name = '".$company_name."' AND company_display='1'";
        $employer_result=$GLOBALS['dbh']->Execute($sql);
        $new_employer_row=$employer_result->FetchRow();
        // Get the 'old' company name
        $sql = "SELECT employer_id, company_name
            FROM history
            WHERE history_id='".$history_id."'";
        $employer_result=$GLOBALS['dbh']->Execute($sql);
        $employer_row=$employer_result->FetchRow();
        // If the modified and old company names don't match, it means someone has modified the company_name, else, do nothing.
        //if (($employer_row['employer_id'] != $new_employer_row['employer_id']) || ($employer_row['company_name'] != $company_name)) {
        if ($employer_row['company_name'] != stripslashes($company_name)) {
            // We change relevent fields in history to match the new company's department name, employer_id, etc.
            $sql = "UPDATE history SET employer_id = '".$new_employer_row['employer_id']."' WHERE history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "UPDATE history SET company_name = '".$company_name."' WHERE history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
            // We set the following fields null for consistency. If a user wants these fields automatically filled out, they have to hit the
            // retrieve information button to set them.
            $sql = "UPDATE history SET company_city = NULL where history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "UPDATE history SET company_street_address_1 = NULL where history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "UPDATE history SET company_street_address_2 = NULL where history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "UPDATE history SET company_postal_code = NULL where history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "UPDATE history SET company_province = NULL where history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "UPDATE history SET company_country = NULL where history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
        }
    }

    // Checks if department name has changed. This part is almost the same as the block of code above.
    if ($department_name <> NULL && $select !="add_history") {
        $sql = "SELECT department_id
            FROM employer_department
            WHERE department_name = '".$department_name."' AND employer_id = '".$new_employer_row['employer_id']."'";
        $department_result=$GLOBALS['dbh']->Execute($sql);
        $new_department_row = $department_result->FetchRow();
        $sql = "SELECT company_department_id, department_name
            FROM history
            WHERE history_id=".$history_id;
        $department_result=$GLOBALS['dbh']->Execute($sql);
        $department_row=$department_result->FetchRow();

        if (($department_row['company_department_id'] != $new_department_row['department_id']) || ($department_row['department_name'] != $department_name)){
            $sql = "UPDATE history SET department_name = '".$department_name."' WHERE history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
            $sql = "UPDATE history SET company_department_id = '".$new_department_row['department_id']."' WHERE history_id = '".$history_id."'";
            $GLOBALS['dbh']->Execute($sql);
        }
    }
    // It is possible for us to have a company name without a department name, so we use this elseif clause to handle that case. 
    elseif ($department_name == NULL && $select != "add_history" && isset($flag)) {
        $sql = "UPDATE history SET department_name = NULL WHERE history_id = '".$history_id."'";
        $GLOBALS['dbh']->Execute($sql);
        $sql = "UPDATE history SET company_department_id = NULL WHERE history_id = '".$history_id."'";
        $GLOBALS['dbh']->Execute($sql);
    }
    // The first time we edit a placement history, $department_name is automatically null. So we skip the elseif statement above if it's the first time
    // we edit this record. (First time, meaning first time this session.)
    if (!isset($flag)) {
        $flag = 1;
    }
    }//End if

    //get information to default the input boxes to
    $history_sql=("
            SELECT h.company_department_id, h.department_id, h.company_name, h.supervisor_position, h.term_id, h.department_name, h.year
            , h.supervisor_name, h.company_position, h.supervisor_email, h.supervisor_fax_number, h.supervisor_phone_number, h.supervisor_cellphone_number
            , h.supervisor_email, h.year, h.term_id, h.company_postal_code, h.company_country, h.company_province, h.company_city
            , h.company_street_address_1, h.company_street_address_2, h.work_term_street_address_1, h.work_term_street_address_2
            , h.work_term_home_phone, h.work_term_city, h.work_term_province, h.work_term_country, h.work_term_postal_code, h.site_visit_date_supervisor
            , h.site_visit_by_supervisor, h.site_visit_type_id_supervisor, h.work_site_visit_notes, h.site_visit_type_id, h.student_number
            , h.comment, h.report_title, h.options, h.wt_status, h.report_subject, h.report_marker, h.report_code, h.site_visit_date, h.site_visit_type_id
            , h.site_visit_by, h.student_work_phone, h.employer_id, h.contact_id, h.job_title, h.term_id, h.work_term_length
            , h.work_email, h.salary, h.company_region_id, ji.job_code, h.salary_period_id, h.work_term_hours_per_week
            , h.work_term_number, h.start_date, h.end_date
            , h.entered_on
            FROM history h
            LEFT JOIN job_info ji
            ON h.job_id = ji.job_id
            LEFT JOIN history_container hc
            ON h.history_id = hc.history_id
            WHERE h.history_id = '".$history_id."'
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

    if ($history_row['student_number']<>NULL){
        $student_sql="SELECT first_name, photo, last_name, email, gender 
            FROM student 
            WHERE student_number='".$history_row['student_number']."'";
        $student_results=$GLOBALS['dbh']->Execute($student_sql);
        $student_row=$student_results->FetchRow();
    }

    if ($history_row['employer_id']<>NULL){
        $employer_sql = ("
                SELECT company_name, street_address_1, street_address_2, city,provstate_id, country_id, postal_code 
                FROM employer_company 
                WHERE employer_id='".$history_row['employer_id']."'
                ");
        $employer_results=$GLOBALS['dbh']->Execute($employer_sql);
        $employer_row=$employer_results->FetchRow();
    }

    if ($history_row['site_visit_by']<>NULL){
        $visitor_sql = ("
                SELECT first_name, last_name 
                FROM contact 
                WHERE contact_id='".$history_row['site_visit_by']."'
                ");
        $visitor_results=$GLOBALS['dbh']->Execute($visitor_sql);
        $visitor_row=$visitor_results->FetchRow();
    }

    $discipline_sql = ("
            SELECT d.discipline_name, sd.department_id, sd.discipline_id, edit_history_grace_period_days 
            FROM student_department sd
            LEFT JOIN discipline d
            ON sd.discipline_id = d.discipline_id
            WHERE record_id='".$record_id."'
            ");
    $discipline_results=$GLOBALS['dbh']->Execute($discipline_sql); 
    $discipline_row=$discipline_results->FetchRow(); 

    //for history container related matters
    $container_mode = useContainers($history_row['student_number'],$history_row['department_id']);

    if ($container_mode)
    {
        $grace_period = $discipline_row['edit_history_grace_period_days'];
        $grace_end_date = date("Y-m-d",strtotime("+".$grace_period." Day",strtotime($history_row['end_date'])));

        $grace_entered_on = date("Y-m-d",strtotime("+".$grace_period." Day",strtotime($history_row['entered_on'])));

        $today = date("Y-m-d");

        if ((strtotime($today) > strtotime($grace_end_date)) && (strtotime($today) > strtotime($grace_entered_on))) 
        {
            $container_lock = 1;
        }
    }

    $notessql = ("
            SELECT contact_id, date_entered, notes, history_notes_id 
            FROM history_notes 
            WHERE history_id='".$history_id."'
            ");
    $notesresults=$GLOBALS['dbh']->Execute($notessql);

    $street_address_1=$employer_row['street_address_1'];
    $street_address_2=$employer_row['street_address_2'];
    $city=$employer_row['city'];
    $country=$employer_row['country_id'];
    $province=$employer_row['province_id'];
    $postal_code=$employer_row['postal_code'];

    $report_subject_sql = ("
            SELECT hrs.report_subject_id, hrs.report_subject_name 
            FROM history_report_subject AS hrs, department_history_report_subjects AS dhrs
            WHERE dhrs.department_id='" . $auth->department . "'
            AND dhrs.report_subject_id=hrs.report_subject_id
            ");
    $report_subject_results=$GLOBALS['dbh']->Execute($report_subject_sql); 

    $wt_status_sql = "SELECT history_status_name, history_status_id 
        FROM history_status";
    $wt_status_results=$GLOBALS['dbh']->Execute($wt_status_sql);

    $options_sql = "SELECT history_options_name, history_options_id 
        FROM history_options";
    $options_results = $GLOBALS['dbh']->Execute($options_sql);  

    if ($history_row['company_name']==NULL){
        $history_row['company_name']=$employer_row['company_name'];
    }

    //if ($history_row['company_department_id']<>NULL){
    //	$company_department_sql="Select department_name, street_address_1,street_address_2, city, provstate_id,country_id,postal_code from employer_department where employer_id=".$history_row['company_department_id']; 
    //	$company_department_results=$GLOBALS['dbh']->Execute($company_department_sql);
    //	$company_department_row=$company_department_results->FetchRow();

    //	$street_address_1=$company_department_row['street_address_1'];
    //	$street_address_2=$company_department_row['street_address_2'];
    //	$city=$company_department_row['city'];
    //	$country=$company_department_row['country_id'];
    //	$province=$company_department_row['provstate_id'];
    //	$postal_code=$employer_row['postal_code'];
    //}

    if ($province<>NULL){
        $province_sql= ("
                SELECT provstate_name 
                FROM provstate_list 
                WHERE provstate_id='".$province."'
                ");
        $province_results=$GLOBALS['dbh']->Execute($province_sql);
        $province_row=$province_results->FetchRow();
        $province=$province_row['provstate_name'];
    }

    if ($country<>NULL){
        $country_sql= ("
                SELECT country_name 
                FROM country_list 
                WHERE country_id='".$country."'
                ");
        $country_results=$GLOBALS['dbh']->Execute($country_sql);
        $country_row=$country_results->FetchRow();
        $country=$country_row['country_name'];
    }

    if ($student_row['department_id']<>NULL){
        $department_sql= ("
                SELECT department_name 
                FROM department 
                WHERE department_id='".$student_row['department_id']."'
                ");
        $department_results=$GLOBALS['dbh']->Execute($department_sql);
        $department_row=$department_results->FetchRow();
    }

    if ($history_row["company_province"]<>NULL){
        $provincecurrent=$history_row["company_province"];
    }else{
        $provincecurrent=0;
    }

    if ($history_row["work_term_province"]<>NULL){
        $homeprovince=$history_row["work_term_province"];
    }else{
        $homeprovince=0;
    }

    //show the rest of the form

    // Commence dynamic drop down menus

    $max_country_id=0;
    $country_array = array();

    /*
       We need to put blank values at the start of every pull down array
     */

    $max_sql = ("
            SELECT DISTINCT provstate_id
            FROM provstate_list
            ORDER BY provstate_id DESC
            ");
    $max_result = $GLOBALS['dbh']->Execute($max_sql);
    $max_row = $max_result->FetchRow();

    for ($i = 1; $i <= $max_row["provstate_id"]; $i++)
    {
        $prov_array[$i] = ("Array(\"NULL\"),");
    }

    $result = $GLOBALS['dbh']->Execute("
            SELECT c.country_id, provstate_id, provstate_name
            FROM country_list as c LEFT JOIN provstate_list as p ON c.country_id = p.country_id
            ORDER BY c.order_by
            ");

    while ($row = $result->FetchRow())
    {
        if ($row["provstate_id"] == "")
        {

            // If there's already a NULL value in this slot, don't put another one in.

            if ($prov_array[$row["country_id"]] == "Array(\"NULL\"),")
            {
                $str = "";
            }
            else
            {
                $str = "Array(\"NULL\"),";
                //$str = "Array(),";
            }

        }
        else
        {
            $str = "Array(\"" . $row["provstate_id"] . "\", \"" . $row["provstate_name"] . "\"),";
        }
        $prov_array[$row["country_id"]] .= $str;

        if (!in_array($row["country_id"], $country_array))
        {    
            $country_array[] =$row["country_id"];
        }
        if ($row["country_id"] > $max_country_id)
        {
            $max_country_id = $row["country_id"];
        }
    }
    sort($country_array);

    $str = "Array(";
    for($i=0;$i<=$max_country_id;$i++)
    {
        if ($prov_array[$i] == "")
        {
            $str .= "Array(\"NULL\"),";
            //$str .= "Array(),";
        }
        else
        {
            $str .= "Array(" . substr($prov_array[$i], 0, strlen($prov_array[$i])-1) . "),";
        }
    }
    $str = substr($str, 0, strlen($str)-1) . ")";
    $prov_state_array = $str;

    $max_prov_id = 0;


    // We need to put blank values at the start of every pull down array


    $max_sql = ("
            SELECT DISTINCT region_id
            FROM region_list
            ORDER BY region_id DESC
            ");
    $max_result = $GLOBALS['dbh']->Execute($max_sql);
    $max_row = $max_result->FetchRow();

    for ($i = 1; $i <= $max_row["region_id"]; $i++)
    {
        $region_array[$i] = ("Array(\"NULL\"),");
    }

    $result = $GLOBALS['dbh']->Execute("
            SELECT p.provstate_id, r.region_id, r.region_name
            FROM provstate_list as p 
            LEFT JOIN region_list as r 
            ON p.provstate_id = r.provstate_id
            ORDER BY p.provstate_id
            ");

    while ($row = $result->FetchRow())
    {
        if ($row["region_id"] == "")
        {
            // If there's already a NULL value in this slot, don't put another one in.

            if ($region_array[$row["provstate_id"]] == "Array(\"NULL\"),")
            {
                $str = "";
            }
            else
            {
                $str = "Array(\"NULL\"),";
                //$str = "Array(),";
            }
        }
        else
        {
            $str = "Array(\"" . $row["region_id"] . "\", \"" . $row["region_name"] . "\"),";
        }
        $region_array[$row["provstate_id"]] .= $str;

        if ($row["provstate_id"] > $max_prov_id)
        {
            $max_prov_id = $row["provstate_id"];
        }
    }

    $str = "Array(";
    for($i=0;$i<=$max_prov_id;$i++)
    {
        if ($region_array[$i] == "")
        {
            $str .= "Array(\"NULL\"),";
        }
        else
        {
            $str .= "Array(" . substr($region_array[$i], 0, strlen($region_array[$i]) - 1) . "),";
        }
    }
    $str = substr($str, 0, strlen($str)-1) . ")";
    $region_array = $str;

    $max_region_id = 0;
    $sql = ("
            SELECT region_id, city_name
            FROM region_list
            ORDER BY region_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        if ($row["city_name"] == "")
        {
            $str = "";
        }
        else
        {
            $str = $row["city_name"];
        }

        $city_array[$row["region_id"]] = $str;

        if ($row["region_id"] > $max_region_id)
        {
            $max_region_id = $row["region_id"];
        }
    }

    $str = "Array(";
    for ($i = 0; $i <= $max_region_id; $i++)
    {
        if ($city_array[$i] == "")
        {
            $str .= "\"\",";
        }
        else
        {
            $str .= "\"" . $city_array[$i] . "\",";
        }
    }

    $str = substr($str, 0, (strlen($str) - 1)) . ")";
    $city_array = $str;

    ?>
        <script type='text/javascript' language='javascript'>
        <!--javascript

        var city_array = new <?php echo($city_array); ?>;
    var region_array = new <?php echo $region_array?>;
    var prov_array = new <?php echo $prov_state_array?>;
    var cntry_array = new <?php echo $country_array?>;

    function update_prov()
    {
        index = document.myform.company_country[document.myform.company_country.selectedIndex].value;
        prov = prov_array[index];
        if (document.myform.company_country[document.myform.company_country.selectedIndex].value == "")
        {
            message = "Choose a Country";
            document.myform.company_province.options.length = 1;
            document.myform.company_province.options[0] = new Option(message, "");
            document.myform.company_province.selectedIndex = 0;
            document.myform.company_region.options.length = 1;
            document.myform.company_region.options[0] = new Option(message, "");
            document.myform.company_region.selectedIndex = 0;
        }

        else
        {
            if (prov.length==1 && prov[0] == "NULL")
            {
                document.myform.company_province.options.length = 1;
                document.myform.company_province.options[0] = new Option("N/A", "");
                document.myform.company_province.selectedIndex = 0;
                document.myform.company_region.options.length=1;
                document.myform.company_region.options[0] = new Option("N/A", "");
                document.myform.company_region.selectedIndex = 0;
            }

            else
            {
                document.myform.company_province.options.length =0;
                document.myform.company_province.options.length = prov.length;
                for($i=0;$i<prov.length;$i++)
                {
                    if (prov[$i] == "NULL")
                    {
                        document.myform.company_province.options[$i] = new Option(" ", "");
                    }
                    else
                    {
                        document.myform.company_province.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                    }
                }
                document.myform.company_province.selectedIndex = 0;

                index2=document.myform.company_province[document.myform.company_province.selectedIndex].value;
                region = region_array[index2];
                if (document.myform.company_province[document.myform.company_province.selectedIndex].value == "")
                {
                    if (document.myform.company_country[document.myform.company_country.selectedIndex].value == "")
                    {
                        message = "Choose a Country";
                    }
                    else
                    {
                        message = "Choose a Province/State";
                    }
                    document.myform.company_region.options.length = 1;
                    document.myform.company_region.options[0] = new Option(message, "");
                    document.myform.company_region.selectedIndex = 0;
                }
                else
                {
                    if (region.length==1 && region[0] == "NULL")
                    {
                        document.myform.company_region.options.length=1;
                        document.myform.company_region.options[0] = new Option("N/A", "");
                    }
                    else
                    {
                        document.myform.company_region.options.length=0;
                        document.myform.company_region.options.length=region.length;
                        for($i=0;$i<region.length;$i++)
                        {
                            document.myform.company_region.options[$i] = new Option(region[$i][1], region[$i][0]);
                        }
                    }
                }
            }
        }
    }

    function update_region()
    {
        document.myform.company_region.options.length=0;
        index2=document.myform.company_province[document.myform.company_province.selectedIndex].value;
        region = region_array[index2];
        if (document.myform.company_province[document.myform.company_province.selectedIndex].value == "")
        {
            if (document.myform.company_country[document.myform.company_country.selectedIndex].value == "")
            {
                message = "Choose a Country";
            }
            else
            {
                message = "Choose a Province/State";
            }

            document.myform.company_region.options.length = 1;
            document.myform.company_region.options[0] = new Option(message, "");
            document.myform.company_region.selectedIndex = 0;
        }

        else
        {
            if (region.length==1 && region[0] == "NULL")
            {
                document.myform.company_region.options.length=1;
                document.myform.company_region.options[0] = new Option("N/A", "");
            }

            else
            {
                document.myform.company_region.options.length = region.length;
                for($i=0;$i<region.length;$i++)
                {
                    if (region[$i] == "NULL")
                    {
                        document.myform.company_region.options[$i] = new Option(" ", "");
                    }
                    else
                    {
                        document.myform.company_region.options[$i] = new Option(region[$i][1], region[$i][0]);
                    }
                }
            }
            document.myform.company_region.selectedIndex = 0;
        }
    }

    function update_city()
    {
        index3 = document.myform.company_region[document.myform.company_region.selectedIndex].value;

        if (index3)
        {
            city = city_array[index3];
            document.myform.company_city.value = city;
        }
    }

    function update_work_term_prov()
    {
        index = document.myform.work_term_country[document.myform.work_term_country.selectedIndex].value;
        prov = prov_array[index];
        if (document.myform.work_term_country[document.myform.work_term_country.selectedIndex].value == "")
        {
            message = "Choose a Country";
            document.myform.work_term_province.options.length = 1;
            document.myform.work_term_province.options[0] = new Option(message, "");
            document.myform.work_term_province.selectedIndex = 0;
        }

        else
        {
            if (prov.length==1 && prov[0] == "NULL")
            {
                document.myform.work_term_province.options.length = 1;
                document.myform.work_term_province.options[0] = new Option("N/A", "");
                document.myform.work_term_province.selectedIndex = 0;
            }

            else
            {
                document.myform.work_term_province.options.length =0;
                document.myform.work_term_province.options.length = prov.length;
                for($i=0;$i<prov.length;$i++)
                {
                    if (prov[$i] == "NULL")
                    {
                        document.myform.work_term_province.options[$i] = new Option(" ", "");
                    }
                    else
                    {
                        document.myform.work_term_province.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                    }
                }
                document.myform.work_term_province.selectedIndex = 0;

                index2=document.myform.work_term_province[document.myform.work_term_province.selectedIndex].value;
                if (document.myform.work_term_province[document.myform.work_term_province.selectedIndex].value == "")
                {
                    if (document.myform.work_term_country[document.myform.work_term_country.selectedIndex].value == "")
                    {
                        message = "Choose a Country";
                    }
                    else
                    {
                        message = "Choose a Province/State";
                    }
                }
            }
        }
    }
    //-->
    </script>

        
        <form action="<?php echo $PHP_SELF."&amp;select=edithistory&amp;history_id=".$history_id?>" method="post" name="myform">
        <table id="editHistory"  class='row0' cellspacing=0 width=100% border=0>
        <tr>
        <td class='row1' align='center' colspan='2'><h2><b class='black'><?php echo $student_row['first_name']." ".$student_row['last_name']." (".$history_row['student_number'].") at ".$history_row['company_name']?></b></h2></td>
        </tr>

        <tr>
        <td class='row1' align='center' colspan='2'><h2><b class='black'><?php echo $history_row['term_name']." ".$history_row['year']?></b></h2></td>
        </tr>
        <tr>
        <td width=75%>
        <table cellspacing=1 width=100%>
        <tr>
        <td class="row1" width=100%><b class='black'>Employment Info</b></td>
        </tr>
        </table>
        <table cellspacing=0>
        <tr>
        <td class="row0"><b class='black'>Company Name:</b>&nbsp;&nbsp;</td><td><input type='hidden' name='company_name' value="<?php echo stripslashes($history_row['company_name']);?>" /><?php echo stripslashes($history_row['company_name']);?>&nbsp;&nbsp;<?php echo $changes['company_name'];?></td>
        </tr>
        <tr>
        <td class="row0"><b class='black'>Division Name:</b>&nbsp;&nbsp;</td><td><input type='hidden' name='department_name' value="<?php echo stripslashes($history_row['department_name']);?>" /><?php echo stripslashes($history_row['department_name']);?>&nbsp;&nbsp;<?php echo $changes['department_name'];?><input type='hidden' name='changed_company_division' value='0' />
</td>
                </tr>
        <tr>
        <td>&nbsp;</td>
        <td class="row0"><input type='button' onclick='popUpCompany("company_name", "myform");' value='Edit Company/Division Name' /></td>
        </tr>
        <tr>
        <td valign='top'><b class='black'>Work Site Address:</b>&nbsp;&nbsp;</td>
        <td class="row0"><input type='text' size=35 name='company_street_address_1' value="<?php echo $history_row['company_street_address_1']?>" /><?php echo $changes['company_street_address_1']?></td>
        </tr>
        <tr>
        <td>&nbsp;</td>
        <td><input type='text' size=35 name='company_street_address_2' value="<?php echo $history_row['company_street_address_2']?>" /><?php echo $changes['company_street_address_2']?><input type="button" name="btn" value="Retrieve Address from Employer Information" onclick="insertCompanyInfo()" /></td>
        </tr>
        <tr>
        <td><b class='black'>Country:&nbsp;&nbsp;</b></td>
        <td>
        <?php
        $country_sql = "SELECT country_id, country_name 
        FROM country_list 
        ORDER BY order_by";
    $countryresults=$GLOBALS['dbh']->Execute($country_sql);
    ?>
        <select name="company_country" onchange='update_prov()'>
        <option value=''>&nbsp;</option>
        <?php
        $selected = NULL;
    while ($countryrow=$countryresults->FetchRow()){
        if ($countryrow['country_id']==$history_row['company_country']){
            $selected = selected;
        }
        ?>

            <option value="<?php echo $countryrow["country_id"]?>" <?php echo $selected?>><?php echo $countryrow["country_name"]?></option> 
            <?php
       
            $selected=NULL;
    }
    ?>
        </select>
        <?php echo $changes['company_country']?>
        </td>
        </tr>
        <tr>
        <td><b class='black'>Province:&nbsp;</b></td>
        <td>
        <?php
        if ($history_row['company_country'] == USA){
            $country = USA;
        }else{
            $country = CANADA;
        }

    $province_sql = ("
            SELECT provstate_id, provstate_name 
            FROM provstate_list 
            WHERE country_id='".$country."'
            ORDER BY provstate_name
            "); 
        $provinceresults=$GLOBALS['dbh']->Execute($province_sql);

    ?>
        <select name="company_province" onchange='update_region()'>
        <option value=''>&nbsp;</option>
        <?php
        $selected = NULL;

    while ($provincerow=$provinceresults->FetchRow()){
        if ($provincerow['provstate_id']==$history_row['company_province']){
            $selected = selected;
        }
        ?>

            <option value="<?php echo $provincerow["provstate_id"]?>" <?php echo $selected?>><?php echo $provincerow["provstate_name"]?></option> 
            <?php
            $selected=NULL;
    }
    ?>
        </select>					
        <?php echo $changes['company_province']?>
        </td>
        </tr>
        <tr>
        <td><b class='black'>Region:&nbsp;&nbsp;</b></td>
        <td>
        <select name="company_region" onchange='update_city()'>
        <?php
        if (!$history_row['company_country'])
        {
            echo("<option value=''>Choose a Country</option>");

        }
    elseif (!$history_row['company_province'])
    {
        echo("<option value=''>Choose a Province/State</option>");
    }
        else
        {
            $sql = ("
                    SELECT DISTINCT region_id, region_name
                    FROM region_list
                    WHERE provstate_id='" . $history_row['company_province'] . "'
                    ORDER BY region_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount())
            {
                echo("<option value='' ");
                if (!$history_row['company_region_id'] || $history_row['company_region_id'] == '')
                {
                    echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["region_id"] . "' ");
                    if ($history_row['company_region_id'] == $row["region_id"])
                    {
                        echo("selected='selected'");
                    }
                    echo(">" . $row["region_name"] . "</option>");
                }
            }
            else
            {
                echo("<option value='' selected='selected'>N/A</option>");
            }
        }
    ?>
        </select>
        </td>
        <tr>
        <td><b class='black'>City:&nbsp;&nbsp;</b></td>
        <td><input type='text' size=35 name='company_city' value="<?php echo $history_row['company_city']?>" /><?php echo $changes['company_city']?></td>
        </tr>
        <tr>
        <td><b class='black'>Postal Code:&nbsp;&nbsp;</b></td>
        <td><input type='text' size=35 name='company_postal_code' value="<?php echo $history_row['company_postal_code']?>" /><?php echo $changes['company_postal_code']?></td>
        </tr>

        <tr>
        <td><b class='black'>Job Code:&nbsp;&nbsp;</b></td>
        <td>
        <?php 
        if ($history_row['job_code']) 
        {
            echo $history_row['job_code'];
        }
        else
        {
            echo "Own Job / Returning Job";
        }
    ?>
        </td>
        </tr>

        <tr>
        <td class="row0"><b class='black'>Supervisor:</b>&nbsp;&nbsp;</td><td><input type='text' name='supervisor_name' size=35 value="<?php echo $history_row['supervisor_name']?>" /><?php echo $changes['supervisor_name']?></td>
        </tr>
        <tr>
        <td class="row0"><b class='black'>Supervisor Title:</b>&nbsp;&nbsp;</td><td><input type='text' name="supervisor_position" size=35 value="<?php echo $history_row['supervisor_position']?>" /><?php echo $changes['supervisor_position']?></td>
        </tr>

        <tr>
        <td class="row0"><b class='black'>Supervisor's Phone Number:</b>&nbsp;&nbsp;</td><td><input type='text' size=35 name='supervisor_phone_number' value="<?php echo $history_row['supervisor_phone_number']?>" /><?php echo $changes['supervisor_phone_number']?></td>
        </tr>
        <tr>
        <td class="row0"><b class='black'>Supervisor's Cell Number:</b>&nbsp;&nbsp;</td><td><input type='text' size=35 name='supervisor_cellphone_number' value="<?php echo $history_row['supervisor_cellphone_number']?>" /><?php echo $changes['supervisor_cellphone_number']?></td>
        </tr>
        
        <tr>
        <td class="row0"><b class='black'>Supervisor's Fax Number:</b>&nbsp;&nbsp;</td><td><input type='text' size=35 name='supervisor_fax_number' value="<?php echo $history_row['supervisor_fax_number']?>" /><?php echo $changes['supervisor_fax_number']?></td>
        </tr>
        <tr>
        <td class="row0"><b class='black'>Supervisor's E-mail:</b>&nbsp;&nbsp;</td><td><input type='text' name='supervisor_email' size=35 value="<?php echo $history_row['supervisor_email']?>" /><?php echo $changes['supervisor_email']?></td>
        </tr>
        <tr>
        <td class="row0"><b class='black'>Salary:</b>&nbsp;&nbsp;</td><td><input type='text' size='10' name='salary' value="<?php echo $history_row['salary']?>" />
        <select name="salary_period_id">
        <?php
        $salary_period_sql = ("
                SELECT salary_period_id, salary_period_name
                FROM salary_period
                ORDER BY order_by
                ");
    $salary_results = $GLOBALS['dbh']->Execute($salary_period_sql);

    echo("<option value=''>&nbsp;</option>");
    while($salary_period_row = $salary_results->FetchRow())
    {
        $selected = NULL;
        if ($history_row['salary_period_id'] == $salary_period_row['salary_period_id'])
        {
            $selected = "selected='selected'";
        }

        echo("<option value='".$salary_period_row['salary_period_id']."' ".$selected.">".$salary_period_row['salary_period_name']."</option>");
    }

    ?>
        </select>
        <?php echo $changes['salary']." ".$changes['salary_period_id']; ?>
        </td>
        </tr>

        <tr>
        <td class="row0"><b class='black'>Hours per Week:</b>&nbsp;&nbsp;</td>
        <td>
        <?php
        if ($container_lock)
        {
            echo($history_row['work_term_hours_per_week']);
            echo("<input type='hidden' name='work_term_hours_per_week' value='".$history_row['work_term_hours_per_week']."' />");
        }
        else
        {
            echo("<input type='text' size='6' name='work_term_hours_per_week' value='".$history_row['work_term_hours_per_week']."' />");
            echo($changes['work_term_hours_per_week']);
        }
        ?>
        </td>
        </tr>
        <tr>
        <td class="row0"><b class='black'>Term:</b>&nbsp;&nbsp;</td>
        <td>
        <?php
        if ($container_mode)
        {
            echo(getTermName($history_row['term_id'])." ".$history_row['year']);
            echo("<input type='hidden' name='term' value='".$history_row['term_id']."' />");
            echo("<input type='hidden' name='year' value='".$history_row['year']."' />");
        }
        else
        {
            echo("<select name='term'>");
            $sqlterm = ("
                SELECT term_id,term_name,year_order 
                FROM term 
                ORDER BY year_order
                ");
            $sqlterm_result = $GLOBALS['dbh']->Execute($sqlterm);
            while ($sqltermrow = $sqlterm_result->FetchRow()) {
                if ($history_row['term_id'] == $sqltermrow['term_id']) {
                    echo("<option value='".$sqltermrow['term_id']."' selected='selected'>".$sqltermrow['term_name']."</option>");
                } else {
                    echo("<option value='".$sqltermrow['term_id']."'>".$sqltermrow['term_name']."</option>");
                }
            }
            echo("</select>");
            echo("<select name='year'>");
            $sqlyear = ("
                SELECT min(year) AS year
                FROM history
                WHERE year != '0'
                ");
            $sqlyear_result = $GLOBALS['dbh']->Execute($sqlyear);
            $year_row = $sqlyear_result->FetchRow();
            $end_year = date("Y") + 2;

            for ($i=$end_year;$i>=(int)$year_row['year'];$i--)
            {
                $selected="";
                if ($history_row['year']==$i){
                    $selected="selected='selected'";
                }
                echo("<option ". $selected ." value='". $i ."'>". $i ."</option>");
                $selected="";
            }
            echo("</select>");
            echo($changes['term_id']." ".$changes['year']);
        }
        ?>
        </td>
        </tr>
        <tr>
        <td>
        <b class="black">Start Date:</b>
        </td>
        <td>
        <?php

        if ($container_lock)
        {
            echo($history_row['start_date']);
            echo("<input type='hidden' name='start_date' value='".$history_row['start_date']."' />");
        }
        else
        {
            echo("<input type='text' name='start_date' size='10' value='".$history_row['start_date']."' />");
            echo(" ".popup('start_date','myform') . $changes['start_date']);
        }

        ?>
        </td>
        </tr>
        <tr>
        <td>
        <b class="black">End Date:</b>
        </td>
        <td>
        <?php

        if ($container_lock || $container_mode == CONTAINER_TERM)
        {
            echo($history_row['end_date']);
            echo("<input type='hidden' name='end_date' value='".$history_row['end_date']."' />");
        }
        else
        {
            echo("<input type='text' name='end_date' size='10' value='".$history_row['end_date']."' />");
            echo(" ".popup('end_date','myform') . $changes['end_date']);
        }
        
        ?>
        </td>
        </tr>
        <?php
        if ($container_mode == CONTAINER_TERM)
        {
            echo("<tr>");
            echo("<td width='$col1width'><b class='black'>Work Term Length:</b></td>");
            if ($container_lock)
            {
                echo("<td width='$col2width' align='left'>". $history_row["work_term_length"]." month".(($history_row['work_term_length']==1) ? "" : "s" . "</td>"));
                echo("<input type='hidden' name='work_term_length' value='".$history_row['work_term_length']."' />");
            }
            else
            {
                $length_sql = ("
                    SELECT max_num_workterms, history_containers_length_term
                    FROM discipline
                    WHERE discipline_id = '".$discipline_row['discipline_id']."'
                    ");
                $length_result = $GLOBALS['dbh']->Execute($length_sql);
                $length_row = $length_result->FetchRow();
                $max_work_term_length = $length_row['max_num_workterms'] * $length_row['history_containers_length_term'];

                echo("<td><select name='work_term_length'>");

                for ($i = 1; $i <= $max_work_term_length; $i++) 
                {
                    if ($history_row['work_term_length'] == $i)
                    {
                        $selected = "selected='selected'";
                    }
                    echo("<option value='" .$i. "' ".$selected. ">" . $i . ($i == 1 ? " month" : " months") ."</option>");
                    $selected = NULL;
                }
                echo("</select> ".$changes['work_term_length']."</td>");
            }
            echo("</tr>");
        }
        elseif ($container_mode == CONTAINER_HOURS)
        {
            $total_days = count_days($history_row["start_date"],$history_row["end_date"]);
            $hours_per_day = $history_row["work_term_hours_per_week"] / 7;
            $total_hours = round($total_days * $hours_per_day);

            echo("<tr><td width='$col1width'><b class='black'>Total Hours:</b></td>
                <td width='$col2width' align='left'>". $total_hours."");
            echo("</td></tr>");
        }

        ?>
        </table>
        </td>
        <td width='25%' valign='top'>
        <table width='100%' cellspacing='0'>
        <tr>
        <td class="row1" width=100%><b class="black">Photo</b></td>
        </tr>
        <tr>
        <td valign='middle'>
        <table>
        <tr>
        <?php
        if ($student_row["photo"]==NULL){
            ?>
                <td align='center' valign='middle' class="row0"><b class="black">No Photo Available</b></td>
                <?php
        }else{
            // hack to work around an IE image cache bug
            $num = rand(0,100);
            ?>
                <td valign='middle' align='center' class="row0">
                <img alt="<?php echo $student_row['first_name']." ".$student_row['last_name']?>" src="<?php print $PHP_SELF_MENU."?select=student_photo_viewer&amp;no_html=1&amp;record_id=".$record_id."&amp;num=".$num; ?>" /></td>
                <?php
        }
        ?>
        </tr>
        </table>
        </td>

        </tr>
        </table>
        </td>
        </tr>
        <tr>
        <td colspan='2'>
        <table width=100% cellspacing=1>
        <tr>
        <td class="row1"><b class="black">Student Information&nbsp;&nbsp;</b></td>
        </tr>
        </table>

        <table class='row0' width=60% cellspacing=1>
        <tr>
        <td><b class='black'>Work Term Number:</b>&nbsp;&nbsp;</td><td><?php echo $history_row['work_term_number']?></td>
        </tr>
        <tr>
        <td><b class='black'>Work Term Status:</b>&nbsp;&nbsp;</td><td>
        <select name="wt_status">
        <option value="NULL">&nbsp;</option>
        <?php
        $selected=NULL;
    while ($wt_status_row=$wt_status_results->FetchRow()){ 
        if ($wt_status_row['history_status_id']==$history_row['wt_status']){
            $selected="selected='selected'";
        }
        ?>
            <option <?php echo $selected?>  value="<?php echo $wt_status_row['history_status_id']?>"><?php echo $wt_status_row['history_status_name']?></option>
            <?php
            $selected="NULL";
    }
    ?>
        </select>
        <?php echo $changes['history_status']?>
        </td>
        </tr>
        <tr>
        <td><b class='black'>Discipline:</b>&nbsp;&nbsp;</td><td><?php echo $discipline_row['discipline_name']?></td>
        </tr>
        <tr>
        <td class="row0"><b class='black'>Student Position:</b>&nbsp;&nbsp;</td><td><input name='job_title' type='text' size='35' value="<?php echo $history_row['job_title']?>" /><?php echo $changes['job_title']?></td>
        </tr>
        <?php			
        /*
           <tr>
           <td>
           <b class='black'>Option:</b> 
           </td>
           <td>
           <select name="options">
           <option value="0">&nbsp;</option>
           <?php
           $selected=NULL;
           while ($options_row=$options_results->FetchRow()){ 
           if ($options_row['history_options_id']==$history_row['options']){
           $selected="selected";
           }
           ?>
           <option <?php echo $selected?> value="<?php echo $options_row['history_options_id']?>"><?php echo $options_row['history_options_name']?></option>
           <?php
           $selected=NULL;
           }
           ?>
           </select>
           <?php echo $changes['history_options']?>
           </td>
           </tr>
         */
        ?>
        <tr>
        <td><b class='black'>Work Term Home Address:</b></td> 
        <td><input type='text' size='35' name='work_term_street_address_1' value="<?php echo $history_row['work_term_street_address_1']?>" /><?php echo $changes['work_term_street_address_1']?></td>
        </tr>
        <tr>
        <td>&nbsp;</td>
        <td><input type='text' size='35' name='work_term_street_address_2' value="<?php echo $history_row['work_term_street_address_2']?>" /><?php echo $changes['work_term_street_address_2']?></td>
        </tr>
        <tr>
        <td><b class='black'>Country:</b></td>
        <td>
        <?php
        $country_sql = "SELECT country_id, country_name 
        FROM country_list 
        ORDER BY order_by";
    $countryresults=$GLOBALS['dbh']->Execute($country_sql);
    ?>
        <select name="work_term_country" onchange='update_work_term_prov()'>
        <option value=''>&nbsp;</option>
        <?php
        $selected = NULL;
    while ($countryrow=$countryresults->FetchRow()){
        if ($countryrow['country_id']==$history_row['work_term_country']){
            $selected ="selected='selected'";
        }
        ?>

            <option value="<?php echo $countryrow["country_id"]?>" <?php echo $selected?>><?php echo $countryrow["country_name"]?></option> 
            <?php
            $selected=NULL;
    }
    ?>
        </select>
        <?php echo $changes['work_term_country']?>
        </td>
        </tr>
        <tr>
        <td><b class='black'>Province:</b></td>

        <td>
        <?php
        if ($history_row['work_term_country'] == USA){
            $country = USA;
        }else{
            $country = CANADA;
        }

    $province_sql = "SELECT provstate_id, provstate_name 
        FROM provstate_list 
        WHERE country_id=".$country." 
        ORDER BY provstate_name"; 
        $provinceresults=$GLOBALS['dbh']->Execute($province_sql);

    ?>
        <select name="work_term_province">
        <option value=''>&nbsp;</option>
        <?php
        $selected = NULL;

    while ($provincerow=$provinceresults->FetchRow()){
        if ($provincerow['provstate_id']==$history_row['work_term_province']){
            $selected = "selected='selected'";
        }
        ?>

            <option value="<?php echo $provincerow["provstate_id"]?>" <?php echo $selected?>><?php echo $provincerow["provstate_name"]?></option> 
            <?php
            $selected=NULL;
    }
    ?>
        </select>					
        <?php echo $changes['work_term_province']?>
        </td>
        </tr>
        <tr>
        <td><b class='black'>City:</b></td>
        <td><input type='text' size='35' name='work_term_city' value="<?php echo $history_row['work_term_city']?>" /><?php echo $changes['work_term_city']?></td>
        </tr>
        <tr>
        <td><b class='black'>Postal Code:</b></td>
        <td><input type='text' size='35' name='work_term_postal_code' value="<?php echo $history_row['work_term_postal_code']?>" /><?php echo $changes['work_term_postal_code']?></td>
        </tr>

        <tr>
        <td><b class='black'>Work Term Home Phone Number:</b>&nbsp;&nbsp;</td><td><input type='text' size='35' name='work_term_home_phone' value="<?php echo $history_row['work_term_home_phone']?>" /><?php echo $changes['work_term_home_phone']?></td>
        </tr>
        <tr>
        <td><b class='black'>Student Work Number:</b>&nbsp;&nbsp;</td><td><input type='text' size='35' name='student_work_phone' value="<?php echo $history_row['student_work_phone']?>" /><?php echo $changes['student_work_phone']?></td>
        </tr>
        <tr>
        <td><b class='black'>Student Work E-mail:</b>&nbsp;&nbsp;</td><td><input type='text' size='35' name='work_email' value="<?php echo $history_row['work_email']?>" /><?php echo $changes['work_email']?></td>
        </tr>
        </table>
        </td>
        </tr>
        <tr>
        <td colspan='2'>
        <table width=100% cellspacing=1>
        <tr>
        <td width="20%" class='row1' colspan='2'><b class='black'>Work Term Visit Information</b></td>
        </tr>
        <tr>
        <td width="20%"><b class='black'>Student Work Term Visit On:</b>&nbsp;&nbsp;</td><td align=left><input type="text" size=35 name="site_visit_date" value="<?php echo $history_row['site_visit_date']?>" /><?php echo popup('site_visit_date','myform')?><?php echo $changes['site_visit_date']?></td>
        </tr>
        <tr>
        <td width="20%"><b class='black'>Student Work Term Visit by:</b>&nbsp;&nbsp;</td>
        <td align=left>
        <select name="site_visit_by">
        <option value="">&nbsp;</option>
        <?php
        $check_flag = 0; 
    $contact_sql = "SELECT c.first_name, c.last_name, ci.contact_id 
        FROM contact_internal AS ci, contact AS c 
        WHERE ci.site_visit_list='1' AND c.contact_id=ci.contact_id 
        AND ci.department_id = '".$history_row['department_id'] ."' 
        ORDER BY c.last_name,c.first_name"; 
        $results=$GLOBALS['dbh']->Execute($contact_sql);	
    while ($contact_row=$results->FetchRow()){
        $selected = "";
        if ($history_row['site_visit_by']==$contact_row['contact_id']){
            $check_flag = 1; 
            $selected = "selected ='selected'";
        }

        echo "<option ".$selected."value='".$contact_row['contact_id']."'>".$contact_row['first_name']." "
            .$contact_row['last_name']."</option>";
    }

    echo "<option value='-1'>----------------------</option>";

    if ($history_row['site_visit_by']==SITE_VISIT_FACULTY)
    {
        echo "<option value='".SITE_VISIT_FACULTY."' selected='selected'>Faculty</option>";
    }
    else
    {
        echo "<option value='".SITE_VISIT_FACULTY."'>Faculty</option>";
    }

    if ($history_row['site_visit_by']==SITE_VISIT_OTHER)
    {
        echo "<option value='".SITE_VISIT_OTHER."'selected='selected'>Other</option>";
    }
    else
    {
        echo "<option value='".SITE_VISIT_OTHER."'>Other</option>";
    }
    echo "<option value='-1'>----------------------</option>";

    $contact_sql = "SELECT c.first_name, c.last_name, ci.contact_id, d.department_code 
        FROM contact_internal AS ci 
        LEFT JOIN contact c ON c.contact_id=ci.contact_id 
        LEFT JOIN department d ON d.department_id = ci.department_id 
        WHERE ci.site_visit_list='1' 
        AND ci.department_id != '". $history_row['department_id'] ."'
        ORDER BY d.department_code, c.last_name,c.first_name";

    $results=$GLOBALS['dbh']->Execute($contact_sql);
    while ($contact_row=$results->FetchRow()){
        $selected = "";
        if ($history_row['site_visit_by']==$contact_row['contact_id']){
            $check_flag = 1; 
            $selected = "selected='selected' ";
        }

        echo "<option ".$selected."value='".$contact_row['contact_id']."'>".$contact_row['first_name']." "
            .$contact_row['last_name']." [".$contact_row['department_code']."]</option>";
    }

    if($check_flag == 0 && $history_row['site_visit_by'] 
            && $history_row['site_visit_by'] != SITE_VISIT_FACULTY 
            && $history_row['site_visit_by'] !=SITE_VISIT_OTHER )
    {
        echo "<option>--Not Flagged--</option>";
        $contact_name = getContactName($history_row['site_visit_by']);
        echo "<option selected='selected' value='".$history_row['site_visit_by']."'>".$contact_name["first_name"]." "
            .$contact_name["last_name"]."</option>";
    }

    ?>
        </select>
        <?php echo $changes['site_visit_by']?>
        </td>
        </tr>
        <tr>
        <td width="20%"><b class='black'>Student Work Term Visit Type:</b>&nbsp;&nbsp;</td><td align=left>
        <select name="site_visit_type_id">
        <option value="">&nbsp;</option>
        <?php
        $site_visit_type_sql = "SELECT site_visit_type_id, description 
        FROM site_visit_type";
    $results = $GLOBALS['dbh']->Execute($site_visit_type_sql);
    while ($site_visit_type_row=$results->FetchRow()){
        $selected=NULL;
        if ($history_row['site_visit_type_id']==$site_visit_type_row['site_visit_type_id']){
            $selected="selected='selected'";
        }
        ?>
            <option <?php echo $selected?> value="<?php echo $site_visit_type_row['site_visit_type_id']?>"><?php echo $site_visit_type_row['description']?></option>
            <?php
    }
    ?>
        </select>
        <?php echo $changes['site_visit_type_id']?>
        </td>
        </tr>
        <tr>
        <td width="20%"><b class='black'>Supervisor Work Term Visit On:</b>&nbsp;&nbsp;</td><td align=left><input type="text" size=35 name="site_visit_date_supervisor" value="<?php echo $history_row['site_visit_date_supervisor']?>" /><?php echo popup('site_visit_date_supervisor','myform')?><?php echo $changes['site_visit_date_supervisor']?></td> 
        </tr>

        <tr>
        <td width="20%"><b class='black'>Supervisor Work Term Visit by:</b>&nbsp;&nbsp;</td>
        <td align=left>
        <select name="site_visit_by_supervisor">
        <option value="">&nbsp;</option>
        <?php

        $check_flag = 0;
    $contact_sql = "SELECT c.first_name, c.last_name, ci.contact_id
        FROM contact_internal AS ci, contact AS c
        WHERE ci.site_visit_list='1' AND c.contact_id=ci.contact_id
        AND ci.department_id = '". $history_row['department_id'] ."'
        ORDER BY c.last_name,c.first_name";
    $results=$GLOBALS['dbh']->Execute($contact_sql);
    while ($contact_row=$results->FetchRow()){
        $selected = "";
        if ($history_row['site_visit_by_supervisor']==$contact_row['contact_id']){
            $check_flag = 1;
            $selected = "selected ='selected'";
        }

        echo "<option ".$selected."value='".$contact_row['contact_id']."'>".$contact_row['first_name']." "
            .$contact_row['last_name']."</option>";
    }

    echo "<option value='-1'>----------------------</option>";

    if ($history_row['site_visit_by_supervisor']==SITE_VISIT_FACULTY)
    {
        echo "<option value='".SITE_VISIT_FACULTY."' selected='selected'>Faculty</option>";
    }
    else
    {
        echo "<option value='".SITE_VISIT_FACULTY."'>Faculty</option>";
    }

    if ($history_row['site_visit_by_supervisor']==SITE_VISIT_OTHER)
    {
        echo "<option value='".SITE_VISIT_OTHER."'selected>Other</option>";
    }
    else
    {
        echo "<option value='".SITE_VISIT_OTHER."'>Other</option>";
    }
    echo "<option value='-1'>----------------------</option>";

    $contact_sql = "SELECT c.first_name, c.last_name, ci.contact_id, d.department_code
        FROM contact_internal AS ci
        LEFT JOIN contact c ON c.contact_id=ci.contact_id
        LEFT JOIN department d ON d.department_id = ci.department_id
        WHERE ci.site_visit_list='1'
        AND ci.department_id != '". $history_row['department_id'] ."'
        ORDER BY d.department_code, c.last_name,c.first_name";

    $results=$GLOBALS['dbh']->Execute($contact_sql);
    while ($contact_row=$results->FetchRow()){
        $selected = "";
        if ($history_row['site_visit_by_supervisor']==$contact_row['contact_id']){
            $check_flag = 1;
            $selected = "selected ='selected'";
        }

        echo "<option ".$selected."value='".$contact_row['contact_id']."'>".$contact_row['first_name']." "
            .$contact_row['last_name']." [".$contact_row['department_code']."] </option>";
    }

    if($check_flag == 0 && $history_row['site_visit_by_supervisor'] 
            && $history_row['site_visit_by_supervisor'] != SITE_VISIT_FACULTY
            && $history_row['site_visit_by_supervisor'] !=SITE_VISIT_OTHER )
    {
        echo "<option>--Not Flagged--</option>";
        $contact_name = getContactName($history_row['site_visit_by_supervisor']);
        echo "<option selected='selected' value='".$history_row['site_visit_by_supervisor']."'>".$contact_name["first_name"]." "
            .$contact_name["last_name"]."</option>";

    }
    ?>
        </select>
        <?php echo $changes['site_visit_by_supervisor']?>
        </td>
        </tr>
        <tr>
        <td width="20%"><b class='black'>Supervisor Work Term Visit Type:</b>&nbsp;&nbsp;</td><td align=left>
        <select name="site_visit_type_id_supervisor">
        <option value="">&nbsp;</option>
        <?php
        $site_visit_type_sql = "SELECT site_visit_type_id, description 
        FROM site_visit_type";
    $results = $GLOBALS['dbh']->Execute($site_visit_type_sql);
    while ($site_visit_type_row=$results->FetchRow()){
        $selected=NULL;
        if ($history_row['site_visit_type_id_supervisor']==$site_visit_type_row['site_visit_type_id']){
            $selected="selected";
        }
        ?>
            <option <?php echo $selected?> value="<?php echo $site_visit_type_row['site_visit_type_id']?>"><?php echo $site_visit_type_row['description']?></option>
            <?php
    }
    ?>
        </select>
        <?php echo $changes['site_visit_type_id_supervisor']?>
        </td>
        </tr>



        <tr>
        <td width="20%"><b class='black'>Work Term Visit Notes:</b>&nbsp;&nbsp;</td><td align=left><textarea name="work_site_visit_notes" cols="80" rows="4"><?php echo $history_row['work_site_visit_notes']?></textarea><?php echo $changes['work_site_visit_notes']?></td>
        </tr>
        </table>
        </td>
        </tr>
        <tr>
        <td colspan='2'>
        <table width=100% cellspacing=1>
        <tr>
        <td class='row1' colspan="2"><b class='black'>Work Term Reports</b></td>
        </tr>
        <tr>
        <td width="20%"><b class='black'>Title:</b>&nbsp;&nbsp;</td><td><input type="text" name="report_title" size=70 value="<?php echo $history_row['report_title']?>" /><?php echo $changes['report_title']?></td>
        </tr>
        <tr>
        <td width="20%"><b class='black'>Subject:</b>&nbsp;&nbsp;</td>
        <td>
        <select name="report_subject">
        <option value="0">&nbsp;</option>
        <?php
        $selected = NULL;
    while ($report_subject_row=$report_subject_results->FetchRow()){ 
        if ($history_row['report_subject']==$report_subject_row['report_subject_id']){
            $selected = "selected='selected'";
        }
        ?>
            <option <?php echo $selected?> value="<?php echo $report_subject_row['report_subject_id']?>"><?php echo $report_subject_row['report_subject_name']?></option>

            <?php
            $selected=NULL;
    }
    ?>
        </select>
        <?php echo $changes['report_subject']?>
        </td>
        </tr>
        <tr>
        <td width="20%"><b class='black'>Marker:</b>&nbsp;&nbsp;</td><td><input size=35 name="report_marker" type="text" value="<?php echo $history_row['report_marker']?>" /><?php echo $changes['report_marker']?></td>
        </tr>
        <tr>
        <td width="20%"><b class='black'>Report Code:</b>&nbsp;&nbsp;</td><td><input size=35 name="report_code" type="text" value="<?php echo $history_row['report_code']?>" /><?php echo $changes['report_code']?></td>
        </tr>
        </table>
        </td>
        </tr>
        </table>
        <table width=100% cellspacing=1>
        <tr>
        <td colspan="6" class="row1"><b class="black">Flags</b></td>
        </tr>
        <?php
        $count = 1;
        $arrFlags=history_flags($history_id,$discipline_row['department_id']);
        foreach ($arrFlags as $id => $flag){
            if ($count%3==1) echo "<tr>";
            if ($flag['checked']){
                $checked = " checked='checked'";
                $flags[$id] = "True";
            } else { 
                $checked = "";
            }
            
            print "<td width=3% class='row0'><input type='checkbox'".$checked." name='history_flags[".$id."]' value='True' /></td><td width=30% class='row0'>".$flag['description']."</td>";
            if ($count++%3==0){
                echo "</tr>";
            }
        }
        ?>
            <tr>
        <td colspan='6'>
        <?php echo $changes['flags']?>	
        </td>
        </tr>

        </table>
        <table width=100%>
        <tr>
        <td colspan="2" class="row1"><b class="black">Notes</b></td>
        </tr>
        <tr>
        <td class="row0"><textarea name="notes" cols="80" rows="4"></textarea></td><td class='row0'><center><b class='black'>Enter new note here</b></center>
        </tr>
        <?php
        if ($fromPage == "employer info")
        {
            echo("<input type='hidden' name='employer_id' value='$employer_id' />
                    <input type='hidden' name='com_dep' value='$com_dep' />
                    <input type='hidden' name='year' value='$year' />
                    <input type='hidden' name='term_id' value='$term_id' />
                    <input type='hidden' name='department_id' value='" . $department_id . "' />
                    <input type='hidden' name='departments' value='" . $departments . "' />
                    <input type='hidden' name='fromPage' value='employer info' />");
        }
    $count=0;
    while($rstNotes=$notesresults->FetchRow()){
        echo "<tr><td class='row0'>";
        $contact_id_row=getContactInfoByID($rstNotes['contact_id']);
        $contact_name=$contact_id_row['first_name']." ".$contact_id_row['last_name'];
        $notes_output .= "<b class='blue'>".$contact_name."</b><br /><b class='blue'>".$rstNotes['date_entered']."</b><br /><p>";
        $temp=explode("\n",$rstNotes["notes"]);
        $notes_output .= implode("<br />",$temp);
        $notes_output .= "<br />";
        echo $notes_output;
        echo "</td><td class='row0'><center>Delete Note <input type='checkbox' name='oldnotes[".$count."]' value='".$rstNotes['history_notes_id']."' /></center></td></tr>";
        $notes_output=NULL;
        $count=$count+1;
    }
    //class definition for the concurrent editing checking
    $editHistory=NULL;
    $editHistory = new editHistory('');
    $editHistory->setVariables(
            $history_row['term_id'],$history_row['year'],$history_row['student_number'],$history_row['company_name']
            ,$history_row['department_name'],$history_row['company_street_address_1']
            ,$history_row['company_street_address_2'],$history_row['company_street_address_3'],$history_row['company_city']
            ,$history_row['company_province'],$history_row['company_country'],$history_row['company_region_id']
            ,$history_row['company_postal_code'],$history_row['supervisor_name'],$history_row['supervisor_fax_number']
            ,$history_row['supervisor_phone_number'],$history_row['supervisor_cellphone_number'],$history_row['supervisor_email'],$history_row['supervisor_position']
            ,$history_row['salary'],$history_row['salary_period_id'],$history_row['work_term_hours_per_week']
            ,$history_row['start_date'], $history_row['end_date'], $history_row['work_term_length'], $history_row['wt_status']
            ,$history_row['job_title'],$history_row['options'],$history_row['work_term_street_address_1']
            ,$history_row['work_term_street_address_2'],$history_row['work_term_street_address_3'],$history_row['work_term_city']
            ,$history_row['work_term_postal_code'],$history_row['work_term_province'],$history_row['work_term_country']
            ,$history_row['work_term_home_phone'],$history_row['student_work_phone'],$history_row['work_email']
            ,$history_row['site_visit_date_supervisor'],$history_row['site_visit_date'],$history_row['site_visit_type_id']
            ,$history_row['site_visit_type_id_supervisor'],$history_row['site_visit_by'],$history_row['site_visit_by_supervisor']
            ,$history_row['work_site_visit_notes'],$history_row['report_title'],$history_row['report_code'],$history_row['report_subject']
            ,$history_row['report_marker'],$history_row['student_number'],$flags
            );
    ?>
        </table>
        <input type='hidden' name='searchHistory' value='<?php echo $searchHistory?>' />
        <input type='hidden' name='history_list' value='<?php echo $history_list?>' />
        <input type='hidden' name='flag' VALUE ='<?php echo $flag?>' />
        <?php

        if ($fromPage == 'simple search')
        {
            echo("<input type='hidden' name='fromPage' VALUE ='simple search' />");
        }
    ?>
        <input type='hidden' name='editHistory' value='<?php echo packObject($editHistory)?>' />
        <?php

        // For history containers. If a critical field like start or end date is modified, we have to recalculate 
        // our work terms and potentially generate new history records. 
        if ($container_change_flag) {
            echo("<input type='hidden' name='container_change_flag' value='".$container_change_flag."' />");
        }
        ?>
        <center><input type="submit" name="btnsubmit" value="Save Changes" /></center>
        </form>
        <script type='text/javascript' language='javascript'>
        <!--javascript
        function change_country(menu_name,country_id,target_province,target_region)
        {
            for (var i=0;i < document.myform.elements.length;i++){
                if (document.myform.elements[i].name==menu_name){
                    theindex=i;
                    break;
                }
            }
            for (var i=0;i < document.myform.elements[theindex].options.length;i++){
                if (document.myform.elements[theindex].options[i].value == country_id){
                    select=i;
                    break;
                }
            }
            document.myform.elements[theindex].options.selectedIndex=select;
            change_prov("company_province",country_id,target_province,target_region);
        }

    function change_prov(menu_name,country_id,target_province,target_region)
    {
        var theindex;

        for (var i = 0; i < document.myform.elements.length; i++)
        {
            if (document.myform.elements[i].name == menu_name){
                theindex=i;
                break;
            }
        }

        var list_length = prov_array[country_id].length;
        document.myform.elements[theindex].options.length = prov_array[country_id].length;

        if (list_length == 1)
        {
            document.myform.elements[theindex].options[0]=new Option("N/A","");
            document.myform.elements[theindex].options[0].selected = true;
        }
        else
        {
            document.myform.elements[theindex].options[0] = new Option("","");
            for (var i = 1; i < prov_array[country_id].length; i++)
            {

                document.myform.elements[theindex].options[i] = new Option(prov_array[country_id][i][1],prov_array[country_id][i][0]);
                if (target_province == prov_array[country_id][i][0])
                {
                    document.myform.elements[theindex].options[i].selected = true;
                }
            }
        }
        change_region("company_region",target_province,target_region);
    }

    function change_region(menu_name,provstate_id,target_region)
    {
        var theindex;

        for (var i = 0; i < document.myform.elements.length; i++)
        {
            if (document.myform.elements[i].name == menu_name){
                theindex=i;
                break;
            }
        }

        var list_length = region_array[provstate_id].length;
        document.myform.elements[theindex].options.length = region_array[provstate_id].length;

        if (list_length == 1)
        {
            document.myform.elements[theindex].options[0]=new Option("N/A","");
            document.myform.elements[theindex].options[0].selected = true;

        }
        else
        {
            document.myform.elements[theindex].options[0] = new Option("","");

            for (var i = 1; i < region_array[provstate_id].length; i++)
            {
                document.myform.elements[theindex].options[i]=new Option(region_array[provstate_id][i][1],region_array[provstate_id][i][0]);
                if (target_region == region_array[provstate_id][i][0])
                {
                    document.myform.elements[theindex].options[i].selected = true;
                }
            }
        }
    }


    function insertCompanyInfo(){
        <?php
            if ($history_row['company_department_id'])
            {
                $sql = ("
                        SELECT location_info,street_address_1,street_address_2,provstate_id,country_id,postal_code,city,region_id
                        FROM employer_department 
                        WHERE department_id='".$history_row['company_department_id']."'
                        ");
                $results = $GLOBALS['dbh']->Execute($sql);
                $row=$results->FetchRow();

                if ($row['location_info']=="Use Company")
                {
                    $sql = ("
                            SELECT street_address_1,street_address_2,provstate_id,country_id,postal_code,city,region_id
                            FROM employer_company 
                            WHERE employer_id='".$history_row['employer_id']."'
                            ");
                    $results = $GLOBALS['dbh']->Execute($sql);
                    $row=$results->FetchRow();
                }
            }
        elseif ($history_row['employer_id'])
        {
            $sql = ("
                    SELECT street_address_1,street_address_2,provstate_id,country_id,postal_code,city,region_id
                    FROM employer_company 
                    WHERE employer_id='".$history_row['employer_id']."'
                    ");
            $results = $GLOBALS['dbh']->Execute($sql);
            $row=$results->FetchRow();	
        }
        ?>
            document.myform.company_street_address_1.value="<?php echo $row['street_address_1']?>";
        document.myform.company_street_address_2.value="<?php echo $row['street_address_2']?>";
        document.myform.company_postal_code.value="<?php echo $row['postal_code']?>";
        document.myform.company_city.value="<?php echo $row['city']?>";
        <?php
            //:Commented out by Chan, silly piece of logic below.
            //if (($row['country_id']) && ($row['provstate_id']<>NULL) || $row['provstate_id']){
            if ($row['country_id'] || $row['provstate_id'])
            {
                ?>
                    change_country("company_country",<?php echo $row['country_id']?>,<?php echo $row['provstate_id']?>,<?php echo $row['region_id']?>);
                <?php
            }
        ?>
    }

    function popUpCompany(slotName, formName)
    {
        var str = "document." + formName + ".elements";

        var j = eval(str);
        for (var i = 0; i < j.length; i++)
        {
            var e = j[i];
            if (e.name == slotName)
            {
                var slot = i;
                break;
            }
        }

        window.open("mamook.php?select=company_department_choose_4&no_headers=1&company_slot="+slot+"&parentFormName="+formName+"&autosubmit=true", "CompanyChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes,width=285,height=175");
    }
    //-->
    </script>
        <?php
        //}
        ?>
