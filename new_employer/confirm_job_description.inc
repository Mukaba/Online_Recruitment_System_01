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
 | Filename: confirm_job_description.inc                                        |
 +------------------------------------------------------------------------------+
 | Description: This code displays the users job inputs on screen, and then     |
 | gives them the choice of either confirming or editing those zany inputs.     |
 +------------------------------------------------------------------------------+

*/

if ($newEmployer->salary_amount1 == '' XOR $newEmployer->salary_amount2 == '') 
{
	/*
	 This should never happen, but just in case it does...
	*/

	if ($newEmployer->salary_amount1 == '')
	{
		$newEmployer->salary_amount1 = $newEmployer->salary_amount2;
	}
	if ($newEmployer->salary_amount2 == '')
	{
		$newEmployer->salary_amount2 = $newEmployer->salary_amount1;
	}
}

instruct("Below is a preview of the Job Description information that you have entered.  Please ensure that all information is correct 
	before continuing to the Final Confirmation page.<br /><br />If you would like to make a correction, click on the 'Edit Inputs' button 
	and your Job Description Form will be reloaded with your inputs displayed for you to edit.");


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
		echo($newEmployer->job_position_title);
		echo("</h3>");
		echo("</td>");
	echo("</tr>");

	// If they have specified that they would like
	// to receive online applications, then display	
	// the email address that they will be sent to.

	if ($newEmployer->job_apply_method == JOB_APP_METHOD_ONLINE)
	{
		echo("<tr>");
			echo("<td colspan='2' align='center' nowrap='nowrap'><b>Send online applications to:</b>&nbsp;");
			echo($newEmployer->job_receive_address); 
			echo("</td>");
		echo("</tr>");
	}

	if ($newEmployer->job_host_department)
	{
		echo("<tr>");
			echo("<td align='center' colspan='2'><br /><b>Preferred host department:</b>&nbsp;");
			echo(getDepartmentName($newEmployer->job_host_department));
			echo("</td>");
		echo("</tr>");
	}

    if ($newEmployer->job_comments)
    {
        echo("<tr>");
            echo("<td colspan='2' align='center'><br /><br /><b>Additional comments:</b></td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td colspan='2' align='center'><br />" . $newEmployer->job_comments."</td>");
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

	if ($newEmployer->job_employer_code)
	{
		echo("<tr>");
			echo("<td align='right'><b>Employer job code:</b>");
			echo("</td>");
			echo("<td><b class='black'>" . ($newEmployer->job_employer_code) . "</b>");
			echo("</td>");
		echo("</tr>");
	}

	echo("<tr>");
		echo("<td align='right'><b>Title of position:</b>");
		echo("</td>");
		echo("<td>" . ($newEmployer->job_position_title));
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'><b>Location of job:</b>");
		echo("</td>");
		echo("<td>");
		echo($newEmployer->job_city);

		if (trim($newEmployer->job_provstate_id))
		{
			echo(", " . getProvstateName($newEmployer->job_provstate_id));
		}

		echo(", " . getCountryName($newEmployer->job_country_id));
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'><b>Number of positions:</b>");
		echo("</td>");
		echo("<td>" . $newEmployer->job_num_positions); 
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'>");
		echo("<b>Apply:</b>");
		echo("</td>");
		echo("<td>");

		switch(trim($newEmployer->job_apply_method))
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
		echo("<td align='right'><b>Start of workterm:</b>");
		echo("</td>");
		echo("<td>");
		echo(getMonth(getTermMonth($newEmployer->job_start_term)));
		echo(", " . $newEmployer->job_start_year);
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'><b>Length of workterm:</b>");
		echo("</td>");
		echo("<td>");

        if ($newEmployer->job_start_date && $newEmployer->job_end_date)
        {
            echo("Other - see start and end dates");
        }
		elseif (sizeof($newEmployer->job_workterm_lengths) == 1)
		{
			echo($newEmployer->job_workterm_lengths[0] . " months");
		}
		else
		{
			for ($i = 0; $i < (sizeof($newEmployer->job_workterm_lengths) - 1); $i++)
			{
				echo($newEmployer->job_workterm_lengths[$i] . ", ");
			}
			echo("or " . $newEmployer->job_workterm_lengths[$i] . " months");
		}

		echo("</td>");
	echo("</tr>");

    if ($newEmployer->job_start_date && $newEmployer->job_end_date)
    {
        echo("<tr>");
            echo("<td align='right'><b>Start date:</b>");
            echo("</td>");
            echo("<td>");
            echo(formatSpecialZeroDate($newEmployer->job_start_date));
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td align='right'><b>End date:</b>");
            echo("</td>");
            echo("<td>");
            echo(formatSpecialZeroDate($newEmployer->job_end_date));
            echo("</td>");
        echo("</tr>");
    }

	if ($newEmployer->job_salary_amount1 != '')
	{
		echo("<tr>");
			echo("<td align='right'><b>Approximate<br />salary range:</b>");
			echo("</td>");
			echo("<td>");
			if ($newEmployer->job_salary_amount1 == $newEmployer->job_salary_amount2)
			{
				echo("$" . ($newEmployer->job_salary_amount1));
			}
			else
			{
				echo("$" . ($newEmployer->job_salary_amount1) . " to $" . ($newEmployer->job_salary_amount2));
			}
			echo(" per " . $newEmployer->job_salary_period);
			echo("</td>");
		echo("</tr>");
	}

	if ($newEmployer->job_workterm_hours != '')
	{
		echo("<tr>");
			echo("<td align='right'><b>Hours of work per week:</b>");
			echo("</td>");
			echo("<td>");
			echo($newEmployer->job_workterm_hours);
			echo("</td>");
		echo("</tr>");
	}

	if ( trim($newEmployer->job_other_postings) != '')
	{
		echo("<tr>");
			echo("<td align='right'><b>Job is also posted at:</b>");
			echo("</td>");
			echo("<td>");
			echo( trim($newEmployer->job_other_postings));
			echo("</td>");
		echo("</tr>");
	}

	echo("<tr>");
		echo("<td align='right'><b>Applicable Disciplines:</b>");
		echo("</td>");
		echo("<td>");
		echo("<b class='black'>");
		foreach ($newEmployer->job_disciplines AS $jd)
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
		echo($newEmployer->job_min_academic . " year");
		echo(($newEmployer->job_min_academic == '1') ? "" : "s");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td align='right'><b>Minimum number of workterms completed:</b>");
		echo("</td>");
		echo("<td>");
		echo($newEmployer->job_min_workterms . " workterm");
		echo(($newEmployer->job_min_workterms == '1') ? "" : "s");
		echo("</td>");
	echo("</tr>"); 

	echo("<tr>");
        echo("<td nowrap='nowrap' align='right'>");
        echo("<b>Receive applications from<br />students in these departments:</b>");
        echo("</td>");
        echo("<td>");
        
        for ($i = 0; $i < (sizeof($newEmployer->job_apply_departments) - 1); $i++) 
        {
            echo(getDepartmentName($newEmployer->job_apply_departments[$i]) . ", ");
        }
        echo(getDepartmentName($newEmployer->job_apply_departments[$i]));
        
        echo("</td>");
    echo("</tr>");

	echo("<tr><td colspan='2'>&nbsp;</td></tr>");

	echo("<tr>");
		echo("<td valign='top' align='right'><b>Job description:</b>");
		echo("</td>");
		echo("<td>");

		$temp = split("\n",$newEmployer->job_description);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			echo($temp[$i]);
			echo("<br />");
		}

		echo("<br />");
		echo("</td>");
	echo("</tr>");

	echo("<tr>");
		echo("<td valign='top' align='right'><b>Skills required:</b>");
		echo("</td>");
		echo("<td>");

		$temp = split("\n",$newEmployer->job_skills_required);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			echo($temp[$i]);
			echo("<br />");
		}

		echo("<br />");
		echo("</td>");
	echo("</tr>");

	if ($newEmployer->job_special_requirements != '')
 	{
		echo("<tr>");
			echo("<td valign='top' align='right' nowrap='nowrap'><b>Special requirements:</b>");
			echo("</td>");
			echo("<td>");
	
			$temp = split("\n",$newEmployer->job_special_requirements);
	
			for ($i = 0; $i < sizeof($temp); $i++)
			{
				echo($temp[$i]);
				echo("<br />");
			}
	
			echo("<br />");
			echo("</td>");
		echo("</tr>");
	}
	
    if ($newEmployer->job_displayname_on_coverltr == '1')
    {   
        echo("<tr>");
            echo("<td align='right' nowrap='nowrap'><b>Address applications to:</b>");
            echo("</td>");
            echo("<td align='left' nowrap='nowrap'>");
                echo(($newEmployer->contact_first_name && $newEmployer->contact_last_name) ? $newEmployer->contact_first_name." ".$newEmployer->contact_last_name : "&nbsp;");
            echo("</td>");
        echo("</tr>");
    }

	echo("<tr>");
		echo("<td colspan='2' valign='top' align='center'>");
        echo("<form method='post' action='" . $PHP_SELF . "'>");
        echo("<input type='hidden' name='select' value='new_employer' />");
        echo("<input type='hidden' name='newEmployer' value='" . packObject($newEmployer) . "' />");
        echo("<input type='hidden' name='inputsFor' value='job' />");
		echo("<input type='hidden' name='continue' value='save_job_description' />");
		echo("<input type='submit' value='Continue' />");
		echo("&nbsp;&nbsp;");
		echo("<input type='submit' name='continue' value='Edit Inputs' />");
        echo("</form>");
		echo("</td>");
	echo("</tr>");


echo("</table>");
echo("</td>");
echo("</tr>");
echo("</table>");
echo("</center>");

?>
