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
 | Filename: edit_student_form.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This is the main form loaded up by edit_student.inc.  It        |
 | displays all of the student's inputs to the user, and collects any changes   |
 | they wish to make.                                                           |
 +------------------------------------------------------------------------------+

*/
// We need to set up arrays for our javascript
$max_country_id=0;
$country_array = array();

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
        FROM provstate_list as p LEFT JOIN region_list as r ON p.provstate_id = r.provstate_id
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

// We need to put blank values at the start of every pull down array

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

$result = $GLOBALS['dbh']->Execute("SELECT c.country_id, provstate_id, provstate_name
        FROM country_list as c LEFT JOIN provstate_list as p ON c.country_id = p.country_id
        ORDER BY c.order_by");
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
    }
    else
    {
        $str .= "Array(" . substr($prov_array[$i], 0, strlen($prov_array[$i])-1) . "),";
    }
}
$str = substr($str, 0, strlen($str)-1) . ")";
$prov_state_array = $str;

$max_prov_id = 0;

// End of setting up the arrays.
?>

<script type='text/javascript' language='javascript'>
<!--JAVASCRIPT (hide from old browsers)
var prov_array = new <?php echo($prov_state_array); ?>;
var cntry_array = new <?php echo($country_array); ?>;
var region_array = new <?php echo $region_array?>;

function update_prov(current_or_permanent)
{
    if (current_or_permanent == "current")
    {
        country_field = document.myform.country_current;
        province_field = document.myform.province_current;
        region_field = document.myform.region_current;
        update_region_current();
    }
    else
    {
        country_field = document.myform.country_permanent;
        province_field = document.myform.province_permanent;
        region_field = document.myform.region_permanent;
        update_region_permanent();
    }

    index = country_field[country_field.selectedIndex].value;
    prov = prov_array[index];
    if (country_field[country_field.selectedIndex].value == "")
    {
        message = "Choose a Country";
        province_field.options.length = 1;
        province_field.options[0] = new Option(message, "");
        province_field.selectedIndex = 0;

        message2 = "Choose a Country"; 
        region_field.options.length = 1;
        region_field.options[0] = new Option(message2, "");
        region_field.selectedIndex = 0;
    }
    else
    {
        if (prov.length==1 && prov[0] == "NULL")
        {
            province_field.options.length = 1;
            province_field.options[0] = new Option("N/A", "");
            province_field.selectedIndex = 0;

        }
        else
        {
            province_field.options.length =0;
            province_field.options.length = prov.length;
            for($i=0;$i<prov.length;$i++)
            {
                if (prov[$i] == "NULL")
                {
                    province_field.options[$i] = new Option(" ", "");
                }
                else
                {
                    province_field.options[$i] = new Option(prov[$i][1], prov[$i][0]);
                }
            }
            province_field.selectedIndex = 0;
        }
    }
}

function update_region_current()
{
    document.myform.region_current.options.length=0;
    index2=document.myform.province_current[document.myform.province_current.selectedIndex].value;
    region = region_array[index2];
    
    
    if (document.myform.province_current[document.myform.province_current.selectedIndex].value == "")
    {
        if (document.myform.country_current[document.myform.country_current.selectedIndex].value == "")
        {
            message = "Choose a Country";
        }
        else
        {
            message = "Choose a Province/State";
        }

        document.myform.region_current.options.length = 1;
        document.myform.region_current.options[0] = new Option(message, "");
        document.myform.region_current.selectedIndex = 0;
    }

    else
    {
        if (region.length==1 && region[0] == "NULL")
        {
            document.myform.region_current.options.length=1;
            document.myform.region_current.options[0] = new Option("N/A", "");
        }

        else
        {
            document.myform.region_current.options.length = region.length;
            for($i=0;$i<region.length;$i++)
            {
                if (region[$i] == "NULL")
                {
                    document.myform.region_current.options[$i] = new Option(" ", "");
                }
                else
                {
                    document.myform.region_current.options[$i] = new Option(region[$i][1], region[$i][0]);
                }
            }
        }
        document.myform.region_current.selectedIndex = 0;
    }
}

function update_region_permanent()
{
    document.myform.region_permanent.options.length=0;
    index2=document.myform.province_permanent[document.myform.province_permanent.selectedIndex].value;
    region = region_array[index2];


    if (document.myform.province_permanent[document.myform.province_permanent.selectedIndex].value == "")
    {
        if (document.myform.country_permanent[document.myform.country_permanent.selectedIndex].value == "")
        {
            message = "Choose a Country";
        }
        else
        {
            message = "Choose a Province/State";
        }

        document.myform.region_permanent.options.length = 1;
        document.myform.region_permanent.options[0] = new Option(message, "");
        document.myform.region_permanent.selectedIndex = 0;
    }

    else
    {
        if (region.length==1 && region[0] == "NULL")
        {
            document.myform.region_permanent.options.length=1;
            document.myform.region_permanent.options[0] = new Option("N/A", "");
        }

        else
        {
            document.myform.region_permanent.options.length = region.length;
            for($i=0;$i<region.length;$i++)
            {
                if (region[$i] == "NULL")
                {
                    document.myform.region_permanent.options[$i] = new Option(" ", "");
                }
                else
                {
                    document.myform.region_permanent.options[$i] = new Option(region[$i][1], region[$i][0]);
                }
            }
        }
        document.myform.region_permanent.selectedIndex = 0;
    }
}
//JAVASCRIPT-->
</script>
<?php

// Include the quickmenu

// Display the header of links across the top of the screen.
echo("<table border='0' width='100%'>");
echo("<tr>");
    echo("<td align='center' colspan='2'>");
    echo("<b><a class='blue' href='#student_information'>Student Information </a>| </b>");
    echo("<b><a class='blue' href='#academic_information'>Academic Information </a>| </b>");
    echo("<b><a class='blue' href='#flags'>Flags </a>| </b>");
    echo("<b><a class='blue' href='#contact_information'>Contact Information </a>| </b>");
    echo("<b><a class='blue' href='#notes'>Notes </a></b>");
    echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<form method='post' action='" . $PHP_SELF . "' ENCTYPE='multipart/form-data' name='myform'>");
echo("<input type='hidden' name='select' value='edit_student' />");
echo("<input type='hidden' name='record_id' value='" . $record_id . "' />");
echo("<input type='hidden' name='editStudent' value='" . packObject($editStudent) . "' />");
echo("<input type='hidden' name='checkChanges' value='" . packObject($checkChanges) . "' />");
echo("<a name='student_information'></a>");

// Show student information.
echo("<table border='0' width='100%' cellspacing='1'>");
echo("<tr>");
    echo("<td>");
    echo("<h4>Student Information:</h4>");
    echo("</td>");
    echo("<td align='right'>");
    echo("<a href='#top'>Top</a>");
    echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<table border='0' width='100%' class='row1'>");
echo("<tr class='row1'>");
    echo("<td>");
    echo("<table width='100%'CLASS='row1'>");
    echo("<tr>");
        echo("<td width='75%' class='row1'>");
        echo("<table class='row1'>");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Student Number:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo($editStudent->student_number);
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>First Name:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo("<input type='text' name='first_name' size='35' value=\"" . $editStudent->first_name . "\" />");
            echo($changes_array["first_name"] ? "<br /><b class='black'>" . $changes_array["first_name"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Last Name:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo("<input type='text' name='last_name' size='35' value=\"" . $editStudent->last_name . "\" />");
            echo($changes_array["last_name"] ? "<br /><b class='black'>" . $changes_array["last_name"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Middle Name:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo("<input type='text' name='middle_name' size='35' value=\"" . $editStudent->middle_name . "\" />");
            echo($changes_array["middle_name"] ? "<br /><b class='black'>" . $changes_array["middle_name"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Preferred Name:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo("<input type='text' name='preferred_name' size='35' onfocus='changed()' value=\"" . $editStudent->preferred_name . "\" />");
            echo($changes_array["preferred_name"] ? "<br /><b class='black'>" . $changes_array["preferred_name"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Birthdate:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo("<input type='text' name='birth' size='35' value=\"" . $editStudent->birth . "\" />&nbsp;");
            echo(popup('birth','myform'));
            echo($changes_array["birth"] ? "<br /><b class='black'>" . $changes_array["birth"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Citizen:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo("<input type='text' name='citizen' size='35' value=\"" . $editStudent->citizen . "\" />");
            echo($changes_array["citizen"] ? "<br /><b class='black'>" . $changes_array["citizen"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>High School:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo("<input type='text' name='high_school' size='35' value=\"" . $editStudent->high_school . "\" />");
            echo($changes_array["high_school"] ? "<br /><b class='black'>" . $changes_array["high_school"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Gender:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo("<select name='gender'>");
            if (!$editStudent->gender)
            {
                echo("<option value='' selectedv='selected'>&nbsp;</option>");
            }

            echo("<option value='M' ");
            echo(($editStudent->gender == "M") ? "selected='selected'" : "");
            echo(">Male</option>");

            echo("<option value='F' ");
            echo(($editStudent->gender == "F") ? "selected='selected'" : "");
            echo(">Female</option>");

            echo("</select>");
            echo($changes_array["gender"] ? "<br /><b class='black'>" . $changes_array["gender"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("</table>");
        echo("</td>");

        // Display the photo
        echo("<td align='center' valign='middle' class='row1'>");
            echo("<table>");

            if ($editStudent->photo == NULL)
            {
                echo("<tr>");
                    echo("<td align=center>");
            
                    echo("<b class='black'>No Photo Available</b>");
                    echo("</td>");
                echo("</tr>\n");

                echo("<tr>");
                    echo("<td>");
                    echo("<input type='file' name='studentphoto' />");
                    echo("</td>");
                echo("</tr>\n");
            }
            else
            {
                // hack to prevent IE from caching the wrong image... ahh, IE is such a great browser :| 
                $num = rand(0,100);
                echo("<tr>");
                    echo("<td align=center>");
                    echo("<img alt='" . $editStudent->first_name . " " . $editStudent->last_name . "' src='".$PHP_SELF_MENU."?select=student_photo_viewer&amp;no_html=1&amp;record_id=".$record_id."&amp;num=".$num."' />");
                    echo("<input type='hidden' name='photo' value='" . $editStudent->photo . "' />");
                    echo("</td>");
                echo("</tr>\n");

                echo("<tr>");
                    echo("<td align='center'>");
                    echo("<input type='checkbox' name='deletephoto' value='True' />Delete Photo");
                    echo($changes_array["deletephoto"] ? "<br /><b class='black'>Deleted</b>" : "");
                    echo("</td>");
                echo("</tr>\n");

                echo("<tr>");
                    echo("<td align='center'>");
                    echo("<br />-Or-<br /><br />Replace photo:");
                    echo("</td>");
                echo("</tr>\n");

                echo("<tr>");
                    echo("<td align='center'>");
                    echo("<input type='file' name='studentphoto' />");
                    echo("</td>");
                echo("</tr>\n");
            }

            echo("</table>");
        echo("</td>");

    echo("</tr>\n");
    echo("</table>");
echo("</td>");
echo("</tr>\n");
echo("</table>");

// Show academic information.
echo("<a name='academic_information'></a>");

echo("<table border='0' width='100%' cellspacing='1'>");
echo("<tr>");
    echo("<td>");
    echo("<h4>Academic Information:</h4>");
    echo("</td>");
    echo("<td align='right'>");
    echo("<a href='#top'>Top</a>");
    echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<table border='0' width='100%' class='row1'>");
echo("<tr class='row1'>");
    echo("<td>");
    echo("<table width='100%'CLASS='row1'>");
    echo("<tr>");
        echo("<td width='75%' class='row1'>");
        echo("<table class='row1'>");
        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Discipline:&nbsp;&nbsp;</b>");
            echo("</td>");
            echo("<td>");

            $discipline_sql = ("
                SELECT d.discipline_name, d.discipline_id
                FROM discipline as d, student_department as sd 
                WHERE sd.department_id = d.department_id
                AND sd.record_id='" . $editStudent->record_id . "'
                AND (d.obsolete='0' OR d.discipline_id = '" . $editStudent->discipline_id . "')
                ORDER BY d.discipline_name
                ");

            $discipline_results = $GLOBALS['dbh']->Execute($discipline_sql);

            echo("<select name='discipline_id'>");
            echo("<option value=''>&nbsp;</option>");
            while ($discipline_row = $discipline_results->FetchRow())
            {
                echo("<option value='" . $discipline_row["discipline_id"] . "'");
                echo(($discipline_row["discipline_id"] == $editStudent->discipline_id) ? "selected='selected'" : "");
                echo(">" . $discipline_row["discipline_name"] . "</option>");
            }
            echo("</select>");
            echo($changes_array["discipline_id"] ? "<br /><b class='black'>" . getDisciplineName($changes_array["discipline_id"]) . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Academic Year:&nbsp;&nbsp;</b>");
            echo("</td>");
            echo("<td>");
            echo("<input type='text' name='academic_year' value=\"" . $editStudent->academic_year . "\" />");
            echo($changes_array["academic_year"] ? "<br /><b class='black'>" . $changes_array["academic_year"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Expected Graduating Year:&nbsp;&nbsp;</b>");
            echo("</td>");
            echo("<td>");
            echo("<input type='text' name='grad_year' value=\"" . $editStudent->grad_year . "\" />");
            echo($changes_array["grad_year"] ? "<br /><b class='black'>" . $changes_array["grad_year"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Faculty Advisor:&nbsp;&nbsp;</b>");
            echo("</td>");
            echo("<td>");
            echo("<input type='text' name='advisor' size='35' value=\"" . $editStudent->advisor . "\" />");
            echo($changes_array["advisor"] ? "<br /><b class='black'>" . $changes_array["advisor"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            $departments_in_g = department_groups($editStudent->department_id);
            $depts_in_group = implode("','",$departments_in_g);
            $depts_in_group = "('".$depts_in_group."')";
            echo("<b class='black'>Co-op Advisor:&nbsp;&nbsp;</b>");
            echo("</td>");
            echo("<td>");
            $coop_advisor_sql = (" SELECT DISTINCT c.contact_id, c.first_name, c.last_name, d.department_code 
                    FROM contact AS c
                    INNER JOIN contact_internal AS ci 
                    ON ci.contact_id=c.contact_id
                    INNER JOIN department AS d 
                    ON ci.department_id = d.department_id
                    WHERE ci.general_email=1 AND ci.department_id IN ".$depts_in_group."
                    ORDER BY c.last_name, c.first_name
                    ");
            $coop_advisor_results = $GLOBALS['dbh']->Execute($coop_advisor_sql);
            
            echo("<select name='coop_advisor'>");
            echo("<option value=''");
            echo(($editStudent->coop_advisor != NULL)? "selected='selected'" : "");
            echo(">&nbsp;</option>");
            
            $flag = 0;  
            while ($coop_advisor_row = $coop_advisor_results->FetchRow())
            {
                if($editStudent->coop_advisor == $coop_advisor_row["contact_id"])
                {
                  $flag = 1;
                }
                echo("<option value='" . $coop_advisor_row["contact_id"] . "'");  
                echo(($coop_advisor_row["contact_id"] == $editStudent->coop_advisor) ? "selected='selected'" : "");
                echo(">" . $coop_advisor_row["first_name"]." ".$coop_advisor_row["last_name"]." (".$coop_advisor_row['department_code'].")". "</option>");
            }          
           
            if($flag == 0)
            {   
                if($editStudent->coop_advisor == NULL)
                {    
                    echo("<option value= '" .$editStudent->coop_advisor . "' selected='selected' >");
                    $ca_name = getContactName($editStudent->coop_advisor);
                    echo( $ca_name["first_name"]." ".$ca_name["last_name"]." (".$coop_advisor_row["department_code"].")". "</option>");
                }
           }
            echo("</select>");
            
            if($changes_array["coop_advisor"])
            {
                $coop_advisor_sql=("
                    SELECT c.first_name, c.last_name
                    FROM contact as c
                    WHERE c.contact_id = '".$changes_array["coop_advisor"]."'
                    ");
                $coop_advisor_results = $GLOBALS['dbh']->Execute($coop_advisor_sql);

                $coop_advisor_row = $coop_advisor_results->FetchRow();
                $ca_first_name = $coop_advisor_row["first_name"];
                $ca_last_name = $coop_advisor_row["last_name"] ;
            }
            
            echo($changes_array["coop_advisor"] ? "<br /><b class='black'>" . $ca_first_name." " .$ca_last_name . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");
        
        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Admittance Date:&nbsp;&nbsp;</b>");
            echo("</td>");
            echo("<td>");
            echo("<input type='text' name='admit' value=\"" . $editStudent->admit . "\" />");
            echo("&nbsp;" . popup('admit','myform'));
            echo($changes_array["admit"] ? "<br /><b class='black'>" . $changes_array["admit"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Withdrawal Date:&nbsp;&nbsp;</b>");
            echo("</td>");
            echo("<td>");
            echo("<input type='text' name='withdraw' value=\"" . $editStudent->withdraw . "\" />");
            echo("&nbsp;" . popup('withdraw','myform'));
            echo($changes_array["withdraw"] ? "<br /><b class='black'>" . $changes_array["withdraw"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Convocation Date:&nbsp;&nbsp;</b>");
            echo("</td>");
            echo("<td>");
            
            //Convocation Month Dropdown List
            $convocation_month_sql = "SELECT month
                                      FROM convocation_month";

            $conovcation_month_results = $GLOBALS['dbh']->Execute($convocation_month_sql);

            echo("<select name='convocation_month'>");
            echo("<option value=''>&nbsp;</option>");
            //while there are still months to put in list
            while($convocation_month_row = $conovcation_month_results->FetchRow())
            {
                //set numerical value of month
                echo("<option value='".$convocation_month_row['month']."' ");
                //if month previously selected make it default    
                echo(($convocation_month_row["month"] == $editStudent->convocation_month) ? "selected='selected'" : "");
                //convert month numerical version to full name
                echo(">". date( "F", mktime( 0,0,0,$convocation_month_row['month']+1,0,0) ) ."</option>");
            } 
            echo("</select>&nbsp;");

            //Convocation Year Dropdown List
            $convocation_year_sql = "SELECT MIN( date_format(admit, '%Y') ) AS min_year
                                     FROM student_department
                                     WHERE admit IS NOT NULL AND admit != 0000";

            $conovcation_year_results = $GLOBALS['dbh']->Execute($convocation_year_sql);
            $convocation_year_row = $conovcation_year_results->FetchRow();

            //minimum year a student has been admitted
            $min_convocation_year = $convocation_year_row["min_year"];
            //the current year plus one
            $max_convocation_year = date( "Y" ) + 1;
            
            echo("<select name='convocation_year'>");
            echo("<option value=''>&nbsp;</option>");
            //make a list from min to max year
            for($year_count = $max_convocation_year; $year_count >= (int)$min_convocation_year; $year_count--)
            {
                echo("<option value='$year_count' ");
                //if year previously selected make it default
                echo(($year_count == $editStudent->convocation_year) ? "selected='selected'" : "");                     
                echo(">$year_count</option>");
            }
            echo("</select>&nbsp;");
            
            echo($changes_array["convocation_month"] ? "<br /><b class='black'>" . $changes_array["convocation_month"] . "</b>" : ""); 
            echo($changes_array["convocation_year"] ? "<br /><b class='black'>" . $changes_array["convocation_year"] . "</b>" : "");

            //The grad variable is kept for historical reasons. We did not want to 
            //  lose the value stored in the db (which would happen if this variable
            //  was not passed on).
            echo("<input type='hidden' name='grad' value='".$editStudent->grad."' />");
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Grad GPA:&nbsp;&nbsp;</b>");
            echo("</td>");
            echo("<td>");
            echo("<input type='text' name='grad_gpa' value=\"" . $editStudent->grad_gpa . "\" />");
            echo($changes_array["grad_gpa"] ? "<br /><b class='black'>" . $changes_array["grad_gpa"] . "</b>" : "");
            echo("</td>");
        echo("</tr>\n");

        echo("</table>");
        echo("</td>");

        echo("<td>");
        if($editStudent->start_year != NULL)
        {
            echo("<b class='black'>Note: Clearing the start year will erase all of the entries in the table below</b><br />");
        }
        echo("<center><b class='black'>Start Year:</b>");
        echo("&nbsp;&nbsp;<input type='text' name='start_year' value=\"" . $editStudent->start_year . "\" />");
        echo($changes_array["start_year"] ? "<br /><b class='black'>" . $changes_array["start_year"] . "</b>" : "");
        echo("</center>");

        if ($editStudent->start_year == NULL)
        {
            echo("<br />No semesters table will show up until a start year is entered for the student");
        }
        else
        {
            echo("<table cellspacing=1>");
            echo("<tr>");
                echo("<td class='row6'>&nbsp;</td>");

                // Display the term codes
                $sql = "SELECT DISTINCT term_code
                        FROM term
                        ORDER BY order_by";
                
                $result = $GLOBALS['dbh']->Execute($sql);
                while ($row = $result->FetchRow())
                {
                    echo("<td class='row6'>" . $row["term_code"] . "</td>");
                }
            echo("</tr>\n");
            $count = 0;

            // Get all the semesters that the student already is using along with any that the department is allowed to set.
            // Then, use these as options for our select menu.

            $sql = ("
                SELECT DISTINCT s.semesters_id 
                FROM semesters AS s
                INNER JOIN semesters_table AS st ON st.semesters_id=s.semesters_id
                WHERE st.record_id='" . $editStudent->record_id . "'
                ORDER BY st.term_id, st.year
                ");
            
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow()) 
            {
                $student_semester_string .= "'" . $row[semesters_id] . "',";
            }
            
            if ($student_semester_string)
            {
                $student_semester_string = substr($student_semester_string, 0, -1);
            }
            else
            {
                $student_semester_string = "''";
            }

            $sql = ("
                SELECT DISTINCT a.display, a.semesters_id 
                FROM semesters AS a, department_semesters AS b 
                WHERE (b.department_id='" . $editStudent->department_id . "' AND b.semesters_id=a.semesters_id) 
                OR 
                (a.semesters_id IN (" . $student_semester_string . ")) 
                ORDER BY display
                ");
            
            $result = $GLOBALS['dbh']->Execute($sql);

            while ($row = $result->FetchRow())
            {
                $semesters_row[] = $row;
            }

            // Determine where to start and end the term iteration.
            $sql = ("
                SELECT DISTINCT term_id
                FROM term
                ORDER BY term_id
                ");
            
            $result = $GLOBALS['dbh']->Execute($sql);

            $term_limit = $result->RecordCount() + 1;     
            $row = $result->FetchRow();
            $term_start = $row["term_id"];
            //the first year on the calendar
            $nextYear = $editStudent->start_year;
            //the first term on the calendar
            $nextTerm = $term_start; 

            //Get Saved Default TimeTable Info from database
            $sql = ("
                SELECT timetable_num AS tt, year_num AS y, term_id AS t, semesters_id AS s
                FROM department_default_semesters
                WHERE department_id = '".$editStudent->department_id."'
                ");
            
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                $saved_row[] = $row;
            }

            //Convert results into a 3-dimensional array 

            //used to keep track of how many default timetables we have.
            $timetable_max = 0;

            //if we had any results
            if($saved_row)
            {
                //for each result from the query
                foreach( $saved_row as $s_r ) 
                {
                    $temp_tt = $s_r["tt"];
                    $temp_year = $s_r["y"];
                    $temp_term = $s_r["t"];

                    $default_semesters[$temp_tt][$temp_year][$temp_term] = $s_r["s"];
                    
                    if($temp_tt > $timetable_max)
                    {
                        $timetable_max = $temp_tt;
                    }
                }
            }

            //Used to create a hash where key is the semester code and the value is this code's index within the semester select boxes
            foreach( $semesters_row as $key=>$temp_semester_array )
            {
                $dept_semesters[$temp_semester_array[1]] = $key + 1;
            }

            //Prepare a List of Term/Year in order of appearance on the time table
            $next_term_array[] = $term_start;
            $next_year_array[] = $editStudent->start_year;

            //:TODO: Don't hard code this with 4
            for ($i = 0; $i <= 4; $i++)
            {
                for ($j = 0; $j <= $term_limit - 1; $j++)
                {
                    $next_term_year = getNextTermYear2($next_term_array[sizeof($next_term_array)-1], $next_year_array[sizeof($next_year_array)-1]);
                    $next_term_array[] = $next_term_year["term"];
                    $next_year_array[] = $next_term_year["year"];
                }
            }
             
            //Store current timetable to allow user to revert back to it if they want
            $next_term_year_count = 0;

            //:TODO: Don't hard code this with 5
            for ($year_count =  0; $year_count <= 5; $year_count++)
            {
                for ($term_count = 0; $term_count <= $term_limit - 2; $term_count++)
                {
                    $next_term = $next_term_array[ $next_term_year_count ];
                    $next_year = $next_year_array[ $next_term_year_count ];

                    if(is_numeric($editStudent->semesters[$next_year][$next_term]))
                    {
                        $revert_timetable[$year_count][$term_count] = $editStudent->semesters[$next_year][$next_term];
                    }
                    else
                    {
                        $revert_timetable[$year_count][$term_count] = 0;
                    }

                    $next_term_year_count++;
                }
            }
            
            $default_semesters[3] = $revert_timetable;  //store the original time table with the default timetables for transfer to JS
?>

           <?php
            // Show six years worth of semesters. 
            for ($i = $editStudent->start_year; $i < ($editStudent->start_year + 6); $i++)
            {
                echo("<tr>");
                // Format the start year so that a 1998 start year would look like 98-99
                echo("<td class='row6'>");
                echo(substr($i, -2) . "-" . substr($i + 1 , - 2));
                echo("</td>");

                for ($j = $term_start; $j < $term_limit; $j++)
                {
                    echo("<td class='row6'>");
                    echo("<select name='semesters[" . $nextYear . "][" . $nextTerm . "]'>");

                    echo("<option value='' ");
                    echo((!$editStudent->semesters[$nextYear][$nextTerm]) ? "selected='selected'" : "");
                    echo(">&nbsp;</option>");

                    foreach ($semesters_row as $sr)
                    {
                        echo("<option value='" . $sr["semesters_id"] . "' ");
                        echo(($editStudent->semesters[$nextYear][$nextTerm] == $sr["semesters_id"]) ? "selected='selected'" : "");
                        echo(">" . $sr["display"] . "</option>");
                    }

                    echo("</select>");
                    if ($changes_array["semesters"][$nextYear][$nextTerm])
                    {
                        echo("<br /><b class='black'>" . getSemesterCode($changes_array["semesters"][$nextYear][$nextTerm]) . "</b>");
                    }
                    echo("</td>\n");

                    $nextTermYear = getNextTermYear2($nextTerm, $nextYear);
                    $nextTerm = $nextTermYear["term"];
                    $nextYear = $nextTermYear["year"];
                }

                echo("</tr>\n");
            }

            echo("</table>\n"); 
?> 
 <script type='text/javascript' language='javascript'>
            <!--javascript //hides if this browser cant use java script
                function updateTimeTable( time_table ) //used to update timetable with defaults, PARAM time_table indicates which default
                {
                    var default_semesters;                                      //saved default semesters (3 timetables per dept)
                    var start_year = <?php echo($editStudent->start_year); ?>;  //start year in their program for this student
                    var num_terms = <?php echo($term_limit - 1); ?>;            //the number of terms offered by this institution
                    var dept_semesters;                                         //all the semesters for this department (or dept group)
                    var next_term_array;
                    var next_year_array;
                    var elements = document.myform.elements;
                     
                    //converting php arrays for use in java script
                    <?php echo(arrayToJS($default_semesters,"default_semesters")); ?>
                    <?php echo(arrayToJS($dept_semesters,"dept_semesters")); ?>       
                    <?php echo(arrayToJS($next_term_array,"next_term_array")); ?>
                    <?php echo(arrayToJS($next_year_array,"next_year_array")); ?>
                    
                    //clear the old timetable
                    var next_term_year_count = 0;
                    var next_term;
                    var next_year;
                    for ( var year_count =  0; year_count <= 5; year_count++)
                    {
                        for ( var term_count = 0; term_count <= num_terms - 1; term_count++)
                        {
                            next_term = next_term_array[ next_term_year_count ];
                            next_year = next_year_array[ next_term_year_count ];
                            
                            for( var element_count = 0; element_count <= elements.length - 1; element_count++ )
                            {
                                if( elements[element_count].name == ("semesters["+(next_year)+"]["+(next_term)+"]") )
                                {
                                    document.myform.elements[element_count].selectedIndex = 0;
                                    next_term_year_count++;
                                    break;
                                }
                            }

                        }
                    }
                    
                    //replace with the default timetable
                    next_term_year_count = 0;
                    for ( year_count = 0; year_count <= 5; year_count++)
                    {
                        for ( term_count = 0; term_count <= num_terms - 1; term_count++)
                        {
                            next_term = next_term_array[ next_term_year_count ];
                            next_year = next_year_array[ next_term_year_count ];
                            
                            for( element_count = 0; element_count <= elements.length - 1; element_count++ )
                            {
                                if( elements[element_count].name == ("semesters["+(next_year)+"]["+(next_term)+"]") )
                                {
                                    var temp_semester = default_semesters[time_table][year_count][term_count];
                                    default_index = dept_semesters[ temp_semester ];
                                    
                                    if( default_index )
                                    {
                                        document.myform.elements[element_count].selectedIndex = default_index;
                                    }
                                    else
                                    {
                                        document.myform.elements[element_count].selectedIndex = 0;

                                    }

                                    next_term_year_count++;
                                    break;
                                }
                            }
                        }
                    } 
                }
            //-->
            </script>

<?php
            // Section for displaying 'Default Time Table' buttons
            echo("<hr />\n");
            echo("<center>");
            echo("<table>\n");
            echo("<tr>\n");
                echo("<td colspan='2' align='center'><b class='black'>Default Time Tables</b></td>");
            echo("</tr>\n");
            echo("<tr>\n");
                echo("<td>Set Default 1 Timetable</td>");
                echo("<td><input type='button' name='edit_defaults' value='Default 1' onclick='updateTimeTable(0)' /></td>");
            echo("</tr>\n");
            echo("<tr>\n");
                echo("<td>Set Default 2 Timetable</td>");
                echo("<td><input type='button' name='edit_defaults' value='Default 2' onclick='updateTimeTable(1)' /></td>");
            echo("</tr>\n");
            echo("<tr>\n");
                echo("<td>Set Default 3 Timetable</td>");
                echo("<td><input type='button' name='edit_defaults' value='Default 3' onclick='updateTimeTable(2)' /></td>");
            echo("</tr>\n");
            echo("<tr>\n");
                 echo("<td>Reset to Original Timetable</td>");
                echo("<td><input type='button' name='edit_defaults' value='Revert' onclick='updateTimeTable(3)' /></td>");
            echo("</tr>\n");                                                
            echo("</table>\n");
            echo("</center>");
    }

echo("</td>");
echo("</tr>\n");
echo("</table>");
echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<a name='flags'></a>");


// Show regular flags first.
echo("<table width='100%' cellpadding='3' cellspacing='0' border='0'>");
echo("<tr>");
echo("<td>");
echo("<h4>Flags:</h4>");
echo("</td>");

echo("<td align='right'>");
echo("<a href='#top'>Top</a>");
echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<table width='100%' cellpadding='3' cellspacing='0' border='0'>");
echo("<tr>");
echo("<td class='row1' colspan='2' align='center'>");
echo("<table cellspacing='0' border='0' cellpadding='3' width='100%'>");

$flag_array = get_student_flags($editStudent->record_id, $editStudent->department_id, GENERAL_FLAGS);
$new_column = 0;
$SetFlagCount = 0;
foreach ($flag_array as $fa)
{
    if (!($new_column % 3))
    {
        echo("<tr align='left'>");
    }

    $temp = substr("'", "special_char", $fa["flag_name"]);
    $formname = substr(" ", "_", $temp);

    echo("<td width='3%' class='row1'>");
    echo("<input type='checkbox' name='general_flags[]' value='" . $fa["flag_id"] . "'");
    echo(is_array($editStudent->general_flags) && in_array($fa["flag_id"], $editStudent->general_flags) ? "checked='checked'" : "");
    echo(" />");
    if ($changes_array["changes_made"])
    {
        echo("<br /><b class='black'>");
        echo((is_array($changes_array["general_flags"]) && in_array($fa["flag_id"], $changes_array["general_flags"])) ? "(Checked)" : "(Unchecked)");
        echo("</b>");
    }
    echo("</td>");

    echo("<td width='30%' class='row1'>");
    echo($fa["flag_name"]);
    echo("</td>");

    if ($new_column % 3 == 2)
    {
        echo("</tr>\n");
    }
    $new_column++;
}

// Close any open columns
while ($new_column % 3)
{
    echo("<td width='3%' class='row1'>&nbsp;</td>");
    echo("<td width='30%' class='row1'>&nbsp;</td>");
    if ($new_column % 3 == 2)
    {
        echo("</tr>\n");
    }
    $new_column++;
}

if ($changes_array["gflags"])
{
    echo("<tr>");
    echo("<td colspan='2'>");
    echo($changes_array["gflags"]);
    echo("</td>");
    echo("</tr>\n");
}
echo("</table>");

echo("</td>");
echo("</tr>\n");
echo("</table>");


// Now display term flags.
echo("<table width='100%' cellspacing='0' cellpadding='3' border='0'>");
echo("<tr>");
echo("<td>");
echo("<h4>Term Flags:</h4>");
echo("</td>");

echo("<td align='right'>");
echo("<a href='#top'>Top</a>");
echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<table width='100%' cellpadding='3' cellspacing='0' border='0'>");
echo("<tr>");
echo("<td class='row1' colspan='2' align='center'>");
echo("<table cellspacing='0' border='0' cellpadding='3' width='100%'>");

// Draw out the term_id, name, year from the database that we will be showing eligibility status for.
$sql = ("
        SELECT DISTINCT t.term_id AS eligible_term_id, t.term_name AS eligible_term_name, " . date('Y') . " + esf.year_space AS eligible_year
        FROM student_flags AS sf, department_student_flags AS dsf, special_flags AS spf, student_department AS sd 
        LEFT JOIN edit_student_flags AS esf ON esf.student_flags_id=sf.student_flags_id 
        LEFT JOIN term AS t ON t.term_id=esf.term_id 
        WHERE sd.record_id='" . $editStudent->record_id . "' 
        AND sf.student_flags_id=" . ELIGIBLE_FLAG . " 
        AND dsf.student_flags_id=sf.student_flags_id 
        AND spf.flag_type_id=sf.flag_type_id
        AND sd.department_id=dsf.department_id
        ORDER BY esf.order_by, t.term_id, sf.description
        ");
$result = $GLOBALS['dbh']->Execute($sql);

$new_column = 0;
while ($row = $result->FetchRow())
{
    if (!($new_column % 3))
    {
        echo("<tr align='left'>");
    }

    $term_name = $row["eligible_term_name"];
    $term_id = $row["eligible_term_id"];
    $year = $row["eligible_year"];
    $flag_name = "Eligible (" . $term_name . " " . $year . ")";

    echo("<td width='3%' class='row1'>");
    echo("<center>");
    echo("<input type='checkbox' name='eligible_flags[" . $year . "][" . $term_id . "]' value='true' ");
    echo((is_array($editStudent->eligible_flags) && $editStudent->eligible_flags[$year][$term_id]) ? "checked='checked'" : "");
    echo(" />");
    if ($changes_array["changes_made"])
    {
        echo("<br /><b class='black'>");
        echo((is_array($changes_array["eligible_flags"]) && $changes_array["changes_made"] && $changes_array["eligible_flags"][$year][$term_id]) ? "(Checked)" : "(Unchecked)");
        echo("</b>");
    }
    echo("</center>");
    echo("</td>");

    echo("<td width='30%' class='row1'>");
    echo($flag_name);
    echo("</td>\n");

    if ($new_column % 3 == 2)
    {
        echo("</tr>\n");
    }
    $new_column++;
}

// Fill out any remaining columns
while ($new_column % 3)
{
    echo("<td width='3%' class='row1'>&nbsp;</td>");
    echo("<td width='30%' class='row1'>&nbsp;</td>");
    if ($new_column % 3 == 2)
    {
        echo("</tr>\n");
    }
    $new_column++;
}

// Now show the placements this student has had.
$sql = ('
    SELECT CONCAT(t.term_name, " ", h.year) AS name
    FROM history h
    INNER JOIN term t
    ON h.term_id = t.term_id
    WHERE h.student_number = '.$editStudent->student_number.' AND h.department_id = '.$editStudent->department_id.'
    ORDER BY h.year DESC, t.year_order DESC
    ');

$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount())
{
    echo("<tr>");
    echo("<td colspan='6'>&nbsp;</td>");
    echo("</tr>\n");

    echo("<tr>");
    echo("<td colspan='6' align='center'>");
    echo("Student has been <b class='black'>placed</b> for the following terms:");
    echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
    echo("<td colspan='6'>&nbsp;</td>");
    echo("</tr>\n");

    while ($row = $result->FetchRow())
    {
        echo("<tr>");
        echo("<td colspan='6' align='center'>");
        echo($row["name"]);
        echo("</td>");
        echo("</tr>\n");
    }
}

if ($changes_array["tflags"])
{
    echo("<tr>");
    echo("<td colspan='2'>");
    echo($changes['tflags']);
    echo("</td>");
    echo("</tr>\n");
}

echo("</table>");

echo("</td>");
echo("</tr>\n");
echo("</table>");



// Now show the profile flags
echo("<table width='100%' cellpadding='3' cellspacing='0' border='0'>");
echo("<tr>");
echo("<td>");
echo("<h4>Profile Flags:</h4>");
echo("</td>");

echo("<td align=right>");
echo("<a href='#top'>Top</a>");
echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<table width='100%' cellpadding='3' cellspacing='0' border='0'>");
echo("<tr>");
echo("<td class='row1' colspan='2' align='center'>");
echo("<table cellspacing='0' border='0' cellpadding='3' width='100%'>");

$flag_array = get_student_flags($editStudent->record_id, $editStudent->department_id, PROFILE_FLAGS);
$new_column = 0;
$SetFlagCount = 0;
foreach ($flag_array as $fa)
{
    if (!($new_column % 3))
    {
        echo("<tr align='left'>");
    }

    $temp = substr("'", "special_char", $fa["flag_name"]);
    $formname = substr(" ", "_", $temp);

    echo("<td width='3%' class='row1'>");
    echo("<input type='checkbox' name='profile_flags[]' value='" . $fa["flag_id"] . "'");
    echo(is_array($editStudent->profile_flags) && in_array($fa["flag_id"], $editStudent->profile_flags) ? "checked='checked'" : "");
    echo(" />");
    if ($changes_array["changes_made"])
    {
        echo("<br /><b class='black'>");
        echo((is_array($changes_array["profile_flags"]) && in_array($fa["flag_id"], $changes_array["profile_flags"])) ? "(Checked)" : "(Unchecked)");
        echo("</b>");
    }
    echo("</td>");

    echo("<td width='30%' class='row1'>");
    echo($fa["flag_name"]);
    echo("</td>");

    if ($new_column % 3 == 2)
    {
        echo("</tr>\n");
    }
    $new_column++;
}

// Close any open columns
while ($new_column % 3)
{
    echo("<td width='3%' class='row1'>&nbsp;</td>");
    echo("<td width='30%' class='row1'>&nbsp;</td>");
    if ($new_column % 3 == 2)
    {
        echo("</tr>\n");
    }
    $new_column++;
}

echo("</table>");

echo("</td>");
echo("</tr>\n");
echo("</table>");


// Show contact information
echo("<a name='contact_information'></a>");

echo("<table width='100%' cellspacing='1'>");
echo("<tr>");
    echo("<td width='100%'>");
    echo("<h4>Contact Information:</h4>");
    echo("</td>");

    echo("<td align='right'>");
    echo("<a href='#top'>Top</a>");
    echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<table cellspacing='0' border='0' cellpadding='5' width='100%'>");
echo("<tr>");
    echo("<td class='row1'>");

    echo("<table cellspacing='0' border='0'>");
    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Current Address:&nbsp;&nbsp;</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' name='street_address_current' size='35' value=\"" . $editStudent->street_address_current . "\" />");
        echo($changes_array["street_address_current"] ? "<br /><b class='black'>" . $changes_array["street_address_current"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("&nbsp;");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' name='street_address_current2' size='35' value=\"" . $editStudent->street_address_current2 . "\" />");
        echo($changes_array["street_address_current2"] ? "<br /><b class='black'>" . $changes_array["street_address_current2"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("&nbsp;");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' name='street_address_current3' size='35' value=\"" . $editStudent->street_address_current3 . "\" />");
        echo($changes_array["street_address_current3"] ? "<br /><b class='black'>" . $changes_array["street_address_current3"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Country:</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<select name='country_current' onchange='update_prov(\"current\");'>");

        echo("<option value=''");
        echo((!$editStudent->country_permanent) ? " selected='selected'" : "");
        echo(">&nbsp;</option>");

        $countrysql = ("
            SELECT country_id, country_name 
            FROM country_list 
            ORDER BY order_by
            ");
        $countryresults = $GLOBALS['dbh']->Execute($countrysql); 
        while ($countryrow = $countryresults->FetchRow())
        {
            echo("<option value='" . $countryrow["country_id"] . "'");
            echo(($countryrow["country_id"] == $editStudent->country_current) ? " selected='selected'" : "");
            echo(">" . $countryrow["country_name"] . "</option>");
        }
        echo("</select>");
        echo(($changes_array["country_current"]) ? "<br /><b class='black'>" . getCountryName($changes_array["country_current"]) . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Province/State:</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<select name='province_current'  onchange='update_region_current()'>");

        if (!$editStudent->country_current)
        {
            echo("<option value=''>Choose a Country</option>");
        }
        else
        {
            /*
             A country has been selected if we're at this point, so pull that country's provinces
             or states out of the database and display them.
            */

            $sql = ("
                    SELECT DISTINCT provstate_id, provstate_name
                    FROM provstate_list
                    WHERE country_id='" . addslashes($editStudent->country_current) . "'
                    ORDER BY provstate_name
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount())
            {
                /*
                 If the chosen country has provinces or states, display them,
                 otherwise show N/A.
                */

                echo("<option value='' ");
                if (isset($editStudent->province_current) && !$editStudent->province_current)
                {
                    echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["provstate_id"] . "'");
                    if ($editStudent->province_current == $row["provstate_id"])
                    {
                        echo(" selected='selected'");
                    }
                    echo(">" . $row["provstate_name"] . "</option>");
                }
            }
            else
            {
                echo("<option value='' selected='selected'>N/A</option>");
            }
        }
        echo("</select>");
        echo($changes_array["province_current"] ? "<br /><b class='black'>" . getProvstateName($changes_array["province_current"]) . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Current Region:</b>");
        echo("</td>");
        
        echo("<td class='row0'>");
        echo("<select name='region_current'>");
        if (!$editStudent->country_current)
        {
            echo("<option value=''>Choose a Country</option>");
        }
        elseif (!$editStudent->province_current)
        {
            echo("<option value=''>Choose a Province/State</option>");
        }
        else
        {
            $sql = ("
                    SELECT DISTINCT region_id, region_name
                    FROM region_list
                    WHERE provstate_id='" .$editStudent->province_current . "'
                    ORDER BY region_name
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount())
            {
                echo("<option value='' ");
                if (isset($editStudent->region_current) && !$editStudent->region_current)
                {
                    echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["region_id"] . "' ");
                    if ($editStudent->region_current == $row["region_id"])
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
        echo("</select>");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>City:</b>");
        echo("</td>");
        echo("<td class='row0'>");
        echo("<input type='text' name='city_current' size='35' value=\"" . $editStudent->city_current . "\" />");
        echo($changes_array["city_current"] ? "<br /><b class=\"black\">" . $changes_array["city_current"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Postal Code:</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' name='postal_code_current' size='35' value=\"" . $editStudent->postal_code_current . "\" />");
        echo(($changes_array["postal_code_current"]) ? "<br /><b class='black'>" . $changes_array["postal_code_current"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Current Phone Number:&nbsp;&nbsp;</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' size='35' name='phone_current' value=\"" . $editStudent->phone_current . "\" />");
        echo(($changes_array["phone_current"]) ? "<br /><b class='black'>" . $changes_array["phone_current"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Current Fax Number:&nbsp;&nbsp;</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' size='35' name='fax_current' value=\"" . $editStudent->fax_current . "\" />");
        echo(($changes_array["fax_current"]) ? "<br /><b class='black'>" . $changes_array["fax_current"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td>");
        echo("<b class='black'>Primary E-mail:&nbsp;&nbsp;</b>");
        echo("</td>");

        echo("<td>");
        echo("<input type='text' name='email' size='35' value=\"" . $editStudent->email . "\" />");
        echo($changes_array["email"] ? "<br /><b class='black'>" . $changes_array["email"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td>");
        echo("<b class='black'>Secondary E-mail:&nbsp;&nbsp;</b>");
        echo("</td>");

        echo("<td>");
        echo("<input type='text' name='email2' size='35' value=\"" . $editStudent->email2 . "\" />");
        echo(($changes_array["email2"]) ? "<br /><b class='black'>" . $changes_array["email2"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Permanent Address:&nbsp;&nbsp;</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' size='35' name='street_address_permanent' value=\"" . $editStudent->street_address_permanent . "\" />");
        echo($changes_array["street_address_permanent"] ? "<br /><b class='black'>" . $changes_array["street_address_permanent"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("&nbsp;");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' name='street_address_permanent2' size='35' value=\"" . $editStudent->street_address_permanent2 . "\" />");
        echo(($changes_array["street_address_permanent2"]) ? "<br /><b class='black'>" . $changes_array["street_address_permanent2"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("&nbsp;");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' name='street_address_permanent3' size='35' value=\"" . $editStudent->street_address_permanent3 . "\" />");
        echo(($changes_array["street_address_permanent3"]) ? "<br /><b class='black'>" . $changes_array["street_address_permanent3"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Country:</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<select name='country_permanent' onchange='update_prov(\"permanent\")'>");

        echo("<option value=''");
        echo((!$editStudent->country_permanent) ? " selected='selected'" : "");
        echo(">&nbsp;</option>");

        $countrysql = ("
            SELECT country_id, country_name 
            FROM country_list 
            ORDER BY order_by
            ");
        $countryresults = $GLOBALS['dbh']->Execute($countrysql); 
        while ($countryrow = $countryresults->FetchRow())
        {
            echo("<option value='" . $countryrow["country_id"] . "'");
            echo($countryrow["country_id"] == $editStudent->country_permanent ? " selected='selected'" : "");
            echo(">" . $countryrow["country_name"] . "</option>");
        }
        echo("</select>");

        echo($changes_array["country_permanent"] ? "<br /><b class='black'>" . getCountryName($changes_array["country_permanent"]) . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Province/State:</b>");
        echo("</td>");

        echo("<td class='row0'>");

        echo("<select name='province_permanent' onchange='update_region_permanent()'>");

        if (!$editStudent->country_permanent)
        {
            echo("<option value=''>Choose a Country</option>");
        }
        else
        {
            /*
             A country has been selected if we're at this point, so pull that country's provinces
             or states out of the database and display them.
            */

            $sql = ("
                    SELECT DISTINCT provstate_id, provstate_name
                    FROM provstate_list
                    WHERE country_id='" . addslashes($editStudent->country_permanent) . "'
                    ORDER BY provstate_name
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount())
            {
                /*
                 If the chosen country has provinces or states, display them,
                 otherwise show N/A.
                */

                echo("<option value='' ");
                if (isset($editStudent->province_permanent) && !$editStudent->province_permanent)
                {
                    echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["provstate_id"] . "'");
                    if ($editStudent->province_permanent == $row["provstate_id"])
                    {
                        echo(" selected='selected'");
                    }
                    echo(">" . $row["provstate_name"] . "</option>");
                }
            }
            else
            {
                echo("<option value='' selected='selected'>N/A</option>");
            }
        }
        echo("</select>");

        echo($changes_array["province_permanent"] ? "<br /><b class='black'>" . getProvstateName($changes_array["province_permanent"]) . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Permanent Region:</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<select name='region_permanent'>");
        if (!$editStudent->country_permanent)
        {
            echo("<option value=''>Choose a Country</option>");
        }
        elseif (!$editStudent->province_permanent)
        {
            echo("<option value=''>Choose a Province/State</option>");
        }
        else
        {
            $sql = ("
                    SELECT DISTINCT region_id, region_name
                    FROM region_list
                    WHERE provstate_id='" .$editStudent->province_permanent . "'
                    ORDER BY region_name
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($result->RecordCount())
            {
                echo("<option value='' ");
                if (isset($editStudent->region_permanent) && !$editStudent->region_permanent)
                {
                    echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["region_id"] . "' ");
                    if ($editStudent->region_permanent == $row["region_id"])
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
        echo("</select>");
    echo("</td>");
echo("</tr>\n");


echo("<tr>");
    echo("<td class='row0'>");
    echo("<b class='black'>City:</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' size='35' name='city_permanent' value=\"" . $editStudent->city_permanent . "\" />");
        echo(($changes_array["city_permanent"]) ? "<br /><b class='black'>" . $changes_array["city_permanent"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Postal Code:</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' size='35' name='postal_code_permanent' value=\"" . $editStudent->postal_code_permanent . "\" />");
        echo($changes_array["postal_code_permanent"] ? "<br /><b class='black'>" . $changes_array["postal_code_permanent"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Permanent Phone Number:&nbsp;&nbsp;</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' name='phone_permanent' size='35' value=\"" . $editStudent->phone_permanent . "\" />");
        echo($changes_array["phone_permanent"] ? "<br /><b class='black'>" . $changes_array["phone_permanent"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("<tr>");
        echo("<td class='row0'>");
        echo("<b class='black'>Permanent Fax Number:&nbsp;&nbsp;</b>");
        echo("</td>");

        echo("<td class='row0'>");
        echo("<input type='text' size='35' name='fax_permanent' value=\"" . $editStudent->fax_permanent . "\" />");
        echo(($changes_array["fax_permanent"]) ? "<br /><b class='black'>" . $changes_array["fax_permanent"] . "</b>" : "");
        echo("</td>");
    echo("</tr>\n");

    echo("</table>");

    echo("</td>");
echo("</tr>\n");
echo("</table>");

// Emergency contact information.
echo("<table width='100%' cellspacing='1'>");
echo("<tr>");
    echo("<td width='100%'>");
    echo("<h4>Emergency Contact Information:</h4>");
    echo("</td>");

    echo("<td align='right'>");
    echo("<a href='#top'>Top</a>");
    echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<table width='100%' cellspacing='1'>");
echo("<tr>");
    echo("<td width='100%' class='row1' colspan='2'>");
    echo("<table>");
    echo("<tr>");
        echo("<td>Name:</td>");

        echo("<td>");
        echo("<input type='text' size='35' name='emergency_name' value=\"" . $editStudent->emergency_name . "\" />");
        echo($changes_array["emergency_name"] ? "<br /><b class='black'>" . $changes_array["emergency_name"] . "</b>" : "");
        echo("</td>");
echo("</tr>\n");

echo("<tr>");
    echo("<td>Relationship:</td>");

    echo("<td>");
    echo("<input type='text' size='35' name='emergency_relationship' value=\"" . $editStudent->emergency_relationship . "\" />");
    echo($changes_array["emergency_relationship"] ? "<br /><b class='black'>" . $changes_array["emergency_relationship"] . "</b>" : "");
    echo("</td>");
echo("</tr>\n");

echo("<tr>");
    echo("<td>Home Phone:</td>");

    echo("<td>");
    echo("<input type='text' size='35' name='emergency_home_phone' value=\"" . $editStudent->emergency_home_phone . "\" />");
    echo($changes_array["emergency_home_phone"] ? "<br /><b class='black'>" . $changes_array["emergency_home_phone"] . "</b>" : "");
    echo("</td>");
echo("</tr>\n");

echo("<tr>");
    echo("<td>Work Phone:</td>");
    echo("<td>");
    echo("<input type='text' size='35' name='emergency_work_phone' value=\"" . $editStudent->emergency_work_phone . "\" />");
    echo($changes_array["emergency_work_phone"] ? "<br /><b class='black'>" . $changes_array["emergency_work_phone"] . "</b>" : "");
    echo("</td>");
echo("</tr>\n");

echo("<tr>");
    echo("<td>Home Fax:</td>");

    echo("<td>");
    echo("<input type='text' size='35' name='emergency_home_fax' value=\"" . $editStudent->emergency_home_fax . "\" />");
    echo($changes_array["emergency_home_fax"] ? "<br /><b class='black'>" . $changes_array["emergency_home_fax"] . "</b>" : "");
    echo("</td>");
echo("</tr>\n");

echo("<tr>");
    echo("<td>Work Fax:</td>");

    echo("<td>");
    echo("<input type='text' size='35' name='emergency_work_fax' value=\"" . $editStudent->emergency_work_fax . "\" />");
    echo($changes_array["emergency_work_fax"] ? "<br /><b class='black'>" . $changes_array["emergency_work_fax"] . "</b>" : "");
    echo("</td>");
echo("</tr>\n");

echo("<tr>");
    echo("<td>E-mail:</td>");

    echo("<td>");
    echo("<input type='text' size='35' name='emergency_email' value=\"" . $editStudent->emergency_email . "\" />");
    echo($changes_array["emergency_email"] ? "<br /><b class='black'>" . $changes_array["emergency_email"] . "</b>" : "");
    echo("</td>");
echo("</tr>\n");

echo("</table>");
echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<a name='notes'></a>");

echo("<table width='100%' cellspacing='1'>");
echo("<tr>");
    echo("<td>");
    echo("<h4>Notes:</h4>");
    echo("</td>");

    echo("<td align='right'>");
    echo("<a href='#top'>Top</a>");
    echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<table border='0' width='100%' class='row1'>");
echo("<tr class='row1'>");
    echo("<td>");
    echo("<table width='100%' border='0' class='row1'>");
    echo("<tr>");
        echo("<td align='center' class='row1'>");
        echo("<b class='black'>Enter New Note Here:&nbsp;&nbsp;</b>");
        echo("</td>");

        echo("<td class='row1'>");
    echo("<textarea name='new_note' cols='80' rows='4'>" . $editStudent->new_note . "</textarea>");
    echo("</td>");
echo("</tr>\n");

if ($editStudent->note_ids)
{
    foreach ($editStudent->note_ids AS $ni)
    {
            echo("<tr><td colspan='2'><hr /></td></tr>\n");
            $sql = ("
                    SELECT DISTINCT *
                    FROM student_notes
                    WHERE student_notes_id='" . $ni . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            $contact_name = getContactName($row["contact_id"]);
            $contact_name = $contact_name["first_name"] . " " . $contact_name["last_name"];

            echo("<tr>");
                    echo("<td align='center'><b class='black'>Delete this note</b>&nbsp;&nbsp;");
                    echo("<input type='checkbox' name='delete_note_ids[]' value='" . $row["student_notes_id"] . "' ");
                    echo(($editStudent->delete_note_ids && in_array($row["student_notes_id"], $editStudent->delete_note_ids)) ? "checked='checked'" : "");
                    echo("/>");
                    echo((is_array($changes_array["delete_note_ids"]) && in_array($row["student_notes_id"], $changes_array["delete_note_ids"])) ? "<br /><b class='black'>(Deleted)</b>" : "");
                    echo("</td>");

                    echo("<td align='left'>");
                    if($contact_name!=" "){ 
                        echo("Note entered by: <span class='darktext'>" . $contact_name . "</span><br />");
                    }
                    if($contact_name==" "){
                        echo("Note entered by: <span class='darktext'>&nbsp;</span><br />"); 
                    }
                        echo("Note entered on: <span class='darktext'>" . $row["date_entered"] . "</span><br />");
                        echo("Note: " . str_replace("\n", "<br />", $row["notes"]));
                        echo("</td>");
                echo("</tr>\n");
        }
    }

    echo("</table>");
    echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("<br />");

echo("<table border='0' width='100%' class='row1'>");
echo("<tr class='row1'>");
    echo("<td>");
    echo("<table width='100%' border='0' class='row1'>");

    echo("<tr>");
        echo("<td align='center' colspan='2'>");
        echo("<input type='submit' name='continue' value='Save Changes' />");
        echo("</td>");
    echo("</tr>\n");

    echo("</table>");
    echo("</td>");
echo("</tr>\n");
echo("</table>");

echo("</form>");
?>
