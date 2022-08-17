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
 | Filename: PDFjob.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description: This file might not even be necessary.  Right now, there's no   |
 | problems converting the current confirm_job html code into PDF.  However,    |
 | we've been having trouble with the contact module and it's conversions, so   |
 | this file has been created just in case something DOES go horribly wrong.    |
 +------------------------------------------------------------------------------+

*/


/*
 This is the last part of the job being displayed.  It is added in either situation, but needs to be added at different
 times depending on the situation.  (If showing in PDF we just add it to the end of the form, as done at the bottom 
 when passing PDFOutput and PDFEnd in urlencoded form.  If we are simply displaying the job, we need to tack this part 
 on AFTER adding the form buttons and end).
*/


$PDFEnd .= ("</center>");
$PDFEnd .= ("</table>");
$PDFEnd .= ("</td>");
$PDFEnd .= ("</tr>");
$PDFEnd .= ("</table>");
$PDFEnd .= ("</td>");
$PDFEnd .= ("</tr>");
$PDFEnd .= ("</table>");


$PDFOutput .= ("<center>");
$PDFOutput .= ("<table border='1' width='100%'>");
$PDFOutput .= ("<tr>");
$PDFOutput .= ("<td>");
$PDFOutput .= ("<table border='1' cellpadding='4' cellspacing='0' width='100%'>");
$PDFOutput .= ("	<br />");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td align='center' colspan='2'>");
$PDFOutput .= ("		<h3>");
$PDFOutput .= (			(($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) && $job->job_code != '') ? $job->job_code . ": " : "&nbsp;");
$PDFOutput .= (			($job->position_title));
$PDFOutput .= ("		<br /><br /></h3>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td colspan='2'>");
$PDFOutput .= ("		<table border='1' width='100%'>");
$PDFOutput .= ("		<tr>");
$PDFOutput .= ("		<td nowrap='nowrap'><b>Contact name:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");
$PDFOutput .= (			($job->contact_name) ? $job->contact_name : "&nbsp;");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td nowrap='nowrap'><b>Contact e-mail:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");
$PDFOutput .= (			($job->contact_email) ? $job->contact_email : "&nbsp;");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		</tr>");
$PDFOutput .= ("		<tr>");
$PDFOutput .= ("		<td colspan='4'>");
$PDFOutput .= ("		&nbsp;");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		</tr>");
$PDFOutput .= ("		<tr>");
$PDFOutput .= ("		<td nowrap='nowrap'><b>Contact phone:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");
$PDFOutput .= (			($job->contact_phone) ? $job->contact_phone : "&nbsp;");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td nowrap='nowrap'><b>Contact fax:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");
$PDFOutput .= (			($job->contact_fax) ? $job->contact_fax : "&nbsp;");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		</tr>");
$PDFOutput .= ("		</table>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");
	
	// If they have specified that they would like");
	// to receive online applications, then display	");
	// the e-mail address that they will be sent to.");

$PDFOutput .= ("	<tr>");
$PDFOutput .= ("	<td colspan='2'><br />");
$PDFOutput .= ("	<table border='1' width='100%'>");

			if ($job->apply_method == JOB_APP_METHOD_ONLINE)
			{
				$PDFOutput .= ("
					<tr>
					<td align='center' colspan='2' nowrap='nowrap'><b>Send online applications to:</b>&nbsp;
					" . ($job->receive_address) . "
				 	</td>
				 	</tr>
					");
			}

			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
			{
				$PDFOutput .= ("	
					<tr>
					<td align='center' colspan='2'><br /><b>Status of job (once entered):</b>&nbsp;
					<b class='black'>" . getStatusName($job->status) . "</b>
					</td>
					</tr>
					");
			}

			if ($job->host_department)
                        {               
                                $PDFOutput .= ("
                                        <tr>
                                        <td align='center' colspan='2'><br /><b>Preferred host department:</b>&nbsp;
                                        " . getDepartmentName($job->host_department) . "</td>
                                        </tr>
                                        "); 
                        }
			
			if ($job->comments)
			{
				$PDFOutput .= ("
					<tr>
					<td colspan='2' align='center'><br /><br /><b>Additional comments:</b>
					</td>
					</tr>
					<tr>
					<td colspan='2' align='center'><br />" . $job->comments . "
					</td>
					</tr>
					");
			}

			if ($job->admin_comments && ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR))
			{
				$PDFOutput .= ("
					<tr>
					<td colspan='2' align='center'><br /><br /><b>Admin comments (only seen by office staff):</b>
					</td>
					</tr>
					<tr>
					<td colspan='2' align='center'><br />" . $job->admin_comments . "
					</td>
					</tr>
					");
			}

$PDFOutput .= ("	</table>");
$PDFOutput .= ("	</td>");
$PDFOutput .= ("	</tr>");
$PDFOutput .= ("	<tr><td colspan='2'><br /><hr /></td></tr>");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td colspan='2' align='center'>");
$PDFOutput .= ("		<b class='black'>Students will see the information below</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");
$PDFOutput .= ("	<tr><td colspan='2'><hr /><br /></td></tr>");
$PDFOutput .= ("	<tr align='center'>");
$PDFOutput .= ("	<td colspan='2'>");
$PDFOutput .= ("	<table border='1'>");

			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
			{
				$PDFOutput .= ("
					<tr>
					<td width='30%' align='right'><b>Job code:</b>
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
	$PDFOutput .= ("	<tr>");
	$PDFOutput .= ("		<td align='right'><b>Employer job code:</b>");
	$PDFOutput .= ("		</td>");
	$PDFOutput .= ("		<td><b class='black'>" . ($job->employer_code) . "</b>");
	$PDFOutput .= ("		</td>");
	$PDFOutput .= ("	</tr>");
}


$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td align='right'><b>Location of job offered:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");
$PDFOutput .= (			stripSlashes($job->city));
				
				if (trim($job->provstate))
				{
					$PDFOutput .= (", "
						. getProvstateName($job->provstate)
						);
				}

$PDFOutput .= (", " . getCountryName($job->country));
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td align='right'><b>Number of positions available:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>" . $job->num_positions); 
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td align='right'><b>Title of position:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>" . ($job->position_title));
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("	<td align='right'><b>Receive applications via:</b>");
$PDFOutput .= ("	</td>");
$PDFOutput .= ("	<td>");
		
			switch($job->apply_method)
			{
			case JOB_APP_METHOD_PAPER:
				$PDFOutput .= ("		Paper applications only.");
				break;
			case JOB_APP_METHOD_ONLINE:
            default:
				$PDFOutput .= ("		Online applications only.");
				break;
			}

$PDFOutput .= ("	</td>");
$PDFOutput .= ("	</tr>");

			if (trim($job->salary_amount1) != '')
			{
				$PDFOutput .= ("	
					<tr>
					<td align='right'><b>Approximate salary range:</b>
					</td>
					<td>
					");

				if ($job->salary_amount1 == $job->salary_amount2)
				{
					$PDFOutput .= ("
						$" . stripSlashes($job->salary_amount1)
						);
				}
				else
				{
					$PDFOutput .= ("
						$" . stripSlashes($job->salary_amount1)
						);

					if (trim($job->salary_amount2) != '')
					{
						$PDFOutput .= ("
							- $" . stripSlashes($job->salary_amount2)
							);
					}
				}
					$PDFOutput .= (" per " . $job->salary_period);
					$PDFOutput .= ("</td>");
					$PDFOutput .= ("</tr>");
			}

			if ($job->workterm_hours)
			{
				$PDFOutput .= ("
					<tr>
					<td align='right'><b>Hours of work per week:</b>
					</td>
					<td>	
					" . ($job->workterm_hours) . "
					</td>
					</tr>
					");
			}

$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td align='right'><b>Start of workterm:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>"); 
$PDFOutput .= (			getMonth(getTermMonth($job->start_term)));
$PDFOutput .= (", " . $job->start_year);
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");

			if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
			{
				$PDFOutput .= ("
					<tr>
					<td align='right'><b>Job closes at:</b>
					</td>
					<td>
					<b class='black'>
					");
				if (validDate($job->closing_date) != -1)
				{
					$PDFOutput .= (
						$job->closing_time . ", " .
						formatLongDate($job->closing_date)
						);
				}
				else
				{	
					$PDFOutput .= ("			
						Not yet set
						");
				}

				$PDFOutput .= ("
					</b> 
					</td>
					</tr>
					");
			}

$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td align='right'><b>Length of workterm:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");

			if (sizeof($job->workterm_lengths) > 1)
			{
				for ($i = 0; $i < (sizeof($job->workterm_lengths) - 1); $i++)
				{
					$PDFOutput .= ($job->workterm_lengths[$i] . ", ");
				}
				$PDFOutput .= ("or " . $job->workterm_lengths[$i] . " months");
			}
			else
			{
				$PDFOutput .= ($job->workterm_lengths[0] . " months");
			}

$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");

			if ($job->other_postings != '')
			{
				$PDFOutput .= ("
					<tr>
					<td align='right'><b>This job is also posted at:</b>
					</td>
					<td>
					" . $job->other_postings . "
					</td>
					</tr>
					");
			}

$PDFOutput .= ("        <tr><td colspan='2'><br /></td></tr>");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td align='right' nowrap='nowrap'><b>Minimum academic years<br />completed in program:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");

$PDFOutput .= (			$job->min_academic . " year");
$PDFOutput .= (			($job->min_academic == 1) ? "" : "s");
$PDFOutput .= (			($job->min_academic == '4') ? " or more" : "");

$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td align='right' nowrap='nowrap'><b>Minimum workterms completed:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");

$PDFOutput .= (			$job->min_workterms . " workterm");
$PDFOutput .= (			($job->min_workterms == 1) ? "" : "s");
$PDFOutput .= (			($job->min_workterms == '5') ? " or more" : "");

$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");
$PDFOutput .= ("<tr>"); 
        $PDFOutput .= ("<td nowrap='nowrap' align='right'>");
        $PDFOutput .= ("<b>Receive applications from<br />students in these departments:</b>");
        $PDFOutput .= ("</td>");
        $PDFOutput .= ("<td>");

        for ($i = 0; $i < sizeof($job->apply_departments); $i++)
        {               
                $PDFOutput .= (getDepartmentName($job->apply_departments[$i]) . ", ");
        }                       
                                
        $PDFOutput .= ("</td>");
$PDFOutput .= ("</tr>");        
                                
$PDFOutput .= ("	<tr><td colspan='2'><br /></td></tr>");

$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td align='right'><b>Employer/Company name:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>" . ($job->company_name)); 
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");

			if ($job->company_website != '')
			{
				$PDFOutput .= ("	
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
				$PDFOutput .= ("
					<tr>
					<td valign='top' align='right'>
					<b>Description of <br />Employer/Company:</b>
					</td>
					<td><br />
					");
					
				$temp = split("\n",$job->company_description);

				for ($i = 0; $i < sizeof($temp); $i++)
				{
					$PDFOutput .= ($temp[$i]);
					$PDFOutput .= ("<br />");
				}

				$PDFOutput .= ("	
					</td>
					</tr>
					");
			}

$PDFOutput .= ("	<tr><td colspan='2'><br /></td></tr>");

if ($job->department_id && $job->department_id != 'none')
{
	$PDFOutput .= ("<tr>");
		$PDFOutput .= ("<td align='right'><b>Company Division Name:</b></td>");
		$PDFOutput .= ("<td>" . $job->department_name . "</td>");
	$PDFOutput .= ("</tr>");

	if ($job->department_website != '' && $job->department_website != $job->employer_website)
	{
		$PDFOutput .= ("        
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
		$PDFOutput .= ("
			<tr>
			<td valign='top' align='right'>
			<b>Description of <br />Company Division:</b>
			</td>
			<td><br />
			");

		$temp = split("\n",$job->department_description);

		for ($i = 0; $i < sizeof($temp); $i++)
		{
			$PDFOutput .= ($temp[$i]);
			$PDFOutput .= ("<br />");
		}

		$PDFOutput .= ("
			</td>
			</tr>
			");
	}
}

if ($job->for_student_comments && ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)) {
    $PDFOutput .= ("
            <tr>
            <td colspan='2' align='center'><br /><br /><b>Job status comments for students (only seen by office staff and students):</b>
            </td>
            </tr>
            <tr>
            <td colspan='2' align='center'><br />" . nl2br($job->for_student_comments) . "
            </td>
            </tr>
            ");
}
$PDFOutput .= ("	<tr><td colspan='2'><br /></td></tr>");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td valign='top' align='right'><b>Job description:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");

$PDFOutput .= nl2br($job->job_description);

$PDFOutput .= ("		<br />");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");
$PDFOutput .= ("	<tr>");
$PDFOutput .= ("		<td valign='top' align='right'><b>Skills required:</b>");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("		<td>");

				$temp = split("\n",$job->skills_required);

				for ($i = 0; $i < sizeof($temp); $i++)
				{
					$PDFOutput .= ($temp[$i]);
					$PDFOutput .= ("<br />");
				}

$PDFOutput .= ("		<br />");
$PDFOutput .= ("		</td>");
$PDFOutput .= ("	</tr>");

			if ($job->special_requirements != '')
			{
				$PDFOutput .= ("
					<tr>
					<td valign='top' align='right' nowrap='nowrap'><b>Special requirements:</b>
					</td>
					<td>
					");

				$temp = split("\n",$job->special_requirements);
				
				for ($i = 0; $i < sizeof($temp); $i++)
				{
					$PDFOutput .= ($temp[$i]);
					$PDFOutput .= ("<br />");
				}

			$PDFOutput .= ("		<br />");
			$PDFOutput .= ("		</td>");
			$PDFOutput .= ("	</tr>");
			
			}

		/*
		 End of PDFOutput here.  We don't want to display any buttons or anything when showing
		 the job's information via PDF, so we have to assign the rest to a temporary variable.
		*/

?>
