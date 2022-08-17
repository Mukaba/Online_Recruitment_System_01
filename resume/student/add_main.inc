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
 | Filename: add_main.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description: Allows the student to enter the name of a resume to add to      |
 | the system.                                                                  |
 +------------------------------------------------------------------------------+

*/

?>
<h3>Resumes - Add</h3>
<?php

$error_string = "<h1 class=\"black\">Error</h1><p>You are not allowed to add a resume for a student.</p>";
$resume_name = removeSlashes($specific_name);

if ($type_id == SPECIFIC) {
    // check that if we've chosen a specific resume, there's a name
    if ($specific_name == "") {

        error ("Please provide a name for your Specific Resume");
        // set the editor so it falls properly below 
        $editor = "";
    }

    // check to make sure they don't already have a resume with that name
    else {

        $sql = "SELECT resume_id
            FROM resume
            WHERE student_number = '$student_number'
            AND name = '". addSlashes (stripSlashes ($specific_name)) ."'";
        $result = $GLOBALS['dbh']->Execute ($sql);

        if ($result->RecordCount()) {

            error ("You already have a resume with that name, please choose another name or edit that resume.");
            $editor = "";
        }
    }
    $sql = "SELECT count(*)
            FROM resume
            WHERE student_number = '$student_number'
            AND type_id = ".SPECIFIC;
    $num_specific = $GLOBALS['dbh']->GetOne($sql);
    if ($num_specific >=5){
        error ("You already have 5 specific resumes.  You must delete an existing one before adding another specific resume.");
        $editor = "";
    }
}

if ($editor == "html") {
    if ($userlevel == ADMINSTUDENT) {
        error("<h1 class=\"black\">Error</h1><p>You are not allowed to add a resume for a student.</p>");
    }
    else {
        include ('resume/student/html_editor.inc');
    }
}
else 
{
    include ('resume/student/add_decision.inc');
}
?>
