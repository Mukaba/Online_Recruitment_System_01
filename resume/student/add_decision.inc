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
 | Description: Allows the student to enter the name of a resume to add to      |
 | the system.                                                                  |
 +------------------------------------------------------------------------------+

*/

// find out which resumes they already have
$sql = "SELECT type_id
	FROM resume
	WHERE student_number = '$student_number'";

$result = $GLOBALS['dbh']->Execute ($sql);

$need_generic = true;
$need_template = true;
$num_resumes = 0; // for specific resumes
while ($row = $result->FetchRow()) {
	
	if ($row["type_id"] == GENERIC) {
	
		$need_generic = false;
	}
	
	elseif ($row["type_id"] == TEMPLATE) {
	
		$need_template = false;
	}

	elseif ($row["type_id"] == SPECIFIC) {

		$num_resumes ++;
	}
}

if ($num_resumes >= 5 && !$need_template && !$need_generic) {
    notify ("You already have the maximum number of resumes allowed.  Please delete or edit your existing ones if you would like a new resume.");
    return;
}

echo ("<form method='post' action='$PHP_SELF'>\n");
echo ("<input type='hidden' name='select' value='resume' />\n");
echo ("<input type='hidden' name='page' value='add' />\n");

echo ("<table border='0' class='row1' cellpadding='5'>\n");

// let's do some checking to see if we have to display the table
$allow_html = false;
$sql = "SELECT d.resume_allow_html_editor AS html, d.department_id, d.department_name
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


// ask the user to choose the type of resume they want: generic, template, specific.
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
		echo ("Add a Generic Resume - This is the default resume that will be sent out when a specific resume is not selected while applying.");
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
		echo ("Add a Template Resume - This resume will never be sent out, it is simply to be used as a basis for you to build specific resumes.");
		echo ("</td>\n");
	echo ("</tr>\n");
}
		
if ($num_resumes < 5)
{
    echo ("<tr>\n");
    echo ("<td width='20%'>");
        echo ("<input type='radio' name='type_id' value='". SPECIFIC ."'". ((!$need_template && !$need_generic) ? " checked='checked'" : "") ." /> &nbsp;");
        echo ("<b class='black'>Specific</b>");
        echo ("</td>\n");

        echo ("<td width='80%'>");
        echo ("Add your own specific resume (Maximum 5) You currently have : $num_resumes <br />");
        echo ("Name: &nbsp;");
        echo ("<input type='text' name='specific_name' size='30' maxlength='35' />");

        if ($template) {

            echo ("&nbsp; &nbsp; ");
            echo ("<input type='checkbox' name='use_template' checked='checked' />");
            echo ("Use template");
        }
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
    echo ("<input type='submit' name='continueadd' value=' Continue and Add ' />");
    echo ("</td>\n");
echo ("</tr>\n");
echo ("</table>");
echo ("</form>");

?>
