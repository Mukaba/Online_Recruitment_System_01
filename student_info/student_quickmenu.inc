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
 | Filename: student_quickmenu.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This is the file used to display the quickmenu for student      |
 | pages.  This allows us to ensure a consistent set of options in every        |
 | quickmenu shown within these pages.                                          |
 |                                                                              |
 | The following variable(s) must be set before calling this script:            |
 | $record_id                                                                   |
 +------------------------------------------------------------------------------+

*/

/* 
 :TODO Add the following items to this quickmenu in the future.
 - Job Applications
 - Set Actions
*/

// Display the quickmenu
echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='record_id' value='" . $record_id . "' />");
echo("<input type='hidden' name='student_num' value='" . $student_num . "' />");
echo("<input type='hidden' name='PDF' value='1' />");

echo("<select name='select'>");
echo("<option value='view_student'>View this Student</option>\n");
echo("<option value='student_coverletters'>View Student's Cover Letters</option>\n");
echo("<option value='student_resumes'>View Student's Resumes</option>\n");
echo("<option value='student_transcripts'>View Student's Transcripts</option>\n");
echo("<option value='student_interviews'>View Student's Interviews</option>\n");
echo("<option value='student_history'>View Student's Placement History</option>\n");
echo("<option value='edit_student'>Edit this Student</option>\n");
echo("<option value='place_student_posted_job'>Place this Student in a Posted Job</option>\n");
echo("<option value='place_student_own_job'>Place this Student in \"Own Job\"</option>\n");
echo("<option value='place_student_returning_job'>Place this Student in \"Returning Job\"</option>\n");
echo("<option value='login_as_student'>Log in as this Student</option>\n");
echo("</select>");

echo("&nbsp;");
echo("<input type='image' src='misc/images/arrowg.gif' alt='go' />");
echo("</form>");
//echo("</td>");
