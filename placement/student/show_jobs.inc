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

echo("<table width='100%'>");
echo("<tr>");
echo("<td><h3 align='left'>Job Offers - Applications for ".$placement->first_name." ".$placement->last_name."</h3></td>\n");

// Display the quickmenu
echo("<td align='right'>");
if ($placement->record_id)
{
    $record_id = $placement->record_id;
    include('student_info/student_quickmenu.inc');
}
echo("</td>\n");
echo("</tr>");
echo("</table>");

if (sizeof($error_array) && is_array($error_array))
{
    foreach($error_array as $error_msg)
    {
        echo("<br />");
        error($error_msg);
    }
}

?>
    <h4><?php echo getTermName($placement->term_id)." ".$placement->year." Work Term"?>:</h4>
    <?php

    if (!$placement->isStudentEligible()) {
        notify("This student is not eligible to be placed for ".getTermName($placement->term_id)." ".$placement->year.".");
    }
    echo("<br />");

    $placed_sql = ("
        SELECT history_id 
        FROM history 
        WHERE student_number='".$placement->student_number."' AND term_id='".$placement->term_id."' AND year='".$placement->year."'
        ");
    $placed_results=$GLOBALS['dbh']->Execute($placed_sql);	

    $nonrs_placement = ("
        SELECT history_id, job_id 
        FROM history 
        WHERE term_id='".$placement->term_id."'
        AND year='".$placement->year."' 
        AND student_number='".$placement->student_number."'
        ");
    $nonrs_result = $GLOBALS['dbh']->Execute($nonrs_placement);
    $placed_total = 0;

    if ($placed_results->RecordCount()>0) {
        //:TODO: These variable names are confusing as heck, they should be renamed at some point. 
        $placed_via_coop = 0;
        $placed_total = 1;

        while ($nonrs_row = $nonrs_result->FetchRow()) {
            if ($nonrs_row['job_id']<>'') {
                $placed_via_coop = 1;	
            }
        }
    }
    if ($placed_via_coop == 0) {
        $placed_on_own_sql = ("
            SELECT ec.company_name 
            FROM employer_company as ec
            INNER JOIN history as h 
            ON h.employer_id=ec.employer_id
            WHERE h.term_id='".$placement->term_id."'
            AND h.year='".$placement->year."' 
            AND h.student_number='".$placement->student_number."'
            ");
        $placed_on_own_result = $GLOBALS['dbh']->Execute($placed_on_own_sql);
        while ($placed_own_row=$placed_on_own_result->FetchRow()) {
            echo("<br />");
            echo("<b>".$placement->first_name." ".$placement->last_name."</b> was placed at <b>".$placed_own_row['company_name']."</b> for this term.");
        }
    }

    if ($placement->container_mode)
    {
        // Grab all the job ids where the student has been shortlisted for this term, we want to exclude these job ids. 
        $placed_by_coop = ("
            SELECT ji.job_id
            FROM job_info AS ji
            INNER JOIN applications AS a
            ON ji.job_id = a.job_id AND a.student_number = '".$placement->student_number."'
            INNER JOIN students_shortlisted AS ss
            ON a.job_id = ss.job_id AND a.student_number = ss.student_number
            INNER JOIN employer_company AS ec
            ON ji.employer_id = ec.employer_id
            WHERE ji.term_id = '".$placement->term_id."'
            AND ji.year = '".$placement->year."'
            ");
        $placed_by_coop_results=$GLOBALS['dbh']->Execute($placed_by_coop);
        if ($placed_by_coop_results->RecordCount())
        {
            $excluded_job_ids = array();
            while ($placed_by_coop_row = $placed_by_coop_results->FetchRow())
            {
                $excluded_job_ids[] = $placed_by_coop_row['job_id'];
            }

            $excluded_job_ids = "'".implode("','",$excluded_job_ids)."'";
            // This pulls all history records from the student (based on term and year) where the placement was not an own job,
            // and not one of the jobs listed below (jobs where the student applied for). This was done for history containers. 
            $placed_history_sql = ("
                SELECT ec.company_name, h.history_id 
                FROM employer_company as ec
                INNER JOIN history as h 
                ON h.employer_id=ec.employer_id
                WHERE h.term_id='".$placement->term_id."'
                AND h.year='".$placement->year."' 
                AND h.student_number='".$placement->student_number."'
                AND h.job_id != '' AND h.job_id IS NOT NULL
                AND h.job_id NOT IN (".$excluded_job_ids.")
                ");
            $placed_history_result= $GLOBALS['dbh']->Execute($placed_history_sql);
            while ($placed_history_row=$placed_history_result->FetchRow()) {
                echo("<br />");
                echo("<b>".$placement->first_name." ".$placement->last_name."</b> was placed at <b>".$placed_history_row['company_name']."</b> for this term.");
            }
        }
    }

    // If a student is using containers, the student can be placed multiple times in a term, so we set placed_total to allow us to place this student. 
    if ($placement->container_mode)
    {
        $placed_total = 0;
    }
    ?>
        <form action='<?php echo $PHP_SELF."&amp;select=placement_by_student"; ?>' method='post'>
        <input type='hidden' name='level' value='container_info_form' />
        <input type='hidden' name='placement' value='<?php echo(packObject(($placement))); ?>' />
        <table border='1' cellpadding='0' cellspacing='0'><tr><td>
        <table border='0' cellpadding='2'>
        <tr class='rowgrey'>
        <td align='center'>&nbsp;<b class='white'>Job Code</b>&nbsp;</td>
        <td align='center'>&nbsp;<b class='white'>Company Name</b>&nbsp;</td>
        <td align='center'>&nbsp;<b class='white'>Shortlisted</b>&nbsp;</td>
        <?php
        if ($placed_via_coop == 1 || $placed_total) {
            echo("<td align='center'>&nbsp;<b class='white'>Placed</b>&nbsp;</td>");
        } elseif ($placed_total ==0) {
            echo("<td align='center'>&nbsp;<b class='white'>Place</b>&nbsp;</td>");
        }
    ?>
        </tr>
        <?php
        $count = 0;
    //get information for all jobs in selected and all the students who have applied or been shortlisted to the job
    $sql = ("
        SELECT ec.company_name,ji.job_code,ss.job_id AS shortlisted,a.job_id AS applied, ji.job_id
        FROM job_info AS ji, student AS s,term AS t,employer_company AS ec 
        LEFT JOIN students_shortlisted AS ss 
        ON ss.job_id=ji.job_id AND ss.student_number=s.student_number 
        LEFT JOIN applications AS a 
        ON ji.job_id=a.job_id AND a.student_number=s.student_number 
        WHERE ec.employer_id=ji.employer_id AND s.student_number='".$placement->student_number."' AND t.term_id=ji.term_id 
        AND ji.term_id='".$placement->term_id."' AND ji.year='".$placement->year."' 
        ORDER BY ji.job_code
        ");
    $results=$GLOBALS['dbh']->Execute($sql);

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
                <td class='<?php echo $class?>' align='center'>&nbsp;<?php echo $row['job_code']?>&nbsp;</td>
                <td class='<?php echo $class?>' align='center'>&nbsp;<?php echo $row['company_name']?>&nbsp;</td>
                <td class='<?php echo $class?>' align='center'>&nbsp;<?php echo $shortlisted?>&nbsp;</td>
                <?php

            $placed_in_company_sql = ("
                SELECT history_id 
                FROM history 
                WHERE term_id='".$placement->term_id."'
                AND year='".$placement->year."' AND job_id='".$row['job_id']."' 
                AND student_number='".$placement->student_number."'
                ");
            $placed_in_company_result = $GLOBALS['dbh']->Execute($placed_in_company_sql);
            $placed_in_company_flag = $placed_in_company_result->RecordCount();

            //If the job is not filled
            if (!$placement->isJobFilled($row['job_id']))
            {
                echo("<td class='".$class."' align='center'>");
                if ($placed_total==0 && $placement->isStudentEligible())
                {
                    //echo("&nbsp;<input type='radio' name='placements[]' value='".$placement->student_number."&".$row['job_id']."' />&nbsp;");
                    echo("&nbsp;<input type='radio' name='job_id' value='".$row['job_id']."' />&nbsp;");
                }
                elseif ($placed_in_company_flag)
                {
                    echo("<img src='misc/images/smchk_bl.gif' alt='' />");
                } 
                else 
                {
                    echo("&nbsp;");
                }
                echo("</td>");
            }
            else
            {
                if ($placed_in_company_flag) {
                    echo("<td class='".$class."' align='center'><img src='misc/images/smchk_bl.gif' alt='' /></td>");
                }
                else
                {
                    echo("<td class='".$class."' align='center'> Filled </td>");
                }

            }

            $placed_in_company_flag = null;
            echo("</tr>");

            $count=$count+1;
        }
    }
    if ($placement->isStudentEligible() && $placed_total == 0) {
        ?>
            <tr>
            <td colspan='4' class='<?php echo(($count % 2) ? "row0d" : "row1d"); ?>'>
            <div align='center'><input type='submit' name='btnSubmit' value='Submit' /></div>
            </td>
            </tr>
            <?php
    }
    ?>
        </table>
        </td></tr></table>
        </form>
        <?php
?>
