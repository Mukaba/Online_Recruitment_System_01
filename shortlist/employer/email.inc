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
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/

/*
 Filename: email.inc
 Description: Set up to email interview information to the contact for the 
 job once the employer has selected a shortlist.
 :NOTE: This file is now deprecated and should be removed from the next release.
*/

include ("misc/email.inc");
include ("shortlist/employer/shortlist.class");

$shortlist = new shortlist("", "", "");
$info = unpackObject(($short_list));
$shortlist->restore($info);

$sql = "SELECT job_code
	FROM job_info WHERE job_id = '" . $shortlist->job_id . "'";

// echo "job_code sql : " . $sql  . "<br />";

$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$job_code = $row["job_code"];

/*
$sql = "SELECT c.email
	FROM contact as c, interview as i
	WHERE c.contact_id = i.contact AND i.job_id = '" . $shortlist->job_id . "'";
*/
/*
$sql = "select c.email from contact as c, contact_employer as ce, job_info as j where j.job_id=\"".$shortlist->job_id."\"
        and j.employer_id=ce.employer_id and ce.contact_id=c.contact_id";
*/
/*
$sql = "select c.email from contact_internal as c where c.department_id=\"".$shortlist->department_id."\"";
*/
/*
$sql = "select distinct email from contact_internal as ci, contact as c, job_info as j where ci.contact_id=c.contact_id ".
       "and ci.department_id=j.department_id and j.job_id=\"".$shortlist->job_id."\"";

//echo "email to sql : " . $sql  . "<br />";

$to_list = array ();
$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
  $to_list[] = $row["email"];
}

$to = implode (", ", $to_list);
*/

$sql = "select distinct email from contact_internal as ci, contact as c, job_info as j where ci.contact_id=c.contact_id ".
       "and ci.contact_id=j.internal_contact_id and j.job_id=\"".$shortlist->job_id."\"";
$response = $GLOBALS['dbh']->Execute ($sql);
assert ($response->RecordCount() == 1);

$row = $response->FetchRow();

$to = $row ["email"];

$sql = "SELECT c.email, c.first_name, c.last_name, a.company_name
	FROM contact as c, employer_login as e, employer_company as a
	WHERE c.contact_id = '" . $auth->contact_id . "' AND c.contact_id = e.contact_id AND
		a.employer_id = e.employer_id";

//echo "email from sql : " . $sql  . "<br />";

$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$from = $row["email"];
$contact_name = $row["first_name"] . " " . $row["last_name"];
$company = $row["company_name"];

$subject="$job_code : Interview Set Up"; 
$body = "$contact_name from $company has submitted the following interview request:\n\nEmail: $from\n\nJob Code: " . $job_code . "\n\n" . $interview_info;
$body .= "\n\nThe following students were shortlisted for this job:\n";
for($i=0;$i<count($shortlist->students);$i++){
	$body .= $shortlist->students[$i] . " : " . getStudentName($shortlist->students[$i]) . "\n";
}

//need to enter the email address of the job contact
$job_contact_email = "";

?>
<h3>E-mail Co-op Office</h3>
<form method="post" action="<?=$PHP_SELF?>">
  <input type="hidden" name="level" value="4" />
  <input type="hidden" name="short_list" value="<?=$short_list?>" />
  <input type="hidden" name="interview_info" value="<?=$interview_info?>" />
  <table class="row1" border="0" cellspacing="0" cellpadding="4">
    <tr>
      <td align="right">
        <b class="black">From:</b>
      </td>
      <td>
        <input type="hidden" name="mail_from" value="<?=$from?>" />
        <?=$from?>
      </td>
    </tr>
    <tr>
      <td align="right">
        <b class="black">To:</b>
      </td>
      <td>
        <input size="40" name="mail_to" value="<?=isset ($mail_to) ? $mail_to : $to?>" />
      </td>
    </tr>
    <tr>
      <td align="right">
        <b class="black">Cc:</b>
      </td>
      <td>
        <input size="40" name="mail_cc" value="<?=isset ($mail_cc) ? $mail_cc : ""?>" />
      </td>
    </tr>
    <tr>
      <td align="right">
        <b class="black">Bcc:</b>
      </td>
      <td>
        <input size="40" name="mail_bcc" value="<?=isset ($mail_bcc) ? $mail_bcc : ""?>" />
      </td>
    </tr>
    <tr>
      <td align="right">
        <b class="black">Subject:</b>
      </td>
      <td>
        <input size="40" name="mail_subject" value="<?=isset ($mail_subject) ? $mail_subject : $subject?>" />
      </td>
    </tr>
    <tr>
      <td colspan="2" align="center">
        <textarea name="mail_body" rows="6" cols="60"><?=isset ($mail_body) ? $mail_body : $body?></textarea>
      </td>
    </tr>
    <tr>
      <td align="center" colspan="2"><hr />
        <input type="submit" name="sendmail" value=" Send Mail " />
        <input type="reset" value=" Revert " />
      </td>
    </tr>
  </table>
</form>
<?
/*
$mail = new email($job_contact_email, $subject, $body);
$mail->from = $from;
$mail->cc = "";
$mail->bcc = "";
$mail->headers = "Reply-To: " . $from . "\n";

if (!$mail->send()){
	error ("Interview Information was unable to send.  Please press the back button on your browser and try again. If the problem persists please contact the Coop Office.");
	error ($mail->error_text);
}else{
	?>
	The interview information has been sent to the Coop Office.  Once an interview has been set up you will be contacted
	and will be able to view interview details through the "View Interview Schedule" option.
	<form method="post" action="<?php echo $PHP_SELF?>">
	<input type="hidden" name="select" value="shortlist" />
	<center><input type="submit" value="OK" /></center>
	</form>
	<?
}
*/

?>
