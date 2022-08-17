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
 | Filename: add_choose_items.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This page basically prompts the user for info to apply to a     |
 | job, and asks them if they would like to include a transcript.  $job_id      |
 | must already be supplied and set by the time this file is called.            |
 +------------------------------------------------------------------------------+

*/

// let's check some stuff regardinghtml editor we're allowed to use.
$sql = ("SELECT coverletter_allow_html_editor
		FROM department
		WHERE department_id = '". $auth->department ."'");
$result = $GLOBALS['dbh']->Execute ($sql);
if ($row = $result->FetchRow()) {
	if ($row["coverletter_allow_html_editor"]) {
		$allow_html = true;
	}
}



echo ("<table width='80%' class='row1' border='0' cellpadding='5' cellspacing='0'>");
echo("<tr>");
		echo ("<td>");

echo ("<form method='post' action='" . $PHP_SELF . "'>");

echo ("<table class='row1' border='0' cellpadding='5' cellspacing='0'>");
	echo ("<tr>");
		echo ("<td align='right' width='50%'>");
			echo ("<b class='black'>Cover letter:</b> &nbsp; ");
		echo ("</td>");

		echo ("<td align='left' width='50%'>");
			
            // the drop down menu for coverletters
			echo ("<select name='coverletter_type'>\n");
			$sql = ("SELECT type_id
					FROM coverletter
					WHERE student_number='$student_number'
					AND (type_id=3 OR type_id=2)
					ORDER BY type_id");
            $result = $GLOBALS['dbh']->Execute ($sql);
			if ($allow_html) {
				echo ("<option value='0_0'>Start from empty". ($allow_both ? " (using HTML Editor)" : "") ."</option>\n");
			}
			
			while ($row = $result->FetchRow()) {
				echo ("<option value='". $row["type_id"] . "_0" ."'");
				echo ((($row["type_id"] == TEMPLATE) ? " selected='selected'>Start with template" : ">Use Generic"));
				if ($allow_both && ($row["type_id"] != GENERIC)) {
					echo (" (using HTML Editor)");
				}
				echo ("</option>\n");
			}
			echo ("</select> <br />");
		echo ("</td>");
	echo ("</tr>");


	echo ("<tr>");
		echo ("<td align='right'>");
			echo ("<b class='black'>Resume:</b> &nbsp;");
		echo ("</td>");

		echo ("<td align='left'>");
			// display a dropdown list of resumes currently associated with that student
			$sql = ("SELECT name, resume_id, student_number, type_id FROM resume
					WHERE student_number='$student_number'");
			$result = $GLOBALS['dbh']->Execute ($sql);
			echo ("<select name='resume_id'>");
			while ($row = $result->FetchRow()) {
				echo ("<option value='". $row["resume_id"]. "'> ". $row["name"] ."</option>");
			}
			echo ("</select>");
		echo ("</td>");
	echo ("</tr>");

    // check if a transcript exists
    $num_transcripts = $GLOBALS['dbh']->GetOne("SELECT COUNT(*) FROM transcript WHERE student_number='$student_number'");
    if ($num_transcripts) {
        echo ("<tr>");
            echo ("<td align='right'>");
                echo ("Would you like to include your transcript<br />
                       when applying to this job? (Recommended)");
            echo ("</td>");

            echo ("<td align='left'>");
    //			echo ("Would you like to include your transcript<br /> when applying to this job? (Recommended)<br />");
                echo ("<input type='radio' name='include_transcript' value='1' checked='checked' />Yes&nbsp;&nbsp;");
                echo ("<input type='radio' name='include_transcript' value='0' />No&nbsp;");
            echo ("</td>");
        echo ("</tr>");
    } else {
        echo ('<tr>');
            echo ('<td align="center" colspan="2">');
                echo ('<b class="black">Note: </b>You do not have a transcript in the system to include with your application.');
                echo ('<input type="hidden" name="include_transcript" value="0" />');
            echo ('</td>');
        echo ('</tr>');
    }

	echo ("<tr>");
		echo ("<td colspan='2'><hr /></td>");
	echo ("</tr>");


	echo ("<tr>");
		echo ("<td align='right'>");
			echo ("<input type='submit' name='next_step_edit_letter' value='Continue' />");
            echo ("<input type='hidden' name='select' value='applications' />");
            echo ("<input type='hidden' name='job_id' value='" . $job_id . "' />");
            echo ("<input type='hidden' name='option' value='add_online' />");
		echo ("</td>");

		echo ("<td align='left'>");
            echo("<input type='button' name='cancel' value='Cancel' onclick='javascript:document.cancel_form.submit()' />");
		echo ("</td>");
	echo ("</tr>");

	echo ("</table>");


    echo("</form>");
	echo ("</td>");
echo ("</tr>");
echo ("</table>");

include ('misc/HTMLtoPDFInfo/HTMLtoPDFInfo.inc');

echo ("<form name='cancel_form' method='post' action='" . $PHP_SELF . "'>");
echo ("<input type='hidden' name='select' value='view_job' />");
echo ("<input type='hidden' name='level1' value='job_id' />");
echo ("<input type='hidden' name='job_id' value='" . $job_id . "' />");
echo ("</form>");
?>
