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
| Filename: edit_student_form_faculty.inc                                      |
+------------------------------------------------------------------------------+
| Description: This is the main form loaded up by edit_student.inc for         | 
| FACULTY userlevel access. It displays all of the student's inputs to the     |
| user, and collects any changes they wish to make.                            |
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

echo("<form method='post' action='" . $PHP_SELF . "' ENCTYPE='multipart/form-data' name='myform'>");
echo("<input type='hidden' name='select' value='edit_student' />");
echo("<input type='hidden' name='record_id' value='" . $record_id . "' />");
echo("<input type='hidden' name='editStudent' value='" . $editStudent->packObject() . "' />");
echo("<input type='hidden' name='checkChanges' value='" . $checkChanges->packObject() . "' />");
echo("<a name='student_information'></a>");

// Show student information.
echo("<table border='0' width='100%' cellspacing='1'>");
echo("<tr>");
echo("<td>");
echo("<h4>Student Information:</h4>");
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
            echo($editStudent->first_name);
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Last Name:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo($editStudent->last_name);
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Middle Name:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo($editStudent->middle_name);
            echo("</td>");
        echo("</tr>\n");

        echo("<tr>");
            echo("<td>");
            echo("<b class='black'>Preferred Name:&nbsp;&nbsp;</b>");
            echo("</td>");

            echo("<td>");
            echo($editStudent->preferred_name);
            echo("</td>");
        echo("</tr>\n");

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
echo("</tr>\n");
echo("</table>");

echo("<table border='0' width='100%' class='row1'>");
echo("<tr class='row1'>");
    echo("<td>");
    echo("<table width='100%'CLASS='row1'>");
    echo("<tr>");
        echo("<td>");
        if($editStudent->start_year != NULL)
        {
            echo("<b class='black'>Note: Clearing the start year will erase all of the entries in the table below</b><br />");
        }
        echo("<b class='black'>Start Year:</b>");
        echo("&nbsp;&nbsp;<input type='text' name='start_year' value=\"" . $editStudent->start_year . "\" />");
        echo($changes_array["start_year"] ? "<br /><b class='black'>" . $changes_array["start_year"] . "</b>" : "");

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

            /* 
             Get all the semesters that the student already is using along with any that the department is allowed to set.
             Then, use these as options for our select menu.
            */

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
            $nextYear = $editStudent->start_year;        //the first year on the calendar
            $nextTerm = $term_start;                     //the first term on the calendar

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
            $timetable_max = 0;                //used to keep track of how many default timetables we have.
            if($saved_row)                     //if we had any results
            {
                foreach( $saved_row as $s_r )  //for each result from the query
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
            for ( $year_count =  0; $year_count <= 5; $year_count++)
            {
                for ( $term_count = 0; $term_count <= $term_limit - 2; $term_count++)
                {
                    $next_term = $next_term_array[ $next_term_year_count ];
                    $next_year = $next_year_array[ $next_term_year_count ];

                    if( is_numeric($editStudent->semesters[$next_year][$next_term]) )
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

            <script type='text/javascript' language='JavaScript'>
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
            // Show six years worth of semesters. NOTE:  This is an ugly bit of code, but the best I could do with what Chris had set up.
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

            // Section for displaying 'Default Time Table' buttons
            echo("<hr />\n");
            echo("<table>\n");
            echo("<tr>\n");
                echo("<td colspan='2'><b class='black'>Default Time Tables</b></td>");
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
    }

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
    echo("<input type='text' size='35' name='emergency_home_phone' value=\"" . $editStudent->emergency_home_phone . "\">");
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

echo("<table width='100%' cellspacing=1>");
echo("<tr>");
    echo("<td>");
    echo("<h4>Notes:</h4>");
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
        echo("<textarea name='new_note' cols='80' rows='4' WRAP>" . $editStudent->new_note . "</textarea>");
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
                if ($row["contact_id"] == $auth->contact_id)
                {
                        echo("<td align='center'><b class='black'>Delete this note</b>&nbsp;&nbsp;");
                        echo("<input type='checkbox' name='delete_note_ids[]' value='" . $row["student_notes_id"] . "' ");
                        echo(($editStudent->delete_note_ids && in_array($row["student_notes_id"], $editStudent->delete_note_ids)) ? "CHECKED" : "");
                        echo(" />");
                        echo((is_array($changes_array["delete_note_ids"]) && in_array($row["student_notes_id"], $changes_array["delete_note_ids"])) ? "<br /><b class='black'>(Deleted)</b>" : "");
                        echo("</td>");
                } else {
                        echo("<td align='center'>&nbsp;</td>");
                }

                        echo("<td align='left'>");
                        echo("Note entered by: <span class='darktext'>" . $contact_name . "</span><br />");
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
