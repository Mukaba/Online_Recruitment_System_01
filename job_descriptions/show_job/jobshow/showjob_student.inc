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
 | Filename: showjob_student.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This code takes the job_id that is passed to it and withdraws   |
 | all the information pertaining to the specific job out of the database.      |
 | Then displays that information.                                              |
 +------------------------------------------------------------------------------+

*/

$sql = ("
	SELECT CURRENT_DATE
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$current_date = $row["CURRENT_DATE"];

$job_id = urldecode($job_id);
	
$job = new EditJob('');
$job->loadValues($job_id);

if ($job->salary_amount1 == '' XOR $job->salary_amount2 == '')
{
        // This should never happen, but just in case it does...

        if ($job->salary_amount1 == '')
        {
                $job->salary_amount1 = $job->salary_amount2;
        }
        if ($job->salary_amount2 == '')
        {
                $job->salary_amount2 = $job->salary_amount1;
        }
}



if(($job->status != POSTED) && ($job->status != CLOSED))
{
	error("Sorry, this job has not been posted, and therefore cannot be viewed by students.");
} 
else
{

$jobOutput .= ("
	<center>
	<table border='1' width='100%'>
	<tr>
	<td>
	<table border='0' cellpadding='4' cellspacing='0' width='100%'>
	");

// Students will see all of the information below this point.


	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='center' valign='middle' colspan='2'>");
        $jobOutput .= ("<br />");
		$jobOutput .= ("<h3>" . $job->job_code . " : " . $job->position_title . "</h3><br />");
		$jobOutput .= ("<hr /></td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' align='center'><b>Employer/Company name:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>");
		$jobOutput .= ($job->company_name); 
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' align='center'><b>Company Division Name:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>");
		$jobOutput .= ($job->department_name);
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr align='center'>");
		$jobOutput .= ("<td colspan='2' align='center'><b>Job Code:</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
		$jobOutput .= ("<b class='black'>" . $job->job_code);
		$jobOutput .= ("</b></td>");
	$jobOutput .= ("</tr>");

	if ($job->employer_code)
	{
		$jobOutput .= ("<tr align='center'>");
			$jobOutput .= ("<td colspan='2' align='center'><b>Employer Job Code:</b>&nbsp;&nbsp;&nbsp;&nbsp;");
			$jobOutput .= ("<b class='black'>" . ($job->employer_code) . "</b>");
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}

	$jobOutput .= ("<tr align='center'>");
		$jobOutput .= ("<td colspan='2' align='center'><b>Position Title:</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
		$jobOutput .= ($job->position_title);
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr align='center'>");
		$jobOutput .= ("<td colspan='2' align='center'><b>Term:</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
		$jobOutput .= (getMonth(getTermMonth($job->start_term)) . ", " . $job->start_year);
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr align='center'>");
		$jobOutput .= ("<td colspan='2' align='center'><b>Job closes at:</b>&nbsp;&nbsp;&nbsp;&nbsp;");
		$jobOutput .= ("<b class='black'>" . $job->closing_time . ", ");
		$jobOutput .= (formatLongDate($job->closing_date)); 
		if ($job->closing_date < $current_date)
		{
			$jobOutput .= ("&nbsp;(<font style='color:red;'>closed</font>)");
			$jobClosed = 1;
		}
		$jobOutput .= ("</b></td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr align='center'>");
		$jobOutput .= ("<td colspan='2' align='center'><b>Status of Job:</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b class='black'>");
		$jobOutput .= (getStudentStatusName($job->student_status) . "</b>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2'><hr /><br /></td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' align='center' valign='top'>");
		switch( trim($job->apply_method))
		{
		case JOB_APP_METHOD_PAPER:
            $sqlpaper = "SELECT d.job_app_paper_instructions from department as d, job_info as ji
                        where d.department_id=ji.department_id and ji.job_id='".$job_id."'";
            $resultpaper = $GLOBALS['dbh']->Execute($sqlpaper);
            $rowpaper = $resultpaper->FetchRow();
			$jobOutput .= ("<b class='black'>-- "); 
			if ($rowpaper['job_app_paper_instructions'] != '')
			{
				$temp = split("\n",$rowpaper['job_app_paper_instructions']);
				for ($i = 0; $i < sizeof($temp) -1; $i++)
				{
					$jobOutput .= ($temp[$i]);
					$jobOutput .= ("<br />");
				}
				$jobOutput .= ($temp[$i]);
			}
			else
			{
				$jobOutput .= ("Apply via paper");
			}
			$jobOutput .= (" --</b><br />");
			break;
		case JOB_APP_METHOD_SPECIAL:
			$jobOutput .= ("<b class='black'>-- "); 
			if ($job->special_instructions != '')
			{
				$temp = split("\n",$job->special_instructions);
				for ($i = 0; $i < sizeof($temp) -1; $i++)
				{
					$jobOutput .= ($temp[$i]);
					$jobOutput .= ("<br />");
				}
				$jobOutput .= ($temp[$i]);
			}
			else
			{
				$jobOutput .= ("Read 'Job description' and/or 'Special Instructions' for special instructions");
			}
			$jobOutput .= (" --</b><br />");
			break;
		case JOB_APP_METHOD_ONLINE:
		default:
			$jobOutput .= ("<b class='black'>-- Apply electronically --</b>");
			break;
		}
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2'>&nbsp;</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td width='50%' align='right'><b>Job Location:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td width='50%' align='left'>" . ($job->city));
		if (trim($job->provstate))
		{
			$jobOutput .= (", " . getProvstateName($job->provstate));
		}
		$jobOutput .= (", " . getCountryName($job->country));
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Number of Positions:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td align='left'>" . $job->num_positions);
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Length of Workterm:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td align='left'>");
        // If the job has an end or start date, we tell the student to look at the start date and end date
        // which gives a more precise work term length. 
        if ($job->start_date && $job->end_date)
        {
            $jobOutput .= ("Other - see start and end dates");
        }
		elseif (sizeof($job->workterm_lengths) == 1)
		{
			$jobOutput .= ($job->workterm_lengths[0] . " months");
		}
		else
		{
			for ($i = 0; $i < (sizeof($job->workterm_lengths) - 1); $i++)
			{
				$jobOutput .= ($job->workterm_lengths[$i] . ", ");
			}
			$jobOutput .= ("or " . $job->workterm_lengths[$i] . " months");
		}
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

    if ($job->start_date && $job->end_date)
    {
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Start date:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td align='left'>");
            $jobOutput .= (formatSpecialZeroDate($job->start_date));
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>End date:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td align='left'>");
            $jobOutput .= (formatSpecialZeroDate($job->end_date));
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
    }

	if ($job->salary_amount1 != '')
	{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'>");
		if ($job->salary_amount1 == $job->salary_amount2)
		{
			$jobOutput .= ("<b>Approximate Salary:</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td align='left'> $" . $job->salary_amount1);
		}
		else
		{
			$jobOutput .= ("<b>Approximate Salary Range:</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td align='left'> $" . ($job->salary_amount1) . " to $" . ($job->salary_amount2));
		}
		$jobOutput .= (" per " . $job->salary_period);
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	}

	if ($job->workterm_hours != '')
	{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Hours of work per week:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td align='left'>" . ($job->workterm_hours));
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	}

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Minimum Academic Year Completed:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td align='left'>" . $job->min_academic . " year");
                $jobOutput .= (($job->min_academic == '1') ? "" : "s");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Minimum Workterms Completed:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td align='left'>" . $job->min_workterms . " workterm");
                $jobOutput .= (($job->min_workterms == '1') ? "" : "s");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");	

	$jobOutput .= ("<tr>");
                $jobOutput .= ("<td nowrap='nowrap' align='right'>");
                $jobOutput .= ("<b>Students from these departments<br />can apply to this job:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td align='left'>");
        
                for ($i = 0; $i < sizeof($job->apply_departments); $i++)
                {
			$applyDepts .= (getDepartmentName($job->apply_departments[$i]) . ", ");
		}
		$applyDepts = substr($applyDepts, 0, -2);

		$jobOutput .= ($applyDepts);
                
                $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

	if ($job->job_disciplines)
	{
	        $jobOutput .= ("<tr>");
	                $jobOutput .= ("<td align='right'><b>Applicable disciplines within<br />the above departments:</b>");
	                $jobOutput .= ("</td>");
	                $jobOutput .= ("<td>");
	                $jobOutput .= ("<b class='black'>");
	                foreach ($job->job_disciplines AS $jd)
	                {
	                        $disc_codes .= getDisciplineCode($jd) . ", ";
	                }
	
	                $jobOutput .= substr($disc_codes, 0, -2);
	                $jobOutput .= ("</b>");
	                $jobOutput .= ("</td>");
	        $jobOutput .= ("</tr>");
	}

	if (trim($job->other_postings) != '')
	{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>This job is also posted at:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td align='left'>" . (trim($job->other_postings)));
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	}

	/* We're not doing this anymore.
	$jobOutput .= ("<tr>");
                $jobOutput .= ("<td align='right'><b>Job's Industry:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                $jobOutput .= (getIndustryName($job->industry));
                $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
	*/
	
	$jobOutput .= ("<tr><td><br /></td></tr>");

	if ($job->company_website != '')
	{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Employer's website:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>");
		$jobOutput .= ("<a class='blue' target='employer_website' href='http://" . ($job->company_website) . "'>");
		$jobOutput .= (($job->company_website) . "</a>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	}

	if ($job->company_description != '')
	{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' valign='top' align='center'>");
		$jobOutput .= ("<b>Description of <br />Employer/Company:</b>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' align='center'>");
		$jobOutput .= nl2br($job->company_description);
        $jobOutput .= ("<br />");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	}
                        
	if ($job->department_id && $job->department_id != 'none')
	{                                       
		$jobOutput .= ("<tr><td colspan='2'><br /></td></tr>");

	        $jobOutput .= ("<tr>");         
	                $jobOutput .= ("<td align='right'><b>Company Division Name:</b></td>");
	                $jobOutput .= ("<td>" . $job->department_name . "</td>");
	        $jobOutput .= ("</tr>");        
	                                        
	        if ($job->department_website != '' && $job->department_website != $job->employer_website)
	        {                               
	                $jobOutput .= ("        
	                        <tr>            
	                        <td align='right'><b>Division's website:</b>
	                        </td>
	                        <td>
	                        <a class='blue' target='department_website' href='http://" . stripSlashes($job->department_website) . "'>" . stripSlashes($job->department_website) . "</a>
	                        </td>
	                        </tr>
	                        ");     
	        }                       
	
	        if ($job->department_description != '' && $job->department_description != $job->company_description)
	        {
	                $jobOutput .= ("
	                        <tr>
	                        <td colspan='2' valign='top' align='center'>
	                        <b>Description of <br />Company Division:</b>
	                        </td>
				</tr>
				");
	
                $jobOutput .= ("<tr>");
                $jobOutput .= ("<td align='center' colspan='2'>");

                $jobOutput .= nl2br($job->department_description);
                $jobOutput .= ("<br />");
	
                $jobOutput .= ("
                        </td>
                        </tr>
                        ");
	        }
	}

    if ($job->for_student_comments && ($userlevel == STUDENT || $userlevel == ADMINSTUDENT))
    {
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' align='center'><br /><br /><b class='red'>Job status comments:</b>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
            $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2'><br />");
            $jobOutput .= nl2br($job->for_student_comments);
            $jobOutput .= ("<br />");
            $jobOutput .= ("<br />");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
    }
    
	$jobOutput .= ("<tr><td><br /></td></tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' valign='top' align='center'><b>Job description:</b>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2'>");
        $jobOutput .= nl2br($job->job_description);
		$jobOutput .= ("<br />");
		$jobOutput .= ("<br />");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' valign='top' align='center'><b>Skills required:</b>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2'>");
        $jobOutput .= nl2br($job->skills_required);
		$jobOutput .= ("<br />");
		$jobOutput .= ("<br />");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	if ($job->special_requirements != '')
 	{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' valign='top' align='center' nowrap='nowrap'><b>Special requirements:</b>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2'>");
        $jobOutput .= nl2br($job->special_requirements);
		$jobOutput .= ("<br />");
		$jobOutput .= ("<br />");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	}

	if ($job->special_instructions != '')
	{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' valign='top' align='center' nowrap='nowrap'><b>Special Instructions:</b>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2'>");
        $jobOutput .= nl2br($job->special_instructions);
		$jobOutput .= ("<br />");
		$jobOutput .= ("<br />");
		$jobOutput .= (" </td>");
	$jobOutput .= ("</tr>");
	}


	$formEnd .= ("<tr>");
		$formEnd .= ("<td colspan='2' valign='middle' align='center'>");
        $formEnd .= ("
                <form method='post' action='" . $PHP_SELF . "'>
                <input type='hidden' name='select' value='edit_job' />
                <input type='hidden' name='job' value='" . (isset($job) ? packObject(($job)) : "") . "' />
                <input type='hidden' name='job_id' value='" . ($job_id) . "' />
                <input type='hidden' name='HTMLjob' value='" . urlencode($jobOutput . $jobEnd) . "' />
                <input type='submit' name='PDF' value='View in PDF' />
                ");
        $formEnd .= ("</form>");
		$formEnd .= ("</td>");
	$formEnd .= ("</tr>");


	/*
	 Either display the 'Apply to Job' button, or display the reason that the student cannot apply to the job, using
	 the reasonStudentCantApply function to determine this.
	*/

	if (($auth->userlevel == STUDENT || $auth->userlevel == ADMINSTUDENT) && !$jobClosed && canStudentApply($job_id, $student_number)) 
	{

        $formEnd .= ("<tr>");
            $formEnd .= ("<td colspan='2' align='center'>");
                $formEnd .= ("<form method='post' action='" . $PHP_SELF . "'>");
                $formEnd .= ("<input type='hidden' name='select' value='apply_job' />");
                $formEnd .= ("<input type='hidden' name='job_id' value='" . $job_id . "' />");
                $formEnd .= ("<input type='hidden' name='option' value='add_online' />");
                $formEnd .= ("<input type='submit' value='Apply To Job' />");
                $formEnd .= ("</form>");
            $formEnd .= ("</td>");
        $formEnd .= ("</tr>");

	}
    elseif ($jobClosed)
    {
        $formEnd .= ("<tr>");
            $formEnd .= ("<td colspan='2' align='center'>");
            $formEnd .= ("<b class='black'>-- This job is closed, and so you can't apply to it --</b>");
            $formEnd .= ("</td>");
        $formEnd .= ("</tr>");
    }
	elseif (reasonStudentCantApply($job_id, $student_number) == 'already_applied')
	{
		$formEnd .= ("<tr>");
			$formEnd .= ("<td colspan='2' align='center'>");
			$formEnd .= ("<b class='black'>-- You already have an application --</b>");
			$formEnd .= ("</td>");
		$formEnd .= ("</tr>");
	}
	elseif (reasonStudentCantApply($job_id, $student_number) == 'not_eligible')
	{
		$formEnd .= ("<tr>");
			$formEnd .= ("<td colspan='2' align='center'>");
			$formEnd .= ("<b class='black'>-- You are not registered as being eligible to apply for a workterm, so you may not apply to this job. --</b>");
			$formEnd .= ("</td>");
		$formEnd .= ("</tr>");
	}
	elseif (reasonStudentCantApply($job_id, $student_number) == 'dept_not_applicable')
	{
		$formEnd .= ("<tr>");
			$formEnd .= ("<td colspan='2' align='center'>");
			$formEnd .= ("<b class='black'>-- Your department is not applicable for this job, so you must obtain special permission to apply --</b>");
			$formEnd .= ("</td>");
		$formEnd .= ("</tr>");
	}
	elseif (reasonStudentCantApply($job_id, $student_number) == JOB_APP_METHOD_PAPER)
	{
		$formEnd .= ("<tr>");
			$formEnd .= ("<td colspan='2' align='center'>");
            $sqlpaper = "SELECT d.job_app_paper_instructions from department as d, job_info as ji
                        where d.department_id=ji.department_id and ji.job_id='".$job_id."'";
            $resultpaper = $GLOBALS['dbh']->Execute($sqlpaper);
            $rowpaper = $resultpaper->FetchRow();
			$formEnd .= ("<b class='black'>-- "); 
			if ($rowpaper['job_app_paper_instructions'] != '')
			{
				$temp = split("\n",$rowpaper['job_app_paper_instructions']);
				for ($i = 0; $i < sizeof($temp) -1; $i++)
				{
					$formEnd .= ($temp[$i]);
					$formEnd .= ("<br />");
				}
				$formEnd .= ($temp[$i]);
			}
			else
			{
				$formEnd .= ("Apply via paper");
			}
			$formEnd .= (" --</b><br />");
		$formEnd .= ("</tr>");
	}
	elseif (reasonStudentCantApply($job_id, $student_number) == JOB_APP_METHOD_SPECIAL)
	{
		$formEnd .= ("<tr>");
			$formEnd .= ("<td colspan='2' align='center'>");
			$formEnd .= ("<b class='black'>-- "); 
			if ( trim($job->special_instructions != ''))
			{
				$temp = split("\n",$job->special_instructions);
				for ($i = 0; $i < sizeof($temp) -1; $i++)
				{
					$formEnd .= ($temp[$i]);
					$formEnd .= ("<br />");
				}
				$formEnd .= ($temp[$i]);
			}
			else
			{
				$formEnd .= ("Read 'Job description' and/or 'Special Instructions' for special instructions");
			}
			$formEnd .= (" --</b>");
			if (($job->show_special_instr_button == '1') && (trim($job->special_instr_button) != '')) {
				$formEnd .= ("<form method='post' action='" . $PHP_SELF . "'>");
				$formEnd .= ("<input type='hidden' name='select' value='apply_job' />");
				$formEnd .= ("<input type='hidden' name='job_id' value='" . $job_id . "' />");
				$formEnd .= ("<input type='hidden' name='option' value='add_special' />");
				$formEnd .= ("<input type='submit' value='". ($job->special_instr_button) ."' name='Special Button' />");
				$formEnd .= ("</form>");
			}
			$formEnd .= ("</td>");
		$formEnd .= ("</tr>");
	}

	$jobEnd .= ("</table>");
	$jobEnd .= ("</td>");
	$jobEnd .= ("</tr>");
	$jobEnd .= ("</table>");
	$jobEnd .= ("</center>");

	echo($jobOutput . $formEnd . $jobEnd);

} //else

?>
