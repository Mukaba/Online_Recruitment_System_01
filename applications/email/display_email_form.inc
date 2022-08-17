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
 | Filename: display_email_form.inc                                             |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

/*
 Filename: display_email_form.inc
 Descirption: Displays the form for e-mail students, or whoever, depending on who's doing the e-mailing.
*/

function display_email_form($sending_to, $job_id, $send_students) {

	global $PHP_SELF;
	global $auth;
	global $email_job;
	global $userlevel;
	global $email_who;
	global $selected_app_id;
	global $all_apps;

	if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {

		$sql = "SELECT contact_id FROM employer_login WHERE login_id = '". $auth->login ."'";
		$result = $GLOBALS['dbh']->Execute ($sql);
		$row = $result->FetchRow();
		$user_email = getCiEmailUsingID ($row["contact_id"]);

		$send_bcc_array = $sending_to;
	}

	elseif (($userlevel == OFFICE) || ($userlevel == ADMINEMPLOYER)) {

		//Find the users contact_id.
		$users_contact_id = getContactID($auth->department, $auth->login);

		// Use this contact id to find the users e-mail.
		$user_email = getCiEmailUsingID($users_contact_id);

		$sql = ("
			SELECT DISTINCT a.login_id, a.netlink_id, a.contact_id, CONCAT(b.first_name, ' ', b.last_name) AS name, b.email
			FROM contact_internal AS a, contact AS b
			WHERE a.department_id IN ('" . implode(department_groups($auth->department), "', '"). "')  
			AND (a.application_replyto_list OR a.contact_id='".$users_contact_id."')
			AND a.contact_id=b.contact_id
			ORDER BY b.last_name, b.first_name
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		if (!$result) {
			return false;
        }

		// $send_student determines we're sending to students so we throw everything into the BCC field
		if ($send_students) {
			$send_bcc = implode ($sending_to, ", ");
        }

		// we're sending to  an employer
		else { 
			$send_to = implode ($sending_to, ", "); 
        }
	}

	
	// $email warn is created in apply_admin that calls this file, it's a list of anybody without an address
	if ($email_warn) {
		error($email_warn);
		echo("<br />");
	}


	// if $send_students is true, obviously sending to students, otherwise it's employer
	echo ("<h3>E-mail ". (($send_students) ? "Applicants" : "Employer") ."</h3>");
	
	if ($userlevel == OFFICE) {

		$jsql = "SELECT DISTINCT ji.apply_method, ji.job_code, ji.apply_method, ec.company_name, ji.status, 
			CONCAT(c.first_name, ' ', c.last_name) as contact_name, el.contact_id, c.email
			FROM employer_company as ec, job_info as ji
			LEFT JOIN contact as c ON c.contact_id = ji.contact_id
			LEFT JOIN employer_login as el ON el.contact_id = ji.contact_id
			WHERE ji.job_id='" . $job_id . "' AND ec.employer_id=ji.employer_id";
		$jresult = $GLOBALS['dbh']->Execute($jsql);
		
		if ($jrow = $jresult->FetchRow()) {
			$apply_method = $jrow["apply_method"];
			$job_code = $jrow["job_code"];
			$comp_name = $jrow["company_name"];
			$contact = $jrow["contact_name"];
			$contact_email = $jrow["email"];
			$job_contact_id = $jrow["contact_id"];
		}

		echo ("<table><tr><td align='left'><b>Company Name:</b></td><td>$comp_name</td></tr>
			<tr><td align='left'><b>Job Code:</b></td><td>$job_code</td></tr>");
		if (!$send_students) {
			echo ("<tr><td align='left'><b>Job Contact:</b></td><td>$contact</td></tr>");
			echo ("<tr><td align='left'><b>Contact E-mail:</b></td><td>$contact_email</td></tr>");
		}
		echo ("</table>");
		
		// javascript to throw in employer body
		if (($apply_method == JOB_APP_METHOD_ONLINE) && (!$send_students)) {

			$pattern = array ("/\n/i", "/\r/i");
			$replace = array ("\\n");
			$bresult = $GLOBALS['dbh']->Execute ("SELECT application_employer_body FROM department 
					WHERE department_id = '". $auth->department ."'");
			if ($brow = $bresult->FetchRow()) {
				$emp_body = trim( preg_replace($pattern, $replace, addslashes($brow["application_employer_body"])));
            }
		
			?>

            <script type="text/javascript" language="javascript">
			<!--javascript
				function insertEmployerBody () {
					// set focus first, otherwise it sometimes does wierd things
					document.form1.body.focus();
					document.form1.body.value="<?php echo ($emp_body); ?>";
				}
			//-->
			</script>

			<?php
		} // end if on javascript
	} // end if admin
	

	// finally set it all up
	echo ("<form action='$PHP_SELF' method='post' name='form1'>
		<input type='hidden' name='select' value='applications' />
		<input type='hidden' name='submit' value='Send off the e-mail' />
		<input type='hidden' name='job_id' value='$job_id' />
		<input type='hidden' name='continue' value='send_email' />");
	if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {
		echo ("<input type='hidden' name='email_who' value='$email_who' />");
		if ($email_who == 'selected') {
			echo ("<input type='hidden' name='selected_app_id' value='". packObject(($selected_app_id)) ."' />");
        }
		elseif ($email_who == 'all') {
			echo ("<input type='hidden' name='all_apps' value='$all_apps' />");
        }
	}

	echo ("<table class='row1' cellspacing='0' cellpadding='4' border='0' width='60%'>");
		echo ("<tr>");
			echo ("<td valign='middle'> &nbsp; To:</td>");
			echo ("<td valign='middle'>");
			if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {
				echo ("<b class='black'>");
				echo (substr_replace ($email_who, strtoupper (substr ($email_who, 0, 1)), 0, 1 ));
				echo (" Applicants</b> (e-mail addresses hidden for privacy)<br />\n");
				echo ("<input type='checkbox' name='send_self' value='1' checked='checked' /> E-mail me a copy.\n");
			}
			elseif (($userlevel == OFFICE) || ($userlevel == ADMINEMPLOYER)) {
				echo ("<input type='text' name='to' size='70' value='". (($send_students) ? $user_email : $send_to) ."' />");
            }
			echo ("</td>");
		echo ("</tr>");
		
		echo ("<tr>");
			echo ("<td valign='middle'> &nbsp; Cc:</td>");
			echo ("<td valign='middle'><input type='text' size='70' name='cc' /></td>");
		echo ("</tr>");

		echo ("<tr>");
			echo ("<td valign='middle'> &nbsp; Bcc:</td>");
			echo ("<td valign='middle'>");
			// special case with the bcc, cause it will change the input size for employers
			// (make it smaller cause they shouldn't see the student's e-mail address, and probably won't need as must space)
			if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) { 
				echo ("<input type='text' size='70' name='bcc' />\n");
            }
			elseif (($userlevel == OFFICE) || ($userlevel == ADMINEMPLOYER)) {
				echo ("<textarea name='bcc' cols='70' rows='5'>". ($send_students ? $send_bcc : $user_email) ."</textarea>");
            }
			echo ("</td>");
		echo ("</tr>");
		
		echo ("<tr>");
			echo ("<td valign='top'> &nbsp; From:</td>");
			echo ("<td>". (($user_email) ? $user_email : BRAND_NAME_SYSTEM_EMAIL));
			if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {
				echo ("<br /><input type='checkbox' name='see_address' value='1' />
					Allow my e-mail address to be revealed to recipients. <br />");
				echo (" &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; (required if reply is requested)");
			}
			echo ("</td>");
		echo ("</tr>");

		if ($userlevel == OFFICE) {
			echo ("<tr>");
				echo ("<td valign='top' nowrap='nowrap'> &nbsp; Reply to:</td>");
				echo ("<td valign='top'><select name='reply_to'>");
				while ($row = $result->FetchRow())  {
					echo(" <option value='" . $row["contact_id"] . "'" . ($users_contact_id == $row["contact_id"] ? " selected" : ((($_SESSION['SESSION_netlog'] == SCRIPT_LOGIN && $auth->login == $row["netlink_id"]) || ($_SESSION['SESSION_netlog'] == NIS_LOGIN && $auth->login == $row["login_id"])) ? " selected" : "")) . ">" . $row["name"] . "</option>\n");
                }
				$result->Close();
			
				echo ("</select><br />");
				echo (" Contact information for this address will be automatically appended to the e-mail.");
				echo ("</td>");
			echo ("</tr>");
		}
		
		echo ("<tr>");
			echo ("<td valign='middle'> &nbsp; Subject:</td>");
			echo ("<td valign='middle'><input type='text' name='subject' size='55' value='"); 
				if ($send_students) {
					echo ("Information regarding job ". getJobCode($job_id));
                }
				else {
					echo ("University of Victoria Co-op: Job ". getJobCode($job_id));
                }
			echo ("' /></td>");
		echo ("</tr>");
	
		if (!$send_students && ($userlevel == OFFICE) && ($apply_method == JOB_APP_METHOD_ONLINE)) {
			echo ("<tr><td>");
			echo ("<input type='button' name='insert_employer_body' value='Insert' onclick='insertEmployerBody()' />");
			echo ("</td><td>");
			echo ("This will insert pre-defined text into the e-mail body below (Note: will erase anything already entered).");
			echo ("</td></tr>");
		}
		
		echo ("<tr>");
			echo ("<td colspan='2'> &nbsp; <textarea cols='80' rows='20' name='body'></textarea></td>");
		echo ("</tr>");
		
		echo ("<tr>");
			echo ("<td colspan='2' align='center'>");
				if (!$job_contact_id && ($userlevel == OFFICE) && !$send_students) {
					echo ("<b class='black'>The contact associated with this job does not yet have a login ID and password, would you like to create one?</b><br />");
					echo ("<input type='radio' name='create_login' value='yes' checked='checked' /> &nbsp;<b class='black'>Yes</b> &nbsp; &nbsp; &nbsp; ");
					echo ("<input type='radio' name='create_login' value='no' /> &nbsp;<b class='black'>No</b>");
				}
			echo ("&nbsp;</td>");
		echo ("</tr>");
		
		echo ("<tr>");
			echo ("<td colspan='2' align='center'>
				<hr width='100%' />
				<input type='submit' name='email_step' value='Send' />
				<br />&nbsp;</td>");
		echo ("</tr>
		</table>
	</form>");

}


?>
