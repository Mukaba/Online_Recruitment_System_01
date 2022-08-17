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
 | Filename: confirm_cancel.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/
	assert($job_id); 
	$result = $GLOBALS['dbh']->Execute("
		SELECT job_code 
		FROM job_info 
		WHERE job_id='$job_id'
		");
	$row = $result->FetchRow();
	$job_code = $row["job_code"];

	$sql = ("
		SELECT DISTINCT job_id
		FROM interview_job_join
		WHERE interview_job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result->RecordCount())
	{
		// Not a multi-job interview.  Does it even have a job?
		$sql = ("
			SELECT DISTINCT job_id
			FROM job_info
			WHERE status!='" . INT_ONLY . "'
			AND job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		if ($result->RecordCount())
		{
			// Single job associated with interview.
			$row = $result->FetchRow();
			$associated_job_ids .= ("'" . $row["job_id"] . "'");
		}
	}
	else
	{
		// Multi-job interview.  Get all the job ids.
		
		while ($row = $result->FetchRow())
		{
			$associated_job_ids .= ("'" . $row["job_id"] . "',");
		}
		$associated_job_ids = substr($associated_job_ids, 0, -1);
	}
		
$msg .= ("<b><font size='2' color='black'>Are you sure you want to cancel the interview for </font><font size='3' color='yellow'>". $job_code . "</font></b><br /><br />");
$msg .= ("<form method='post' action='".$PHP_SELF."'>");

if ($associated_job_ids)
{
	$msg .= ("<b><font size='2' color='black'>If yes, please select a status to be assigned to this interview's associated job(s) and hit Confirm.</font></b><br /><br />");
	$msg .= ("<select name='associated_job_status'>");
	for ($i = 13; $i < 16; $i++)
	{
		$msg .= ("<option value='" . $i . "' ");
		if ($associated_job_status == $i)
		{
			$msg .= ("selected='selected'");
		}
		$msg .= (">" . getAdminStatusName($i) . "</option>");
	}
	$msg .= ("</select>");
	$msg .= ("<br /><br />");
}

$msg .= ("<table>");
$msg .= ("<tr>");
	$msg .= ("<td>");
   		$msg .= ("<input type='hidden' name='select' value='edit' />");
   		$msg .= ("<input type='hidden' name='job_id' value='".$job_id."' />");

		if ($associated_job_ids)
		{
			$msg .= ("<input type='hidden' name='associated_job_ids' value='" . urlencode($associated_job_ids) . "' />");
		}

   		$msg .= ("<input type='submit' name='submit' value='Confirm' />&nbsp;&nbsp;");
	//	$msg .= ("</form>");
	$msg .= ("</td>");
	$msg .= ("<td>");
	//	$msg .= ("<form method='post' action='".$PHP_SELF."'>");
   		$msg .= ("<input type='hidden' name='select' value='edit' />");
   		$msg .= ("<input type='hidden' name='job_id' value='".$job_id."' />");
   		$msg .= ("<input type='submit' name='submit' value='Back to Edit' />");
//		$msg .= ("</form>");
	$msg .= ("</td>");
$msg .= ("</tr>");
$msg .= ("</table>");
 $msg .= ("</form>");
//$msg .= ("</td></tr>");
//$msg .= ("</table>");
echo("<table width='100%' class='row1' cellpadding='8' cellspacing='0' border='4'><tr><td>");
echo($msg);
echo("</td></tr></table>");
//instruct($msg);
?>
