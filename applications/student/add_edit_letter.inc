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
 | Filename: add_edit_letter.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: Add or edit a coverletter.                                      |
 +------------------------------------------------------------------------------+

*/

/*echo ("<form method='post' action='" . $PHP_SELF . "'>");
echo ("<input type='hidden' name='select' value='applications' />");
echo ("<input type='hidden' name='option' value='add_online' />");

echo ("<input type='hidden' name='job_id' value='$job_id ' />");
echo ("<input type='hidden' name='resume_id' value='$resume_id' />");
echo ("<input type='hidden' name='include_transcript' value='$include_transcript' />");
*/
$app_letter = ereg_replace("&amp; ", "&amp;amp; ",$app_letter);  // preserves &amp; when editing
$app_letter = ereg_replace("&amp ", "&amp;amp; ",$app_letter);  
$app_letter = ereg_replace("&nbsp;","&amp;nbsp;",$app_letter);   // preserves &nbsp;
$app_letter = ereg_replace("&nbsp","&amp;nbsp;",$app_letter);


echo ("<form method='post' action='" . $PHP_SELF . "'>");
echo ("<input type='hidden' name='select' value='applications' />");
echo ("<input type='hidden' name='option' value='add_online' />");
echo ("<input type='hidden' name='job_id' value='$job_id ' />");
echo ("<input type='hidden' name='resume_id' value='$resume_id' />");
echo ("<input type='hidden' name='include_transcript' value='$include_transcript' />");
echo ("<table width='80%' class='row1' cellpadding='0' cellspacing='0' border='0'>
	<tr>
	<td>");

echo ("<table width='100%' class='row1' cellpadding='3' cellspacing='4' border='0'>
	<tr>");

	echo ("<td colspan='2'>");
	echo ("<h4>Cover Letter HTML Editor:</h4> ");
	echo ("</td>");
echo ("</tr>");

echo ("<tr>");
	echo ("<td colspan='2'>");
	echo ("Please enter the HTML code for your coverletter below.");
	echo ("<hr />");
	echo ("</td>");
echo ("</tr>");

$sql = ("SELECT CONCAT(c.first_name, ' ',c.last_name) as name
         FROM contact AS c
         INNER JOIN job_info AS ji
         ON c.contact_id = ji.contact_id
         WHERE ji.job_id = '".$job_id."'
         AND ji.displayname_on_coverltr = 1
        ");
$result = $GLOBALS['dbh']->Execute ($sql);

if($result->RecordCount())
{
    while($row2 = $result->FetchRow())
    {
        $name = $row2['name'];
    } 
    echo ("<tr>");
        echo ("<td colspan = '2'>");
            echo ("If you would like to personalize your coverletter, you may address it to: $name");
        echo ("</td>");
    echo ("</tr>");
}


echo ("<tr>");
	echo ("<td align='left' colspan='2'>");
	echo ("<textarea name='app_letter' cols='90' rows='30'>$app_letter</textarea>");
	echo ("<br /><br />");
	echo ("<b class='black'>Resume to be attached:</b> $resume_name <br />");
	echo ("<b class='black'>Transcripts:</b> ".(($include_transcript != 0) ? "Attached" : "Suppressed")."<br />");
	echo ("<hr />");
	echo ("</td>");
echo ("</tr>");

echo ("<tr>");
	echo ("<td width='50%' align='right'>");
	echo ("<input type='submit' name='next_step_view_app' value='Continue' />");
    echo ("</td>");
    echo ("<td width='50%' align='left'>");
    echo ("<input type='submit' value='Cancel' onclick='javascript:document.cancelform.submit()' />");
	echo ("</td>");
echo ("</tr>");

echo ("</table>");
echo ("</td>");
echo("	</tr>
	</table>");
echo ("</form>");

echo ("<form method='post' action='" . $PHP_SELF . "' name='cancelform'>");
echo ("<input type='hidden' name='select' value='view_job' />");
echo ("<input type='hidden' name='level1' value='job_id' />");
echo ("<input type='hidden' name='job_id' value='" . $job_id . "' />");
echo ("</form>");

?>
