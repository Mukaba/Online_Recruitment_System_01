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
 | Filename: delete_history.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This filed is called when the user deletes a file.              |
 +------------------------------------------------------------------------------+

*/
?>
&nbsp;</td>
<?php
if ($searchHistory<>NULL){
?>
	<form name='delhistory' method='post' action='<?php  echo  $PHP_SELF?>'>
    <input type='hidden' name='searchHistory' value='<?php echo $searchHistory?>' />
    <input type='hidden' name='btnSearch' Value='Search'>
	<input type='hidden' name='student_list' value='<?php echo $student_list?>' />
    <input type='hidden' name='student_department' value='<?php echo $student_department?>' />
    <input type='hidden' name='searchStudents' value='<?php echo $searchStudents?>' />
	<input type='hidden' name='student_num' value='<?php echo $student_num?>' />	
	<input type='hidden' name='history_list' value='<?php echo $history_list?>' />
	<td>&nbsp;</td>
        <td valign="middle"><b class='black'></b>&nbsp;<a href="javascript:document.delhistory.submit()"><b class="blue">Back to List&nbsp;</b></a></td>
        </form></td>
<?php
}
?>
</td><td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>"i alt="" /></td></tr></table>
<h3 align='left'>Student Information - Placement History</h3>
<?php
if ($edit_record=='Delete Record')
{
	$department_in_str=implode("', '",$departments_in_group);

	$sql = ("
        SELECT job_id 
        FROM history 
        WHERE job_id IS NOT NULL AND history_id='".addslashes($history_id)."'
        ");

	$result=$GLOBALS['dbh']->Execute($sql);
    //automatically sets the job to "not filled"
    if ($result->RecordCount())
    {
        $row=$result->FetchRow();
        $sql = ("
            UPDATE job_info 
            SET filled = '0' 
            WHERE job_id='".$row['job_id']."'
            ");
        $GLOBALS['dbh']->Execute($sql);

        $sql = ("
            UPDATE interview_job_join
            SET filled = '0' 
            WHERE job_id='".$row['job_id']."'
            ");
        $GLOBALS['dbh']->Execute($sql);
    }

    //deletes the history record

	$sql = ("
        SELECT h.student_number, h.department_id, h.term_id, h.year, sd.record_id 
        FROM history h
        LEFT JOIN student_department sd
        ON h.student_number = sd.student_number and h.department_id = sd.department_id
        WHERE h.history_id='".addslashes($history_id)."'
        ");
	$results=$GLOBALS['dbh']->Execute($sql);	
	$row=$results->FetchRow();
	$student_num=$row['student_number'];
    $student_department_id = $row['department_id'];
	$term_id=$row['term_id'];
	$year=$row['year'];
    $record_id = $row['record_id'];
    assert($record_id);

    assert($history_id);
	$sql = ("
        DELETE 
        FROM history 
        WHERE history_id= '".addslashes($history_id)."'
        ");
	$GLOBALS['dbh']->Execute($sql);

	$sql = ("
        DELETE 
        FROM history_flags_join 
        WHERE history_id='".addslashes($history_id)."'
        ");
	$GLOBALS['dbh']->Execute($sql);

    //deletes the student from the eligible_placed table
	
    $sql = ("
        SELECT history_id
        FROM history 
        WHERE student_number = '".$student_num."' AND department_id = '".$student_department_id."' AND term_id = '".$term_id."' AND year = '".$year."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    if ($result->RecordCount() == 0)
    {
        $sql = ("
            DELETE 
            FROM eligible_placed 
            WHERE record_id='".$record_id."' AND term_id='".$term_id."' AND year='".$year."' AND student_flags_id='".PLACED_FLAG."'
            ");
        $GLOBALS['dbh']->Execute($sql);	
    }

	if ($eligible == "checked") 
        
    {
        $sql = ("
            SELECT student_flags_id
            FROM eligible_placed
            WHERE student_flags_id = '".ELIGIBLE_FLAG."' AND record_id = '".$record_id."' AND term_id = '".$term_id."' AND year = '".$year."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        if ($result->RecordCount() == 0)
        {
            $sql = ("
                INSERT INTO eligible_placed 
                (student_flags_id,record_id,term_id,year) 
                VALUES ('".ELIGIBLE_FLAG."','".$record_id."','".$term_id."','".$year."')
                ");
            $GLOBALS['dbh']->Execute($sql);
        }

        if (!wasEverEligible($record_id, $term_id, $year)) {
            setEverEligible($record_id, $term_id, $year);
        }

        //update status in applications table so that employers can view available students

        $app_sql = ("
            SELECT application_id, a.job_id 
            FROM applications a 
            INNER JOIN job_info ji 
            ON a.job_id = ji.job_id 
            WHERE student_number = '".$student_num."' AND year = '".$year."' AND term_id = '".$term_id."'
            AND a.app_status = '".APP_UNAVAILABLE."'
            ");
        
        $app_results = $GLOBALS['dbh']->Execute($app_sql);
        if ($app_results->RecordCount() > 0)
        {
            $flag = 0;
            while ($app_rows=$app_results->FetchRow())
            {
                reactivateStudentJobApplication($app_rows['job_id'],$student_num);
            }
        }
	}
    // Otherwise, they're not eligible, do ineligible clean up just in case. 
    else 
    {
        $sql = ("
            DELETE 
            FROM eligible_placed
            WHERE record_id = '".$record_id."' AND term_id = '".$term_id."' AND year = '".$year."'
            ");
        $GLOBALS['dbh']->Execute($sql);

        include_once('student_info/edit/EditStudent.class');
        EditStudent::disableJobApplications($student_num, $term_id, $year);
    }

    // realign the containers and history records
    include_once('history/container_functions.inc');
    if (useContainers($student_num, $student_department_id)) {
        syncContainersWithHistory($student_num, $student_department_id);
    }
    else {
        include_once('placement/SkeletonHistory.class');
        $sh = new SkeletonHistory('');
        $sh->syncNonContainersWithHistory($student_num, $student_department_id);
    }

    echo("<p>");
    notify('The record has been deleted.');
    echo("</p>");
}
?>
<br /><center><a href="<?php echo $PHP_SELF."?menu_select=100&amp;select=history_choose" ?>">Back to Placement History</a></center>
