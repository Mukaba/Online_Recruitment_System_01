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
 | Filename: add_view_app.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: Show the user their application, and get confirmation from      |
 | them.                                                                        |
 +------------------------------------------------------------------------------+

*/

/*
echo ("<form method='post' action='" . $PHP_SELF . "'>");
echo ("<input type='hidden' name='select' value='applications' />");
echo ("<input type='hidden' name='option' value='add_online' />");

echo ("<input type='hidden' name='app_letter' value='".(urlencode($app_letter))."' />");
echo ("<input type='hidden' name='job_id' value='$job_id ' />");
echo ("<input type='hidden' name='resume_id' value='$resume_id' />");
echo ("<input type='hidden' name='include_transcript' value='$include_transcript' />");
*/

echo("<table width='100%' border='0' cellpadding='5' cellspacing='0'>");

	echo("<tr><td colspan='2'><hr /></td></tr>");

	echo("<tr>");
		echo("<td colspan='2'>");
			echo("<b class='black'>Cover Letter:</b> <br />");
			echo stripslashes($app_letter);
			echo("<br /><br />");
			echo("<b class='black'>Resume to be attached:</b> $resume_name <br />");
			echo("<b class='black'>Transcripts:</b> ".(($include_transcript != 0) ? "Attached" : "Suppressed")."<br />");
	
		echo("</td>");

	echo("</tr>");


	echo("<tr><td colspan='2'><hr /></td></tr>");

	echo("<tr>");
		echo("<td width='20' align='right'>");
echo ("<form method='post' action='" . $PHP_SELF . "'>");
echo ("<input type='hidden' name='select' value='applications' />");
echo ("<input type='hidden' name='option' value='add_online' />");

echo ("<input type='hidden' name='app_letter' value='".(urlencode($app_letter))."' />");
echo ("<input type='hidden' name='job_id' value='$job_id ' />");
echo ("<input type='hidden' name='resume_id' value='$resume_id' />");
echo ("<input type='hidden' name='include_transcript' value='$include_transcript' />");

			echo("<input type='submit' name='next_step_insert' value='Confirm' />");
                	echo("</form>");
		echo("</td>");
		
		echo("<td align='left'>");
                	echo("<form method='post' action='" . $PHP_SELF . "'>");
                        echo("<input type='hidden' name='select' value='view_job' />");
                        echo("<input type='hidden' name='level1' value='job_id' />");
                        echo("<input type='hidden' name='job_id' value='" . $job_id . "' />");
                        echo("<input type='submit' value='Cancel' />");
                	echo("</form>");
		echo("</td>");
	echo("</tr>");

	echo("</table>");
//	echo("</td>");


//echo("</tr>");
//echo("</table>");

?>
