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
 | Filename: applicant_list.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is used to pull all of the applicants out of the      |
 | database for a given job.                                                    |
 +------------------------------------------------------------------------------+

*/

assert(isset($job_id));

if  (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {

	$sql = ("SELECT DISTINCT ji.job_code, ji.term_id, ji.year, ji.position_title, ji.closing_date,
		ji.employer_code, CONCAT(t.term_name, ', ', ji.year) as start_term_year, ji.apply_method
		FROM job_info as ji, term as t
		WHERE ji.job_id = '" . $job_id . "'
		AND ji.employer_id = '$employer_id'
		AND ji.status = '" . CLOSED . "'
		AND ji.term_id = t.term_id");
}

elseif ($userlevel == ADMINEMPLOYER) {

	$sql = ("SELECT DISTINCT ji.job_code, ji.term_id, ji.year, ji.position_title, ji.closing_date,
		ji.employer_code, CONCAT(t.term_name, ', ', ji.year) as start_term_year, ji.apply_method
		FROM job_info as ji, contact_employer as ce, term as t
		WHERE ji.job_id = '" . $job_id . "'
		AND ji.employer_id = ce.employer_id
		AND ce.contact_id = '$employer_contact_id_login' 
		AND ji.status = '" . CLOSED . "'
		AND ji.term_id = t.term_id");
}

else
	include ('misc/loguser.inc');


$result = $GLOBALS['dbh']->Execute($sql);

if (!$result->RecordCount())
{
	/*
	 This should only happen if the employer is URL hacking, in which case he is a l33t hax0r d00d.
	*/

	error("The job you are trying to view applicants for either doesn't exist, is not posted with your company,
		or has not yet closed.  Please push the BACK button on your browser and choose one of the jobs listed.");
}
else
{
	$row = $result->FetchRow();
	
	$job_code = $row["job_code"];
	$emp_code = $row["employer_code"];
	$start_term_year = $row["start_term_year"];
	$position_title = $row["position_title"];
	$job_term = $row["term_id"];
	$job_year = $row["year"];
	$closing_date = $row["closing_date"];
	$apply_method = $row["apply_method"];
	
	if ($apply_method == JOB_APP_METHOD_ONLINE) {
		$is_online = true;
    }

	$apps = $GLOBALS['dbh']->Execute ("SELECT application_id FROM applications
				WHERE job_id = '$job_id'
				AND app_status NOT IN ('". APP_APPLIED_ONLINE ."', '". APP_ADMIN_DELETED ."', '". APP_STUDENT_DELETED ."', '". APP_PAPER_APPLIED ."')");
	$num_apps = $apps->RecordCount();

	$next_term_year = getNextTermYear();
	$prev_term_year = getPreviousTermYear();
	// see if they can view the applications based on the current term/semester
	// this deals with any jobs in the future
	if ($job_year >= $next_term_year["year"]) {
		$correct = true;
    }
	elseif ($job_year == $next_term_year["year"]) {

		// check to see if the job term's starting month is greater than the current month
		$termrow = $GLOBALS['dbh']->GetRow("SELECT start_date FROM term WHERE term_id = '$job_term'");
		$job_mon = $termrow["start_date"];
		
		if (date("m") < $job_mon)
			$correct = true;
	}

	if ($correct ||	(($job_term == getCurrentTermID ()) && ($job_year == getCurrentYear ())) ||
		(($job_term == $prev_term_year["term"]) && ($job_year == $prev_term_year["year"])))

		$view_allowed = true;
	else
		$view_allowed = false;
	
	echo ("<h3>Viewing applicants for: " . $position_title . "</h3>");
	
	echo ("<table>");
	if ($emp_code != '')
		echo ("<tr><td align='left'><b>Your Job Code:</b></td><td align='left'>$emp_code</td></tr>");
	echo ("<tr><td align='left'><b>Co-op Job Code:</b></td><td align='left'>$job_code</td></tr>");
	echo ("<tr><td align='left'><b>Start Term:</b></td><td align='left'>$start_term_year</td></tr>");
	echo ("<tr><td align='left'><b>Applications:</b></td><td align='left'>$num_apps ($apply_method)</td></tr>");
	echo ("</table>");
	
	/*
	 Pull out the applicants for this job.  For now, ignore that they may be with another department.
	 If this module is to be used in the future, THIS MUST BE CHANGED.
	*/
	
	$sql = "SELECT DISTINCT a.application_id, a.resume_id, a.coverletter_id, s.student_number, 
		CONCAT(s.first_name, ' ', s.last_name) AS name, aps.app_status_description
		FROM applications AS a, student AS s, application_status AS aps
		LEFT JOIN employer_apps_flagged AS eaf ON (a.application_id = eaf.application_id 
			AND eaf.contact_id = '". ($userlevel == ADMINEMPLOYER ? $employer_contact_id_login : $contact_id) ."')
		WHERE a.job_id='$job_id'
		AND a.app_status IN ('". APP_ACTIVATED_ONLINE ."', '". APP_UNAVAILABLE ."', '". APP_PAPER_SENT ."')
		AND a.app_status = aps.app_status_id
		AND s.student_number=a.student_number
		ORDER BY s.last_name";
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount())
	{
		if ($view_allowed) {
			instruct ("Below is a list of students who have applied to this job. The applications are viewed in PDF format. 
			 <br /><br />
			    <b class='instruct'>To view <i>all</i> student applications</b>, select 'All' from the first drop-down-menu on the 
			      View Action Bar. Then click on the second drop-down-menu to select the part of the application package you 
			        want to view: Applications, Cover letters, or Resumes.  When you have made your selections, click 'View'.
			<br /><br />	    
                <b class='instruct'>To view <i>selected</i> student applications</b>, first select the students you wish to view 
                by clicking on the checkboxes to the left of the relevant students' names. Then choose 'Selected' from the first drop-down-menu on the
                View Action Bar. Click on the second drop-down-menu to select the part of the application package you
                want to view: Applications, Cover Letters, or Resumes. When you have made your selections, click 'View'.
            <br /><br />
				      <b class='instruct'>To view <i>individual</i> student applications</b>, click on the student's hyper-linked name from the Name column.
				");
		}

		else {
			notify ("The applicants for this job are listed below.  As this job is too far in the past, you can only 
            view the application package of a student you have hired.");
		}
        echo("<br />");

		// test to see if there are any shortlisted students, if so we can it to the dropdown for viewing
		$sls = "SELECT student_number FROM students_shortlisted
			WHERE job_id = '$job_id'";
		$slresult = $GLOBALS['dbh']->Execute ($sls);
		if ($slresult->FetchRow())
			$has_shortlisted = true;
		// now check if they have anybody flagged
		$fls = "SELECT application_id FROM employer_apps_flagged
			WHERE contact_id = '". ($userlevel == ADMINEMPLOYER ? $employer_contact_id_login : $contact_id) ."'";
		$flresult = $GLOBALS['dbh']->Execute ($fls);
		if ($flresult->FetchRow())
			$has_flagged = true;
		
		echo ("<form method='post' action='" . $PHP_SELF . "'>");
		// everywhere else uses this $PDF variable, but it's just annoying here, so I took it out
		echo ("<input type='hidden' name='PDF' value='1' />");
		echo ("<input type='hidden' name='job_id' value='" . urlencode($job_id) . "' />");
		echo ("<input type='hidden' name='select' value='applications' />");

		if ($view_allowed && $is_online) {
			echo ("<table class='row1' cellpadding='4'>");
				echo ("<tr valign='middle'>");
					echo ("<td valign='middle'>&nbsp;");
					echo ("<b class='black'>View:</b> &nbsp; ");
					echo ("<select name='view_who1'>");
						echo ("<option value='all' selected='selected'>All</option>");
						echo ("<option value='selected'>Selected</option>");
						if ($has_flagged)
							echo ("<option value='flagged'>Flagged</option>");
						if ($has_shortlisted)
							echo ("<option value='shortlisted'>Shortlisted</option>");
					echo ("</select> &nbsp; ");
					echo ("<select name='part_of_app1'>");
						echo ("<option value='whole' selected='selected'>Applications</option>");
						echo ("<option value='letter'>Cover letters</option>");
						echo ("<option value='resume'>Resumes</option>");
					echo ("</select> &nbsp; ");
					echo ("<input type='submit' name='view_apps1' value=' View ' />");
					echo ("&nbsp;</td>");
				echo ("</tr>");
			echo ("</table>");
		}
		echo ("<br />");

		echo ("<table cellspacing='0' cellpadding='0' border='1'>");
		echo ("<tr>");
			echo ("<td>");
		        echo ("<table border='0' cellpadding='2'>");
		
		        echo ("<tr>");
				if ($view_allowed) {
					echo ("<td class='rowgrey' align='center'><img src='misc/images/job_flagged.gif' alt='' /></td>");
		        		echo ("<td class='rowgrey' align='center'>&nbsp; &nbsp; </td>");
				}
				echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Name</b>&nbsp;</td>");
				if ($view_allowed)
					echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");
				echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Shortlisted</b>&nbsp;</td>");
				echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Department(s)</b>&nbsp;</td>");
		        echo ("</tr>");
		
			$rowclass = 0;
			// need to reset this since we come back right away sometimes (flags, shortlist)
			$all_apps  = array ();
			while ($row = $result->FetchRow())
			{
				$student_number = $row["student_number"];
				$name = $row["name"];
		                $resume_id = $row["resume_id"];
				$app_id = $row["application_id"];
				$status = $row["app_status_description"];
				if ($status == 'Unavailable')
					$unavailable = true;
				else {
					$unavailable = false;
					// put those that are available into an array so we can view all
					$all_apps[] = $app_id;
				}
				
				$depts = "SELECT d.department_name
					FROM department as d, student_department as sd, student_flags_join as sfj
					WHERE sd.student_number = '". $student_number ."'
					AND d.department_id = sd.department_id
					AND sd.record_id = sfj.record_id
					AND sfj.student_flags_id = '".CURRENT_FLAG."'";
				$dresult = $GLOBALS['dbh']->Execute ($depts);

				$drow = $dresult->FetchRow();
				$department = $drow["department_name"];
				while ($drow = $dresult->FetchRow()) {
					$department = $department ."; ". $drow["department_name"];
				}
	
		                $href = $PHP_SELF . "&amp;select=applications&amp;PDF=1&amp;job_id=" . urlencode($job_id);
				$href2 = ($href . "&amp;view_who=selected&amp;view_part=whole&amp;selected_app_id=$app_id");
		
				echo ("<tr>");
				if ($view_allowed) {
					echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
					echo ("&nbsp;". (is_app_flagged ($app_id, ($userlevel == ADMINEMPLOYER ? $employer_contact_id_login : $contact_id)) 
						? "<img src='misc/images/job_flagged.gif' alt='' />" : "") ."&nbsp;</td>");
					echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
					if ((!$unavailable) || (hasStudentBeenPlacedInJob($job_id, $student_number)))
						echo ("&nbsp;<input type='checkbox' name='selected_app_id[]' value='$app_id' />&nbsp;");
					else
						echo ("&nbsp;");
					echo ("</td>");
				}
		                echo ("<td align='left' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
		                echo ("&nbsp;");
				if ((!$unavailable && $view_allowed && $is_online) || (hasStudentBeenPlacedInJob($job_id, $student_number)))
					echo ("<a class='blue' href='$href2'>$name</a>");
				else
					echo ($name);
				echo ("&nbsp;</td>\n");
				if ($view_allowed) {
					echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>&nbsp;");
					if (hasStudentBeenPlacedInJob($job_id, $student_number))
						echo ("<b class='black'>You have hired</b>");
					else
						echo ($status);
					echo ("&nbsp;</td>");
		
				}
				echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>&nbsp;");
				if (isStudentShortlisted ($job_id, $student_number))
					echo ("<img src='misc/images/smchk_yl.gif' alt='' />");
				echo ("&nbsp;</td>");
				//misc/images/smchk_gr.gif
				echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
				echo ("&nbsp;$department&nbsp;</td>");
				
		                echo ("</tr>\n");
		
		                $rowclass++;
		        }

			if ($view_allowed) {
				echo ("<tr>");
					echo ("<td align='left' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "' colspan='6'>");
					// this is the array of all good applications
					echo ("<input type='hidden' name='all_apps' value='". packObject(($all_apps)) ."' />");
					echo ("<input type='submit' name='flag_unflag' value='Set/Unset Flags' />");
					echo ("</td>");
				echo ("</tr>");
			}
		
			echo ("</table>");
			echo ("</td>");
		echo ("</tr>");
		echo ("</table>");
		
		if ($view_allowed) {
			echo ("<br />");

			// only display the view stuff if it's an online job
			if ($is_online) {
				echo ("<table class='row1' cellpadding='4'>");
					echo ("<tr valign='middle'>");
						echo ("<td valign='middle'>&nbsp;");
						echo ("<b class='black'>View:</b> &nbsp; ");
						echo ("<select name='view_who2'>");
							echo ("<option value='all' selected='selected'>All</option>");
							echo ("<option value='selected'>Selected</option>");
							if ($has_flagged)
								echo ("<option value='flagged'>Flagged</option>");
							if ($has_shortlisted)
								echo ("<option value='shortlisted'>Shortlisted</option>");
						echo ("</select> &nbsp; ");
						echo ("<select name='part_of_app2'>");
							echo ("<option value='whole' selected='selected'>Applications</option>");
							echo ("<option value='letter'>Cover letters</option>");
							echo ("<option value='resume'>Resumes</option>");
						echo ("</select> &nbsp; ");
						echo ("<input type='submit' name='view_apps2' value=' View ' />");
						echo ("&nbsp;</td>");
					echo ("</tr>");
				echo ("</table>");
			}
			
			echo ("<br />");

			// quick check to see if they've sent an interview notice,
			$sql = "SELECT interview_notify_sent
				FROM job_info
				WHERE job_id = '$job_id'";
			$result = $GLOBALS['dbh']->Execute ($sql);

			if ($row = $result->FetchRow()) {
				if ($row["interview_notify_sent"])
					$notify_sent = true;
				else
					$notify_sent = false;
			}
			else
				$notify_sent = false;


			echo ("<table class='row1' cellpadding='4'>");
				echo ("<tr valign='middle'>");
					echo ("<td valign='middle'>&nbsp;");
					echo ("<b class='black'>E-mail:</b> &nbsp; ");
					echo ("<select name='email_who'>");
						echo ("<option value='all' selected='selected'>All Available Applicants</option>");
						echo ("<option value='selected'>Selected Applicants</option>");
						if ($has_flagged)
							echo ("<option value='flagged'>Flagged Applicants</option>");
						if ($has_shortlisted)
							echo ("<option value='shortlisted'>Shortlisted Applicants</option>");
//						if ($notify_sent)
//							echo ("<option value='interview_notify'>Interview Notice to Co-op Office</option>");
					echo ("</select> &nbsp; ");
					echo ("<input type='submit' name='email_submit' value=' E-mail ' />");
					echo ("&nbsp;</td>");
				echo ("</tr>");
			echo ("</table>");
			echo ("<br />");

			// only want to display the button if there isn't already an interview
			if (JobhasInterview ($job_id)) {
				$primary_contact = getShortlistContactfromJobID ($job_id);
				$icontact_info = getContactInfoByID ($primary_contact);
				$icontact_phone = $icontact_info["phone"];
				$icontact_email = getCiEmailUsingID ($primary_contact);
				
				$shortlist_msg = "<b class='instruct'>The interview for this job has either already been set up or is completed.</b><br />";
				$shortlist_msg.="To shortlist more applicants please contact ". implode (getContactName ($primary_contact), " ");
				if ($icontact_phone || $icontact_email) {
					$shortlist_msg.= " (";
					$shortlist_msg.= ($icontact_phone ? $icontact_phone : "");
					$shortlist_msg.= ($icontact_phone && $icontact_email ? ", " : "");
					$shortlist_msg.= ($icontact_email ? $icontact_email : "");
					$shortlist_msg.= ")";
				}
				$shortlist_msg.= " at the Co-op Office.";
				instruct($shortlist_msg);
			}
			
			else {
				echo ("<table class='row1' cellpadding='4'>");
					echo ("<tr valign='middle'>");
						echo ("<td valign='middle'>&nbsp;");
						echo ("<b class='black'>Shortlist:</b> &nbsp; ");
						echo ("<select name='shortlist_who'>");
							echo ("<option value='all'>All Available</option>");
							echo ("<option value='selected' selected='selected'>Selected</option>");
							if ($has_flagged)
								echo ("<option value='flagged'>Flagged</option>");
						echo ("</select> &nbsp; ");
						echo ("<input type='submit' name='shortlist_submit' value=' Shortlist ' />");
						echo ("&nbsp;</td>");
					echo ("</tr>");
				echo ("</table>");
				echo ("<br />");
			}
		} // should we show these?

		echo ("</form>");
	}
	else
	{
		notify("There are currently no applicants for this job.");
	
		echo ("<br />");
	}
	
}

?>
