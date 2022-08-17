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
 | Filename: add_decision.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: Allows the student to enter the name of a coverletter to add    |
 | to the system.                                                               |
 +------------------------------------------------------------------------------+

*/

// quickly find out which coverletters they already have
// if they're full to their limit, let 'em know but don't display anything
$sql = "SELECT type_id
		FROM coverletter
		WHERE student_number = '" . addslashes($student_number) . "'";

$result = $GLOBALS['dbh']->Execute ($sql);

$need_generic = true;
$need_template = true;
$num_coverletters = 0; // for specific coverletters
while ($row = $result->FetchRow()) {
	
	if ($row["type_id"] == GENERIC) {
	
		$need_generic = false;
	}
	
	elseif ($row["type_id"] == TEMPLATE) {
	
		$need_template = false;
	}

	elseif ($row["type_id"] == SPECIFIC) {

		$num_coverletters ++;
	}
}

if (!$need_generic && !$need_template) {

	notify ("You already have a template and a generic coverletter. If you would like to add a specific coverletter for a 
			job, please add it by applying to that job in Job Descriptions.");
}

else {

	echo ("<form method='post' action='$PHP_SELF'>\n");
	echo ("<input type='hidden' name='select' value='coverletter' />\n");
	echo ("<input type='hidden' name='page' value='add' />\n");

	echo ("<table border='0' class='row1' cellspace='0' cellpadding='5'>\n");

	// let's do some checking to see if we have to display the table
	$allow_html = false;
	$sql = "SELECT d.coverletter_allow_html_editor AS html,
			d.department_id, d.department_name
			FROM department AS d, student_department AS sd
			WHERE sd.department_id = d.department_id
			AND sd.student_number = '". $student_number ."'";
	$result = $GLOBALS['dbh']->Execute ($sql);

	while ($row = $result->FetchRow()) {

		if ($row["html"] == 1) {

			$allow_html = true;
		}
	}

	if (!$allow_html) {

		echo ("<tr>\n");
			echo ("<td colspan='2'>");
			echo ("<b class='black'>Your Co-op Department has not allowed you to use the HTML Editor, please inform them.</b>");
			echo ("<br /><br />");
			echo ("</td>\n");
		echo ("</tr>\n");
	}

	elseif ($allow_html) {

		echo ("<input type='hidden' name='editor' value='html' />");
	}

	echo ("<tr>\n");
		echo ("<td colspan='2' align='center'>");
		echo ("<h2>Choose Type</h2>");
		echo ("</td>\n");
	echo ("</tr>\n");

	if ($need_generic) {

		echo ("<tr>\n");
			echo ("<td width='20%'>");
			echo ("<input type='radio' name='type_id' value='". GENERIC ."' checked='checked' /> &nbsp;");
			echo ("<b class='black'>Generic</b>");
			echo ("</td>\n");
			
			echo ("<td width='80%'>");
			echo ("Add a Generic Cover Letter - This is the default coverletter that will be sent out when a specific coverletter is not selected while applying.");
			echo ("</td>\n");
		echo ("</tr>\n");
	}

	if ($need_template) {
		
		echo ("<tr>\n");
			echo ("<td width='20%'>");
			echo ("<input type='radio' name='type_id' value='". TEMPLATE ."'". (!$need_generic ? " checked='checked'" : "" ) ." /> &nbsp;");
			echo ("<b class='black'>Template</b>");
			echo ("</td>\n");

			echo ("<td width='80%'>");
			echo ("Add a Template Cover Letter - This coverletter will never be sent out, it is simply to be used as a basis for you to build specific coverletters.");
			echo ("</td>\n");
		echo ("</tr>\n");
	}


	echo ("<tr>\n");
		echo ("<td colspan='2' align='center'>");
		echo ("<hr />");
		echo ("</td>\n");
	echo ("</tr>\n");

	echo ("<tr>\n");
		echo ("<td colspan='2' align='center'>");
		echo ("<input type='submit' name='' value=' Continue and Add ' />");
		echo ("</td>\n");
	echo ("</tr>\n");

	echo ("</table>");
}
?>
