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
 | Filename: show_jobs.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: This file shows all the jobs that the student has applied for   |
 | and shows the jobs they have been shortlisted for even if they haven't       |
 | applied for it                                                               |
 +------------------------------------------------------------------------------+

*/
?>
&nbsp;</td>
<form name="place_by_term" action="<?php echo($PHP_SELF); ?>" method="post">
	    <input type="hidden" name="select" value="placement_search" />
	    <input type='hidden' name='menu_select' value='<?php echo $menu_select?>' />
            <td width='5'>&nbsp;</td>
            <td valign="middle"><b class='black'>Place by:</b>&nbsp;<a href="javascript:document.place_by_term.submit()"><b class="blue">Term&nbsp;</b></a></td>
</form>
<form name='place_by_student' action='<?php echo($PHP_SELF); ?>' method='post'>
	<input type='hidden' name='select' value='placement_by_student' />
	<input type='hidden' name='menu_select' value='<?php echo $menu_select?>' />
	<td valign='middle'>&nbsp;<a href='javascript:document.place_by_student.submit()'><b class='blue'>Student</b></a></td>
</form>

</td><td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>"></td></tr></table>
<p>
<?php
    $department_string2 = $department_string;
    $arrDepartments = explode(",",$department_string);
    $department_string = "('".implode ("','",$arrDepartments)."')";
   
    if ($student_number_select<>NULL && $student_num == NULL){
        if (isValidStudentNumber($student_number_select)) {
            $student_num = $student_number_select;
        }
    }
    elseif ($student_name_select<>NULL && $student_number_select==NULL && $student_num == NULL)
    {
        $arrName = explode(",",$student_name_select);
        $last_name = trim($arrName[0]);
        $first_name = trim($arrName[1]);
        $sql = ("
            SELECT s.student_number 
            FROM student AS s
            INNER JOIN student_department AS sd
            ON sd.student_number = s.student_number
            WHERE s.first_name LIKE '".$first_name."%' AND s.last_name LIKE '".$last_name."%'
            AND sd.department_id IN ".$department_string."
            ");
        
        $results=$GLOBALS['dbh']->Execute($sql);
        if($results->RecordCount() > 1)
        {
            $show_student_list = true;
        }
        elseif($results->RecordCount() == 1)
        {
            $row=$results->FetchRow();
            $student_num=$row['student_number'];
        }
        
    }
   
    if(!$show_student_list)
    {
        if ($student_num != NULL && isValidStudentNumber($student_num))
        {
            $sql_get_recordid = ("
                SELECT DISTINCT record_id 
                FROM student_department
                WHERE student_number='".$student_num."' AND department_id='".$auth->department."'
                ");
            $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);

            if ($result_get_recordid->RecordCount()==1)
            {
                $row_get_recordid = $result_get_recordid->FetchRow();
                $record_id = $row_get_recordid['record_id'];
            } 
            
            elseif ($result_get_recordid->RecordCount()==0) 
            {
                $sql_get_recordid2 = ("
                    SELECT DISTINCT record_id 
                    FROM student_department
                    WHERE student_number='".$student_num."' AND department_id IN ".$department_string."
                    ");
                $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
                if ($result_get_recordid2->RecordCount()==1)
                {
                    $row_get_recordid2 = $result_get_recordid2->FetchRow();
                    $record_id = $row_get_recordid2['record_id'];
                }
            }

            echo("<table width=99% cellpadding='2'>");
            echo("<tr>");
            echo("<td><h3 align='left'>Job Offers - Applications for ".getStudentName($student_num)."</h3></td>\n");

            // Display the quickmenu
            echo("<td align='right'>");
            if ($record_id)
            {
                include('student_info/student_quickmenu.inc');
            }
            echo("</td>\n");
            echo("</tr>");
            echo("</table>");
            ?>
                <h4><?php echo getTermName($term_id)." ".$year." Work Term"?>:</h4>
                <?php

                if ($term_id == NULL || $year == NULL){
                    $nextTermYear = getNextTermYear();

                    $term_id = $nextTermYear["term"];
                    $year = $nextTermYear["year"];
                }
            $sql = "Select ec.company_name, ji.job_code,ss.student_number from job_info as ji,employer_company as ec,applications as a 
                left join students_shortlisted as ss on ss.job_id=ji.job_id and ss.student_number=a.student_number 
                where ji.job_id=a.job_id and ec.employer_id=ji.employer_id and a.student_number='".$student_num."'
                and ji.term_id='".$term_id."' and ji.year='".$year."'";
            $resultstu=$GLOBALS['dbh']->Execute($sql);	

            if ($resultstu->RecordCount()>0){

                // this term_id defaults to next term if none is detected but if one is selected it would switch to that
                //get information for all jobs in selected and all the students who have applied or been shortlisted to the job
                $sql = "select ec.company_name,ji.job_code,ss.job_id as shortlisted,a.job_id as applied, ji.job_id from job_info as ji,
                student as s,term as t,employer_company as ec left join students_shortlisted as ss on ss.job_id=ji.job_id 
                    and ss.student_number=s.student_number left join applications as a on ji.job_id=a.job_id and a.student_number=s.student_number 
                    where ec.employer_id=ji.employer_id and s.student_number=".$student_num." and t.term_id=ji.term_id 
                    and ji.term_id=".$term_id." and ji.year='".$year."' order by ji.job_code";
                $results=$GLOBALS['dbh']->Execute($sql);

                $placed_sql = "select history_id from history where student_number='".$student_num."' and term_id=".$term_id." and year=".$year;
                $placed_results=$GLOBALS['dbh']->Execute($placed_sql);	

                $nonrs_placement = "SELECT history_id, job_id from history where term_id='".$term_id."'
                    AND year='".$year."' 
                    AND student_number='".$student_num."'";
                $nonrs_result = $GLOBALS['dbh']->Execute($nonrs_placement);
                $placed_total = 0;
                if ($placed_results->RecordCount()>0) {
                    $placed_rs = 0;
                    $placed_total = 1;
                    while ($nonrs_row = $nonrs_result->FetchRow()) {
                        if ($nonrs_row['job_id']<>'') {
                            $placed_rs = 1;	
                        }
                    }
                }
                if ($placed_rs==0) {
                    $placed_on_own_sql = "SELECT ec.company_name from employer_company as ec, history as h where h.term_id='".$term_id."'
                        AND h.year='".$year."' AND h.employer_id=ec.employer_id
                        AND h.student_number='".$student_num."'";
                    $placed_on_own_result = $GLOBALS['dbh']->Execute($placed_on_own_sql);
                    while ($placed_own_row=$placed_on_own_result->FetchRow()) {
                        $student_name = getStudentName($student_num);
                        echo("<br />");
                        echo("<b>".$student_name."</b> was placed at <b>".$placed_own_row['company_name']."</b> for this term.");
                    }
                }
                ?>
                    <form action='<?php echo $PHP_SELF?>' method='post'>
                    <input type='hidden' name='select' value='save_placements' />
                    <p>
                    <table border=1 cellpadding=0 cellspacing=0><tr><td>
                    <table border=0 cellpadding=2>
                    <tr class='rowgrey'>
                    <td align=center>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>
                    <td align=center>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
                    <td align=center>&nbsp;<b class='white'>Shortlisted</b>&nbsp;</td>
                    <?php
                    if ($placed_rs == 0 && $placed_total == 1) {
                    }
                    elseif ($placed_rs == 1) {
                        echo("<td align=center>&nbsp;<b class='white'>Placed</b>&nbsp;</td>");
                    } elseif ($placed_total ==0) {
                        echo("<td align=center>&nbsp;<b class='white'>Place</b>&nbsp;</td>");
                    }
                ?>
                    </tr>
                    <?php
                    $count = 0;
                while($row=$results->FetchRow()){
                    //if the student has been shortlisted or applied to the job then show the job
                    if ($row['shortlisted']<>NULL || $row['applied']<>NULL){
                        if ($count%2==0){
                            $class='row1d';
                        }else{
                            $class='row0d';
                        }
                        if ($row['shortlisted']==NULL){
                            $shortlisted = 'NO';
                        }else{
                            $shortlisted = 'YES';
                        }
                        ?>
                            <tr>
                            <td class='<?php echo $class?>' align=center>&nbsp;<?php echo $row['job_code']?>&nbsp;</td>
                            <td class='<?php echo $class?>' align=center>&nbsp;<?php echo $row['company_name']?>&nbsp;</td>
                            <td class='<?php echo $class?>' align=center>&nbsp;<?php echo $shortlisted?>&nbsp;</td>
                            <?php

                            $student_name=getStudentName($student_num);

                        $sql2 = "SELECT ji.num_positions, ji.job_code, ji.term_id,ji.year,ji.employer_id, ji.employer_department_id, ji.job_id, ec.company_name, ed.department_name
                            FROM job_info AS ji, employer_company AS ec, employer_department AS ed
                            WHERE ji.employer_department_id = ed.department_id AND ec.employer_id=ji.employer_id AND ji.job_id='".$row['job_id']."'";
                        $results2=$GLOBALS['dbh']->Execute($sql2);
                        $company_row=$results2->FetchRow();
                        $term_id2=$company_row['term_id'];
                        $year=$company_row['year'];

                        // If we don't have a term and year, we get one.
                        if (($term_id2==NULL || $term_id2==0) && ($year==NULL || $year==0))
                        {
                            $sql3 = "SELECT term_id
                                FROM term
                                WHERE start_date<='".date(m)."' AND end_date >= '".date(m)."'";
                            $results3=$GLOBALS['dbh']->Execute($sql3);
                            $row3=$results3->FetchRow();
                            $term_id2=$row['term_id'];
                            $year=date(Y);
                        }

                        $sql4 = "SELECT count(history_id) as num_records
                            FROM history
                            WHERE job_id=".$row['job_id'];
                        $results4 = $GLOBALS['dbh']->Execute($sql4);
                        $row4=$results4->FetchRow();
                        $check_number=$row4['num_records'];
                        $check_number=$check_number+1;

                        //If the job is not filled
                        if ($check_number<=$company_row['num_positions'])
                        {
                            if ($placed_rs == 0 && $placed_total == 1)
                            {
                            }

                            elseif ($placed_rs==1 || $placed_total==0) {
                                echo("<td class='".$class."' align=center>");
                                if ($placed_results->RecordCount()==0){
                                    ?>
                                        &nbsp;<input type='radio' name='placements[]' value='<?php echo $student_num."&".$row['job_id']?>' />&nbsp;
                                    <?php
                                }else{
                                    $placed_in_company_sql = "SELECT history_id from history where term_id='".$term_id."'
                                        AND year='".$year."' AND job_id='".$row['job_id']."' 
                                        AND student_number='".$student_num."'";
                                    $placed_in_company_result = $GLOBALS['dbh']->Execute($placed_in_company_sql);
                                    if ($placed_in_company_result->FetchRow()>0) {
                                        ?>
                                            <img src='misc/images/smchk_bl.gif'>
                                            <?php
                                    } else {
                                        ?>
                                            &nbsp;
                                        <?php
                                    }

                                }
                                echo("</td>");
                            }
                        }
                        else
                        {
                            $placed_in_company_sql = "SELECT history_id from history where term_id='".$term_id."'
                                AND year='".$year."' AND job_id='".$row['job_id']."'
                                AND student_number='".$student_num."'";
                            $placed_in_company_result = $GLOBALS['dbh']->Execute($placed_in_company_sql);
                            if ($placed_rs == 0 && $placed_total == 1)
                            {
                            }
                            elseif ($placed_in_company_result->FetchRow()>0) {
                                echo("<td class='".$class."' align=center><img src='misc/images/smchk_bl.gif'></td>");
                            }
                            else
                            {
                                echo("<td class='".$class."' align=center> Filled </td>");
                            }

                        }
                        ?>
                            </tr>

                            <?php
                            $count=$count+1;
                    }
                }
                if ($placed_results->RecordCount()==0) {
                    ?>
                        <tr>
                        <td colspan=4 class='<?php echo(($count % 2) ? "row0d" : "row1d"); ?>'>
                        <center><input type='submit' name='btnSubmit' value='Submit' /></center>
                        </td>
                        </tr>
                        <?php
                }
                ?>
                    </table>
                    </td></tr></table>
                    </form>
                    <?php
            }else{
                notify('This student has not applied to any jobs this term.');
            }
         }
         else
         {
            notify('Student Not Found');
         }
    }
    else
    {

        echo("<form method='post' action='" . $PHP_SELF . "&amp;select=placement_choose'>");
        echo("<input type='hidden' name='continue' value='Chosen Student' />");
        echo("<input type='hidden' name='first_name' value='".$first_name."' />");
        echo("<input type='hidden' name='last_name' value='".$last_name."' />");
        echo("<input type='hidden' name='student_name' value='".$student_name."' />");
        echo("<input type='hidden' name='order_by' value='".$order_by."' />");
        echo("<h3 align='left'>Job Offers - Place by Student</h3>");

        // Get a string of the department ids that belong to the user's group.
        if (sizeof($departments_in_group) > 1)
        {
                $dept_string = "'" . implode("', '", $departments_in_group) . "'";
        }
        else
        {
                $dept_string = "'" . $auth->department . "'";
        }

        $msg = ("Multiple students have been returned that match the name you have typed in.  Please choose the");
        $msg .= (" student that you wish to place.");
        instruct($msg);

        echo("<br />");

        echo("<table cellspacing='0' cellpadding='0' border='1'>");
        echo("<tr>");
        echo("<td>");
        echo("<table border='0' cellpadding='2'>");

        echo("<tr>");
        echo("<td class='rowgrey' align='center'>&nbsp;</td>");
        echo("<td class='rowgrey' align='center'><b class='white'>Student Name</b></td>");
        echo("<td class='rowgrey' align='center'><b class='white'>Student Number</b></td>");
        echo("<td class='rowgrey' align='center'><b class='white'>Student Department</b></td>");
        echo("</tr>");

        $sql = ("
                SELECT DISTINCT CONCAT(s.last_name, ', ', s.first_name) AS name, sd.record_id, sd.department_id, s.student_number
                FROM student AS s
                INNER JOIN student_department AS sd ON sd.student_number=s.student_number
                WHERE sd.department_id IN (" . $dept_string . ")
                ");

        if ($first_name)
        {
            $sql .= ("
                    AND s.first_name LIKE '" . $first_name . "%'
                    ");
        }

        if ($last_name)
        {
            $sql .= ("
                    AND s.last_name LIKE '" . $last_name . "%'
                    ");
        }

        $sql .= " ORDER BY s.last_name, s.first_name";

        $result = $GLOBALS['dbh']->Execute($sql);
        $rowclass = 0;

        while ($row = $result->FetchRow())
        {
            echo("<tr>");
            echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("<input type='radio' name='student_num' onclick='submit();' value='" . $row["student_number"] . "' ");
            echo(" />");
            echo("</td>");

            echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo($row["name"]);
            echo("</td>");

            echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo($row["student_number"]);
            echo("</td>");

            echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo(getDepartmentName($row["department_id"]));
            echo("</td>");
            echo("</tr>");
            $rowclass++;
        }

        echo("<tr>");
        echo("<td colspan='4' nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo("<input type='submit' name='btnSubmit' value='Submit' />
              <input type='hidden' name='department_string' value= '$department_string2' />");
        echo("</td>");
        echo("</tr>\n");

        echo("</table>");
        echo("</td>");
        echo("</tr>");

        echo("</table>");
        echo("</form>");
    }


?>
