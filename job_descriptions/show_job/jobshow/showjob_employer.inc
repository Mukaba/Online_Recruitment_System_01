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
 | Filename: showjob_employer.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This code takes the job_id that is passed to it and withdraws   |
 | all the information pertaining to the specific job out of the database, and  |
 | displays it.                                                                 |
 +------------------------------------------------------------------------------+

*/

echo("<tr>");
echo("<td>");

include('job_descriptions/show_job/jobshow/PDFjob_employer.inc');
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
if ($job->status != INT_ONLY && $job->status != REJECTED)
{
$formStart .= ("
	<form method='post' action='" . ($PHP_SELF) . "'>
	<input type='hidden' name='select' value='edit_job' />
	<input type='hidden' name='job' value='" . ( isset($job) ? packObject(($job)) : "") . "' />
	<input type='hidden' name='job_id' value='" . ($job_id) . "' />
	");


$jobOutput .= ("
	<center>
	<table border='1' width='100%'>
	<tr>
	<td>
	<table border='0' cellpadding='4' cellspacing='0' width='100%'>
	");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='center' colspan='2'>");
        $jobOutput .= ("<br />");
		$jobOutput .= ("<h3>");
		$jobOutput .= ($job->job_code ? $job->job_code . ": " : "&nbsp;");
		$jobOutput .= ($job->position_title);
		$jobOutput .= ("<br /><br /></h3>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");	
		$jobOutput .= ("<td colspan='2'>");
		$jobOutput .= ("<table width='100%'>");
		$jobOutput .= ("<tr>");
		$jobOutput .= ("<td><b>Contact name:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>" . ($job->contact_name ? $job->contact_name : "&nbsp;"));
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td><b>Contact e-mail:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>" . ($job->contact_email ? $job->contact_email : "&nbsp;"));
		$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
		$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='4'>");
		$jobOutput .= ("&nbsp;");
		$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
		$jobOutput .= ("<tr>");
		$jobOutput .= ("<td><b>Contact phone:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>" . ($job->contact_phone ? $job->contact_phone : "&nbsp;"));
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td><b>Contact fax:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>" . ($job->contact_fax ? $job->contact_fax : "&nbsp;"));
		$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
        if($job->contact_cellphone)
        {  
            $jobOutput .= ("<tr><td>&nbsp;</td></tr>");
            $jobOutput .= ("<tr>");
            $jobOutput .= ("<td><b>Contact Cell:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>" .$job->contact_cellphone);
            $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }
        
		$jobOutput .= ("</table>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	
	// If they have specified that they would like to receive online applications, then display	the e-mail address that they will be sent to.

	if ($job->apply_method == JOB_APP_METHOD_ONLINE)
	{
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' align='center' nowrap='nowrap'><b>Send online applications to:</b>&nbsp;");
            $jobOutput .= ($job->receive_address); 
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
	}

	if ($job->comments)
	{
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' align='center'><br /><br /><b>Additional comments:</b>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' align='center'><br />" . $job->comments);
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
    }

	$jobOutput .= ("<tr><td colspan='2'><br /><hr /></td></tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2' align='center'>");
		$jobOutput .= ("<b class='black'>Students will see the information below</b>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr><td colspan='2'><hr /></td></tr>");

    // Students will see all of the information below this point.

	if ($job->job_code != '')
	{
		$jobOutput .= ("<tr>");
            $jobOutput .= ("<td width='50%' align='right'><b>Job code:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td><b class='black'>" . ($job->job_code) . "</b>");
            $jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}

	if ($job->employer_code)
	{
		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td align='right'><b>Employer job code:</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td><b class='black'>" . ($job->employer_code) . "</b>");
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Title of position:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>" . ($job->position_title));
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Location of job:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>");
		$jobOutput .= ($job->city);

		if (trim($job->provstate))
		{
			$jobOutput .= (", " . getProvstateName($job->provstate));
		}

		$jobOutput .= (", " . getCountryName($job->country));
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Number of positions:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>" . $job->num_positions); 
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'>");
		$jobOutput .= ("<b>Apply:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>");

		switch( trim($job->apply_method))
		{
            case JOB_APP_METHOD_PAPER:
                $jobOutput .= ("<b class='black'>Via paper to the dropboxes</b>");
                break;
            case JOB_APP_METHOD_SPECIAL:
                $jobOutput .= ("<b class='black'>Other - Special instructions.</b>");
                break;
            case JOB_APP_METHOD_ONLINE:
            default:
                $jobOutput .= ("<b class='black'>Electronically</b>");
                break;
		}

		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Term:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>");
		$jobOutput .= (getMonth(getTermMonth($job->start_term)));
		$jobOutput .= (", " . $job->start_year);
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

		if ($job->closing_date != '-1')
		{
			$jobOutput .= ("<tr>");
			$jobOutput .= ("<td align='right'><b>Job closes at:</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td><b class='black'>");
			$jobOutput .= ($job->closing_time . ", ");
			$jobOutput .= (formatLongDate($job->closing_date));

			if ($job->closing_date < $current_date)
			{
				$jobOutput .= ("&nbsp;(</b><font style='color:red;'>closed</font><b class='black'>)</b>");
			}
			$jobOutput .= ("</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("</tr>");
		}
	
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Length of workterm:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>");

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
            $jobOutput .= ("<td>");
            $jobOutput .= (formatSpecialZeroDate($job->start_date));
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>End date:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");
            $jobOutput .= (formatSpecialZeroDate($job->end_date));
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
    }
	if ($job->salary_amount1 != '')
	{
		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td align='right'><b>Approximate<br />salary range:</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td>");
			if ($job->salary_amount1 == $job->salary_amount2)
			{
				$jobOutput .= ("$" . ($job->salary_amount1));
			}
			else
			{
				$jobOutput .= ("$" . ($job->salary_amount1) . " to $" . ($job->salary_amount2));
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
			$jobOutput .= ("<td>");
			$jobOutput .= ($job->workterm_hours);
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}

	/* We're not doing this anymore
	$jobOutput .= ("<tr>");
                $jobOutput .= ("<td align='right'><b>Job's Industry:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                $jobOutput .= (getIndustryName($job->industry));
                $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
	*/

	$jobOutput .= ("<tr>");
                $jobOutput .= ("<td nowrap='nowrap' align='right'>");
                $jobOutput .= ("<b>Receive applications from<br />students in these departments:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                
                for ($i = 0; $i < (sizeof($job->apply_departments) - 1); $i++) 
                {
                        $jobOutput .= (getDepartmentName($job->apply_departments[$i]) . ", ");
                }
		$jobOutput .= (getDepartmentName($job->apply_departments[$i]));
                
                $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

	if ($job->job_disciplines)
	{
		$disc_codes = '';
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
			$jobOutput .= ("<td align='right'><b>Job is also posted at:</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td>");
			$jobOutput .= ( trim($job->other_postings));
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}

	$jobOutput .= ("<tr><td colspan='2'><br /></td></tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Minimum academic year completed:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>");
		$jobOutput .= ($job->min_academic . " year");
		$jobOutput .= (($job->min_academic == '1') ? "" : "s");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Minimum number of workterms completed:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>");
		$jobOutput .= ($job->min_workterms . " workterm");
		$jobOutput .= (($job->min_workterms == '1') ? "" : "s");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>"); 

	$jobOutput .= ("<tr><td colspan='2'><br /></td></tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right' nowrap='nowrap'><b>Employer/Company name:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>" . ($job->company_name)); 
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	if ($job->company_website != '')
	{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='right'><b>Employer's website:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td>");
		$jobOutput .= ("<a class='blue' target='employer_website' href='http://" . ($job->company_website . "'>" . ($job->company_website) . "</a>"));
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	}


	if ($job->company_description != '')
	{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td valign='top' align='right'>");
		$jobOutput .= ("<b>Description of <br />Employer/Company:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td><br />");

		$jobOutput .= nl2br($job->company_description);

        $jobOutput .= ("<br />");
        $jobOutput .= ("<br />");

		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	}

    if ($job->department_id && $job->department_id != 'none')
    {
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
                    <td valign='top' align='right'>
                    <b>Description of <br />Company Division:</b>
                    </td>
                    <td><br />
                    ");

            $jobOutput .= nl2br($job->department_description);
            $jobOutput .= ("<br />");

            $jobOutput .= ("
                    </td>
                    </tr>
                    ");
        }
    }
    if ($job->displayname_on_coverltr == '1')
    {
        $jobOutput .= ("<tr>");
        $jobOutput .= ("<td align='right' nowrap='nowrap'><b>Address application to:");
        $jobOutput .= ("</td>");
        $jobOutput .= ("<td align='left' nowrap='nowrap'>");
            $jobOutput .= (($job->contact_name) ? $job->contact_name : "&nbsp;");
        $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
    }

    $jobOutput .= ("<tr><td colspan='2'><br /></td></tr>");
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td valign='top' colspan='2' align='center'><b>Job description:</b>");
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
		$formEnd .= ("<td colspan='2' valign='top' align='center'>");
		if (($job->status != POSTED) && ($select != 'view_job'))
		{
			$formEnd .= ("<input type='submit' value='Edit Job' />");
			$formEnd .= ("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
		}
		$formEnd .= ("<input type='hidden' name='HTMLjob' value='" . urlencode($PDFOutput) . "' />");
		$formEnd .= ("<input type='submit' name='PDF' value='View in PDF' />");
		$formEnd .= ("<br /><br />");
		$formEnd .= ("</td>");
	$formEnd .= ("</tr>");

$jobEnd1 .= ("</table>");
$jobEnd1 .= ("</td>");
$jobEnd1 .= ("</tr>");
$jobEnd1 .= ("</table>");
$jobEnd1 .= ("</center>");
$jobEnd1 .= ("</form>");
$jobEnd1 .= ("</td>");
$jobEnd1 .= ("</tr>");

echo($formStart . $jobOutput . $formEnd . $jobEnd1);

} else
{
    error("Job ".$job->job_code." has no job description as this job is an interview only job, or the job was rejected.");
}

?>

