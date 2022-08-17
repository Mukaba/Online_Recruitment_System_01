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
 | Filename: semesters_form.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This is the main form loaded up by edit_student.inc.  It        |
 | displays all of the student's inputs to the user, and collects any changes   |
 | they wish to make.                                                           |
 +------------------------------------------------------------------------------+

*/

// Select color scheme wanted for the user
$department_id = $auth->department;

//Find out info to fill out the forms

//Determine all the different semesters available for this department
$sql = ("
    SELECT DISTINCT a.display, a.semesters_id
    FROM semesters AS a, department_semesters AS b
    WHERE b.department_id = '".$department_id."' 
    AND b.semesters_id = a.semesters_id
    ORDER BY display
    ");

$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
    $semesters_row[] = $row;
}

// Determine number of terms and the termcodes
$sql = ("
    SELECT DISTINCT term_code
    FROM term
    ORDER BY order_by
    ");

$result = $GLOBALS['dbh']->Execute($sql);
$num_terms = 0;

while ($row = $result->FetchRow())
{
    $term_row[] = $row;
    $num_terms++;
}

//Get Saved Info from database
$sql = ("
    SELECT timetable_num AS tt, year_num AS y, term_id AS t, semesters_id AS s 
    FROM department_default_semesters
    WHERE department_id = '$department_id'
    ");

$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
    $saved_row[] = $row;
}

//Convert results into a 3 degree array (is that the proper terminology?)
if($saved_row)                     //if we had any results
{
    foreach( $saved_row as $s_r )  //for each result from the query
    {
        $temp_tt = $s_r["tt"];
        $temp_year = $s_r["y"];
        $temp_term = $s_r["t"];

        $default_semesters[$temp_tt][$temp_year][$temp_term] = $s_r["s"];
    }
}

//Start displaying the timetables popup
echo("<table class='row1' cellspacing='0' cellpadding='0' height='80%' width='100%'>\n");

echo("<tr>\n");
    echo("<td>");
    echo("<form method='post' action='mamook.php?select=defaultsemesterspopup&no_headers=1'>");
    echo("<input type='hidden' name='num_terms' value=$num_terms />");
    echo("<center><br /><h3>Edit Default Time Tables</h3>\n");
    echo("<h4>".getDepartmentName($department_id)."</h4>\n<hr />");
     
    for($tt_count = 0; $tt_count <= 2; $tt_count++)                                                                   //create each timetable
    {
        echo("<table class='row1'>\n");
        
        echo("<tr>\n");
            echo("<td>");
                echo("<center><b class='black'>Default Time Table ".($tt_count+1)."&nbsp;&nbsp;&nbsp;</b></center>");  //give this timetable a title
            echo("</td>");
            
            echo("<td>");
                echo("<table class='row1'>\n");
                
                echo("<tr>\n");
                    foreach($term_row as $t_r)
                    {
                        echo("<td><center><b class='black'>" . $t_r["term_code"] . "</b></center></td>");             //display termcodes across the top
                    }
                echo("</tr>\n");
                
                for($year_count = 0; $year_count <= 5; $year_count++)                                                 //display 6 years worth of terms
                {
                    echo("<tr>\n");
                        for($term_count = 0; $term_count <= $num_terms-1; $term_count++)                              //for each term display a selection box
                        { 
                            $temp_semester = $default_semesters[$tt_count][$year_count][$term_count];                 //temp variable to make code easier to read
                            
                            echo("<td><select name='default_semesters[$tt_count][$year_count][$term_count]'>");
                            echo("<option value='0' ".( (!$temp_semester || $temp_semester == 0) ? "selected='selected'" : "")."></option>\n");
                            
                            foreach($semesters_row as $s_r)                                                           //fill in each semester type for this dept.
                            {
                                echo("<option value='".$s_r["semesters_id"]."' ");
                                
                                //check if previously set (in the db) if so then set this particular option as the selected value (i.e. restore the saved values)
                                echo( ($s_r["semesters_id"] == $temp_semester ? "selected='selected'" : "") );

                                //display the semester with an abbreviation rather than its numerical code
                                echo(">".getSemesterCode($s_r["semesters_id"])."</option>\n");
                            }
                            echo("</select></td>");
                        }
                    echo("</tr>\n"); 
                }
                
                echo("</table>\n");
            echo("</td>");
        echo("</tr>\n");
        
        echo("</table>\n");
    }
    
    echo("<hr /><input type='submit' name='semesters_update' value='Save' />");                                         //display save button
    echo("&nbsp; &nbsp; &nbsp;<input type='submit' name='semesters_update' value='Cancel' /></center>");                //display cancel button
    echo("</form>");
    echo("</td>");
echo("</tr>\n");

echo("</table>\n");
