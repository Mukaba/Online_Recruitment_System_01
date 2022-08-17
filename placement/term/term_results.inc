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
 | Filename: term_results.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: Used to display results of "search by term" search              |
 +------------------------------------------------------------------------------+

*/

?>
<h3>Job Offers - Place Students by Term</h3>
<?php

if ($userlevel == OFFICE){
    $department_id = $auth->department;

    if ($department_id != "0" && !in_array($department_id, $departments_in_group)){
        $error = ("You do not have access to this department");
        $department_id = $auth->department;
        $page = "";
        unset( $PDF );
    }
    $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
    $department_in_str = $department_in_str ;
}

$arrTerm = explode(",",$term_id);
$term_id=$arrTerm[0];
$year=$arrTerm[1];

if ($department_option == "department")
{
	$department_sql = " and ji.department_id=".$auth->department." ";
}
else
{
	$department_sql = " and ji.department_id in (".$department_in_str.") ";
}
if ($job_setting=='unmatched' && $student_setting=='all')
{
    //:TODO: Refactor these large ugly queries, and make them more manageable. They sometimes reference columns that are obsolete, or not dependable.
	$sql = ("
        SELECT ijj.job_id as JOB,ijj.job_code as J_CODE,ijj.filled as JOB_FILLED,ji.num_positions,ep.student_flags_id AS eligible
        ,CONCAT(s.first_name,\" \",s.last_name) as name,ji.job_id,s.student_number,ji.job_code, ec.company_name 
        FROM job_info as ji, student as s, employer_company as ec 
        LEFT JOIN interview_job_join AS ijj 
        ON ijj.interview_job_id=ji.job_id 
        LEFT JOIN students_shortlisted AS ss 
        ON ss.job_id=ji.job_id 
        LEFT JOIN student_department AS sd 
        ON sd.student_number=s.student_number 
        LEFT JOIN eligible_placed as ep 
        ON ep.student_flags_id='".ELIGIBLE_FLAG."' AND ep.record_id=sd.record_id AND ep.year='".$year."' and ep.term_id='".$term_id."' 
        WHERE s.student_number=ss.student_number AND ji.status!='".CANCELLED."' AND ji.status!='".INT_ONLY."' AND ((ji.filled!=1 AND (ijj.filled!=1 OR ijj.filled IS NULL))) 
        AND ((ji.year='".$year."' AND ji.term_id='".$term_id."') OR (ijj.year='".$year."' AND ijj.term_id='".$term_id."')) ".$department_sql."
        AND ji.employer_id=ec.employer_id 
        ORDER BY ji.job_code,ijj.job_id,s.last_name,eligible DESC
        ");
}
else
{
	if ($job_setting=='all' && $student_setting=='all')
    {
		$sql = ("
            SELECT ijj.job_id as JOB,ijj.job_code as J_CODE,ijj.filled as JOB_FILLED,ji.num_positions,h.job_id as placed_job,ep.student_flags_id AS eligible
            , ji.filled,CONCAT(s.first_name,\" \",s.last_name) as name,h.history_id,ji.job_id,s.student_number,ji.job_code, ec.company_name 
            FROM job_info AS ji, student AS s, employer_company AS ec 
            LEFT JOIN interview_job_join AS ijj 
            ON ijj.interview_job_id=ji.job_id 
            LEFT JOIN students_shortlisted AS ss 
            ON ss.job_id=ji.job_id OR ss.job_id=ijj.job_id 
            LEFT JOIN history AS h 
            ON h.student_number=s.student_number AND ((h.term_id=ji.term_id AND h.year=ji.year) OR (h.term_id=ijj.term_id AND h.year=ijj.year)) 
            LEFT JOIN student_department AS sd 
            ON sd.student_number=s.student_number 
            LEFT JOIN eligible_placed AS ep 
            ON ep.student_flags_id='".ELIGIBLE_FLAG."' AND sd.record_id=ep.record_id AND ep.year='".$year."' AND ep.term_id='".$term_id." '
            WHERE s.student_number=ss.student_number AND ji.status!='".CANCELLED."' AND ji.status!='".INT_ONLY."' 
            AND ((ji.year='".$year."' AND ji.term_id='".$term_id."') OR (ijj.term_id='".$term_id."' AND ijj.year='".$year."'))".$department_sql."
            AND ji.employer_id=ec.employer_id 
            ORDER BY ji.job_code,ijj.job_id,s.student_number,eligible DESC
            ");
	}
    else
    {
		if ($job_setting=='all' && $student_setting=='eligible')
        {
			$sql = ("
                SELECT ijj.job_code AS J_CODE,ijj.filled AS JOB_FILLED,ijj.job_id AS JOB,ji.num_positions,h.job_id,ji.filled,ep.student_flags_id AS eligible
                , CONCAT(s.first_name,\" \",s.last_name) AS name, h.history_id,ji.job_id,s.student_number,ji.job_code, ec.company_name 
                FROM job_info AS ji, student AS s, employer_company AS ec 
                LEFT JOIN interview_job_join AS ijj 
                ON ijj.interview_job_id=ji.job_id 
                LEFT JOIN students_shortlisted AS ss 
                ON ss.job_id=ji.job_id 
                LEFT JOIN history AS h 
                ON h.student_number=s.student_number AND ((h.term_id=ji.term_id AND h.year=ji.year) OR (h.term_id=ijj.term_id AND h.year=ijj.year)) 
                LEFT JOIN student_department AS sd 
                ON sd.student_number=s.student_number 
                LEFT JOIN eligible_placed AS ep 
                ON ep.record_id=sd.record_id AND ep.student_flags_id='".ELIGIBLE_FLAG."' AND ep.year='".$year."' AND ep.term_id='".$term_id."'
                WHERE s.student_number=ss.student_number AND ji.status!='".CANCELLED."' AND ji.status!='".INT_ONLY."'
                AND ((ji.year='".$year."' AND ji.term_id='".$term_id."') OR (ijj.year='".$year."' AND ijj.term_id='".$term_id."')) ".$department_sql."
                AND ji.employer_id=ec.employer_id and ep.student_flags_id IS NOT NULL 
                ORDER BY ji.job_code,ijj.job_id,s.student_number,eligible DESC
                ");
		}
        else
        {
			$sql = ("
                SELECT ijj.job_code AS J_CODE,ijj.filled AS JOB_FILLED,ijj.job_id AS JOB,ji.num_positions,ep.student_flags_id AS eligible
                , CONCAT(s.first_name,\" \",s.last_name) AS name,h.history_id,ji.job_id,s.student_number,ji.job_code, ec.company_name 
                FROM job_info AS ji, student AS s, employer_company AS ec 
                LEFT JOIN interview_job_join AS ijj 
                ON ijj.interview_job_id=ji.job_id 
                LEFT JOIN students_shortlisted AS ss 
                ON ss.job_id=ji.job_id 
                LEFT JOIN history as h 
                ON h.student_number=s.student_number AND ((h.term_id=ji.term_id AND h.year=ji.year) OR (h.term_id=ijj.term_id AND h.year=ijj.term_id)) 
                LEFT JOIN student_department AS sd 
                ON sd.student_number=s.student_number 
                LEFT JOIN eligible_placed AS ep 
                ON (ep.record_id=sd.record_id AND ep.student_flags_id='".ELIGIBLE_FLAG."' AND ep.year='".$year."' and ep.term_id='".$term_id."') 
                WHERE s.student_number=ss.student_number AND ji.status!='".CANCELLED."' AND ji.status!='".INT_ONLY."' 
                AND (ji.filled!=1 AND (ijj.filled!=1 OR ijj.filled is NULL)) 
                AND ((ji.year='".$year."' AND ji.term_id='".$term_id."') OR (ijj.year='".$year."' AND ijj.term_id='".$term_id."'))".$department_sql."
                AND ji.employer_id=ec.employer_id AND ep.student_flags_id IS NOT NULL 
                ORDER BY ji.job_code,ijj.job_id,s.student_number,eligible DESC
                ");
		}
	}	
}

if ($term_id<>NULL && $year<>NULL)
{
    $results=$GLOBALS['dbh']->Execute($sql);
    $count=0;
    $lastjobcode="-1";

    echo("<form action='".$PHP_SELF."&amp;select=placement_by_term' method='post'>");
    echo("<input type='hidden' name='level' value='multiple_department_check' />");
    echo("<input type='hidden' name='term_id' value='".$term_id."' />");
    echo("<input type='hidden' name='year' value='".$year."' />");
    echo("<table cellpadding='3' border='1' cellspacing='0'>");
    echo("<tr>");
    echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>");
    echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>");
    echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Shortlisted Students</b>&nbsp;</td>");

    if ($results->RecordCount()>0)
    {
        $increment = 0;
        while ($row=$results->FetchRow())
        {
            if ($count%2==0){
                $class='row0d';
            }else{
                $class='row1d';
            }
            if ($row['JOB']<>NULL){
                $filled=$row['JOB_FILLED'];
            }else{
                $filled=$row['filled'];
            }
            if ($filled==1){
                $class='row0';
            }
            if ($row['num_positions']>1){
                $num_positions=" (".$row['num_positions']." Positions)";
            }
            if ($row['J_CODE']<>NULL){
                $job_code=$row['J_CODE'];
            }else{
                $job_code=$row['job_code'];
            }
            if ($row['JOB']<>NULL){
                $job_id=$row['JOB'];
            }else{
                $job_id=$row['job_id'];
            }

            // If the student is in a discpline that uses containers, the student can be placed more than once in one term, so 
            // we have to do a containers check.
            $container_check_sql = ("
                SELECT department_id
                FROM student_department 
                WHERE student_number = '".$row['student_number']."'
                ");
            $container_check_result = $GLOBALS['dbh']->Execute($container_check_sql);
            $container_flag = 0;
            while ($container_row = $container_check_result->FetchRow())
            {
                if (useContainers($row['student_number'],$container_row['department_id']))
                {
                    $container_flag = 1;
                    break;
                }
            }

            if (($lastjobcode == $row['job_code'] && $row['JOB']==NULL) || ($row['JOB']==$lastjob && $row['JOB']<>NULL))
            {
                if ($row['student_number']<>$student)
                {
                    echo("<tr>");
                    echo("<td>");
                    echo("<a href='".$PHP_SELF."&amp;select=placement_by_student&amp;level=multiple_department_check&amp;term_id=".$term_id."&amp;year=".$year."&amp;student_num=".$row['student_number']."'>");
                    echo("<b class='blue'>".$row['name']."</b>");
                    echo("</a>");
                    echo("</td>");

                    echo("<td align='right'>");
                    if ($row['job_id']==$row['placed_job'] || ($row['JOB']==$row['placed_job'] && $row['JOB']<>NULL))
                    {
                        echo("<img src='misc/images/smchk_bl.gif' alt='placed' />");
                    }
                    elseif (($row['history_id']==NULL || $container_flag) && $filled<>1 && $row['eligible']<>NULL)
                    {
                        echo("<input type='checkbox' name='placements[".$increment."]' value='".$row['student_number']."&".$job_id."' />");
                    }
                    else
                    {
                        echo "&nbsp;";
                    }

                    echo("</td>");
                    echo("</tr>");
                }

                $student = $row['student_number'];
                $increment = $increment +1;

            }
            else
            {
                $student = NULL;
                $count=$count+1;
                if ($lastjobcode<>"-1")
                {
                    echo("</table>");
                    echo("</td>");
                    echo("</tr>");
                }

                echo("<tr>");
                echo("<td class='".$class."'>".$row['company_name']."</td>");
                echo("<td class='".$class."'>");
                    echo("<a href='".$PHP_SELF."&amp;select=view_job&amp;level1=job_id&amp;job_id=".$row["job_id"]."&amp;show_quick=true' target='_blank'>".$job_code."</a>");
                    echo($num_positions);
                echo("</td>");
                echo("<td class='".$class."'>");

                echo("<table width='100%' cellspacing='3' cellpadding='0'>");
                echo("<tr>");
                echo("<td>");
                    echo("<a href='".$PHP_SELF."&amp;select=placement_by_student&amp;level=multiple_department_check&amp;term_id=".$term_id."&amp;year=".$year."&amp;student_num=".$row['student_number']."'>");
                    echo("<b class='blue'>".$row['name']."</b>");
                    echo("</a>");
                echo("</td>");

                echo("<td align='right'>");

                if ($row['job_id']==$row['placed_job'] || ($row['JOB']==$row['placed_job'] && $row['JOB']<>NULL))
                {
                    echo("<img src='misc/images/smchk_bl.gif' alt='' />");
                }
                elseif (($row['history_id']==NULL || $container_flag) && $filled<>1 && $row['eligible']<>NULL)
                {
                    echo("<input type='checkbox' name='placements[".$increment."]' value='".$row['student_number']."&".$job_id."' />");
                }
                else
                {
                    echo("&nbsp;");
                }

                echo("</td>");
                echo("</tr>");

                $increment = $increment +1;
                $student = $row['student_number'];
            }

            $lastjobcode=$row['job_code'];
            $lastjob=$row['JOB'];
            $num_positions=NULL;
        }//end while
    }// end if $results->RecordCount() > 0

    //:TODO: We probably have a table tag mismatch at some point here 
    echo("</table>");
    if ($results->RecordCount()<>0)
    {
        echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("<p>");
        echo("<input type='submit' name='btnSubmit' Value='Place Students' />");
        echo("</p>");
    }
    else
    {
        notify("No matches found.");
    }
}//end if we have term_id and year
else
{
	error('A term id and year were not found');
}

echo("</form>");

?>
