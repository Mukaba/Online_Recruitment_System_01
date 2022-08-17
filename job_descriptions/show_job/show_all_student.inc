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
 | Filename: show_all_student.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This is the main file that directs the appropriate show page    |
 | to the student. This page will allow the student to view all of the jobs in  |
 | the database, and as such, checks should be made to be sure that it is not   |
 | an employer accessing the database.                                          |
 +------------------------------------------------------------------------------+

*/
?>
<tr>
<td colspan='2'>
    <table border="0" cellspacing="0" cellpadding="0" width="99%">
<?php
include('job_descriptions/edit_job/EditJob.class');
include_once('job_descriptions/folders/functions.inc');


$sql = ("
	SELECT CURRENT_DATE
	");
$date_result = $GLOBALS['dbh']->Execute($sql);
$date_row = $date_result->FetchRow();

$cur_date = explode("-", $date_row["CURRENT_DATE"]);
$cur_year = $cur_date[0];

// by default, show jobs not in any folder
if (!isset($folder_id)) {
    $folder_id = -1;
}

// Below is used by show_job files to determine if a job is new or updated to a student.

$sql = ("
        SELECT DISTINCT view_job_last
        FROM student
        WHERE student_number='" . $student_number . "'
        ");
$last_result = $GLOBALS['dbh']->Execute($sql);
$last_row = $last_result->FetchRow();
$last_viewed_on = $last_row["view_job_last"];
if ($level1 != "job_id")
{
    echo("<tr>");
        echo("<td>");
            echo("<h3 align='left'>Job Descriptions - View</h3>");
        echo("</td>");
    echo("</tr>");

    echo("<tr>\n\t\t");
    echo("<td>\n\t\t");

    echo("<table border='0' cellpadding='2' cellspacing='0'>\n\t\t\t");
    echo("<tr class='row1'>\n\t\t\t\t");
	echo("<td>\n\t\t\t\t");
	echo("<form method='post' action='" . $PHP_SELF . "&amp;select=view_job'>\n\t\t\t\t\t\t");
    echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>\n\t\t\t\t\t");

    echo("<tr align='center'>\n\t\t\t\t\t\t");
	echo("<td>\n\t\t\t\t\t\t");
	$show_future = urldecode($show_future);
	if (!$show_jobs)
	{
		$show_jobs = $auth->department;
	}
    if ($level1 != "Move")
    {
	    echo("<input type='hidden' name='level1' value='" . $level1 . "' />\t\t\t\t\t\t");
    }
    else
    {
	    echo("<input type='hidden' name='level1' value='' />\n\t\t\t\t\t\t");
	}
    echo("<input type='hidden' name='show_future' value='" . ($show_future ? urlencode($show_future) : "") . "' />\n\t\t\t\t\t\t");
    echo("<input type='hidden' name='show' value='true' />\n\t\t\t\t\t\t");
    echo("<input type='hidden' name='student_number' value='". $student_number."' />\n\t\t\t\t\t\t");

	// Set up the term_year array
    $curTerm = getCurrentTermID();
    $curYear = getCurrentYear();
    $term_year_array[] = Array(
        "value" => $curTerm . "-" . $curYear,
        "display" => getTermName($curTerm) . " " . $curYear
        );

    $next_term_year = getNextTermYear();
    $term_year_array[] = Array(
        "value" => $next_term_year["term"] . "-" . $next_term_year["year"],
        "display" => getTermName($next_term_year["term"]) . " " . $next_term_year["year"]
        );
    $sql_next_term = "SELECT DISTINCT year_order from term where term_id='".$next_term_year['term']."'";
    $result_next_term = $GLOBALS['dbh']->Execute($sql_next_term);
    $row_next_term = $result_next_term->FetchRow();
    $year_order_term=$row_next_term[0];
    $sql_future_term = "SELECT DISTINCT CONCAT(t.term_id,' ',ji.year) as term 
        FROM job_info as ji, term as t where t.term_id=ji.term_id and ji.status!='7' 
        and CONCAT(ji.year,t.year_order)>CONCAT('".$next_term_year['year']."','".$year_order_term."') 
        ORDER BY ji.year,t.year_order";
    $result_future_term = $GLOBALS['dbh']->Execute($sql_future_term);

    while ($row_future_term = $result_future_term->FetchRow())
    {    
        list ($term_id, $year) = split(" ", $row_future_term['term']);
        $term_year_array[] = Array(
            "value" => $term_id . "-" . $year,
            "display" => getTermName($term_id) . " " . $year
            );
    }
    // If we don't yet have a show_term_year set, use the next term and year.
    if (!$show_term_year)
    {
        $show_term_year = $next_term_year["term"] . "-" . $next_term_year["year"];
    }

    echo("Show jobs for: ");

    echo("<select name='show_term_year'>\n\t\t\t\t\t\t");
    foreach (array_reverse($term_year_array) AS $tya)
    {
        echo("<option value='" . $tya["value"] . "'");
        echo($show_term_year == $tya["value"] ? "selected='selected'" : "");
        echo(">" . $tya["display"] . "</option>\n\t\t\t\t\t\t\t\t");
    }
    echo("</select>\n\t\t\t\t\t\t");

	echo("</td>\n\t\t\t\t\t\t");
	echo("\t\t\t\t\t\t</tr>\n");


	// Find out if the user wants jobs that match their preferences, department, group, or UVic, or those they've flagged.

	echo("\t\t\t\t\t\t<tr>\n");
	echo("\t\t\t\t\t\t<td>\n");
	echo("Show jobs that&nbsp;&nbsp;");
	echo("\n\t\t\t\t\t\t<select name='show_jobs'>");

	/*
	Commented out for now.

	echo("<option value='preferences' ");
	if ($show_jobs == 'preferences')
	{
		echo("selected='selected'");
	}
	echo(">match your preferences</option>");
	*/

	echo("\n\t\t\t\t\t\t\t\t<option value='flagged' ");
	if ($show_jobs == 'flagged')
	{
		echo("selected='selected'");
	}
	echo(">you have flagged</option>");

	echo("<option value='applied' ");
	if ($show_jobs == 'applied')
	{
		echo("selected='selected'");
	}
	echo(">you have applied for</option>");

    $sql = ("
        SELECT DISTINCT d.department_id, d.department_name
        FROM student_department AS sd
        INNER JOIN department AS d
            ON d.department_id=sd.department_id
        WHERE sd.student_number='" . $student_number . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        echo("\n\t\t\t\t\t\t\t\t<option value='" . $row["department_id"] . "' ");
        echo(($show_jobs == $row["department_id"]) ? "selected='selected'" : "");
        echo(">are in " . $row["department_name"] . "</option>");
    }

	/*
	Not here for now
	echo("<option value='group' ");
	if ($show_jobs == 'group')
	{
		echo("selected='selected'");
	}
	echo(">match your Co-op group</option>");
	*/

	echo("\n\t\t\t\t\t\t\t\t<option value='uvic' ");
	if ($show_jobs == 'uvic')
	{
		echo("selected='selected'");
	}
	echo(">are registered at ".ORGANIZATION_CODE."</option>");

	echo("\n\t\t\t\t\t\t</select>");
	echo("\n\t\t\t\t\t\t</td>");
	echo("\n\t\t\t\t\t\t</tr>");
       
    //view Jobs in a pasrticular folder
    echo("<tr align='center'>");
	echo("<td>Show jobs that are in &nbsp;");
    echo("<select name = 'folder_id'>");
    echo("<option value='-1' ".($folder_id == -1 ? 'selected=\'selected\'' : '').">NO</option>\n");
    echo("<option value='0' ".($folder_id == 0 ? 'selected=\'selected\'' : '').">ANY/NO</option>\n");
    $sql = ("
            SELECT folder_id
            FROM student_job_folder
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        echo("<option value='".$row["folder_id"]."'");
        if($folder_id == $row["folder_id"])
        {
            echo(" selected='selected'");
        }
        echo(">".getFolderName($row["folder_id"],$student_number)."</option>\n");
    }
    unset($selected);
    echo("</select>&nbsp;Folder");
    echo("</td>");
    echo("</tr>"); 
    
    echo("<tr align='center'>");
	echo("<td><hr />");
    //Manage Folders: Rename, Empty etc. 
	echo("<input type='submit' name='level1' value='Manage Folders' />");
    
	echo("&nbsp;&nbsp;<input type='submit' value='Go' />");
    echo("</td>");
    echo("</tr>");

//	echo("</td>");
//	echo("</tr>");

	echo("\n\t\t\t\t\t</table>");

    echo("</form>");
	echo("\n\t\t\t\t</td>");
	echo("\n\t\t\t\t</tr>");
    echo("\n\t\t\t</table>");

    echo("\n\t\t</td>");
    echo("\n\t\t</tr>");
}

    echo("\n\t\t<tr>");
    echo("\n\t\t<td>");
//echo("<table cellspacing='0' width='96%' cellpadding='0' border='0'>");
//echo("<tr>");
//echo("<td>");

switch ($level1)
{
    case "job_id":
        if ($userlevel == STUDENT || $userlevel == ADMINSTUDENT)
        {
            include('job_descriptions/show_job/jobshow/showjob_student.inc');
        } 
        else
        {
            include('misc/loguser.inc');
        }
        break;

    case "Flag/Unflag Selected Jobs":
        // The student would like to mark these jobs as flagged.  Trim out blanks in array first.

        $jobs_to_flag = trim_array($jobs_to_flag);
        $jobs_to_unflag = trim_array($jobs_to_unflag);

        if ($jobs_to_flag)
        {
            foreach ($jobs_to_flag AS $jtf)
            {
                $sql = ("
                    INSERT INTO student_jobs_flagged
                    (job_id, student_number)
                    VALUES (
                    '" . $jtf . "',
                    '" . $student_number . "'
                    )
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
        }

        if ($jobs_to_unflag)
        {
            foreach ($jobs_to_unflag AS $jtu)
            {
                $sql = ("
                    DELETE FROM student_jobs_flagged
                    WHERE job_id='" . $jtu . "'
                    AND student_number='" . $student_number . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
        }
        include('job_descriptions/show_job/student/show_job_list.inc');
        break;
   case "Move":
        //To move jobs from one folder to the other
        $old_folder_id = $folder_id;
        $folder_id = $move_folder_id;
        
        $job_array = trim_array(array_merge((array)$jobs_to_flag, (array)$jobs_to_unflag));
        if($folder_id != 0)
        {
            if ($job_array)
            {
                foreach ($job_array AS $jid)
                {
                    $check_sql = ("
                            SELECT folder_id, job_id, student_number
                            FROM student_job_folder_join
                            WHERE job_id = '".$jid."'
                            AND student_number = '".$student_number."'
                            ");

                    $check_result = $GLOBALS['dbh']->Execute($check_sql);
                    if ($check_result->RecordCount() != 0)
                    {
                        $sql = ("
                                DELETE FROM student_job_folder_join
                                WHERE job_id = '".$jid."'
                                AND student_number = '".$student_number."'
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                    }               
                    
                    if($folder_id != -1)
                    {

                        $sql = ("
                                INSERT INTO student_job_folder_join
                                (folder_id, job_id, student_number)
                                VALUES (
                                    '" . $folder_id . "',
                                    '" . $jid . "',
                                    '" . $student_number . "'
                                    )
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                    }
                }
                
                notify("The selected job(s) have been successfully moved ");
                
            }
            else
            {
                error("You must select at least one job to move");
            }
        }
        else
        {
            error("You must select a folder to move the selected jobs into");
        }
        unset($folder_id);
        $folder_id = $old_folder_id;
        include('job_descriptions/show_job/student/show_job_list.inc');
        break;
    
    case "Manage Folders":
            include('job_descriptions/folders/folders.inc');
            break;

    default:
        if ($userlevel == STUDENT || $userlevel == ADMINSTUDENT)
        {
            include('job_descriptions/show_job/student/show_job_list.inc');
        }
        else
        {
            include('misc/loguser.inc');
        }
        break;

} //switch

?>
</td>
</tr>
</table></td></tr></table>
