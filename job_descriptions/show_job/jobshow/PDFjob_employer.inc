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
 | Filename: PDFjob_employer.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This code takes the job_id that is passed to it and withdraws   |
 | all the information pertaining to the specific job out of the database, and  |
 | displays it.                                                                 |
 +------------------------------------------------------------------------------+

*/

$job_id = urldecode($job_id);

$job = new EditJob('');
$job->loadValues($job_id);

if ($job->salary_amount1 == '' XOR $job->salary_amount2 == '') 
{
	/*
	 This should never happen, but just in case it does...
	*/

	if ($job->salary_amount1 == '')
	{
		$job->salary_amount1 = $job->salary_amount2;
	}
	if ($job->salary_amount2 == '')
	{
		$job->salary_amount2 = $job->salary_amount1;
	}
}

$PDFOutput .= ("
	<center>
	<table border='1' width='100%'>
	<tr>
	<td>
	<table border='0' cellpadding='4' cellspacing='0' width='100%'>
	");

	$PDFOutput .= ("<br />");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='center' colspan='2'>");
		$PDFOutput .= ("<h3>");
		$PDFOutput .= ($job->job_code ? $job->job_code . ": " : "&nbsp;");
		$PDFOutput .= ($job->position_title);
		$PDFOutput .= ("<br /><br /></h3>");
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	$PDFOutput .= ("<tr>");	
		$PDFOutput .= ("<td colspan='2'>");
		$PDFOutput .= ("<table width='100%'>");
		$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td><b>Contact name:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>" . ($job->contact_name ? $job->contact_name : "&nbsp;"));
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td><b>Contact e-mail:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>" . ($job->contact_email ? $job->contact_email : "&nbsp;"));
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("</tr>");
		$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td colspan='4'>");
		$PDFOutput .= ("&nbsp;");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("</tr>");
		$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td><b>Contact phone:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>" . ($job->contact_phone ? $job->contact_phone : "&nbsp;"));
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td><b>Contact fax:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>" . ($job->contact_fax ? $job->contact_fax : "&nbsp;"));
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("</tr>");
		$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td colspan='4'>");
		$PDFOutput .= ("&nbsp;");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("</tr>");
		$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td><b>Contact Cellphone:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>" . ($job->contact_cellphone ? $job->contact_cellphone : "&nbsp;"));
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("</tr>");
		$PDFOutput .= ("</table>");
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	


	// If they have specified that they would like
	// to receive online applications, then display	
	// the e-mail address that they will be sent to.

	if ($job->apply_method == JOB_APP_METHOD_ONLINE)
	{
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td colspan='2' align='center' nowrap='nowrap'><b>Send online applications to:</b>&nbsp;");
		$PDFOutput .= ($job->receive_address); 
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	}

	if ($job->comments)
	{
	$PDFOutput .= ("<tr>");
                $PDFOutput .= ("<td colspan='2' align='center'><br /><br /><b>Additional comments:</b>");
                $PDFOutput .= ("</td>");
        $PDFOutput .= ("</tr>");

        $PDFOutput .= ("<tr>");
                $PDFOutput .= ("<td colspan='2' align='center'><br />" . $job->comments);
                $PDFOutput .= ("</td>");
        $PDFOutput .= ("</tr>");
        }

	$PDFOutput .= ("<tr><td colspan='2'><br /><hr /></td></tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td colspan='2' align='center'>");
		$PDFOutput .= ("<b class='black'>Students will see the information below</b>");
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	$PDFOutput .= ("<tr><td colspan='2'><hr /></td></tr>");

// Students will see all of the information below this point.
	
        $PDFOutput .= ("<tr>");
        $PDFOutput .= ("<td colspan='2' align='center'>");

        $PDFOutput .= ("<table border='0' width='100%' align='center'>");

	if ($job->job_code != '')
	{
		$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td width='50%' align='right'><b>Job code:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td width='50%'><b class='black'>" . ($job->job_code) . "</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("</tr>");
	}

	if ($job->employer_code)
	{
		$PDFOutput .= ("<tr>");
			$PDFOutput .= ("<td align='right'><b>Employer job code:</b>");
			$PDFOutput .= ("</td>");
			$PDFOutput .= ("<td><b class='black'>" . ($job->employer_code) . "</b>");
			$PDFOutput .= ("</td>");
		$PDFOutput .= ("</tr>");
	}

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Title of position:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>" . ($job->position_title));
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Location of job:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");
		$PDFOutput .= ($job->city);

		if (trim($job->provstate))
		{
			$PDFOutput .= (", " . getProvstateName($job->provstate));
		}

		$PDFOutput .= (", " . getCountryName($job->country));
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Number of positions:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>" . $job->num_positions); 
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'>");
		$PDFOutput .= ("<b>Apply:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");

		switch( trim($job->apply_method))
		{
		case JOB_APP_METHOD_PAPER:
			$PDFOutput .= ("<b class='black'>Via paper to the dropboxes</b>");
			break;
		case JOB_APP_METHOD_SPECIAL:
			$PDFOutput .= ("<b class='black'>Other - Special instructions.</b>");
			break;
		case JOB_APP_METHOD_ONLINE:
		default:
			$PDFOutput .= ("<b class='black'>Electronically</b>");
			break;
		}

		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Term:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");
		$PDFOutput .= (getMonth(getTermMonth($job->start_term)));
		$PDFOutput .= (", " . $job->start_year);
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

		if ($job->closing_date != '-1')
		{
			$PDFOutput .= ("<tr>");
			$PDFOutput .= ("<td align='right'><b>Job closes at:</b>");
			$PDFOutput .= ("</td>");
			$PDFOutput .= ("<td><b class='black'>");
			$PDFOutput .= ($job->closing_time . ", ");
			$PDFOutput .= (formatLongDate($job->closing_date));

			if ($job->closing_date < $current_date)
			{
				$PDFOutput .= ("&nbsp;(</b><font style='color:red;'>closed</font><b class='black'>)</b>");
			}
			$PDFOutput .= ("</td>");
			$PDFOutput .= ("</tr>");
		}
	
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Length of workterm:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");

        if ($job->start_date && $job->end_date)
        {
            $PDFOutput .= ("Other - see start and end dates");
        }
		elseif (sizeof($job->workterm_lengths) == 1)
		{
			$PDFOutput .= ($job->workterm_lengths[0] . " months");
		}
		else
		{
			for ($i = 0; $i < (sizeof($job->workterm_lengths) - 1); $i++)
			{
				$PDFOutput .= ($job->workterm_lengths[$i] . ", ");
			}
			$PDFOutput .= ("or " . $job->workterm_lengths[$i] . " months");
		}

		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

    if ($job->start_date && $job->end_date)
    {
        $PDFOutput .= ("<tr>");
            $PDFOutput .= ("<td align='right'><b>Start date:</b>");
            $PDFOutput .= ("</td>");
            $PDFOutput .= ("<td>");
            $PDFOutput .= (formatSpecialZeroDate($job->start_date));
            $PDFOutput .= ("</td>");
        $PDFOutput .= ("</tr>");

        $PDFOutput .= ("<tr>");
            $PDFOutput .= ("<td align='right'><b>End date:</b>");
            $PDFOutput .= ("</td>");
            $PDFOutput .= ("<td>");
            $PDFOutput .= (formatSpecialZeroDate($job->end_date));
            $PDFOutput .= ("</td>");
        $PDFOutput .= ("</tr>");
    }

	if ($job->salary_amount1 != '')
	{
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Approximate<br />salary range:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");
		if ($job->salary_amount1 == $job->salary_amount2)
		{
			$PDFOutput .= ("$" . ($job->salary_amount1));
		}
		else
		{
			$PDFOutput .= ("$" . ($job->salary_amount1) . " to $" . ($job->salary_amount2));
		}
		$PDFOutput .= (" per " . $job->salary_period);
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	}

	if ($job->workterm_hours != '')
	{
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Hours of work per week:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");
		$PDFOutput .= ($job->workterm_hours);
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	}

	if ( trim($job->other_postings) != '')
	{
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Job is also posted at:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");
		$PDFOutput .= ( trim($job->other_postings));
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	}

	/*
	$PDFOutput .= ("<tr>");
                $PDFOutput .= ("<td align='right'><b>Job's Industry:</b>");
                $PDFOutput .= ("</td>");
                $PDFOutput .= ("<td>");
                $PDFOutput .= (getIndustryName($job->industry));
                $PDFOutput .= ("</td>");
        $PDFOutput .= ("</tr>");
	*/

	if ($job->job_disciplines)
        {
                $PDFOutput .= ("<tr>");
                        $PDFOutput .= ("<td align='right'><b>Applicable Disciplines:</b>");
                        $PDFOutput .= ("</td>");
                        $PDFOutput .= ("<td>");
                        $PDFOutput .= ("<b class='black'>");
                        foreach ($job->job_disciplines AS $jd)
                        {
                                $disc_codes .= getDisciplineCode($jd) . ", ";
                        }

                        $PDFOutput .= substr($disc_codes, 0, -2);
                        $PDFOutput .= ("</b>");
                        $PDFOutput .= ("</td>");
                $PDFOutput .= ("</tr>");
        }

	$PDFOutput .= ("<tr><td colspan='2'><br /></td></tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Minimum academic year completed:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");
		$PDFOutput .= ($job->min_academic . " year");
		$PDFOutput .= (($job->min_academic == '1') ? "" : "s");
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Minimum number of workterms completed:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");
		$PDFOutput .= ($job->min_workterms . " workterm");
		$PDFOutput .= (($job->min_workterms == '1') ? "" : "s");
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>"); 

	$PDFOutput .= ("<tr>");
                $PDFOutput .= ("<td nowrap='nowrap' align='right'>");
                $PDFOutput .= ("<b>Receive applications from<br />students in these departments:</b>");
                $PDFOutput .= ("</td>");
                $PDFOutput .= ("<td>");
                
                for ($i = 0; $i < (sizeof($job->apply_departments) - 1); $i++) 
                {
                        $PDFOutput .= (getDepartmentName($job->apply_departments[$i]) . ", ");
                }
		$PDFOutput .= (getDepartmentName($job->apply_departments[$i]));
                
                $PDFOutput .= ("</td>");
        $PDFOutput .= ("</tr>");

	$PDFOutput .= ("<tr><td colspan='2'><br /></td></tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right' nowrap='nowrap'><b>Employer/Company name:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>" . ($job->company_name)); 
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	if ($job->company_website != '')
	{
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Employer's website:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");
		$PDFOutput .= ("<a class='blue' target='employer_website' href='http://" . ($job->company_website . "'>" . ($job->company_website) . "</a>"));
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	}

	if ($job->company_description != '')
	{
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td valign='top' align='right'>");
		$PDFOutput .= ("<b>Description of <br />Employer/Company:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td><br />");

		$temp = split("\n",$job->company_description);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$PDFOutput .= ($temp[$i]);
			$PDFOutput .= ("<br />");
		}

		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	}
    if ($job->displayname_on_coverltr == '1')
    {
        $PDFOutput .= ("<tr>");
        $PDFOutput .= ("<td align='right' nowrap='nowrap'><b>Address application to:</b>");
        $PDFOutput .= ("</td>");
        $PDFOutput .= ("<td align='left' nowrap='nowrap'>");
            $PDFOutput .= (($job->contact_name) ? $job->contact_name : "&nbsp;");
        $PDFOutput .= ("</td>");
        $PDFOutput .= ("</tr>");
    }


	$PDFOutput .= ("<tr><td colspan='2'><br /></td></tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td valign='top' align='right'><b>Job description:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");

		$temp = split("\n",$job->job_description);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$PDFOutput .= ($temp[$i]);
			$PDFOutput .= ("<br />");
		} 

		$PDFOutput .= ("<br />");
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td valign='top' align='right'><b>Skills required:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");

		$temp = split("\n",$job->skills_required);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$PDFOutput .= ($temp[$i]);
			$PDFOutput .= ("<br />");
		}

		$PDFOutput .= ("<br />");
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");

	if ($job->special_requirements != '')
 	{
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Special requirements:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");

		$temp = split("\n",$job->special_requirements);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$PDFOutput .= ($temp[$i]);
			$PDFOutput .= ("<br />");
		}

		$PDFOutput .= ("<br />");
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	}

	if ($job->special_instructions != '')
	{
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td valign='top' align='right' nowrap='nowrap'><b>Special Instructions:</b>");
		$PDFOutput .= ("</td>");
		$PDFOutput .= ("<td>");
	
		$PDFOutput .= nl2br($job->special_instructions);

        $PDFOutput .= ("<br />");
		$PDFOutput .= ("<br />");
		$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	}

	$PDFOutput .= ("</table>");
	$PDFOutput .= ("</td>");
	$PDFOutput .= ("</tr>");
	
$PDFOutput .= ("</center>");
$PDFOutput .= ("</table>");
$PDFOutput .= ("</td>");
$PDFOutput .= ("</tr>");
$PDFOutput .= ("</table>");

?>

