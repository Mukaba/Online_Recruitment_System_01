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
 | Filename: confirm_delete.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: Confirm the deletion of an application.                         |
 +------------------------------------------------------------------------------+

*/

if ($userlevel == ADMINSTUDENT) {
    error("<h1 class=\"black\">Error</h1><p>You are not allowed to delete a student's application from here.
            You must log out of the student section and choose \"Remove an application\" in the \"Send\" section of \"Job Applications\".</p>");
}
else {
    echo ("<table border='1' cellspacing='0' cellpadding='0' class='error'>\n");
    echo ("<tr class='error'>\n");
        echo ("<td>");
        echo ("<table width='100%' cellpadding='5' cellspacing='0'>");
        echo ("<tr>");
            echo ("<td colspan='2' align='center'>");
            echo ("You have chosen to delete your application");

            // now need to get the job associated with this app
            $sql = "SELECT ji.position_title, ec.company_name
                    FROM job_info AS ji, applications AS a, employer_company AS ec
                    WHERE ji.job_id = a.job_id
                    AND ec.employer_id = ji.employer_id
                    AND a.application_id = '$application_id'";

            $result = $GLOBALS['dbh']->Execute ($sql);

            if ($row = $result->FetchRow()) {

                echo (" for the position of <br /><b>". $row["position_title"] ."</b><br />");
                echo ("at <b>". $row["company_name"] ."</b>");
            }

            echo ("<br />This will also delete your specific cover letter associated with this job.");
            echo ("<br /><br />Are you sure you would like to continue?");

            echo ("</td>");
        echo ("</tr>");

        echo ("<tr align='center'>");
            echo ("<td align='right'>");
                echo ("<form method='post' action='$PHP_SELF' name='form'>\n");
    echo ("<input type='hidden' name='select' value='applications' />\n");
    echo ("<input type='hidden' name='application_id' value='$application_id' />\n");
    echo ("<input type='hidden' name='option' value='edit_delete' />\n");
    echo ("<input type='hidden' name='delete_confirm' value='1' />\n");


            echo ("<input type='submit' value=' Yes ' />");
                    echo ("</form>");
                    echo ("</td>");
                    echo ("<td align='left'>");
                    echo ("<form method='post' action='" . $PHP_SELF . "'>");
                            echo ("<input type='hidden' name='select' value='applications' />");
                            echo ("<input type='hidden' name='option' value='edit_delete' />");
                            echo ("<input type='submit' value=' No ' />");
                    echo ("</form>");
        echo ("</table>");
        echo ("</td>");
    echo ("</tr>");
    echo ("</table>");
}

?>
