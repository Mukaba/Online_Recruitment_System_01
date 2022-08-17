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
 | Filename: view_job_info.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This code draws out all of the job inputs from the database     |
 | for the user and allows them to view them.  $pendingEmployer must be set in  |
 | the script that includes this file.                                          |
 +------------------------------------------------------------------------------+

*/
if (trim($pendingEmployer->job_salary_amount1) == '' XOR trim($pendingEmployer->job_salary_amount2) == '')
{
	/*
	 This should never happen, but just in case it does...
	*/

	if (trim($pendingEmployer->job_salary_amount1) == '')
	{
		$pendingEmployer->job_salary_amount1 = $pendingEmployer->job_salary_amount2;
	}
	if (trim($pendingEmployer->job_salary_amount2) == '')
	{
		$pendingEmployer->job_salary_amount2 = $pendingEmployer->job_salary_amount1;
	}
}

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='pending_jobs' />");
echo("<input type='hidden' name='job_id' value='" . $pendingEmployer->job_id . "' />");

echo("<center>");

echo("<table border='1' width='100%'>");
echo("<tr>");
	echo("<td>");
	echo("<table border='0' cellpadding='4' cellspacing='0' width='100%'>");

	echo("<tr>");
		echo("<td colspan='2'>&nbsp;</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='center' colspan='2'>");
		echo("<h3>");
		echo($pendingEmployer->job_position_title);
		echo("</h3>");
		echo("</td>");
	echo("</tr>");

	// If they have specified that they would like
	// to receive online applications, then display	
	// the email address that they will be sent to.

	if ($pendingEmployer->job_apply_method == JOB_APP_METHOD_ONLINE) {
		echo("<tr>");
			echo("<td colspan='2' align='center' nowrap='nowrap'><b>Send online applications to:</b>&nbsp;");
			echo($pendingEmployer->job_receive_address); 
			echo("</td>");
		echo("</tr>");
	}

	echo("<tr>");
		echo("<td colspan='2' align='center' nowrap='nowrap'><br /><b>Preferred host department:</b>&nbsp;<b class='black'>");
		echo($pendingEmployer->job_host_department ? getDepartmentName($pendingEmployer->job_host_department) : "None");
		echo("</b>");
		echo("</td>");
	echo("</tr>");

	if ($pendingEmployer->job_comments)
	{
		echo("<tr>");
        	        echo("<td colspan='2' align='center'><br /><b>Additional comments:</b>");
        	        echo("</td>");
        	echo("</tr>");
	
	        echo("<tr>");
	                echo("<td colspan='2' align='center'><br />" . $pendingEmployer->job_comments);
	                echo("</td>");
	        echo("</tr>");
        }

	echo("<tr><td colspan='2'><br /><hr /></td></tr>");

	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("<b class='black'>Students will see the information below</b>");
		echo("</td>");
	echo("</tr>");

	echo("<tr><td colspan='2'><hr /></td></tr>");

//
// Students will see all of the information below this point.
//

	if ($pendingEmployer->job_employer_code)
	{
		echo("<tr>");
			echo("<td align='right'><b>Employer job code:</b>");
			echo("</td>");
			echo("<td><b class='black'>" . ($pendingEmployer->job_employer_code) . "</b>");
			echo("</td>");
		echo("</tr>");
	}

	echo("<tr>");
		echo("<td align='right'><b>Title of position:</b>");
		echo("</td>");
		echo("<td>" . ($pendingEmployer->job_position_title));
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'><b>Location of job:</b>");
		echo("</td>");
		echo("<td>");
		echo($pendingEmployer->job_city);

		if (trim($pendingEmployer->job_provstate_id))
		{
			echo(", " . getProvstateName($pendingEmployer->job_provstate_id));
		}

		echo(", " . getCountryName($pendingEmployer->job_country_id));
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'><b>Number of positions:</b>");
		echo("</td>");
		echo("<td>" . $pendingEmployer->job_num_positions); 
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>");
		echo("<b>Apply:</b>");
		echo("</td>");
		echo("<td>");

		switch(trim($pendingEmployer->job_apply_method))
		{
		case JOB_APP_METHOD_PAPER:
			echo("<b class='black'>Via paper to the dropboxes</b>");
			break;
		case JOB_APP_METHOD_ONLINE:
		default:
			echo("<b class='black'>Electronically</b>");
			break;
		}

		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'><b>Term:</b>");
		echo("</td>");
		echo("<td>");
		echo(getMonth(getTermMonth($pendingEmployer->job_start_term)));
		echo(", " . $pendingEmployer->job_start_year);
		echo("</td>");
	echo("</tr>");

    echo("<tr>");
        echo("<td align='right'><b>Length of workterm:</b>");
        echo("</td>");
        echo("<td>");

        if ($pendingEmployer->job_start_date && $pendingEmployer->job_end_date)
        {
            echo("Other - see start and end dates");
        }
        elseif (sizeof($pendingEmployer->job_workterm_lengths) == 1)
        {
            echo($pendingEmployer->job_workterm_lengths[0] . " months");
        }
        else
        {
            for ($i = 0; $i < (sizeof($pendingEmployer->job_workterm_lengths) - 1); $i++)
            {
                echo($pendingEmployer->job_workterm_lengths[$i] . ", ");
            }
            echo("or " . $pendingEmployer->job_workterm_lengths[$i] . "  months");
        }

        echo("</td>");
    echo("</tr>");

    if ($pendingEmployer->job_start_date && $pendingEmployer->job_end_date)
    {
        echo("<tr>");
            echo("<td align='right'><b>Start date:</b>");
            echo("</td>");
            echo("<td>");
            echo(formatSpecialZeroDate($pendingEmployer->job_start_date));
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'><b>End date:</b>");
            echo("</td>");
            echo("<td>");
            echo(formatSpecialZeroDate($pendingEmployer->job_end_date));
            echo("</td>");
        echo("</tr>");
    }

	if ($pendingEmployer->job_salary_amount1 != '')
	{
		echo("<tr>");
			echo("<td align='right'><b>Approximate<br />salary range:</b>");
			echo("</td>");
			echo("<td>");
			if ($pendingEmployer->job_salary_amount1 == $pendingEmployer->job_salary_amount2)
			{
				echo("$" . ($pendingEmployer->job_salary_amount1));
			}
			else
			{
				echo("$" . ($pendingEmployer->job_salary_amount1) . " to $" . ($pendingEmployer->job_salary_amount2));
			}
			echo(" per " . $pendingEmployer->job_salary_period);
			echo("</td>");
		echo("</tr>");
	}

	if ($pendingEmployer->job_workterm_hours != '')
	{
		echo("<tr>");
			echo("<td align='right'><b>Hours of work per week:</b>");
			echo("</td>");
			echo("<td>");
			echo($pendingEmployer->job_workterm_hours);
			echo("</td>");
		echo("</tr>");
	}

	if ( trim($pendingEmployer->job_other_postings) != '')
	{
		echo("<tr>");
			echo("<td align='right'><b>Job is also posted at:</b>");
			echo("</td>");
			echo("<td>");
			echo( trim($pendingEmployer->job_other_postings));
			echo("</td>");
		echo("</tr>");
	}

	echo("<tr>");
		echo("<td align='right'><b>Applicable Disciplines:</b>");
		echo("</td>");
		echo("<td>");
		echo("<b class='black'>");
		foreach ($pendingEmployer->job_disciplines AS $jd)
		{
			$disc_codes .= getDisciplineCode($jd) . ", ";
		}
		echo(substr($disc_codes, 0, -2));
		echo("</b>");
		echo("</td>");
	echo("</tr>");
	

	echo("<tr>");
		echo("<td align='right'><b>Minimum academic year completed:</b>");
		echo("</td>");
		echo("<td>");
		echo($pendingEmployer->job_min_academic . " year");
		echo(($pendingEmployer->job_min_academic == '1') ? "" : "s");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'><b>Minimum number of workterms completed:</b>");
		echo("</td>");
		echo("<td>");
		echo($pendingEmployer->job_min_workterms . " workterm");
		echo(($pendingEmployer->job_min_workterms == '1') ? "" : "s");
		echo("</td>");
	echo("</tr>"); 

	echo("<tr>");
                echo("<td nowrap='nowrap' align='right'>");
                echo("<b>Receive applications from<br />students in these departments:</b>");
                echo("</td>");
                echo("<td>");
                
                for ($i = 0; $i < (sizeof($pendingEmployer->job_apply_departments) - 1); $i++) 
                {
                        echo(getDepartmentName($pendingEmployer->job_apply_departments[$i]) . ", ");
                }
		echo(getDepartmentName($pendingEmployer->job_apply_departments[$i]));
                
                echo("</td>");
        echo("</tr>");

	echo("<tr><td colspan='2'>&nbsp;</td></tr>");

	echo("<tr>");
		echo("<td colspan='2' valign='top' align='center'><b>Job description:</b>");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2'>");

		echo(nl2br($pendingEmployer->job_description));

		echo("<br />");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2' valign='top' align='center'><b>Skills required:</b>");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td colspan='2'>");

		echo(nl2br($pendingEmployer->job_skills_required));

		echo("<br />");
		echo("</td>");
	echo("</tr>");

	if ($pendingEmployer->job_special_requirements != '')
 	{
		echo("<tr>");
			echo("<td colspan='2' valign='top' align='center' nowrap='nowrap'><b>Special requirements:</b>");
			echo("</td>");
		echo("</tr>");

		echo("<tr>");
			echo("<td colspan='2'>");
	
			echo(nl2br($pendingEmployer->job_special_requirements));
	
			echo("<br />");
			echo("</td>");
		echo("</tr>");
	}
	
	echo("<tr>");
		echo("<td colspan='2' align='center'>");
		echo("<hr />");
		echo("</td>");
	echo("</tr>");

	// If the user is a traffic director, display the assign host department choice, otherwise properly space the warning message.
	
	if ($userlevel == OFFICE)
	{
		echo("<tr>");
			echo("<td colspan='2'>&nbsp;</td>");
		echo("</tr>");
	}

	echo("<tr>");
		echo("<td colspan='2' valign='middle' align='center'>");
		echo("<table border='0' width='80%'>");
		echo("<tr>");
		echo("<td align='center'>");
		echo("<b class='black'>This job's company and contact must be approved before its status and inputs can be changed.</b>");
		echo("</td>");
		echo("</tr>");
		echo("</table>");
		echo("</td>");
	echo("</tr>");
	
	if ($userlevel == TRAFFICDIRECTOR)
	{
	
		echo("<tr>");
        	        echo("<td colspan='2' align='center'>");
        	        echo("<hr />");
        	        echo("</td>");
        	echo("</tr>");

		echo("<tr>");
			echo("<td colspan='2' align='center'>");
			echo("<input type='submit' name='continue' value='Assign Host Department' />");
			echo("</td>");
		echo("</tr>");
	}
	else
	{
		echo("<tr>");
			echo("<td colspan='2' align='center'>&nbsp;</td>");
		echo("</tr>");
	}

echo("</table>");
echo("</td>");
echo("</tr>");
echo("</table>");
echo("</center>");
echo("</form>");

?>

