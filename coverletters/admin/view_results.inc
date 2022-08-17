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
 | Filename: view_results.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: Displays a list of all the coverletters for the selected        |
 | student(s)                                                                   |
 +------------------------------------------------------------------------------+

 //:TODO: This file's pagination is inefficient as it makes quite a few calls to the database using essentially the same query. 
*/
?>
<script type='text/javascript' language='javascript'>
<!--javascript

function load_order(order)
{
        document.form_order.order_by.value=order;
        document.form_order.submit();
}
//-->
</script>
<?php
$show = 1;
$errorset = 0;

echo("<table width='100%'>");
echo("<tr>");
echo("<td>");
/*
	Search for Coverletters...
	Variables sent by view_main.inc are : student_name, student_num, s_view

	Once it finds items, it compares the job to the specialties of the department.. if they dont match it is not displayed.
*/
    if ($departments_in_group) {
            $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
            $department_in_str = "(" . $department_in_str . ")";
    } else {
            $department_in_str = "('$department_id')";
    }

	if ($student_num2 != "") 
    {
		$student_num = $student_num2;
        // Get record_id for use in quickmenu
        $sql_get_recordid = "Select distinct record_id from student_department
            WHERE student_number='".$student_num."' and department_id='".$auth->department."'";
        $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
        if ($result_get_recordid->RecordCount()==1)
        {
            $row_get_recordid = $result_get_recordid->FetchRow();
            $record_id = $row_get_recordid['record_id'];
        } elseif ($result_get_recordid->RecordCount()==0) {
            $sql_get_recordid2 = "Select distinct record_id from student_department
                WHERE student_number='".$student_num."' and department_id IN ".$department_in_str."";
            $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
            if ($result_get_recordid2->RecordCount()==1)
            {
                $row_get_recordid2 = $result_get_recordid2->FetchRow();
                $record_id = $row_get_recordid2['record_id'];
            }
        }
	}
    else
    {
        if ($student_num != "") {
            // Get record_id for use in quickmenu
            $sql_get_recordid = "Select distinct record_id from student_department
                WHERE student_number='".$student_num."' and department_id='".$auth->department."'";
            $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
            if ($result_get_recordid->RecordCount()==1)
            {
                $row_get_recordid = $result_get_recordid->FetchRow();
                $record_id = $row_get_recordid['record_id'];
            } elseif ($result_get_recordid->RecordCount()==0) {
                $sql_get_recordid2 = "Select distinct record_id from student_department
                    WHERE student_number='".$student_num."' and department_id IN ".$department_in_str."";
                $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
                if ($result_get_recordid2->RecordCount()==1)
                {
                    $row_get_recordid2 = $result_get_recordid2->FetchRow();
                    $record_id = $row_get_recordid2['record_id'];
                }
            }
        }
        else
        {
            if ($student_name != "")
            {
                $student_name = trim($student_name);
                if (preg_match("/,.*,/", $student_name))
                {
                    $errorset = 1;
                    error("You may only enter one comma in the student name field.");
                }
                elseif ($student_name == ",")
                {
                    $errorset = 1;
                    error("You must include a partial last name to view coverletters based on the name field.");
                }
                else
                {
                    // Strip wild card character from student name.
                    $student_names = explode(",", $student_name);
                    $first_name = trim($student_names[1]);
                    $last_name = trim($student_names[0]);

                    // Get record_id for use in quickmenu
                    $sql_get_students="SELECT distinct s.student_number from student as s, student_department as sd
                        WHERE s.first_name like '".$first_name."%' and s.last_name like '".$last_name."%'
                        AND s.student_number=sd.student_number AND sd.department_id IN ".$department_in_str."";
                    $result_get_students=$GLOBALS['dbh']->Execute($sql_get_students);
                    if ($result_get_students->RecordCount()==1)
                    {
                        $row_get_students = $result_get_students->FetchRow();
                        $student_number = $row_get_students['student_number'];
                        $sql_get_recordid = "Select distinct record_id from student_department
                            WHERE student_number='".$student_number."' and department_id='".$auth->department."'";
                        $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
                        if ($result_get_recordid->RecordCount()==1)
                        {
                            $row_get_recordid = $result_get_recordid->FetchRow();
                            $record_id = $row_get_recordid['record_id'];
                        } elseif ($result_get_recordid->RecordCount()==0) {
                            $sql_get_recordid2 = "Select distinct record_id from student_department
                                WHERE student_number='".$student_number."' and department_id IN ".$department_in_str."";
                            $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
                            if ($result_get_recordid2->RecordCount()==1)
                            {
                                $row_get_recordid2 = $result_get_recordid2->FetchRow();
                                $record_id = $row_get_recordid2['record_id'];
                            }
                        }
                    }
                }
            } else {
                $first_name = "";
                $last_name = "";
            }
        }
    }

    if ($student_num != "") {

        $sql="Select student_number from student_department where student_number='".$student_num."' AND department_id IN ".$department_in_str;
        
        $results=$GLOBALS['dbh']->Execute($sql);

        if ($results->RecordCount()==0){
            error("You do not have permission to view this student's cover letters.");
            $student_num=0;
            $errorset=1;
        }
    }

    if ($term_year_submit != 'Go' && !$year && !$term)
    {
        $nextTermYear = getNextTermYear();
        $term = $nextTermYear["term"];
        $year = $nextTermYear["year"];
    }

if ($errorset == '0')
{
    //Setting up query
    if($order_by)
    {
        if($order_by == "company_name")
        {
            $select_sql = "SELECT ec.company_name, CONCAT(s.first_name, ' ', s.last_name) as student_name, c.coverletter_id AS coverletter_id, s.student_number,
            j.job_code, a.job_id, ct.description as type, ed.department_name, j.position_title, DATE_FORMAT(c.last_modified, '%b %d, %Y') as last_modified ";
        }
        elseif($order_by == "last_modified")
        {
            $select_sql = "SELECT DATE_FORMAT(c.last_modified, '%b %d, %Y') as last_modified, CONCAT(s.first_name, ' ', s.last_name) as student_name,
            c.coverletter_id AS coverletter_id, s.student_number, j.job_code, a.job_id, ct.description as type, ec.company_name, ed.department_name,
            j.position_title";
        }
        if($order_by == "student_name")
        {
            $select_sql = "SELECT CONCAT(s.first_name, ' ', s.last_name) as student_name, c.coverletter_id AS coverletter_id, s.student_number,
            j.job_code, a.job_id, ct.description as type, ec.company_name, ed.department_name, j.position_title, DATE_FORMAT(c.last_modified, '%b %d, %Y') as last_modified ";
        }
        else
        {
            $select_sql = "SELECT $order_by, c.coverletter_id AS coverletter_id, s.student_number, CONCAT(s.first_name, ' ', s.last_name) as student_name,
            j.job_code, a.job_id, ct.description as type, ec.company_name, ed.department_name,
            j.position_title, DATE_FORMAT(c.last_modified, '%b %d, %Y') as last_modified ";
        }
    }
    else
    {
        $select_sql = "SELECT CONCAT(s.first_name, ' ', s.last_name) as student_name, c.coverletter_id AS coverletter_id, s.student_number,
        j.job_code, a.job_id, ct.description as type, ec.company_name, ed.department_name,
        j.position_title, DATE_FORMAT(c.last_modified, '%b %d, %Y') as last_modified ";
    }

    $from_sql = "FROM student as s 
                LEFT JOIN coverletter as c ON s.student_number = c.student_number 
				LEFT JOIN applications as a ON a.coverletter_id = c.coverletter_id
				LEFT JOIN coverletter_type as ct ON ct.type_id = c.type_id 
				LEFT JOIN job_info as j ON j.job_id = a.job_id
                LEFT JOIN employer_company as ec ON ec.employer_id=j.employer_id
                LEFT JOIN employer_department as ed ON ed.department_id=j.employer_department_id
				LEFT JOIN student_department as sd on sd.student_number=s.student_number
			WHERE
			s.first_name like '".$first_name."%' AND
			s.last_name like '".$last_name."%' AND
			s.student_number like '".$student_num."%' AND 
			c.job_id IS NOT NULL AND 
            sd.department_id IN ".$department_in_str." AND
		    (ct.type_id IN ('2', '3') OR (ct.type_id = '1' and j.term_id='$term' and j.year='$year')) ";

    if($order_by)
    {
        if($order_by == "last_modified")
        {
            $order_by = "c.last_modified";
        }
                                                    
        if(($lastorder == $order_by) && ($data_flow == "ASC"))
        {
            $data_flow = "DESC";
        }
        elseif(($lastorder == $order_by) && ($data_flow == "DESC"))
        {
            $data_flow = "ASC";
        }
        elseif(!$data_flow)
        {
            $data_flow = "DESC";
        }
        if($order_by == "student_name")
        {
            $order_by = "s.last_name $dataflow, s.first_name ";
        }

        $order_sql = "ORDER BY $order_by $data_flow, ct.type_id DESC ";
    }
    else
    {
        $order_sql = "ORDER BY ct.type_id DESC, ec.company_name ";
        $order_by = "company_name";
        $data_flow = "ASC";
    }


    $sql = $select_sql." ".$from_sql." ".$order_sql;
    //The form that chooses the term
    echo("<table width='100%'>");
        echo("<tr>");
            echo("<td>
            <h3 align='left'>Student Information - Cover Letters</h3></td>\n");
           // Display the quickmenu
            echo("<td align='right'>");
            if ($record_id) {
                include('student_info/student_quickmenu.inc');
            }
            else {
                echo("&nbsp;");
            }
            echo("</td>\n");
        echo("</tr>");
    echo("</table>");
           
    echo("<form method='post' action='" . $PHP_SELF . "'>");
    
    echo("<table class='row1' cellspacing='0' cellpadding='3' border='0'>");
    
        echo("<tr>");
        echo("<td>");
        echo("<input type='hidden' name='select' value='coverletter' />");
        echo("<input type='hidden' name='page' value='View Cover Letters' />");
        echo("<input type='hidden' name='student_num' value='".$student_num."' />");
        echo("<input type='hidden' name='student_num2' value='".$student_num2."' />");
        echo("<input type='hidden' name='record_id' value='".$record_id."' />");
    $last_name=stripslashes($last_name);
    $first_name=stripslashes($first_name);
        if ($last_name != '') {
            echo("\n<input type='hidden' name='student_name' value='".$last_name.",".$first_name."' />");
        }
        
        echo("<b class='black'>Term: </b>&nbsp;</td>");
            echo("<td>");
            echo("<select name='term'>");
            $sqlterm = "SELECT term_id,term_name,year_order
                FROM term
                ORDER BY year_order";
            $sqlterm_result = $GLOBALS['dbh']->Execute($sqlterm);
            while ($sqltermrow = $sqlterm_result->FetchRow()) {
                if ($term == $sqltermrow['term_id']) {
                    echo("<option value='".$sqltermrow['term_id']."' selected='selected'>".$sqltermrow['term_name']."</option>");
                } else {
                    echo("<option value='".$sqltermrow['term_id']."'>".$sqltermrow['term_name']."</option>");
                }
            }
            echo("</select>");
            echo("<select name='year'>");
            $sqlyear = "SELECT min(ji.year) AS min_year, max(ji.year) AS max_year
                FROM job_info as ji, applications as a 
                WHERE ji.year != '0'
                AND ji.job_id=a.job_id
                ";
            $sqlyear_result = $GLOBALS['dbh']->Execute($sqlyear);
            $year_row = $sqlyear_result->FetchRow();
            $end_year = $year_row['max_year'];

            for ($i=$year_row['min_year'];$i<=$end_year;$i++)
            {
                $selected="";
                if ($year==$i){
                    $selected="selected='selected'";
                }
                echo("<option $selected value='$i'>$i</option>");
                $selected="";
            }
            echo("</select>");
            echo("</td>");
            echo("<td>");
            echo("<input type='submit' name='term_year_submit' value='Go' />");
            echo("</td>");
        echo("</tr>
        </table>");
        echo("</form>");


        //paging form
        $results = $GLOBALS['dbh']->Execute($sql);
        $num_jobs = $results->RecordCount();
        if ($num_jobs == 0) {
            notify("No Coverletters found.\n");
        }
        echo("<h4>Choose a Cover Letter($num_jobs):</h4>");
        if ($jobs_per_page == "") { $jobs_per_page = 20; }
        if ($jobs_per_page < 5) { $jobs_per_page = 5; }
        if ($job_page == '') { $job_page = 0; }

        // display the thingy if necessary
        if ($num_jobs > 20) {

            $num_pages = ceil($num_jobs / $jobs_per_page);

            $job_pos_start = $job_page * $jobs_per_page;
            $job_pos_end = $job_pos_start + $jobs_per_page;

            $sql .= " LIMIT $job_pos_start, $jobs_per_page";

            unset($PDF);
            echo ("\n<form method='post' action='$PHP_SELF'>");
            echo ("\n<input type='hidden' name='select' value='coverletter' />");
            echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
            echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
            echo ("\n<input type='hidden' name='page' value='View Cover Letters' />");
            echo ("\n<input type='hidden' name='student_num' value='".$student_num."' />");
            echo ("\n<input type='hidden' name='student_num2' value='".$student_num2."' />");
            echo ("\n<input type='hidden' name='record_id' value='".$record_id."' />");
            echo ("\n<input type='hidden' name='term' value='".$term."' />");
            echo ("\n<input type='hidden' name='year' value='".$year."' />");
            if ($last_name != '') {
                echo("\n<input type='hidden' name='student_name' value='".$last_name.",".$first_name."' />");
            }

            echo ("\n<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

                echo ("\n<tr>");
                    echo ("\n<td valign='bottom'>");
                    $page_number = $job_page + 1;
                    echo ($num_jobs . " cover letter" . (($num_jobs != 1) ? "s" : "") . " on " . $num_pages . " page" . (($num_pages > 1) ? "s" : "") .
                    ": ");
                    echo ("\n</td>");
                    echo ("\n<td valign='bottom'>");
                    echo ("<input type='text' name='jobs_per_page' size='3' value='$jobs_per_page' /> cover letters per page");
                    echo ("\n</td>");
                echo ("\n</tr>");

                echo ("\n<tr>");
                    echo ("\n<td>");
                    echo ("\n<select name='job_page'>");

                    for ($j = 0; $j < $num_pages; $j ++) 
                    {

                        $from_pos = $j * $jobs_per_page;
                        $to_pos = $from_pos + $jobs_per_page - 1;
                        if ($to_pos >= $num_jobs) $to_pos = $num_jobs - 1;

                        $results->Move($from_pos);
                        $from_row = $results->FetchRow();
                        $from = $from_row[0];

                        $results->Move($to_pos);
                        $to_row = $results->FetchRow();
                        $to = $to_row[0];

                        // special cases to concatenate the strings
                        if ($order == 'term_year') 
                        {
                            $from = substr($from, 0, 2) . ". " . substr($from, -4, 4);
                            $to = substr($to, 0, 2) . ". " . substr($to, -4, 4);
                        }
                        elseif ($order == 'h.department_id')
                        {
                            $from = getDepartmentName($from);
                            $to = getDepartmentName($to);
                        }
                        else 
                        {
                            if (strlen($from) > 12) { $from = substr($from, 0, 12) . "..."; }
                            if (strlen($to) > 12) { $to = substr($to, 0, 12) . "..."; }
                        }

                        echo ("\n<option value='$j'". (($j == $job_page) ? " selected='selected'" : "") .">");
                        echo ((($from == '') ? "n/a" : $from) ." to  ". (($to == '') ? "n/a" : $to));
                        echo ("\n</option>");
                    }
                    echo ("\n</select>");
                    echo ("\n</td>");

                    echo ("\n<td align='left'>");
                    echo ("\n<input type='submit' value='View' />");
                    echo ("\n</td>");
                echo ("\n</tr>");
            echo ("\n</table>");
            echo ("\n</form>");
        }

        $result = $GLOBALS['dbh']->Execute($sql);

        // Start the main table that contains all the display elements below. 
        echo("<form method='post' action='$PHP_SELF'>");
        echo("<table cellspacing='0' cellpadding='0' border='0'>");
        echo("<tr>");
        echo("<td>");

        if ($result->RecordCount() != 0) {

            // arrows
            if ($num_jobs > 20) {
                echo ("<table cellspacing='0' cellpadding='0' border='0' width='100%'>");
                echo ("<tr>");
                echo ("<td align='left'>");

                if ($job_page > 0) {
                    echo("<input type='button' value='&lt;' onclick='javascript:document.prev_form.submit()' />");
                }

                else {
                    echo ("&nbsp;");
                }

                echo ("\n</td>");
                echo ("\n<td align='right'>");

                if ($job_page < ($num_pages - 1)) {
                    echo("<input type='button' value='&gt;' onclick='javascript:document.next_form.submit()' />");
                }

                else {
                    echo ("&nbsp;");
                }

                echo ("</td>");
                echo ("</tr>");

                echo("</table>");
            }
        } // end if ($result->RecordCount() != 0)

         echo ("
                 <table border='1' cellpadding='0' cellspacing='0'>
                 <tr>
                 <td>
                 <table border='0' cellpadding='2'>
                 <tr>
                <td class='rowgrey' align='center'>&nbsp;</td>
                 <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"student_name\")'><b class='white'>&nbsp;Student Name&nbsp;</b></a></td>
                 <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"j.job_code\")'><b class='white'>&nbsp;Job Code&nbsp;</b></a></td>
                 <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"ec.company_name\")'><b class='white'>&nbsp;Company&nbsp;</b></a></td>
                 <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"j.position_title\")'><b class='white'>&nbsp;Position Title&nbsp;</b></a></td>
                 <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"ct.description\")'><b class='white'>&nbsp;Type&nbsp;</b></a></td>
                 <td class='rowgrey' align='center'><a class='orderable' href='javascript:load_order(\"last_modified\")'><b class='white'>&nbsp;Last Modified&nbsp;</b></a></td>
                 </tr>
                 "); 
            
            $get_coverletters = $GLOBALS['dbh']->Execute($sql);
			$numrows = $get_coverletters->RecordCount();
			
            while (($row = $get_coverletters->FetchRow())) {
					$class = ($j%2 == 0) ? "row1d" : "row0d";
					$j++;
						
					echo("\n<tr>");
					echo("\n\t<td class='".$class."' align='center' nowrap='nowrap'><input type='Radio' name='coverletter_id' value='".$row["coverletter_id"]."' /></td>");
					echo("\n\t<td class='".$class."' nowrap='nowrap'>&nbsp;".$row["student_name"]."</td>");
					echo("\n\t<td class='".$class."' nowrap='nowrap'>&nbsp;".$row["job_code"]."</td>");

                    if ($row["company_name"] == $row["department_name"])
                    {
					    echo("\n\t<td class='".$class."' nowrap='nowrap'>&nbsp;".$row['company_name']."</td>");
                    } else {
					    echo("\n\t<td class='".$class."'>&nbsp;".$row['company_name']." (".$row['department_name'].")</td>");
                    }

					echo("\n\t<td class='".$class."'>&nbsp;".$row["position_title"]."</td>");
					echo("\n\t<td class='".$class."' nowrap='nowrap'>&nbsp;".$row["type"]."</td>");
					echo("\n\t<td class='".$class."' nowrap='nowrap'>&nbsp;".$row["last_modified"]."</td></tr>");
			}

			if ($j == 0) {
				echo 	"<tr>
						<td class='row0d' colspan='7' align='center' valign='middle'><br />
							No Matches For Your Search Criteria, please <a href='$PHP_SELF&amp;select=coverletter&amp;page=view'>try again</a>.<br />&nbsp;	
						</td>
					</tr>";
			} 
?>

			</table>
		</td>
    </tr>
    </table>
    </td></tr></table>
    <br />
    <input type="hidden" value="<?php echo $department_id?>" name="department_id" />
    <input type="hidden" value="<?php echo $last?>" name="last" />
    <input type="hidden" value="<?php echo $s_view?>" name="s_view" />
    <input type="hidden" value="<?php echo $student_num?>" name="student_num_passthru" />
    <input type="hidden" value="1" name="PDF" />
    <input type="hidden" value="coverletter" name="select" />
    <input type="hidden" value="<?php echo($record_id)?>" name="record_id" />
    <input type="hidden" value="<?php echo($term)?>" name="term" />
    <input type="hidden" value="<?php echo($year)?>" name="year" />
    <input type="hidden" value="<?php echo($term_year_submit)?>" name="term_year_submit" />

    <?php

    if ($j != 0)
    {
        echo("<table class='row1' cellspacing='0' cellpadding='5' border='0'>");
        echo("<tr>");
        echo("<td>");
        echo("<input type='submit' value='View HTML' name='page' />&nbsp;&nbsp;");
        echo("<input type='submit' value='View PDF' name='page' />&nbsp;&nbsp;");
        echo("<input type='submit' value='Edit' name='page' />");
        echo("</td>");
        echo("</tr>");
        echo("</table>");
    }
    echo("</form>");
    echo("</td></tr></table>");
}

// start form_order
echo ("<form name='form_order' action='".$PHP_SELF."' method='post'>");
echo ("\n<input type='hidden' name='select' value='coverletter' />");
echo ("\n<input type='hidden' name='order_by' value='' />");
echo ("\n<input type='hidden' name='page' value='View Cover Letters' />");
echo ("\n<input type='hidden' name='lastorder' value='".$order_by."' />");
echo ("\n<input type='hidden' name='data_flow' value='".$data_flow."' />");
echo ("\n<input type='hidden' name='student_num' value='".$student_num."' />");
echo ("\n<input type='hidden' name='student_num2' value='".$student_num2."' />");
echo ("\n<input type='hidden' name='term' value='".$term."' />");
echo ("\n<input type='hidden' name='year' value='".$year."' />");
if ($last_name != '') {
    echo("<input type='hidden' name='student_name' value='".$last_name.",".$first_name."' />");
}
echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
echo ("<input type='hidden' name='job_page' value='". $job_page ."' />");
echo ("</form>");
// end form_order

// start prev_form
echo ("\n<form method='post' name='prev_form' action='$PHP_SELF'>");
echo ("\n<input type='hidden' name='select' value='coverletter' />");
echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
echo ("\n<input type='hidden' name='page' value='View Cover Letters' />");
echo ("\n<input type='hidden' name='student_num' value='".$student_num."' />");
echo ("\n<input type='hidden' name='student_num2' value='".$student_num2."' />");
echo ("\n<input type='hidden' name='record_id' value='".$record_id."' />");
echo ("\n<input type='hidden' name='term' value='".$term."' />");
echo ("\n<input type='hidden' name='year' value='".$year."' />");
if ($last_name != '') {
    echo("<input type='hidden' name='student_name' value='".$last_name.",".$first_name."' />");
}
echo ("\n<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
echo ("\n<input type='hidden' name='job_page' value='". ($job_page - 1) ."' />");
echo ("\n</form>");
// end prev_form

// start next_form
echo ("\n<form method='post' name='next_form' action='$PHP_SELF'>");
echo ("\n<input type='hidden' name='select' value='coverletter' />");
echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
echo ("\n<input type='hidden' name='page' value='View Cover Letters' />");
echo ("\n<input type='hidden' name='student_num' value='".$student_num."' />");
echo ("\n<input type='hidden' name='student_num2' value='".$student_num2."' />");
echo ("\n<input type='hidden' name='record_id' value='".$record_id."' />");
echo ("\n<input type='hidden' name='term' value='".$term."' />");
echo ("\n<input type='hidden' name='year' value='".$year."' />");
if ($last_name != '') {
    echo("<input type='hidden' name='student_name' value='".$last_name.",".$first_name."' />");
}
echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
$next_page = $job_page + 1;
echo ("<input type='hidden' name='job_page' value='". $next_page ."' />");
echo ("</form>");
// end next_form
?>
