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
 | Filename: edit_main.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: Displays a list of cover letters the student can choose from    |
 | to edit.                                                                     |
 +------------------------------------------------------------------------------+

*/

?>
<h3>Cover Letters - Edit</h3>
<?php
if ($userlevel == ADMINSTUDENT) {
    error("<h1 class=\"black\">Error</h1><p>You are not allowed to edit a cover letter from here.  You must log out of
            the student section and choose \"Cover Letters\" under \"Student Info\" to edit cover letters.</p>");
}
else
{

// if we have a coverletter_id, means we're actually editing it
    // if we have a coverletter_id, we have a coverletter, if not display the list
    if ($coverletter_id) {

        // find out what type of coverletter we have 
        $sql = "SELECT coverletter_id, student_number
                FROM coverletter
                WHERE coverletter_id = '$coverletter_id'
                AND student_number = '$student_number'";
        $result = $GLOBALS['dbh']->Execute ($sql);

        if ($row = $result->FetchRow()) {
                include ('coverletters/student/html_editor.inc');
        }

        else  {

            error ("Cover letter not found.");
            include ('coverletters/student/coverletter_list.inc');
        }
    }

    else {
        include ('coverletters/student/coverletter_list.inc');
    }
}
