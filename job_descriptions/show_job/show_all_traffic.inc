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
 | Filename: show_all_traffic.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This is the main file that directs the appropriate show page    |
 | to the OFFICE.                                                               |
 +------------------------------------------------------------------------------+

*/


include('job_descriptions/edit_job/EditJob.class');

$nextTermYear = getNextTermYear();

?>
   <tr>
	<td colspan='2'>
	<table border="0" cellspacing="0" cellpadding="0" width='99%'>
	<?php
    
    if($show_jobs == "")
    {
        $show_jobs = 'uvic';
    }

    //if $show_closed is not set, then set it to the user's desired default settings for dropdown menus to hide or show closed jobs
    if($show_closed == "")
    {
        $sql = ("
            SELECT show_closed_cancelled 
            FROM contact_internal
            WHERE department_id = '" .$auth->department. "'
            AND (login_id='" .$login_id. "' OR netlink_id = '" .$login_id. "')
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        if($row['show_closed_cancelled'] == 1)
        {
            $show_closed = 'Show Closed Jobs';
        }
        else
        {
            $show_closed = 'Hide Closed jobs';
        }
    }
    
    if ($level1 != "job_id")
    {
        echo("<tr>");
            echo("<td>");
                echo("<h3 align='left'>Job Descriptions - View</h3>");
            echo("</td>");
        echo("</tr>");
		
        echo("<tr>");
			echo("<td>");
            if($false_job_code)
            {
                error("You must enter a valid Job Code");
            }
			echo("<table border='0' cellpadding='2' cellspacing='0'>");
			echo("<tr align='center'>");
			echo("<td class='row1'>");
			echo("<form method='post' action='" . $PHP_SELF . "&amp;select=view_job'>");
			echo("<input type='hidden' name='level1' value='" . $level1 . "' />");
			echo("<input type='hidden' name='show_closed' value='" . urlencode($show_closed) . "' />");
			echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>"); 

			echo("<tr align='center'>");
			echo("<td>");
			$show_closed = urldecode($show_closed);
			$show_jobs = urldecode($show_jobs);


			echo("Show jobs for:&nbsp;");

			// Get term and year.

			$sql = ("
				SELECT DISTINCT term_id, term_name
				FROM term
				ORDER BY term_id
				");
			$term_result = $GLOBALS['dbh']->Execute($sql);
			
			$sql = ("
				SELECT MIN(year) AS min_year
				FROM job_info
				WHERE (status!='" . INT_ONLY . "' AND status!='" . REJECTED . "')
				AND year > 1960
				");
			$year_result = $GLOBALS['dbh']->Execute($sql);
			$year_row = $year_result->FetchRow();
			$start_year = $year_row["min_year"];

			$sql = ("
				SELECT CURRENT_DATE
				");
			$year_result = $GLOBALS['dbh']->Execute($sql);
			$year_row = $year_result->FetchRow();
			
			$year_result = explode("-", $year_row["CURRENT_DATE"]);
			$year = $year_result[0];
	
			$limit = $year + 2;
			
			echo("<select name='show_term'>");
			echo("<option value='ALL' ");
			if ($show_term == 'ALL')
			{
				echo("selected='selected'");
			}
			echo(">ALL</option>");
			while ($term_row = $term_result->FetchRow())
			{
				echo("<option value='" . $term_row["term_id"] . "' ");
				if ($show_term == $term_row["term_id"] || (!$show_term && $term_row["term_id"] == $nextTermYear["term"]))
				{
					echo("selected='selected'");
				}
				echo(">" . $term_row["term_name"] . "</option>");
			}
			echo("</select>");
			echo("&nbsp;");
			echo("<select name='show_year'>");
			echo("<option value='ALL' ");
			if ($show_year == 'ALL')
			{
				echo("selected='selected'");
			}
			echo(">ALL</option>");
			for ($i = $limit; $i >= (int) $start_year; $i--)
			{
				echo("<option value='" . $i . "' ");
				if ($show_year == $i || (!$show_year && $i == $nextTermYear["year"]))
				{
					echo("selected='selected'");
				}
				echo(">" . $i . "</option>");
			}
			echo("</select>");
			echo("</td>");
		echo("</tr>");

		// Closed and Cancelled jobs?

		echo("<tr align='center'>");
			echo("<td>");
			echo("Show closed and cancelled jobs?&nbsp;");
			echo("<select name='show_closed'>");
			echo("<option value='Hide Closed Jobs' ");
			if ($show_closed == 'Hide Closed Jobs' || !$show_closed)
			{
				echo("selected='selected'");
			}
			echo(">No</option>");
			echo("<option value='Show Closed Jobs' ");
			if ($show_closed == 'Show Closed Jobs')
			{
				echo("selected='selected'");
			}
			echo(">Yes</option>");
			echo("</select>");
			echo("</td>");
		echo("</tr>");

        // individual department or all jobs at uvic?

		echo("<tr align='center'>");
			echo("<td>");
	            echo("Show jobs for&nbsp;&nbsp;");
	            echo("<select name='show_jobs'>");
	            echo("<option value='uvic' ");
	            if ($show_jobs == 'uvic' || $show_jobs == '')
	            {
	                echo("selected='selected'");
	            }
	            echo(">All of ".ORGANIZATION_CODE."</option>");
			    $sql = ("
				    SELECT DISTINCT department_id, department_name
				    FROM department
				    WHERE job_list
				    ORDER BY department_name
				    ");
            
			    $result = $GLOBALS['dbh']->Execute($sql);

			    while ($row = $result->FetchRow())
			    {
				    echo("<option value='" . $row["department_id"] . "' ");
				    if ($show_jobs == $row["department_id"])
				    {
					    echo("selected='selected'");
				    }
				    echo(">" . $row["department_name"] . "</option>");
			    }
	            echo("</select>");
			echo("</td>");
		echo("</tr>");

        echo("<tr align='center'>");
            echo("<td>");
                echo("-OR- Enter Job Code &nbsp;");
                echo("<input type='text' value= '' size='15' name='job_code' />");
             echo("</td>");
        echo("</tr>");


		echo("<tr align='center'>");
			echo("<td><hr />");
			echo("<input type='submit' value='Go' />");
			echo("</td>");
		echo("</tr>");

		echo("</table>");
	echo("</form>");
		echo("</td>");
		echo("</tr>");
		echo("</table>");
		echo("</td>");
		echo("</tr>");
	}
	echo("<tr>");
	echo("<td>");

switch ($level1)
{
	case "job_id":
	if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
	{
		include('job_descriptions/show_job/jobshow/showjob_admin.inc');
	} else
	{
		include('misc/loguser.inc');
	}
	break;

	default:
	if ($userlevel == TRAFFICDIRECTOR || $auth->department==DIRECTORS_OFFICE_DEPARTMENT)
	{
		//include('job_descriptions/show_job/admin/traffic_show_job_list.inc');
		include('job_descriptions/show_job/admin/show_job_list.inc');
	}
	elseif ($userlevel == OFFICE)
	{
		include("job_descriptions/show_job/admin/show_job_list.inc");
	}
	else
	{
		include("misc/loguser.inc");
	}
	break;
}

?>
</td></tr></table></td></tr></table>

