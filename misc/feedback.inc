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
 | Filename: feedback.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description: This file is a form where users can provide feedback. The       |
 |              entered comments are then e-mailed to the FEEDBACK_EMAIL        |
 |              address which is a defined constant. We use the user's e-mail   |
 |              address from the system, so anonymous e-mails are not allowed   |
 +------------------------------------------------------------------------------+
 */

echo("</td><td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td></tr></table>");
echo("<table border='0' cellspacing='0' cellpadding='2' width='98%'>");
echo("<tr>");
echo("<td>");
echo("<h3 align='left'>Feedback</h3>");

$clean_in = array();
$clean_out = array();

switch ($_REQUEST['email_send'])
{
	case "Send":

        if (!trim($_POST['body']) && !trim($_POST['subject']))
        {
            error("<h1 class='black'>Error</h1><p>Please fill in the subject line and the body of the message. Please hit the back button and fill in the form again.</p>");
            break;
        }
        // We query for the feedback submitter's e-mail and name at the very last minute here because we don't want people
        // poisoning variables and putting in fake e-mail addresses. 
        if ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR || $auth->userlevel == FACULTY)
        {
            $contact_id = getContactID($auth->department, $auth->login);

            $users_email = getCiEmailUsingID($contact_id);
            $clean_in['contact_id'] = $contact_id; 
            $clean_out['users_email'] = $users_email;

            $sql = ("
                SELECT c.first_name, c.last_name
                FROM contact c 
                WHERE c.contact_id = ? 
                ");
            $result = $GLOBALS['dbh']->Execute($sql, array($clean_in['contact_id']));
            $row = $result->FetchRow();
            $clean_out['users_name'] = $row['first_name']." ".$row['last_name'];
        }
        elseif ($auth->userlevel == STUDENT)
        {
            $clean_out['users_email'] = getStudentEmail($auth->student_number);
            
            $sql = ("
                SELECT s.first_name, s.last_name
                FROM student s
                WHERE s.student_number = ? 
                ");
            $result = $GLOBALS['dbh']->Execute($sql, array($auth->student_number));
            $row = $result->FetchRow();
            $clean_out['users_name'] = $row['first_name']." ".$row['last_name'];
        }

        if (!$clean_out['users_email']) {
            $clean_out['users_email'] = NULL_EMAIL;
        }

        // This chunk of code attaches important information about the user that put in the feedback. It helps us debug a problem faster. 

        $appended_msg = ("Feedback from ".BRAND_NAME."\n");
        $appended_msg .= ("=====================\n");

        if ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR)
        {
            $appended_msg .= ("Contact Name: ".$clean_out['users_name']."\n");
            $appended_msg .= ("E-mail: ".$clean_out['users_email']."\n");
            $appended_msg .= ("Department: ".getDepartmentName($auth->department)."\n");
        }
        elseif ($auth->userlevel == FACULTY)
        {
            $appended_msg .= ("Faculty Name: ".$clean_out['users_name']."\n");
            $appended_msg .= ("E-mail: ".$clean_out['users_email']."\n");
            $appended_msg .= ("Department: ".getDepartmentName($auth->department)."\n");
        }
        elseif ($auth->userlevel == STUDENT)
        {
            $appended_msg .= ("Student Name: ".$clean_out['users_name']."\n");
            $appended_msg .= ("E-mail: ".$clean_out['users_email']."\n");
            $appended_msg .= ("Department: ".getDepartmentName($auth->department)."\n");
        }
        $appended_msg .= ("=====================\n");
        $appended_msg .= ("Technical Information\n");
        $appended_msg .= ("P: ".$HTTP_USER_AGENT."\n");
        $appended_msg .= ("I: ".$REMOTE_ADDR."\n");

        $body = $appended_msg."\n".$_POST['body'];

        $from = $users_name." <".$clean_out['users_email'].">";
        $to = FEEDBACK_EMAIL;
        $bcc = FEEDBACK_BCC_EMAIL;

        $subject = "FEEDBACK: ".$_POST['subject'];

		if (send_email($to, $bcc, $from, $subject, $body))
		{ 
            notify("Your feedback has been sent successfully. Thank you for your input.");
		}
        else
        {
            error("<h1 class='black'>Error</h1><p>An error has occurred while sending your feedback. Please hit the back button and resend your message.</p>");
        }
		break;
	
	default:
		display_email_form();
		break;
}
echo("</td>");
echo("</tr>");
echo("</table>");

function display_email_form()
{
	global $PHP_SELF;
	global $auth;
	
    if ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR || $auth->userlevel == FACULTY)
    {
        $contact_id = getContactID($auth->department, $auth->login);

        // user is using an admin account that doesn't require logging in, can't use feedback because
        // they don't have info in the database related to contact ids and emails
        if (!$contact_id) {
            error("Feedback is unavailable for this special admin account.");
            return;
        }

        // Use this contact id to find the users email.
        $users_email = getCiEmailUsingID($contact_id);

        $sql = ("
            SELECT c.first_name, c.last_name
            FROM contact c 
            WHERE c.contact_id = ? 
            ");
        $result = $GLOBALS['dbh']->Execute($sql, array($contact_id));
        $row = $result->FetchRow();
        $users_name = $row['first_name']." ".$row['last_name'];
    }
    elseif ($auth->userlevel == STUDENT)
    {
        // user is using an admin account that doesn't require logging in, can't use feedback because
        // they don't have info in the database related to student numbers and emails
        if (!$auth->student_number) {
            error("Feedback is unavailable for this special admin account.");
            return;
        }
        // Use this student number to find the users email.
        $users_email = getStudentEmail($auth->student_number);
        
        $sql = ("
            SELECT s.first_name, s.last_name
            FROM student s
            WHERE s.student_number = ? 
            ");
        $result = $GLOBALS['dbh']->Execute($sql, $auth->student_number);
        $row = $result->FetchRow();
        $users_name = $row['first_name']." ".$row['last_name'];
    }

    $from_string = htmlentities($users_name." <".$users_email.">");

    if ($auth->userlevel == ADMINSTUDENT)
    {
        error("<h1 class='black'>Error</h1><p>You may not submit feedback while logged in as a student. Please log out of the student account before submitting feedback.</p>");
    }
    else
    {
        $instruct_message="We welcome your ideas and suggestions.<br /><br />";

        if ($auth->userlevel == STUDENT)
        {
            $instruct_message.="If you have feedback about your workterm or general co-op experiences,
            please contact your Co-op Coordinator or use one of the evaluation forms
            provided under the 'Evaluations' menu.<br /><br />";
        }

        $instruct_message.="Please use the form below to inform us of technical problems or to 
        provide suggestions for improving our system.";

        instruct($instruct_message);
        echo("<br />");
        ?>

        <form action="<?php echo $PHP_SELF."&amp;select=feedback" ?>" method="post" enctype="multipart/form-data">
            <table class='row1' cellspacing='0' cellpadding='4' border='0'>
                <tr>
                    <td>To:</td>
                    <td colspan='2'><?php echo(BRAND_NAME); ?> Feedback</td>
                </tr>
                <tr>
                    <td>From:</td>
                    <td colspan='2'><?php echo($from_string); ?></td>
                </tr>
                <tr>
                    <td>Subject:</td>
                    <td colspan="2"><input type="text" name="subject" size="55" /></td>
                </tr>
                <tr>
                    <td colspan="3"><textarea cols="70" rows="20" name="body" ></textarea></td>
                </tr>
                <tr>
                    <td colspan="3" align="center"><hr />
                    <input type="submit" name="email_send" value="Send" />
                    </td>
                </tr>
            </table>
        </form>
        <?php
    }
}


function send_email($to, $bcc, $from, $subject, $body)
{
	include('email.inc');
	
	$mail = new email($to, $subject, unslash($body));
		
	$mail->from = $from;
    $mail->bcc = $bcc;
	
	if (!$mail->send())
	{
		unset($mail);
		return false;
	}
	else
	{
		unset($mail);	
		return true;
	}
}

?>
