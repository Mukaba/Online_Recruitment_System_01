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
 | Filename: confirm_inputs.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is called after all of the inputs for a job have      |
 | been entered.  It allows the user to see everything they have entered, and   |
 | shows them what the students will be able to see and not see.  It then       |
 | prompts them to either add the job, or start over.  This file can be called  |
 | so that it will not display the buttons at the bottom of the screen (for     |
 | purposes of just displaying the job).                                        |
 +------------------------------------------------------------------------------+

*/

include_once('contact/contact_display_functions.inc');

/*
 This is the last part of the job being displayed.  It is added in either situation, but needs to be added at different
 times depending on the situation.  (If showing in PDF we just add it to the end of the form, as done at the bottom 
 when passing jobOutput and jobEnd in urlencoded form.  If we are simply displaying the job, we need to tack this part 
 on AFTER adding the form buttons and end).
*/

$jobEnd .= ("</table>");
$jobEnd .= ("</td>");
$jobEnd .= ("</tr>");
$jobEnd .= ("</table>");
$jobEnd .= ("</center>");

if (!$no_buttons)
{
        instruct("Below is a preview of the new Job Description you have just created. Please ensure that all information is accurate 
	before submitting. When you finish checking over your Job Description scroll to the bottom of the page and click the 'Add Job
	Now' button.");

	if ($job->status == HOLDING)
	{
		$msg = ("Please note that you have entered this job's status as: <b>" . getAdminStatusName($job->admin_status) . "</b>.  Once you ");
		$msg .= ("save this job, you will not be able to change the status until the 24 hour holding period has ended.");
		notify($msg);
	}
}

$jobOutput .= ("<center>");
$jobOutput .= ("<table border='1' width='100%'>");
$jobOutput .= ("<tr>");
	$jobOutput .= ("<td>");
	$jobOutput .= ("<table border='0' cellpadding='4' cellspacing='0' width='100%'>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td align='center' colspan='2'>");
        $jobOutput .= ("<br />");
		$jobOutput .= ("<h3>");
		$jobOutput .= ((($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) && $job->job_code != '') ? $job->job_code . ": " : "&nbsp;");
		$jobOutput .= (($job->position_title));
		$jobOutput .= ("<br /><br /></h3>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");

	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td colspan='2'>");
		$jobOutput .= ("<table border='0' width='100%'>");
		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td><b>Contact name:</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td>");
			$jobOutput .= (($job->contact_name) ? $job->contact_name : "&nbsp;");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td><b>Contact e-mail:</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td>");
			$jobOutput .= (($job->contact_email) ? $job->contact_email : "&nbsp;");
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
			$jobOutput .= ("<td>");

			if ($job->contact_phone == USE_COMPANY)
			{
				$sql = ("
					SELECT DISTINCT phone
					FROM employer_company
					WHERE employer_id='" . $job->employer_id . "'
					");
				$result = $GLOBALS['dbh']->Execute($sql);
				$row = $result->FetchRow();
				
				$company_phone = $row["phone"];

				$jobOutput .= ($company_phone . "<br /><b class='black'>(Using Company Phone)</b>");
			}
			elseif ($job->contact_phone == USE_DEPARTMENT)
			{
				$sql = ("
					SELECT DISTINCT ed.phone
					FROM employer_department AS ed, contact_employer AS ce
					WHERE ce.contact_id='" . $job->contact_id . "'
					AND ed.department_id=ce.department_id
					");
				$result = $GLOBALS['dbh']->Execute($sql);
				$row = $result->FetchRow();

				$department_phone = $row["phone"];

				if ($department_phone == USE_COMPANY)
				{
					$sql = ("
						SELECT DISTINCT phone
						FROM employer_company
						WHERE employer_id='" . $job->employer_id . "'
						");
					$result = $GLOBALS['dbh']->Execute($sql);
					$row = $result->FetchRow();
					
					$department_phone = $row["phone"];
				}

				$jobOutput .= ($department_phone . "<br /><b class='black'>(Using Division Phone)</b>");
			}
			else
			{
				$jobOutput .= (($job->contact_phone) ? $job->contact_phone : "&nbsp;");
			}

			$jobOutput .= ("		</td>");
			$jobOutput .= ("		<td><b>Contact fax:</b>");
			$jobOutput .= ("		</td>");
			$jobOutput .= ("		<td>");

			if ($job->contact_fax == USE_COMPANY)
			{
				$sql = ("
					SELECT DISTINCT fax
					FROM employer_company
					WHERE employer_id='" . $job->employer_id . "'
					");
				$result = $GLOBALS['dbh']->Execute($sql);
				$row = $result->FetchRow();

				$company_fax = $row["fax"];

				$jobOutput .= ($company_fax . "<br /><b class='black'>(Using Company Fax)</b>");
			}
			elseif ($job->contact_fax == USE_DEPARTMENT)
			{
				$sql = ("
					SELECT DISTINCT ed.fax
					FROM employer_department AS ed, contact_employer AS ce
					WHERE ce.contact_id='" . $job->contact_id . "'
					AND ed.department_id=ce.department_id 
					");
				$result = $GLOBALS['dbh']->Execute($sql);
				$row = $result->FetchRow();

				$department_fax = $row["fax"];

				if ($department_fax == USE_COMPANY)
				{
					$sql = ("
						SELECT DISTINCT fax
						FROM employer_company
						WHERE employer_id='" . $job->employer_id . "'
						");
					$result = $GLOBALS['dbh']->Execute($sql);
					$row = $result->FetchRow();

					$department_fax = $row["fax"];
				}

				$jobOutput .= ($department_fax . "<br /><b class='black'>(Using Division Fax)</b>");
			}
			else
			{
				$jobOutput .= (			($job->contact_fax) ? $job->contact_fax : "&nbsp;");
			}

			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
        if($job->contact_cellphone)
        {
            $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='4'>");
            $jobOutput .= ("&nbsp;");
            $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
            $jobOutput .= ("<tr>");
            $jobOutput .= ("<td><b>Contact Cellphone:</b></td>");
            $jobOutput .= ("<td>");
            $jobOutput .= ($job->contact_cellphone);
            $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }
		$jobOutput .= ("</table>");
		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
	
	// If they have specified that they would like");
	// to receive online applications, then display	");
	// the e-mail address that they will be sent to.");

	if ($job->apply_method == JOB_APP_METHOD_ONLINE)
	{
		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td align='center' colspan='2' nowrap='nowrap'><b>Send online applications to:</b>&nbsp;");
			$jobOutput .= ($job->receive_address);
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}
	if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
	{
		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td align='center' colspan='2'><br /><b>Status of job (once entered):</b>&nbsp;");
			$jobOutput .= ("<b class='black'>" . getAdminStatusName($job->admin_status) . "</b>");
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}

	if ($job->host_department)
	{
		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td align='center' colspan='2'><br /><b>Preferred host department:</b>&nbsp;");
			$jobOutput .= (getDepartmentName($job->host_department));
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}
	
	if ($job->comments)
	{
		$jobOutput .= ("<tr><td colspan='2'>&nbsp;</td></tr>");

		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td colspan='2' align='center'><b>Additional comments:</b>");
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");

		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td colspan='2' align='center'>");

			$jobOutput .= nl2br($job->comments);
            $jobOutput .= ("<br />");

			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}

	if ($job->admin_comments && ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR))
	{
		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td colspan='2' align='center'><br /><br /><b>Admin comments (only seen by office staff):</b>");
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td colspan='2' align='center'>");
			$jobOutput .= nl2br($job->admin_comments); 
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}

    $contact_action_string = showMostRecentContactActions($job->contact_id);
    if (strlen($contact_action_string) && ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)) {
        $jobOutput .= ("<tr><td colspan='2'><hr />");
        $jobOutput .= $contact_action_string;
        $jobOutput .= ("</td></tr>");
    }

$jobOutput .= ("	<tr><td colspan='2'><br /><hr /></td></tr>");
$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td colspan='2' align='center'>");
$jobOutput .= ("		<b class='black'>Students will see the information below</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

$jobOutput .= ("	<tr><td colspan='2'><hr /><br /></td></tr>");

if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{
	$jobOutput .= ("
		<tr>
		<td width='50%' align='right'><b>Job code:</b>
		</td>
		<td><b class='black'>" . 
		($job->job_code ? $job->job_code : "Not yet set") . "
		</b>
		</td>
		</tr>
	");
}

if ($job->employer_code)
{
	$jobOutput .= ("	<tr>");
	$jobOutput .= ("		<td width='50%' align='right'><b>Employer job code:</b>");
	$jobOutput .= ("		</td>");
	$jobOutput .= ("		<td><b class='black'>" . ($job->employer_code) . "</b>");
	$jobOutput .= ("		</td>");
	$jobOutput .= ("	</tr>");
}


$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td width='50%' align='right'><b>Location of job offered:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("		<td>");
$jobOutput .= (			stripSlashes($job->city));
				
if (trim($job->provstate))
{
	$jobOutput .= (", " . getProvstateName($job->provstate));
}

$jobOutput .= (", " . getCountryName($job->country));
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td width='50%' align='right'><b>Number of positions available:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("		<td>" . $job->num_positions); 
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td width='50%' align='right'><b>Title of position:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("		<td>" . ($job->position_title));
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

$jobOutput .= ("	<tr>");
$jobOutput .= ("	<td width='50%' align='right'><b>Receive applications via:</b>");
$jobOutput .= ("	</td>");
$jobOutput .= ("	<td>");
		
switch($job->apply_method)
{
case JOB_APP_METHOD_PAPER:
	$jobOutput .= ("Paper applications only.");
	break;
case JOB_APP_METHOD_ONLINE:
	$jobOutput .= ("Online applications only.");
	break;
case JOB_APP_METHOD_SPECIAL:
	$jobOutput .= ("Other - Special instructions.");
	break;
}

$jobOutput .= ("	</td>");
$jobOutput .= ("	</tr>");

if (trim($job->salary_amount1) != '')
{
	$jobOutput .= ("	
		<tr>
		<td align='right'><b>Approximate salary range:</b>
		</td>
		<td>
		");

	if ($job->salary_amount1 == $job->salary_amount2)
	{
		$jobOutput .= ("
			$" . stripSlashes($job->salary_amount1)
			);
	}
	else
	{
		$jobOutput .= ("
			$" . stripSlashes($job->salary_amount1)
			);

		if (trim($job->salary_amount2) != '')
		{
			$jobOutput .= ("
				- $" . stripSlashes($job->salary_amount2)
				);
		}
	}
		$jobOutput .= (" per " . $job->salary_period);
		$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
}

if ($job->workterm_hours)
{
	$jobOutput .= ("
		<tr>
		<td align='right'><b>Hours of work per week:</b>
		</td>
		<td>	
		" . ($job->workterm_hours) . "
		</td>
		</tr>
		");
}

$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td align='right'><b>Term:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("		<td>"); 
$jobOutput .= (			getMonth(getTermMonth($job->start_term)));
$jobOutput .= (", " . $job->start_year);
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{
	$jobOutput .= ("
		<tr>
		<td align='right'><b>Job closes at:</b>
		</td>
		<td>
		<b class='black'>
		");
	if (validDate($job->closing_date) != -1)
	{
		$jobOutput .= (
			$job->closing_time . ", " .
			formatLongDate($job->closing_date)
			);
	}
	else
	{	
		$jobOutput .= ("			
			Not yet set
			");
	}

	$jobOutput .= ("
		</b> 
		</td>
		</tr>
		");
}

$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td align='right'><b>Length of workterm:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("		<td>");

if ($job->start_date && $job->end_date)
{
    $jobOutput .= ("Other - see start and end dates");
}
elseif (sizeof($job->workterm_lengths) > 1)
{
	for ($i = 0; $i < (sizeof($job->workterm_lengths) - 1); $i++)
	{
		$jobOutput .= ($job->workterm_lengths[$i] . ", ");
	}
	$jobOutput .= ("or " . $job->workterm_lengths[$i] . " months");
}
else
{
	$jobOutput .= ($job->workterm_lengths[0] . " months");
}

$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

if ($job->start_date && $job->end_date)
{
    $jobOutput .= ("	<tr>");
    $jobOutput .= ("		<td align='right'><b>Start date:</b>");
    $jobOutput .= ("		</td>");
    $jobOutput .= ("		<td>"); 
    $jobOutput .= (			formatSpecialZeroDate($job->start_date));
    $jobOutput .= ("		</td>");
    $jobOutput .= ("	</tr>");

    $jobOutput .= ("	<tr>");
    $jobOutput .= ("		<td align='right'><b>End date:</b>");
    $jobOutput .= ("		</td>");
    $jobOutput .= ("		<td>"); 
    $jobOutput .= (			formatSpecialZeroDate($job->end_date));
    $jobOutput .= ("		</td>");
    $jobOutput .= ("	</tr>");
}

if ($job->other_postings != '')
{
	$jobOutput .= ("
		<tr>
		<td align='right'><b>This job is also posted at:</b>
		</td>
		<td>
		" . $job->other_postings . "
		</td>
		</tr>
		");
}

$jobOutput .= ("<tr>");
	$jobOutput .= ("<td align='right'><b>Applicable Disciplines:</b>");
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

$jobOutput .= ("        <tr><td colspan='2'><br /></td></tr>");

$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td align='right' nowrap='nowrap'><b>Minimum academic years<br />completed in program:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("		<td>");

$jobOutput .= (			$job->min_academic . " year");
$jobOutput .= (			($job->min_academic == 1) ? "" : "s");
$jobOutput .= (			($job->min_academic == '4') ? " or more" : "");

$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td align='right' nowrap='nowrap'><b>Minimum workterms completed:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("		<td>");

$jobOutput .= (			$job->min_workterms . " workterm");
$jobOutput .= (			($job->min_workterms == 1) ? "" : "s");
$jobOutput .= (			($job->min_workterms == '5') ? " or more" : "");

$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

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

$jobOutput .= ("	<tr><td colspan='2'><br /></td></tr>");

$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td align='right'><b>Employer/Company name:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("		<td>" . ($job->company_name)); 
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

if ($job->company_website != '')
{
	$jobOutput .= ("	
		<tr>
		<td align='right'><b>Employer's website:</b>
		</td>
		<td>
		<a class='blue' target='employer_website' href='http://" . stripSlashes($job->company_website) . "'>" . stripSlashes($job->company_website) . "</a>
		</td>
		</tr>
		");
}

if ($job->company_description != '')
{
	$jobOutput .= ("<tr>");
		$jobOutput .= ("<td valign='top' align='right'>");
		$jobOutput .= ("<b>Description of <br />Employer/Company:</b>");
		$jobOutput .= ("</td>");
		$jobOutput .= ("<td><br />");

		$jobOutput .= nl2br($job->company_description);

		$jobOutput .= ("</td>");
	$jobOutput .= ("</tr>");
}

$jobOutput .= ("	<tr><td colspan='2'><br /></td></tr>");

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

        $jobOutput .= ("
                </td>
                </tr>
                ");
	}
	if ($job->displayname_on_coverltr == '1')
	{
		$jobOutput .= ("<tr>");
			$jobOutput .= ("<td align='right'><b>Address application to:</b>");
			$jobOutput .= ("</td>");
			$jobOutput .= ("<td align='left'>");
			$jobOutput .= (($job->contact_name) ? $job->contact_name : "&nbsp;");
			$jobOutput .= ("</td>");
		$jobOutput .= ("</tr>");
	}
}

if ($job->for_student_comments && ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR))
{
    $jobOutput .= ("<tr>");
        $jobOutput .= ("<td colspan='2' align='center'><br /><br /><b class='red'>Job status comments:</b>");
        $jobOutput .= ("</td>");
    $jobOutput .= ("</tr>");
    $jobOutput .= ("<tr>");
        $jobOutput .= ("<td colspan='2'>");
        $jobOutput .= nl2br($job->for_student_comments); 
        $jobOutput .= ("</td>");
    $jobOutput .= ("</tr>");
}


$jobOutput .= ("	<tr><td colspan='2'>&nbsp;</td></tr>");

$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td colspan='2' valign='top' align='center'><b>Job description:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td colspan='2'>");

$jobOutput .= nl2br($job->job_description);
$jobOutput .= ("		<br />");

$jobOutput .= ("		<br />");
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");
$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td colspan='2' valign='top' align='center'><b>Skills required:</b>");
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");
$jobOutput .= ("	<tr>");
$jobOutput .= ("		<td colspan='2'>");

$jobOutput .= nl2br($job->skills_required);
$jobOutput .= ("		<br />");

$jobOutput .= ("		<br />");
$jobOutput .= ("		</td>");
$jobOutput .= ("	</tr>");

if ($job->special_requirements != '')
{
	$jobOutput .= ("
		<tr>
		<td colspan='2' valign='top' align='center' nowrap='nowrap'><b>Special requirements:</b>
		</td>
		</tr>");

	$jobOutput .= ("
		<tr>
		<td colspan='2'>
		");

	$jobOutput .= nl2br($job->special_requirements);
    $jobOutput .= ("		<br />");
	
    $jobOutput .= ("		<br />");
    $jobOutput .= ("		</td>");
    $jobOutput .= ("	</tr>");

}

if ((($job->special_instructions != '') || ($job->show_special_instr_button == '1')) && ($job->apply_method == JOB_APP_METHOD_SPECIAL))
{
	$jobOutput .= ("
		<tr>
		<td colspan='2' valign='top' align='center' nowrap='nowrap'><b>Special Instructions:</b>
		</td>
		</tr>");

	$jobOutput .= ("
		<tr>
		<td colspan='2'>
		");

	$jobOutput .= nl2br($job->special_instructions);

	if ($job->show_special_instr_button == '1')
	{
		$jobOutput .= ("Using special instructions button: Yes <br />");

		$jobOutput .= ("Special instructions button text: \"" . ($job->special_instr_button) . "\"");
	}
	
	$jobOutput .= ("               </td>");
	$jobOutput .= ("        </tr>");
}


/*
 End of jobOutput here.  We don't want to display any buttons or anything when showing
 the job's information via PDF, so we have to assign the rest to a temporary variable.
*/

/*
 Now we must form $formEnd.  This variable is used to show the end of the form and its buttons
 when we display this.  This variable is ignored when showing in PDF, as we don't want to 
 display and form inputs or buttons when showing in PDF format.
*/

if (!$no_buttons)
{
	$formEnd .= ("
		<tr>
			<td colspan='2'>
			<table border='0' width='100%'>
			<tr align='center'>
				<td width='50%'>
				<br /><br />
                <form method='post' action='" . $PHP_SELF . "'>
                <input type='hidden' name='select' value='add_job' />
                <input type='hidden' name='job' value='" . packObject($job) . "' />
				<input type='submit' name='continue' value='Add job now' />
				</form>
				</td>
				<td>
				<br /><br />
				<form method='post' action='" . $PHP_SELF . "'>
				<input type='hidden' name='select' value='add_job' />
				<input type='submit' value='Start Over' />
				</form>
				</td>
			</tr>
			</table>
			</td>
		</tr>
		");
}
elseif ($no_buttons)
{
	$formEnd .= ("
		<tr>
			<td colspan='2' align='center'>
			<table border='0'>
			<tr align='center'>
				<td width='50%'>
                <form method='post' action='" . $PHP_SELF . "'>
                <input type='hidden' name='select' value='add_job' />
                <input type='hidden' name='job' value='" . packObject($job) . "' />
				<input type='hidden' name='HTMLjob' value='" . urlencode($jobOutput . $jobEnd) . "' />
				<input type='hidden' name='PDF' value='1' />
				<input type='hidden' name='select' value='jobOutput' />
				<input type='submit' value='View in PDF' />
				</form>
				</td>
				<td width='50%'>
				<form method='post' action='" . $PHP_SELF . "'>
				<input type='hidden' name='job_id' value='" . $job_id . "' />
				<input type='hidden' name='select' value='edit_job' />
				<input type='submit' value='Edit Job' />
				</form>
				</td>
			</tr>
			</table>
			</td>
		</tr>
		");
}

echo($jobOutput . $formEnd . $jobEnd);


?>
