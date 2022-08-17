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
 | Filename: email_actual.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/

/*
 Filename: email_actual.inc
 Description: Does the actual work of sending an email for the
 shortlisting system. (Moved away from email.inc)
 :NOTE: This file is now deprecated and should be removed from the next release.
*/

/* Validate input. */
$error = 0;
if (isset ($mail_from) && !isValidEmail ($mail_from))
{
  error ("The MAIL FROM field is invalid ...");
  $error = 1;
}

/* Validate each address in the TO, CC, and BCC fields. */
foreach (array ("mail_to", "mail_cc", "mail_bcc") as $rcpt_type)
{
  if (strlen ($$rcpt_type) > 0)
  {
    foreach (explode (",", $$rcpt_type) as $rcpt_addy)
    {
      $rcpt_addy = ltrim (rtrim ($rcpt_addy));
      if (!isValidEmail ($rcpt_addy))
      {
        error ("Address <b class=\"black\">$rcpt_addy</b> in $rcpt_type is invalid.");
        $error = 1;
      }
    }
  }
}

if ($error > 0)
{
  // Something happen!
  include ("shortlist/employer/email.inc");
}
else
{
  /* Required for shortlist class operations */
  include ("shortlist/employer/shortlist.class");

  $shortlist = new shortlist("", "", "");
  $info = unpackObject(($short_list));
  $shortlist->restore($info);

  // Send mail ...
  include ("misc/email.inc");

  $mail = new email ($mail_to, $mail_subject, $mail_body);
  $mail->cc = $mail_cc;
  $mail->bcc = $mail_bcc;
  $mail->from = $mail_from;

  if (!$mail->send ())
  {
    error($mail->error_text." Please push the BACK button on your browser and try again.");
  }
  else
  {
  ?>
  <form method="post" action="<?=$PHP_SELF?>">
    <h3>Complete<!-- (FIXME: SEND MAIL! :^) --></h3>
    <table class="row4" border="0" cellspacing="0" cellpadding="8">
      <tr>
        <td>
          These students have been successfully shortlisted:

          <ul>
            <?
            foreach ($shortlist->students as $guy)
              echo "<li>".getStudentName ($guy)."</li>";
            ?>            
          </ul>

          A request for an interview to be set up has been sent to the Co-op office. Once an interview has been
          set up, you will be contacted and will be able to view interview details through the "View Interview
          Schedule" option.<br /><br />

          <center>
            <input type="hidden" name="select" value="shortlist" />
            <input type="submit" value=" OK " />
          </center>
        </td>
      </tr>
    </table>
  </form>
  <?
  }
}

?>
