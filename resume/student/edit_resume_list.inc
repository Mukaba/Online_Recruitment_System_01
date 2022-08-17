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
 | Filename: edit_resume_list.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: Displays a list of coverletters to edit to the student.         |
 +------------------------------------------------------------------------------+

*/

$sql = "SELECT r.resume_id, r.name, 
		t.description, DATE_FORMAT(r.last_modified, '%M %d, %Y %H:%i') as last_modified
		FROM resume as r LEFT JOIN coverletter_type as t ON t.type_id = r.type_id
		WHERE student_number = '" . addslashes($student_number) . "'
		ORDER BY r.type_id DESC, r.name";

$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount() == 0) {

	echo "<p>No resumes currently in the system.  To add a resume click on the 'Add' option under the 'Resume' menu.</p>";
}

else {

	echo ("<form action='$PHP_SELF' method='post'>");
	echo ("<input type='hidden' name='select' value='resume' />");
	echo ("<input type='hidden' name='page' value='edit' />");

	echo ("<table cellspacing='0' cellpadding='0' border='1'>");
	echo ("<tr>");
	echo ("<td>");

		echo ("<table border='0' cellpadding='2'>");
		echo ("<tr>");
			echo ("<td class='rowgrey' align='center'>");
			echo ("&nbsp; &nbsp; &nbsp;");
			echo ("</td>");

			echo ("<td class='rowgrey' align='center'>");
			echo ("<b class='white'>Resume</b>");
			echo ("</td>");

			echo ("<td class='rowgrey' align='center'>");
			echo ("<b class='white'>Type</b>");
			echo ("</td>");

			echo ("<td class='rowgrey' align='center'>");
			echo ("<b class='white'>Last Modified</b>");
			echo ("</td>");
		echo ("</tr>");
		
		$i = 0;
		while ($row = $result->FetchRow()) {
		
			(($i % 2 == 0) ? $class="row0d" : $class="row1d");
			$i ++; 
			$href = $PHP_SELF . "&amp;select=resume&amp;page=Edit&amp;resume_id=" . $row["resume_id"];

			echo ("<tr>");
				echo ("<td class='$class'>");
				echo ("&nbsp;<input class='$class' type='radio' name='resume_id' value='". $row["resume_id"] ."' />&nbsp;");
				echo ("</td>");

				echo ("<td class='$class'>");
				echo ("&nbsp; ". $row["name"] ." &nbsp;");
				echo ("</td>");

				echo ("<td class='$class'>");
				echo ("&nbsp; ". $row["description"] ." &nbsp;");
				echo ("</td>");

				echo ("<td class='$class'>");
				echo ("&nbsp; ". $row["last_modified"] ." &nbsp;");
				echo ("</td>");
			echo ("</tr>");
		}

		echo ("</table>");

		echo ("</td>");
	echo ("</tr>");
	echo ("</table>");
	echo ("<br />");

	if ($i > 0) {

		echo ("<input type='Submit' value=' Edit Resume ' />");
        echo ('&nbsp;&nbsp;&nbsp;&nbsp;');
        echo ('<input type="submit" name="continue" value="Rename Resume" />');
	}

	echo ("</form>");
}

include('misc/HTMLtoPDFInfo/HTMLtoPDFInfo.inc');

?>
