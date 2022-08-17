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
 | Description: This is the main code for editing a resume.                     |
 +------------------------------------------------------------------------------+

*/
?>
<h3>Resumes - Edit</h3>
<?php

if ($userlevel == ADMINSTUDENT) {
    error ("<h1 class=\"black\">Error</h1><p>You are not allowed to edit a student's resume from here.  You must
            log out of the student section and choose \"Resumes\" under \"Student Info\" to edit resumes.</p>");
}
else
{
    // renaming a resume
    if ($continue == 'Rename Resume') {
        if (!$resume_id) {
            error('Please select a resume to rename.');
        } else {
            $name = $GLOBALS['dbh']->GetOne("SELECT name FROM resume WHERE resume_id='$resume_id'");

            echo '<table border="0" cellpadding="2" cellspacing="0" class="row1">';
                echo '<tr>';
                echo '<td valign="middle" align="center">';
                    echo '<p><form action="',$PHP_SELF,'" method="post">';
                    echo '<input type="hidden" name="select" value="resume" />';
                    echo '<input type="hidden" name="page" value="edit" />';
                    echo '<input type="hidden" name="resume_id" value="',$resume_id,'" />';
                    echo '<p>&nbsp;Rename <b class="black">',$name,'</b> to ';
                    echo '<input type="text" name="name" maxlength="50" size="20" />';
                    echo '&nbsp;<input type="submit" name="continue" value="Rename" />&nbsp;';
                    echo '</p></form></p>';
               echo '</td></tr>';
            echo '</table>';
            return;
        }
    } elseif ($continue == 'Rename') {
        $name = removeSlashes($name);
        $GLOBALS['dbh']->Execute("UPDATE resume SET name='".addslashes($name)."' WHERE resume_id='$resume_id'");
        notify("Resume renamed to $name");
        unset($resume_id);
    }

    // if we have a resume_id, we have a resume, if not display the list
    if ($resume_id) {

        // find out what type of resume we have
        $sql = "SELECT resume_id, student_number
            FROM resume
            WHERE resume_id = '$resume_id'
            AND student_number = '$student_number'";
        $result = $GLOBALS['dbh']->Execute ($sql);

        if ($row = $result->FetchRow()) {
                include ('resume/student/html_editor.inc');
        
        } else {

            error ("Resume not found.");
            include ('resume/student/edit_resume_list.inc');
        }
    }

    else {

        include ('resume/student/edit_resume_list.inc');
    }
}


?>
