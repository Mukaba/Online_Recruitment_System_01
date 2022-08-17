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
 | Filename: email.inc                                                          |
 +------------------------------------------------------------------------------+
 | Description: This file is called by applications to send an e-mail to        |
 | applicants, or whoever.                                                      |
 +------------------------------------------------------------------------------+

*/


// things we need to do the stuff we do below, put 'em up here so they're included before being called
include ('applications/email/display_email_form.inc');
include ('applications/email/send_email.inc');


switch ($email_step) {
	
	case "Send":
		global $auth;
		global $job_id;

		if (($userlevel == ADMINEMPLOYER) || ($userlevel == OFFICE)) 
        {
			$from = ($from = getCiEmailUsingID(getContactID($auth->department, $auth->login))) ? $from : BRAND_NAME_SYSTEM_EMAIL;
			$reply_id = $reply_to;		
			$reply_to = ($reply_to = getCiEmailUsingID($reply_to)) ? $reply_to : "";		
		}

		elseif (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {

			// if already bcc, just append students addresses
			if (trim ($bcc))
				$bcc .= ", ";
			include ('applications/email/emp_email_who.inc');
			if ($email_who == 'selected')
				$bcc .= implode (emp_email_who ($email_who, $selected_app_id), ",");
			elseif ($email_who == 'all')
				$bcc .= implode (emp_email_who ($email_who, $all_apps), ",");
			else
				$bcc .= implode (emp_email_who ($email_who), ",");

			// for privacy, don't show students the employer's address, so send it bcc as well
			// unless of course they say they can let them view it 
			if ($send_self) {
				if ($see_address) {
					if (trim ($to))
						$to .= ", ";
					$to = getCiEmailUsingID ($contact_id);
				}
				else {
					if (trim ($bcc))
						$bcc .= ", ";
					$bcc .= getCiEmailUsingID ($contact_id);
				}
			}

			// since we need a to and there's no input for the employers, throw in the null
			if (!(trim ($to)))
				$to = NULL_EMAIL;

			// just some things to throw at the top of the body of the e-mail
			$extra_body = "An employer from ". getCompanyAndDivisionName ($contact_id) .
				" would like to pass on the following information.\n";

			if ($see_address)
				$from = getCiEmailUsingID ($contact_id);
			else {
				$from = NULL_EMAIL;
				// tack on a message in the body
				$extra_body .= "Please do not reply to this e-mail as it will not be sent to the employer.\n";
			}

			// complete the message
			$body = $extra_body . "----------------------------------------------\n\n" .$body;
		}
	/* used for testing, without actually sending	
		echo ("to: $to <br />
			cc: $cc <br />
			bcc: $bcc <br />
			from: $from <br />
			reply-to: $reply_to <br />
			subject: $subject <br />
			body: <br /> $body");  */

		if(send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to, $reply_id)) {

			notify ("E-mail has been successfully sent.");
	
			if ($userlevel == OFFICE) {

				// if it's employer, see if they said they want to send a login/password
				if ($create_login  == 'yes') {
					$continue = "Employer Login";
					$come_from_apps = '1';
					include ('applications/admin/apply_admin.inc');
				}
				else
					include('applications/admin/applicant_list.inc');
			}

			elseif (($userlevel == EMPLOYER) || ($userlevel == ADMINEMPLOYER) || ($userlevel == HREMPLOYER))
				include ('applications/employer/applicant_list.inc');
		}

		break;

	case "create":
	default:
		// if $send_students is true, we pass the array, but in the function it's put in the BCC field
		// otherwise we're sending to employers, we pass it the same way, but the function puts it in the TO field
		//      PS even if it's an employer, we still pass an array (later on we could send it to more than 1)
		display_email_form($sending_email_to, $job_id, $send_students);
}

?>
